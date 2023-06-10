// Fill out your copyright notice in the Description page of Project Settings.

#include "PhantomPawn.h"
#include"BasePawn.h"
#include "MyGameMode.h"

//DEBUG:
	//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("Sono Qua !")));
	//DEBUG:
	//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("CurrentGridCoords: X=%f, Y=%f "), CurrentGridCoords.X, CurrentGridCoords.Y));
	//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("valid for walk: X=%f "), TheGridGen->IsNodeValidForWalk(Node)));
	//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("CurrentDirection: X=%f, Y=%f, Z=%f "), CurrentDirection.X , CurrentDirection.Y, CurrentDirection.Z));





//COSTRUTTORE di BasePawn
APhantomPawn::APhantomPawn()
{
	// N.B il pawn viene spawnato automaticamente nella posizione del player start
	// dato che il pawn di default è stato impostato nei setting come BP_GridPawn
	//  
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	CurrentDirection = FVector(0, 0, 0);
	////posizione iniziale  del pawn nelle coordinate di griglia 
	CurrentGridCoords = FVector2D(0,0);
	//// nodi
	LastNode = nullptr;  //L'ultimo nodo in cui si è trovato il pawn
	TargetNode = nullptr; //Il nodo in cui il pawn sta per spostarsi 
	NextNode = nullptr; // Il prossimo nodo nella direzione in cui sta andando il Pawn

	

}



int32 APhantomPawn::GetGhostCounterLimit()
{
	return GhostCounterLimit;
}

void APhantomPawn::SetGhostCounterLimit(int32 Limit)
{
	GhostCounterLimit = Limit;
}

int32 APhantomPawn::GetGhostCounter()
{
	return 0;
}

void APhantomPawn::IncrementGhostCounter()
{
}

void APhantomPawn::ResetGhostCounter()
{
}

void APhantomPawn::CheckGhostCounter()
{
	if (Mooveset == ATHOUSE) 
	{
		if (GetGhostCounter() >= GhostCounterLimit) 
		{
	    //DEBUG:
		//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("Qualcuno sta per uscire di casa  !")));
		//Mooveset = EXITHOUSE;
		}
	}
}



// Called when the game starts or when spawned
void APhantomPawn::BeginPlay()
{
	Super::BeginPlay();
	 
	
	//Voglio un riferimento al BasePawn che è già in gioco e posseduto dal player 
	// La funzione GetActorOfClass prende il primo oggetto di quella classe già presente nel mondo 
	//AInteractableObject* InteractableObject = Cast<AInteractableObject>(UGameplayStatics::GetActorOfClass(World, AInteractableObject::StaticClass()));
	Pacman = Cast<ABasePawn>(UGameplayStatics::GetActorOfClass(GetWorld(), ABasePawn::StaticClass()));
	// Debug : Verifica se il pawn è stato trovato(Funziona !)
	/*
	 if (Pacman)
	{
		FVector2D PacmanPosition = Pacman->GetGridPosition();
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("stampa la posizione di pacman : x=%f , y=%f ") , PacmanPosition.X ,PacmanPosition.Y));

	}
	*/

	//Prendo un riferimento al MyGameMode e al GameField 
	GameMode = (AMyGameMode*)(GetWorld()->GetAuthGameMode());
	TheGridGen = GameMode->GField;
	//Get the GameInstance reference 
	GameInstance = GameMode->GameInstance;

	FVector2D StartNode = TheGridGen->GetXYPositionByRelativeLocation(GetActorLocation());
	LastNode = TheGridGen->TileMap[StartNode];

	//Setto le velocità dei fantasmi nelle varie condizioni (in base al livello )
	SetSpeeds();
	// Setto Direzione iniziale del fantasma e CurrentGridCoords
	CurrentGridCoords = TheGridGen->GetXYPositionByRealLocation(GetActorLocation());
	CurrentDirection = FVector(0, 0, 0);
	LoadSpecialSpot();
	
}


//Funzioni sullo stato del fantasma 
TEnumAsByte<EPhantomStatus> APhantomPawn::GetGhostStatus()
{
	return GhostStatus;
}

void APhantomPawn::SetGhostStatus(const TEnumAsByte<EPhantomStatus> Status)
{
	if (Status == SCATTER)
	{
		DirectionInvercted = false;
	}
	GhostStatus = Status;
}

TEnumAsByte<EMooveset> APhantomPawn::GetGhostMooveset()
{
	return Mooveset;
}

void APhantomPawn::SetGhostMooveset(const TEnumAsByte<EMooveset> Behavior)
{
	Mooveset = Behavior;
}



//E' una funzione che uso per settare alcuni attributi dentro ai fantasmi
void APhantomPawn::LoadSpecialSpot() 
{

}


// Called every frame
//Il Tick è un metodo che viene chiamato ad ogni frame e che noi sfrutteremo per far muovere gli oggetti nel campo nda gioco
void APhantomPawn::Tick(float DeltaTime)
{

	Super::Tick(DeltaTime);

	//Controllo i contatori dei fantasmi per farli uscire dalla casa 
	CheckGhostCounter();
	//Così la GameIstance ha il controllo sul movimento di tutti i Pawn 
	if (!GameInstance->GetBlockAllPawn()) {
		//Chiamiamo la funzione che definiamo qua sotto e che serve a gestire il movimento e velocità dei Pawn nel campo da gioco
		APhantomPawn::HandleMovement();
	}
}

//Setta il TargetNode sulla base del NextNode 
void APhantomPawn::HandleMovement()
{
	
	//Se TargetNode è già pieno posso muovere il Pawn
	MoveToCurrentTargetNode();

	//Se il campo TargetNode è vuoto (Il nodo in cui sto per muovermi)  
	if (TargetNode == nullptr)
	{
		//Se il campo NextNode è NON vuoto (Il prossimo nodo nella direzione in cui mi sto muovendo)
		if (NextNode != nullptr)
		{
			SetTargetNode(NextNode);
			SetNextNode(nullptr);
		}
	}
	// questo codice fa si che il pawn si muova autonomamente fino a che il next node è walkable ovvero fino a che va dritto 

	//Se TargetNode e NextNode sono vuoti 
	if (!TargetNode && !NextNode) //<-------------------------PARTE CHE DECIDE LA DIREZIONE DEL FANTASMA NEL LABIRINTO -------------------------------------------------------------
	{
		
		//Se il prossimo nodo nella direzione inserita LastInputDirection è WALKABLE (non serve)
		//if (TheGridGen->IsNodeValidForWalk(TheGridGen->GetNextNode(CurrentGridCoords, LastValidInputDirection)))
		 
		//Se il fantasma è in giro per il labirinto e non a casa 
		if (Mooveset == NORMAL) {
			if (CrossingDetection())
			{
				WhereAmIGoingUpdate();
				FVector NewDirection = ChoseNewDirection();
				SetCurrentDirection(NewDirection);
				//Setto LastValidDirection = LastInputDirection 
				//SetLastValidDirection(LastInputDirection);<------------------------------------------------------------------------------------------e anche qua 
			}

			SetNodeGeneric(CurrentDirection);
		}
		//-----------------------------------------------------------MOVIMENTO DEL FANTASMA DENTRO LA CASA----------------------------------------------------
		else if (Mooveset = ATHOUSE)
		{
			
			float Down = TheGridGen->GetXYPositionByRealLocation(TheGridGen->GetPinkySpawn()).X;
			float Up = Down + 2;
			if (CurrentGridCoords.X == Down) 
			{
				 HouseDirection = FVector(1, 0, 0);
			}
			else if(CurrentGridCoords.X == Up)
			{
				 HouseDirection = FVector(-1, 0, 0);
			}
			else 
			{
			
			}
	        //DEBUG:
			//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("CurrentGridCoords: X=%f, Y=%f "), CurrentGridCoords.X, CurrentGridCoords.Y));
			SetNodeGeneric(HouseDirection);
		}
		//---------------------------------------------------------PARTE CHE FA USCIRE I FANTASMI DALLA CASA ------------------------------------------------------
		else if(Mooveset= EXITHOUSE)
		{
			//DEBUG:
	        GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("Debug :Exit the House move")));
		}
	}
}


//--------------------------------MOVIMENTO DEL Pawn in una nuova casella (Se c'è gia il TargetNode non null)-------------------------------------------------
void APhantomPawn::MoveToCurrentTargetNode()
{
	//Se TargetNode è null non faccio niente ed esco dalla funzione  
	if (TargetNode == nullptr) return;

	//Calcolo la distanza fra il TargetNode e la posizione del Pawn  
	const float Dist = FMath::Abs(FVector::Dist2D(TargetNode->GetActorLocation(), GetActorLocation()));

	//Se la distanza è minore o uguale a 4
	if (Dist <= AcceptedDistance)
	{
		OnNodeReached();
		return;
	}
	// funzione di interpolazione che fa muovere il pawn verso una nuova posizione data la posizione corrente
	const FVector2D StartVector = TheGridGen->GetFVector2DFromFVector3D(GetActorLocation());
	const FVector2D EndVector = TheGridGen->GetFVector2DFromFVector3D(TargetNode->GetActorLocation());

	//?? cosa fa la questo Pos ??
	const auto Pos = FMath::Vector2DInterpConstantTo(StartVector, EndVector, GetWorld()->GetDeltaSeconds(), CurrentMovementSpeed);
	const FVector Location(Pos.X, Pos.Y, GetActorLocation().Z);
	//Setta la posizione del Pawn ad ogni Tick
	SetActorLocation(Location);
}

//----------------------------------------------------Pacman EAT ---------------------------------------------------------------------------------
void APhantomPawn::Eat() 
{
	
	if (TargetNode == nullptr) return;
	//Prendo la posizione di Pacman 
	if (GhostStatus == CHASE || GhostStatus == SCATTER)
	{
		if (Pacman->GetGridPosition() == TargetNode->GetGridPosition())
		{
			//DEBUG:
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT(" Pacman e' stato mangiato ")));
			GameMode->GameInstance->DecrementCurrentLives();
			//<---------------------------------------------------------------------------------------------------------------------DaCompletare
			//Controllo il numero di righe vite di pacman per vedere se sono 0
			if (GameMode->GameInstance->GetCurrentLives() < 0) 
			{
			//Decreto il GameOver
				GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT(" GAME-OVER ")));
			}
			
		}
	}
	
}

//-------------------------------------------------------------------------------------------------------------------------------------------------


//Quando il Pawn raggiunge il TargetNode
void APhantomPawn::OnNodeReached()
{
	if (GetGhostStatus() == SCATTER)
	{
		if(!DirectionInvercted)
		{
			//Inverto la direzione la prima volta che entro in un nodo in modalità scatter
			CurrentDirection = CurrentDirection * -1;
			//Setto direction invercted true in modo tale che non inverta la direzione ogni volta che raggiunge un nodo ma solo la prima volta 
			DirectionInvercted = true;
		}
	}
	//Mangia quello che si trova nel Node 
	Eat();

	//Aggiorna gli attributi del Pawn di conseguenza
	CurrentGridCoords = TargetNode->GetGridPosition();
	LastNode = TargetNode;
	SetTargetNode(nullptr);
	//--------------------------------------------------------Tunnel Node (riduci la velocità del fantasma)--------------------------------------------------
	
	TunnelCheck();
	


	//---------------------------------------------------------Teleport Sinistro-------------------------------------------------------------------------------

	//Mi prendo la posizione sulla griglia dei due teleport 
	FVector2D LeftTeleportGridPosition = TheGridGen->GetXYPositionByRealLocation(TheGridGen->GetLeftTeleportLocation());
	FVector2D RightTeleportGridPosition = TheGridGen->GetXYPositionByRealLocation(TheGridGen->GetRightTeleportLocation());

	//Se pacman è al teleport sinisto e ci va contro  
	if (CurrentGridCoords == LeftTeleportGridPosition && CurrentDirection == FVector(0, -1, 0))
	{
		//Teletrasporto il pawn al teleport Destro
		const FVector Location(TheGridGen->GetRightTeleportLocation());
		SetActorLocation(Location);
		//Aggiorno gli attributi del Pawn di conseguenza
		CurrentGridCoords = RightTeleportGridPosition;

	}
	//---------------------------------------------------------Teleport Destro-------------------------------------------------------------------------------
	if (CurrentGridCoords == RightTeleportGridPosition && CurrentDirection == FVector(0, 1, 0))
	{
		//Teletrasporto il pawn all'altro capo del teleport
		const FVector Location(TheGridGen->GetLeftTeleportLocation());
		SetActorLocation(Location);
		//Aggiorno gli attributi del Pawn di conseguenza
		CurrentGridCoords = LeftTeleportGridPosition ;

	}
}

void APhantomPawn::SetTargetNode(ABaseNode* Node)
{
	TargetNode = Node;
}

void APhantomPawn::SetNextNode(ABaseNode* Node)
{
	NextNode = Node;
}

//Setta in nodo con Il getNextNode se è WALKABLE
void APhantomPawn::SetNodeGeneric(const FVector Dir)
{
	const auto Node = TheGridGen->GetNextNode(CurrentGridCoords, Dir);

	if (TheGridGen->IsNodeValidForWalk(Node))
	{
		SetTargetNode(Node);
		
	}
}




FVector2D APhantomPawn::CheckPacmanGridPosition()
{
   FVector2D PacmanGridPosition = Pacman->GetGridPosition();
	return PacmanGridPosition;
}

FVector APhantomPawn::CheckPacmanDirection()
{
	FVector PacmanDirection = Pacman->GetLastValidDirection();
	return PacmanDirection;
}

void APhantomPawn::WhereAmIGoingUpdate()
{

}

void APhantomPawn::SetCurrentMovementSpeed(float NewSpeed)
{
	CurrentMovementSpeed = NewSpeed;
}

float APhantomPawn::GetCurrentMovementSpeed()
{
	return CurrentMovementSpeed;
}

void APhantomPawn::TunnelCheck()
{
	
	bool IsInTheTunnel = false;

	for (const auto& element : TheGridGen->GetTunnelNodeLocations())
	{
		FVector2D TunnelNodeGridPosition = TheGridGen->GetXYPositionByRealLocation(element);
		if (CurrentGridCoords == TunnelNodeGridPosition)
		{
			IsInTheTunnel = true;
		}
	}

	if(IsInTheTunnel)
	{
		SetCurrentMovementSpeed(GhostTunnelSpeed);
	}
	else
	{
		if (GhostStatus == NORMAL || GhostStatus == CHASE)
		{
			SetCurrentMovementSpeed(GhostSpeed);
		}
		//Il fantasma è in Fright
		else 
		{
			SetCurrentMovementSpeed(FrightGhostSpeed);
		}
	}
	
}

//<-----------------------------------------------------------------------------------------------------------------------------------------Va sostituita(Cancellata)
/*
void APhantomPawn::SetVerticalInput(float AxisValue)
{
	
	if (AxisValue == 0) return;
	const FVector Dir = (GetActorForwardVector() * AxisValue).GetSafeNormal();
	LastInputDirection = Dir.GetSafeNormal();
	SetNextNodeByDir(LastInputDirection);
	
}
*/
//<-------------------------------------------------------------------------------------------------------------------------------------------Va sostituita (Cancellata)
/*
void APhantomPawn::SetHorizontalInput(float AxisValue)
{
	
	if (AxisValue == 0) return;
	const FVector Dir = (GetActorRightVector() * AxisValue).GetSafeNormal();
	LastInputDirection = Dir;
	SetNextNodeByDir(LastInputDirection);
	
}
*/
//<-----------------------------------------------------------------CrossingDetection---------------------------------------------------------------------------------------------------------
bool APhantomPawn::CrossingDetection()
{
	//Questa funzione serve a controllare se il Fantasma si trova in un incrocio , da true se è ad un incrocio false altrimenti 
	int count = 0;
	FVector AllDir[] = { FVector(0,1,0),FVector(1,0,0), FVector(0,-1,0),FVector(-1,0,0) };
	for (FVector PossibleDir : AllDir) {
		//Escludo la direzione attuale del fantasma e la sua opposta (visto che non possono fare dietrofront )
		if (PossibleDir != CurrentDirection && PossibleDir != CurrentDirection * (-1))
		{
			//Prendo il prossimo nodo in quella direzione 
			auto Node = GameMode->GField->GetNextNode(CurrentGridCoords, PossibleDir);
			//Verifico che il nodo sia walkable 
			if (GameMode->GField->IsNodeValidForWalk(Node)) 
			{
				count = count + 1;
			}
		}
	}
	if (count != 0) { return true; }
	return false ;
}
void APhantomPawn::SetSpeeds()
{
	//Dipendono dal livello <------------------------------------------------------------------------------------------DaModificare
	//Prendo la velocità standard dalla GameInstance e assegno le varie velocità
	float StandardSpeed = GameInstance->GetStandardMovementSpeed();
	//DEBUG:
	//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("SetSpeeds: StandardSpeed=%f "), StandardSpeed));
	 GhostTunnelSpeed = (4.0f/10.0f)*StandardSpeed;
		 GhostSpeed = (3.0f/4.0f ) * StandardSpeed;
		 FrightGhostSpeed =(1.0f/ 2.0f)* StandardSpeed;
		 //DEBUG:
		 //GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("SetSpeeds: GhostTunnelSpeed=%f,  GhostSpeed=%f,  FrightGhostSpeed=%f "), GhostTunnelSpeed, GhostSpeed, FrightGhostSpeed));
}
//<----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

FVector  APhantomPawn::From2To3SizeVector(FVector2D input)
{
	FVector output;
	output.X = input.X;
	output.Y = input.Y;
	output.Z = 0;
	return output;
}




//-------------------------------------------------------------ChoseNewDirection-----------------------------------------------------------------------------------------------
FVector APhantomPawn::ChoseNewDirection()
{
	

	//Definisco un vettore che conterrà le coppie direzione e distanza 
	TArray<APhantomPawn::DirectionAndDistance> DirectionAndDistanceVector ;
	
	FVector AllDir[] = { FVector(0,1,0),FVector(1,0,0), FVector(0,-1,0),FVector(-1,0,0) };
	float Min = std::numeric_limits<float>::max();// Prendo il massimo numero contenuto in un float 
	//DEBUG:
	//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("Minimo iniziale che dovrebbe essere il massimo contenibile in un float =%f  "),Min));
	for (FVector PossibleDir : AllDir) {
		//Escludo la direzione opposta all'attuale del fantasma (visto che non possono fare dietrofront )
		if ( PossibleDir != CurrentDirection * (-1))
		{
			//Prendo il prossimo nodo in quella direzione 
			auto Node = GameMode->GField->GetNextNode(CurrentGridCoords, PossibleDir);
			//Verifico che il nodo sia walkable 
			if (GameMode->GField->IsNodeValidForWalk(Node))
			{
			  //Calcolo la distanza fra il nodo e WhereImGoing 
				const float Dist = FMath::Abs(FVector::Dist2D(From2To3SizeVector(Node->GetGridPosition()), From2To3SizeVector(GetWhereImGoing()))); 
			//Mi calcolo il minimo fra le distanze 
				if (Dist < Min) 
				{
					Min = Dist;
				}
			
				//Aggiung0 l'istanza di struct al vector 
				APhantomPawn::DirectionAndDistance VAndF;
				VAndF.Direction = PossibleDir;
				VAndF.Distance = Dist;
				DirectionAndDistanceVector.Add(VAndF);
			}
		}
	}
	
	//Aggiungo a  CandidateDirectionVector le direzioni con Distanza uguale al minimo 
	TArray<FVector> CandidateDirectionVector;
	for (const auto& element : DirectionAndDistanceVector)
	{
		
		if (element.Distance == Min)
		{
			CandidateDirectionVector.Add(element.Direction);
		}
	}
	//Ora vado a scegliere la direzione fra quelle in CandidateDirectionVector dando la precedenza a su poi sinistra , giu , destra 
	for (const auto& element : CandidateDirectionVector) 
	{
		//Se è su
		if (element == FVector(1, 0, 0)) { return(element); }
	}
	for (const auto& element : CandidateDirectionVector)
	{
		//Se è sinistra
		if (element == FVector(0, -1, 0)) { return(element); }
	}
	for (const auto& element : CandidateDirectionVector)
	{
		//Se è giu
		if (element == FVector(-1, 0, 0)) { return(element); }
	}
	for (const auto& element : CandidateDirectionVector)
	{
		//Se è destra
		if (element == FVector(0, 1, 0)) { return(element); }
	}
	//DEBUG:
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("Qualcosa è andato storto nell'algoritmo di scelta della direzione del fansma:PhantoPawn riga ChoseNewDirection ")));
	return FVector(0, 0, 0);
	}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void APhantomPawn::SetNextNodeByDir(FVector InputDir)
{
	const FVector2D Coords = TargetNode ? TargetNode->GetGridPosition() : LastNode->GetGridPosition();//<-----------------------------------------------------------Cosa significa questa sintassi ?
	const auto Node = GameMode->GField->GetNextNode(Coords, InputDir);
	if (GameMode->GField->IsNodeValidForWalk(Node))
	{
		SetNextNode(Node);
		SetCurrentDirection(InputDir);
	}
}

FVector2D APhantomPawn::GetSpecialSpotPosition()
{
	return SpecialSpotPosition;
}

FVector2D APhantomPawn::GetWhereImGoing()
{
	return WhereImGoing;
}

FVector APhantomPawn::GetCurrentDirection() const
{
	return CurrentDirection;
}

FVector2D APhantomPawn::GetGridPosition() const
{
	return CurrentGridCoords;
}

void APhantomPawn::SetCurrentDirection(FVector Dir)
{
	if (Dir == FVector::ZeroVector) return;
	CurrentDirection = Dir;
}