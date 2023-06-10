// Fill out your copyright notice in the Description page of Project Settings.


#include "BasePawn.h"
#include "MyGameMode.h"

//COSTRUTTORE di BasePawn
ABasePawn::ABasePawn()
{
	// N.B il pawn viene spawnato automaticamente nella posizione del player start
	// dato che il pawn di default è stato impostato nei setting come BP_GridPawn
	//  
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	//// Set this pawn to be controlled by the lowest-numbered player
	AutoPossessPlayer = EAutoReceiveInput::Player0;

	
	//// vettore di direzione inizializzato con zero (il pawn non si muove allo start del gioco fino a che non
	//   viene premuto uno dei tasti W-A-S-D )
	LastInputDirection = FVector(0, 0, 0);
	LastValidInputDirection = FVector(0, -1, 0);
	
	////posizione iniziale  del pawn nelle coordinate di griglia (5,14) la posizione su cui avverrà lo spawn
	CurrentGridCoords = FVector2D(5,14);   //Spawna corrttamente e si muove con qualsiasi vettore diverso da (0,0)
	//// nodi
	LastNode = nullptr;  //L'ultimo nodo in cui si è trovato il pawn
	TargetNode = nullptr; //Il nodo in cui il pawn sta per spostarsi 
	NextNode = nullptr; // Il prossimo nodo nella direzione in cui sta andando il Pawn
	
	
}

// Called when the game starts or when spawned
void ABasePawn::BeginPlay()
{
	Super::BeginPlay();
	GameMode = (AMyGameMode*)(GetWorld()->GetAuthGameMode());
	TheGridGen = GameMode->GField;
	//Get the GameInstance reference 
	GameInstance = GameMode->GameInstance;

	
	// posizione iniziale del pawn (è quella del PlayerStart)
	FVector2D StartNode = TheGridGen->GetXYPositionByRelativeLocation(GetActorLocation());
	LastNode = TheGridGen->TileMap[StartNode];
	//Setto le velocità di Pacman
	SetPacmanSpeeds();
	
}

// Called every frame
//Il Tick è un metodo che viene chiamato ad ogni frame e che noi sfrutteremo per far muovere gli oggetti nel campo nda gioco
void ABasePawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (!GameInstance->GetBlockAllPawn()) {

		
		//Chiamiamo la funzione che definiamo qua sotto e che serve a gestire il movimento e velocità dei Pawn nel campo da gioco
		ABasePawn::HandleMovement();
	}
}

//Setta il TargetNode sulla base del NextNode 
void ABasePawn::HandleMovement()
{
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
	// questo codice fa si che il pawn si muova autonomamente fino a che il next node è walkable

	if (!TargetNode && !NextNode)
	{
		//Se il prossimo nodo nella direzione inserita LastInputDirection è WALKABLE 
		if (TheGridGen->IsNodeValidForWalk(TheGridGen->GetNextNode(CurrentGridCoords, LastInputDirection)))
		{
			//Setto LastValidDirection = LastInputDirection 
			SetLastValidDirection(LastInputDirection);
		}
		//Se il prossimo nodo nella direzione data è WALKABLE diventa il TargetNode
		SetNodeGeneric(LastValidInputDirection);
	}
}
//--------------------------------MOVIMENTO DEL Pawn in una nuova casella -------------------------------------------------
void ABasePawn::MoveToCurrentTargetNode()
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
void ABasePawn::Eat() {
	if (TargetNode == nullptr) return;
	//Prendo il food legato al nodo
	//const auto Node = GameMode->GField->GetNextNode(Coords, InputDir);
	const auto TargetFood = GameMode->GField->GetFood(TargetNode);
	//Controllo se posso mangiarlo 
	//if (TheGridGen->IsNodeValidForWalk(TheGridGen->GetNextNode(CurrentGridCoords, LastInputDirection)))
	if (TheGridGen->IsEatable(TargetFood))
	{
		//Ora che so che il food è mangiabile posso contrassegnarlo come mangiato (Settando Eaten a true ) 
		//DEBUG:
		//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("FoodEatenPosition : X=%f  Y=%f"), TargetFood->GetFoodGridPosition().X, TargetFood->GetFoodGridPosition().Y));
		TargetFood->SetFoodEaten(true);

		//Decremento il contatore che tiene conto dei foodie e delle energyfood ancora in gioco 
		TargetFood->DecrementFoodieCounter();

		//Controllo che il valore del foodCounter
		if (GameInstance->GetFoodieCounter() == 0) 
		{
			//DEBUG:
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("Ho Vinto !!")));
		}
		//Sommo il punteggio del food mangiato al punteggio nella GameIstance
		
			//DEBUG:
		//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("PointsEarned : =%d "), TargetFood->GetPointsFromFood()));

        GameInstance->AddToScore(TargetFood->GetPointsFromFood());
		
		//Ora posso "rimuoverlo" dal GameField , in realtà lo nascondo sotto il GameField 
		TheGridGen->HideFood(TargetFood);

		//Setto come velocità di pacman la velocità che ha mentre mangia 
		CurrentMovementSpeed = PacmanEatSpeed;

		//Quando Pacman mangia qualcosa resetta il Timer
		GetWorld()->GetTimerManager().ClearTimer(GameMode->PacmanEatTimer);

			
	
		//------------------------------------------Se ci sono dei fantasmi nella casa incrementa i loro contatori ----------------------
		if (GameMode->Pinky->GetGhostMooveset()==ATHOUSE) 
		{
		//Incrementa il PinkyCounter
			GameMode->Pinky->IncrementGhostCounter();
			//DEBUG:
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("PinkyCounter= x=%d   "), GameMode->Pinky->GetGhostCounter()));
		}
		else if (GameMode->Inky->GetGhostMooveset() == ATHOUSE) 
		{
			GameMode->Inky->IncrementGhostCounter();
			//DEBUG:
			//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("InkyCounter= x=%d   "), GameMode->Inky->GetGhostCounter()));
		}
		else if (GameMode->Clyde->GetGhostMooveset() == ATHOUSE)
		{
			GameMode->Clyde->IncrementGhostCounter();
		}
		else { /*Non fa nulla*/ }
	}
	//Setto come velocità di pacman la velocità che ha normalmente
	else 
	{
		//Se almeno un fantasma è Frightened 
		if ((GameMode->Blinky->GetGhostStatus() == FRIGHTENED)|| (GameMode->Pinky->GetGhostStatus() == FRIGHTENED) || 
			(GameMode->Inky->GetGhostStatus() == FRIGHTENED) || (GameMode->Clyde->GetGhostStatus() == FRIGHTENED))
		{
			CurrentMovementSpeed = PacmanFrightSpeed;
		}
		//Altrimenti Pacman ha la sua velocità normale
		else 
		{
			CurrentMovementSpeed = NormalPacmanSpeed;
		}
	}

}

//-------------------------------------------------------------------------------------------------------------------------------------------------


//Quando il Pawn raggiunge il TargetNode
void ABasePawn::OnNodeReached()
{
	//DEBUG:
	//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("Pacman CurrentGridCoords: X=%f, Y=%f "), CurrentGridCoords.X, CurrentGridCoords.Y));
	
	//Mangia quello che si trova nel Node 
	Eat();

	//Aggiorna gli attributi del Pawn di conseguenza
	CurrentGridCoords = TargetNode->GetGridPosition();
	LastNode = TargetNode;
	SetTargetNode(nullptr);
	//---------------------------------------------------------Teleport Sinistro-------------------------------------------------------------------------------
	if (CurrentGridCoords == TheGridGen->GetXYPositionByRelativeLocation(TheGridGen->GetLeftTeleportLocation()) && LastValidInputDirection == FVector(0, -1, 0))
	{
		//Teletrasporto il pawn all'altro capo del teleport
		const FVector Location(GetActorLocation() + FVector(0, 27 * (TheGridGen->NodeSize), 0));
		SetActorLocation(Location);
		//Aggiorno gli attributi del Pawn di conseguenza
		CurrentGridCoords = CurrentGridCoords + FVector2D(0, 27);

	}
	//---------------------------------------------------------Teleport Destro-------------------------------------------------------------------------------
	if (CurrentGridCoords.X == 14 && CurrentGridCoords.Y == 27 && LastValidInputDirection == FVector(0, 1, 0))
	{
		//Teletrasporto il pawn all'altro capo del teleport
		const FVector Location(GetActorLocation() - FVector(0, 27 * (TheGridGen->NodeSize), 0));
		SetActorLocation(Location);
		//Aggiorno gli attributi del Pawn di conseguenza
		CurrentGridCoords = CurrentGridCoords - FVector2D(0, 27);

	}
}

void ABasePawn::SetTargetNode(ABaseNode* Node)
{
	TargetNode = Node;
}

void ABasePawn::SetNextNode(ABaseNode* Node)
{
	NextNode = Node;
}

//Setta in nodo con Il getNextNode se è WALKABLE
void ABasePawn::SetNodeGeneric(const FVector Dir)
{
	const auto Node = TheGridGen->GetNextNode(CurrentGridCoords, Dir);
	if (TheGridGen->IsNodeValidForWalk(Node))
	{
		SetTargetNode(Node);
	}
}

//La funzione che quando clicki su una tile ti stampa a schermo le coordinate  
void ABasePawn::OnClick()
{
	FHitResult Hit = FHitResult(ForceInit);
	GetWorld()->GetFirstPlayerController()->GetHitResultUnderCursor(ECollisionChannel::ECC_Pawn, true, Hit);
	if (ABaseNode* CurrTile = Cast<ABaseNode>(Hit.GetActor()))
	{
		FVector2D CurrCoords = CurrTile->GetGridPosition();
		//Stampa a schermo la posizione della tile su cui hai clickato 
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("Position x=%f  Y=%f "), CurrCoords.X, CurrCoords.Y));
	}

}

void ABasePawn::SetPacmanSpeeds()
{
	float StandardSpeed = GameInstance->GetStandardMovementSpeed();
	//La velocità di Pacman in condizioni normali 
		 NormalPacmanSpeed = StandardSpeed*(4.0f/5.0f);
	//La velocità di pacman mentre mangia 
	 PacmanEatSpeed = StandardSpeed * (71.0f / 100.0f);
//La velocità di pacman Quando i fantasmi sono in Fright status
   PacmanFrightSpeed = StandardSpeed * (9.0f / 10.0f);
}

void ABasePawn::SetVerticalInput(float AxisValue)
{
	if (AxisValue == 0) return;
	const FVector Dir = (GetActorForwardVector() * AxisValue).GetSafeNormal();
	LastInputDirection = Dir.GetSafeNormal();
	SetNextNodeByDir(LastInputDirection);
}

void ABasePawn::SetHorizontalInput(float AxisValue)
{
	if (AxisValue == 0) return;
	const FVector Dir = (GetActorRightVector() * AxisValue).GetSafeNormal();
	LastInputDirection = Dir;
	SetNextNodeByDir(LastInputDirection);
}

FVector2D ABasePawn::GetGridPosition()
{
	return CurrentGridCoords;
}

void ABasePawn::SetNextNodeByDir(FVector InputDir)
{
	const FVector2D Coords = TargetNode ? TargetNode->GetGridPosition() : LastNode->GetGridPosition();
	const auto Node = GameMode->GField->GetNextNode(Coords, InputDir);
	if (GameMode->GField->IsNodeValidForWalk(Node))
	{
		SetNextNode(Node);
		SetLastValidDirection(InputDir);
	}
}

FVector ABasePawn::GetLastValidDirection() const
{
	return LastValidInputDirection;
}

void ABasePawn::SetLastValidDirection(FVector Dir)
{
	if (Dir == FVector::ZeroVector) return;
	LastValidInputDirection = Dir;
}