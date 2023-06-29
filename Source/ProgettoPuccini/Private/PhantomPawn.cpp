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
	CurrentGridCoords = FVector2D(0, 0);
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
			Mooveset = EXITHOUSE;
		}
	}
}

void APhantomPawn::ResetAllGhostCounter()
{
	GameMode->Blinky->ResetGhostCounter();
	GameMode->Pinky->ResetGhostCounter();
	GameMode->Inky->ResetGhostCounter();
	GameMode->Clyde->ResetGhostCounter();
}



// Called when the game starts or when spawned
void APhantomPawn::BeginPlay()
{
	Super::BeginPlay();



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

	//Voglio un riferimento al BasePawn che è già in gioco e posseduto dal player 
	// La funzione GetActorOfClass prende il primo oggetto di quella classe già presente nel mondo 
	//AInteractableObject* InteractableObject = Cast<AInteractableObject>(UGameplayStatics::GetActorOfClass(World, AInteractableObject::StaticClass()));
	Pacman = GameMode->Pacman; //Cast<ABasePawn>(UGameplayStatics::GetActorOfClass(GetWorld(), ABasePawn::StaticClass()));

	TheGridGen = GameMode->GField;
	//Get the GameInstance reference 
	GameInstance = GameMode->GameInstance;

	FVector2D StartNode = TheGridGen->GetXYPositionByRelativeLocation(GetActorLocation());
	LastNode = TheGridGen->TileMap[StartNode];


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
	if (Status == SCATTER || Status == FRIGHTENED)
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


void APhantomPawn::SetGhostBlue()
{
	FRotator NewRotation = FRotator(180.0f, 0.0f, 0.0f);
	SetActorRotation(NewRotation);


}

void APhantomPawn::SetGhostNonBlue()
{
	FRotator NewRotation = FRotator(0.0f, 0.0f, 0.0f);
	SetActorRotation(NewRotation);
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
		//Mangia quello che si trova nel Node 
		Eat();

	}
}

UStaticMeshComponent* APhantomPawn::GetStaticMeshComponent() const
{
	return nullptr;
}

void APhantomPawn::SetStaticMeshComponent(UStaticMeshComponent* NewMesh)
{

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
		if (Mooveset == NORMAL && GhostStatus != FRIGHTENED)
		{
			if (CrossingDetection())
			{
				WhereAmIGoingUpdate();
				FVector NewDirection = ChoseNewDirection();
				SetCurrentDirection(NewDirection);

			}

			SetNodeGeneric(CurrentDirection);
		}
		//Parte che decide la direzione del fantasma quando è FRIGHTENED 
		else if (Mooveset == NORMAL && GhostStatus == FRIGHTENED)
		{
			//Controllo se è ad un incrocio 
			if (CrossingDetection())
			{
				//Non mi importa di WhereImGoing ma solo della direzione che verà scelta casualmente 
				FVector NewDirection = ChoseNewRandomDirection();
				SetCurrentDirection(NewDirection);
			}
			SetNodeGeneric(CurrentDirection);
		}
		//---------------------------------------------MOVIMENTO DEL FANTASMA PER FARLO TORNARE A CASA QUANDO VIENE MANGIATO --------------------------------
		else if (Mooveset == GOTOHOUSE)
		{

			//La tile sopra al cancello corrisponde a quella dove spawna Blinky
			FVector2D GateEntry = GameMode->GField->GetXYPositionByRealLocation(GameMode->GField->GetBlinkySpawn());

			if (CurrentGridCoords == GateEntry)
			{


				EnterHouse();

			}
			//Se è in giro per il labirinto e non a casa  cerca di tornare al GateEntry
			else if (InGhostHome == false) {
				if (CrossingDetection())
				{
					WhereImGoing = GateEntry;
					FVector NewDirection = ChoseNewDirection();
					SetCurrentDirection(NewDirection);

				}

				SetNodeGeneric(CurrentDirection);

			}
			else
			{
				if (CurrentGridCoords == FVector2D(GateEntry.X - 2, GateEntry.Y) || CurrentGridCoords == FVector2D(GateEntry.X - 1, GateEntry.Y))
				{
					//Il fantasmino deve solo scendere di due tile e poi è a casa 
					FVector DownDirection = FVector(-1, 0, 0);
					const auto Node = TheGridGen->GetNextNode(CurrentGridCoords, DownDirection);
					SetTargetNode(Node);


				}
				else
				{
					//Sono dentro la casa non mi resta che far tornare i fantasmini alla loro spawnPosition :

					GoToSpawnLocation();
				}

			}
		}



		//-----------------------------------------------------------MOVIMENTO DEL FANTASMA DENTRO LA CASA----------------------------------------------------
		else if (Mooveset == ATHOUSE)
		{

			float Down = TheGridGen->GetXYPositionByRealLocation(TheGridGen->GetPinkySpawn()).X;
			float Up = Down + 2;
			if (CurrentGridCoords.X == Down)
			{
				HouseDirection = FVector(1, 0, 0);
			}
			else if (CurrentGridCoords.X == Up)
			{
				HouseDirection = FVector(-1, 0, 0);
			}
			else
			{
				//Non fa nulla 
			}
			//DEBUG:
			//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("CurrentGridCoords: X=%f, Y=%f "), CurrentGridCoords.X, CurrentGridCoords.Y));
			SetNodeGeneric(HouseDirection);
		}
		//---------------------------------------------------------PARTE CHE FA USCIRE I FANTASMI DALLA CASA ------------------------------------------------------
		else
		{
			FVector2D ExitNodeLocation = GameMode->GField->GetXYPositionByRealLocation((GameMode->GField->GetExitNodeLocation()));
			//Se il fantasma è sull'exitNode 
			if (CurrentGridCoords == FVector2D(ExitNodeLocation.X + 2, ExitNodeLocation.Y))
			{
				//Sono uscito dal labirinto e quindi Setto il Mooveset a NORMAL
				Mooveset = NORMAL;
				InGhostHome = false;
			}
			else
				//Se il fantasma è nell'exitNode o nel GateNode 
				if (CurrentGridCoords == ExitNodeLocation || CurrentGridCoords == FVector2D(ExitNodeLocation.X + 1, ExitNodeLocation.Y))
				{
					//Vado su fino a che non sono fuori dal labirinto ignorando che GateNode sia notWalkable 
					FVector UpDirection = FVector(1, 0, 0);
					const auto Node = TheGridGen->GetNextNode(CurrentGridCoords, UpDirection);
					SetTargetNode(Node);


				}
			//Il comportamento Che assume in EXITHOUSE quando sta ancora vagando nella casa 
				else {
					//DEBUG:
					//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("Debug :Exit the House move")));
					if (CrossingDetection())
					{
						//Mando il fantasma all'exitNode fissando l'exitNode in WhereImGoing
						WhereImGoing = ExitNodeLocation;
						//DEBUG:
						//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("ExitNode: X=%f, Y=%f "), WhereImGoing.X, WhereImGoing.Y));
						//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("ExitNode location: X=%f, Y=%f Z=%f"), GameMode->GField->GetExitNodeLocation().X, GameMode->GField->GetExitNodeLocation().Y, GameMode->GField->GetExitNodeLocation().Z));
						FVector NewDirection = ChoseNewDirection();
						SetCurrentDirection(NewDirection);

					}
					SetNodeGeneric(CurrentDirection);
				}
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





//----------------------------------------------------Phantom EAT ---------------------------------------------------------------------------------
void APhantomPawn::Eat()
{

	if (TargetNode == nullptr) return;
	//Se i fantasmi sono in SCATTER o In CHASE Possono mangiare pacman
	if (GhostStatus == CHASE || GhostStatus == SCATTER)
	{
		//Prendo la posizione di Pacman 
		if (Pacman->GetGridPosition() == CurrentGridCoords)
		{
			//DEBUG:
			//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT(" Pacman e' stato mangiato ")));
			GameMode->GameInstance->DecrementCurrentLives();

			//Controllo il numero di righe vite di pacman per vedere se sono 0
			if (GameMode->GameInstance->GetCurrentLives() < 0)
			{
				//Decreto il GameOver
				//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT(" GAME-OVER ")));
				GameMode->OnGameOver();
			}
			else
			{
				//Decreto il GameOver
				//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("Pacman viene mangiato e perde una vita")));
				GameMode->OnPacmanLoseLife();
			}

		}
	}

}

void APhantomPawn::SetLastNode(ABaseNode* Node)
{
	LastNode = Node;
}

void APhantomPawn::ResetAllPhantomNodes()
{
	LastNode = nullptr;
	TargetNode = nullptr;
	NextNode = nullptr;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------





//Quando il Pawn raggiunge il TargetNode
void APhantomPawn::OnNodeReached()
{

	if (GetGhostStatus() == SCATTER || GetGhostStatus() == FRIGHTENED)
	{
		if (!DirectionInvercted)
		{
			//Inverto la direzione la prima volta che entro in un nodo in modalità scatter
			CurrentDirection = CurrentDirection * -1;
			//Setto direction invercted true in modo tale che non inverta la direzione ogni volta che raggiunge un nodo ma solo la prima volta 
			DirectionInvercted = true;
		}
	}

	//Aggiorna gli attributi del Pawn di conseguenza
	CurrentGridCoords = TargetNode->GetGridPosition();
	LastNode = TargetNode;
	SetTargetNode(nullptr);
	//--------------------------------------------------------Tunnel Node (riduci la velocità del fantasma)--------------------------------------------------

	GhostSpeedCheck();



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
		CurrentGridCoords = LeftTeleportGridPosition;

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

void APhantomPawn::GoToSpawnLocation()
{

}

void APhantomPawn::EnterHouse()
{
	InGhostHome = true;
	FVector DownDirection = FVector(-1, 0, 0);
	const auto Node = TheGridGen->GetNextNode(CurrentGridCoords, DownDirection);
	SetTargetNode(Node);
}

void APhantomPawn::SetCurrentMovementSpeed(float NewSpeed)
{
	CurrentMovementSpeed = NewSpeed;
}

float APhantomPawn::GetCurrentMovementSpeed()
{
	return CurrentMovementSpeed;
}

float APhantomPawn::GetIncorporealGhostSpeed()
{
	return IncorporealGhostSpeed;
}

float APhantomPawn::GetGhostSpeed() const
{
	return GhostSpeed;
}



void APhantomPawn::GhostSpeedCheck()
{
	bool IsInTheTunnel = false;
	//Controllo se il fantasma è in uno dei Tunnel Node
	for (const auto& element : TheGridGen->GetTunnelNodeLocations())
	{
		FVector2D TunnelNodeGridPosition = TheGridGen->GetXYPositionByRealLocation(element);
		if (CurrentGridCoords == TunnelNodeGridPosition)
		{
			IsInTheTunnel = true;
		}
	}
	if (GhostStatus == INCORPOREAL)
	{
		SetCurrentMovementSpeed(IncorporealGhostSpeed);
	}
	else
		if (IsInTheTunnel)
		{
			SetCurrentMovementSpeed(GhostTunnelSpeed);
		}
		else
		{
			if (GhostStatus == SCATTER || GhostStatus == CHASE)
			{
				SetCurrentMovementSpeed(GhostSpeed);
			}
			//Il fantasma è in Fright
			else if (GhostStatus == FRIGHTENED)
			{
				SetCurrentMovementSpeed(FrightGhostSpeed);
			}
			else
			{
				//DEBUG:
				GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("Errore : GhostSpeedCheck non riconosce il PhantomStatus")));
			}
		}
	//DEBUG:
	//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("CurrentGhostSpeed: %f "), CurrentMovementSpeed));
}


//-----------------------------------------------------------------CrossingDetection---------------------------------------------------------------------------------------------------------
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
	return false;
}
void APhantomPawn::SetSpeeds()
{
	//Dipendono dal livello <------------------------------------------------------------------------------------------DaModificare
	//Prendo la velocità standard dalla GameInstance e assegno le varie velocità
	float StandardSpeed = GameInstance->GetStandardMovementSpeed();
	int32 CurrentLevel = GameInstance->GetLevel();

	if (CurrentLevel == 1) {
		//DEBUG:
		//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("SetSpeeds: StandardSpeed=%f "), StandardSpeed));
		GhostTunnelSpeed = (4.0f / 10.0f) * StandardSpeed;
		GhostSpeed = (3.0f / 4.0f) * StandardSpeed;
		FrightGhostSpeed = (1.0f / 2.0f) * StandardSpeed;
		//DEBUG:
		//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("SetSpeeds: GhostTunnelSpeed=%f,  GhostSpeed=%f,  FrightGhostSpeed=%f "), GhostTunnelSpeed, GhostSpeed, FrightGhostSpeed));
	}
	else if (CurrentLevel > 1 && CurrentLevel < 5)
	{
		GhostTunnelSpeed = (45.0f / 100.0f) * StandardSpeed;
		GhostSpeed = (85.0f / 100.0f) * StandardSpeed;
		FrightGhostSpeed = (55.0f / 100.0f) * StandardSpeed;
	}
	else if (CurrentLevel > 4)
	{
		GhostTunnelSpeed = (50.0f / 100.0f) * StandardSpeed;
		GhostSpeed = (95.0f / 100.0f) * StandardSpeed;
		FrightGhostSpeed = (60.0f / 100.0f) * StandardSpeed;
	}


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
	TArray<APhantomPawn::DirectionAndDistance> DirectionAndDistanceVector;

	FVector AllDir[] = { FVector(0,1,0),FVector(1,0,0), FVector(0,-1,0),FVector(-1,0,0) };
	float Min = std::numeric_limits<float>::max();// Prendo il massimo numero contenuto in un float 
	//DEBUG:
	//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("Minimo iniziale che dovrebbe essere il massimo contenibile in un float =%f  "),Min));
	for (FVector PossibleDir : AllDir) {
		//Escludo la direzione opposta all'attuale del fantasma (visto che non possono fare dietrofront )
		if (PossibleDir != CurrentDirection * (-1))
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



FVector APhantomPawn::ChoseNewRandomDirection()
{
	FVector AllDir[] = { FVector(1,0,0),FVector(0,1,0), FVector(-1,0,0),FVector(0,-1,0) };
	//Genero un numero randomico da 0 a 3
	//UE_ARRAY_COUNT(AllDir)-1    Calcola la lunghezza dell'array
	const int32 Rand = FMath::RandRange(0, UE_ARRAY_COUNT(AllDir) - 1);
	//DEBUG:
	//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("Rand: X=%d "), Rand));
	int32 i = Rand;

	while (true)
	{
		FVector CandidateDirection = AllDir[i];
		//Escludo la direzione da cui sta venendo il fantasma
		if (CandidateDirection != CurrentDirection * (-1))
		{
			//Prendo il prossimo nodo in quella direzione
			auto Node = GameMode->GField->GetNextNode(CurrentGridCoords, CandidateDirection);

			//Controllo che il nodo non sia un muro
			if (GameMode->GField->IsNodeValidForWalk(Node))
			{
				return CandidateDirection;
			}
		}
		//incremento i in modulo 4
		i = (i + 1) % 4;
	}
}









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

void APhantomPawn::SetGridPosition(FVector2D Location)
{
	CurrentGridCoords = Location;
}

void APhantomPawn::SetCurrentDirection(FVector Dir)
{
	if (Dir == FVector::ZeroVector) return;
	CurrentDirection = Dir;
}