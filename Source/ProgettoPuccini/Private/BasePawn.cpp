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
	CurrentGridCoords = FVector2D(5, 14);   //Spawna corrttamente e si muove con qualsiasi vettore diverso da (0,0)
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
	PacmanCurrentStatus = DEFAULT;

}

// Called every frame
//Il Tick è un metodo che viene chiamato ad ogni frame e che noi sfrutteremo per far muovere gli oggetti nel campo nda gioco
void ABasePawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (!GameInstance->GetBlockAllPawn()) {

		if (CallOnWin == true)
		{
			GameMode->OnWin();
			CallOnWin = false;
		}
		//Controllo il TimerPacmanEat se deve ripartire o va lasciato stare 
		CheckPacmanEatTimer();

		//Chiamiamo la funzione che definiamo qua sotto e che serve a gestire il movimento e velocità dei Pawn nel campo da gioco
		ABasePawn::HandleMovement();

	}
}

void ABasePawn::ResetPacmanNodes()
{
	LastNode = nullptr;
	TargetNode = nullptr;
	NextNode = nullptr;
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
	EatPhantom();
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
		//Setto come velocità di pacman la velocità che ha mentre mangia 
		CurrentMovementSpeed = PacmanEatSpeed;

		//Ora che so che il food è mangiabile posso contrassegnarlo come mangiato (Settando Eaten a true ) 
		//DEBUG:
		//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("FoodEatenPosition : X=%f  Y=%f"), TargetFood->GetFoodGridPosition().X, TargetFood->GetFoodGridPosition().Y));
		TargetFood->SetFoodEaten(true);



		//Decremento il contatore che tiene conto dei foodie e delle energyfood ancora in gioco 
		TargetFood->HandleFood();

		//Controllo che il valore del foodCounter
		int32 FoodieRemaining = GameInstance->GetFoodieCounter();
		if (FoodieRemaining == 0)
		{
			//DEBUG:
			//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("Ho Vinto !!")));
			CallOnWin = true;
		}
		if (FoodieRemaining == 169 || FoodieRemaining == 69)
		{
			//Il frutto appare a 70 foodie mangiati o a 170 :
		//Il foodie counter fa il conto alla rovescia da 239 quindi a 70 foodie mangiati sarà a 169 e poi 69 
			
			//Ricavo il frutto dalla location usando la FoodMap
			FVector2D FruitPosition = TheGridGen->GetXYPositionByRealLocation(TheGridGen->GetFruitLocation());
			ABaseNode* Node = TheGridGen->GetTileMAp()[FruitPosition];
			ABaseFood* Fruit = TheGridGen->GetFoodMAp()[Node];
			//Ora che ho trovato il frutto chiamo una funzione al suo interno che gestisce il suo restore ed il punteggio 
			Fruit->ShowFruit();
		}

		//Sommo il punteggio del food mangiato al punteggio nella GameIstance

			//DEBUG:
		//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("PointsEarned : =%d "), TargetFood->GetPointsFromFood()));

		GameInstance->AddToScore(TargetFood->GetPointsFromFood());

		//Ora posso "rimuoverlo" dal GameField , in realtà lo nascondo sotto il GameField 
		TheGridGen->HideFood(TargetFood);


		//Quando Pacman mangia qualcosa resetta il Timer e setto PacmanEatTimerEnd a true per segnalare alla tick di far ripartire il Timer 
		GetWorld()->GetTimerManager().ClearTimer(PacmanEatTimer);
		PacmanEatTimerEnd = true;



		//------------------------------------------Se ci sono dei fantasmi nella casa incrementa i loro contatori ----------------------
		if (GameMode->Pinky->GetGhostMooveset() == ATHOUSE)
		{
			//Incrementa il PinkyCounter
			GameMode->Pinky->IncrementGhostCounter();
			//DEBUG:
			//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("PinkyCounter= x=%d   "), GameMode->Pinky->GetGhostCounter()));
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
		if ((GameMode->Blinky->GetGhostStatus() == FRIGHTENED) || (GameMode->Pinky->GetGhostStatus() == FRIGHTENED) ||
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


//------------------------------------------------PACMAN Mangia i fantasmi Frightened---------------------------------------------------------------
void ABasePawn::EatPhantom() {

	FVector2D NodePosition = TargetNode->GetGridPosition();
	FVector2D BlinkyPosition = GameMode->Blinky->GetGridPosition();
	FVector2D PinkyPosition = GameMode->Pinky->GetGridPosition();
	FVector2D InkyPosition = GameMode->Inky->GetGridPosition();
	FVector2D ClydePosition = GameMode->Clyde->GetGridPosition();

	if (/*(NodePosition == BlinkyPosition && GameMode->Blinky->GetGhostStatus() == FRIGHTENED) ||*/ (CurrentGridCoords == BlinkyPosition && GameMode->Blinky->GetGhostStatus() == FRIGHTENED))
	{
		//DEBUG:
		//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("Blinky got eated ")));
		GameMode->Blinky->SetGhostNonBlue();
		GameMode->Blinky->SetGhostMooveset(GOTOHOUSE);
		//Setto il fantasma come incorporeal così non può essere mangiato più volte 
		GameMode->Blinky->SetGhostStatus(INCORPOREAL);
		GameMode->Blinky->SetCurrentMovementSpeed(GameMode->Blinky->GetIncorporealGhostSpeed());
		//Ogni volta che Pacman si pappa un fantasmino si guadagnano 200 punti .
		GameMode->GameInstance->AddToScore(200);


	}
	if (/*(NodePosition == PinkyPosition && GameMode->Pinky->GetGhostStatus() == FRIGHTENED) ||*/ (CurrentGridCoords == PinkyPosition && GameMode->Pinky->GetGhostStatus() == FRIGHTENED))
	{
		//DEBUG:
		//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("Pinky got eated")));
		GameMode->Pinky->SetGhostNonBlue();
		GameMode->Pinky->SetGhostMooveset(GOTOHOUSE);
		//Setto il fantasma come incorporeal così non può essere mangiato più volte 
		GameMode->Pinky->SetGhostStatus(INCORPOREAL);
		GameMode->Pinky->SetCurrentMovementSpeed(GameMode->Pinky->GetIncorporealGhostSpeed());
		//Ogni volta che Pacman si pappa un fantasmino si guadagnano 200 punti .
		GameMode->GameInstance->AddToScore(200);

	}
	if (/*(NodePosition == InkyPosition && GameMode->Inky->GetGhostStatus() == FRIGHTENED) ||*/ (CurrentGridCoords == InkyPosition && GameMode->Inky->GetGhostStatus() == FRIGHTENED))
	{
		//DEBUG:
		//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("Inky got eated")));
		GameMode->Inky->SetGhostNonBlue();
		GameMode->Inky->SetGhostMooveset(GOTOHOUSE);
		//Setto il fantasma come incorporeal così non può essere mangiato più volte 
		GameMode->Inky->SetGhostStatus(INCORPOREAL);
		GameMode->Inky->SetCurrentMovementSpeed(GameMode->Inky->GetIncorporealGhostSpeed());
		//Ogni volta che Pacman si pappa un fantasmino si guadagnano 200 punti .
		GameMode->GameInstance->AddToScore(200);

	}
	if (/*(NodePosition == ClydePosition && GameMode->Clyde->GetGhostStatus() == FRIGHTENED) ||*/ (CurrentGridCoords == ClydePosition && GameMode->Clyde->GetGhostStatus() == FRIGHTENED))
	{
		//DEBUG:
		//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("Clyde got eated")));
		GameMode->Clyde->SetGhostNonBlue();
		GameMode->Clyde->SetGhostMooveset(GOTOHOUSE);
		//Setto il fantasma come incorporeal così non può essere mangiato più volte 
		GameMode->Clyde->SetGhostStatus(INCORPOREAL);
		GameMode->Clyde->SetCurrentMovementSpeed(GameMode->Clyde->GetIncorporealGhostSpeed());
		//Ogni volta che Pacman si pappa un fantasmino si guadagnano 200 punti .
		GameMode->GameInstance->AddToScore(200);

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
	//Mi prendo la posizione sulla griglia dei due teleport 
	FVector2D LeftTeleportGridPosition = TheGridGen->GetXYPositionByRealLocation(TheGridGen->GetLeftTeleportLocation());
	FVector2D RightTeleportGridPosition = TheGridGen->GetXYPositionByRealLocation(TheGridGen->GetRightTeleportLocation());


	//Se pacman è al teleport sinisto e ci va contro  
	if (CurrentGridCoords == LeftTeleportGridPosition && LastValidInputDirection == FVector(0, -1, 0))
	{
		//Teletrasporto il pawn al teleport Destro
		const FVector Location(TheGridGen->GetRightTeleportLocation());
		SetActorLocation(Location);
		//Aggiorno gli attributi del Pawn di conseguenza
		CurrentGridCoords = RightTeleportGridPosition;

	}
	//---------------------------------------------------------Teleport Destro-------------------------------------------------------------------------------
	if (CurrentGridCoords == RightTeleportGridPosition && LastValidInputDirection == FVector(0, 1, 0))
	{
		//Teletrasporto il pawn all'altro capo del teleport
		const FVector Location(TheGridGen->GetLeftTeleportLocation());
		SetActorLocation(Location);
		//Aggiorno gli attributi del Pawn di conseguenza
		CurrentGridCoords = LeftTeleportGridPosition;

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

void ABasePawn::SetLastNode(ABaseNode* Node)
{
	LastNode = Node;
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



//Funzioni per Settare CurrentGridCoords di Pacman dall'esterno


void ABasePawn::SetCurrentGridCoords(FVector2D Location)
{
	CurrentGridCoords = Location;
}








void ABasePawn::CheckPacmanEatTimer()
{
	//Se il timer PacmanEat precedente si è concluso  
	if (PacmanEatTimerEnd == true)
	{
		//Setto PacmanEatTimerEnd a false e faccio partire il timer 
		PacmanEatTimerEnd = false;
		//PacmanEatTimer , Questo timer viene resettato e fatto ripartire ogni volta che Pacman mangia
		GetWorld()->GetTimerManager().SetTimer(PacmanEatTimer, [&]()
			{
				//<------------------------------------------------------------------------Il timer si deve risettare ogni volta che termina o viene interrotto 
				//DEBUG:
				//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("Se c'è un fantasma in casa lo faccio uscire ")));
				//SetGhostMooveset
				//Se Pinky è a casa la faccio uscire , altrimenti controllo se è a casa il prossimo fantasma 
				if (GameMode->Pinky->GetGhostMooveset() == ATHOUSE)
				{
					GameMode->Pinky->SetGhostMooveset(EXITHOUSE);
				}
				else if (GameMode->Inky->GetGhostMooveset() == ATHOUSE)
				{
					GameMode->Inky->SetGhostMooveset(EXITHOUSE);
				}
				else if (GameMode->Clyde->GetGhostMooveset() == ATHOUSE)
				{
					GameMode->Clyde->SetGhostMooveset(EXITHOUSE);
				}
				else {
					//DEBUG:
					//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("Non ci sono fantasmi in casa da far uscire ")));
				}
				PacmanEatTimerEnd = true;
			}, 4, false);
	}
}

void ABasePawn::SetPacmanSpeeds()
{
	int32 CurrentLevel = GameInstance->GetLevel();
	float StandardSpeed = GameInstance->GetStandardMovementSpeed();

	if (CurrentLevel == 1) {

		//La velocità di Pacman in condizioni normali 
		NormalPacmanSpeed = StandardSpeed * (80.0f / 100.0f);
		//La velocità di pacman mentre mangia 
		PacmanEatSpeed = StandardSpeed * (71.0f / 100.0f);
		//La velocità di pacman Quando i fantasmi sono in Fright status
		PacmanFrightSpeed = StandardSpeed * (90.0f / 100.0f);
	}
	else if (CurrentLevel > 1 && CurrentLevel < 5)
	{
		//La velocità di Pacman in condizioni normali 
		NormalPacmanSpeed = StandardSpeed * (90.0f / 100.0f);
		//La velocità di pacman mentre mangia 
		PacmanEatSpeed = StandardSpeed * (79.0f / 100.0f);
		//La velocità di pacman Quando i fantasmi sono in Fright status
		PacmanFrightSpeed = StandardSpeed * (95.0f / 100.0f);
	}
	else if (CurrentLevel > 4 && CurrentLevel < 21)
	{
		//La velocità di Pacman in condizioni normali 
		NormalPacmanSpeed = StandardSpeed;
		//La velocità di pacman mentre mangia 
		PacmanEatSpeed = StandardSpeed * (87.0f / 100.0f);
		//La velocità di pacman Quando i fantasmi sono in Fright status
		PacmanFrightSpeed = StandardSpeed;

	}
	else if (CurrentLevel > 20)
	{
		//La velocità di Pacman in condizioni normali 
		NormalPacmanSpeed = StandardSpeed * (90.0f / 100.0f);
		//La velocità di pacman mentre mangia 
		PacmanEatSpeed = StandardSpeed * (79.0f / 100.0f);
		//La velocità di pacman Quando i fantasmi sono in Fright status
		PacmanFrightSpeed = StandardSpeed * (100.0f / 100.0f);

	}
}

void ABasePawn::BeginEnergizedMode()
{
	//DEBUG:
	//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("Inizio EnergizedMode")));

	//Metto i Fantasmi in modalità Frightned 
	GameMode->Blinky->SetGhostStatus(FRIGHTENED);
	GameMode->Inky->SetGhostStatus(FRIGHTENED);
	GameMode->Pinky->SetGhostStatus(FRIGHTENED);
	GameMode->Clyde->SetGhostStatus(FRIGHTENED);

	if (GameMode->Blinky->GetGhostMooveset() == NORMAL)
	{
		GameMode->Blinky->SetGhostBlue();
	}

	if (GameMode->Pinky->GetGhostMooveset() == NORMAL)
	{
		GameMode->Pinky->SetGhostBlue();
	}
	if (GameMode->Inky->GetGhostMooveset() == NORMAL)
	{
		GameMode->Inky->SetGhostBlue();
	}
	if (GameMode->Clyde->GetGhostMooveset() == NORMAL)
	{
		GameMode->Clyde->SetGhostBlue();
	}
	//Cambio la staticMesh dei fantasmini :
	//GameMode->Blinky->StaticMeshComponent->SetStaticMesh(GameMode->FrightGhostClass);

	//Decido in base al livello quanto dura il Timer 
	int32 FrightTime = 1;
	int32 CurrentLevel = GameMode->GameInstance->GetLevel();
	if (CurrentLevel == 1)
	{
		FrightTime = 6;
	}
	else if (CurrentLevel == 2 || CurrentLevel == 6 || CurrentLevel == 10)
	{
		FrightTime = 5;
	}
	else if (CurrentLevel == 3)
	{
		FrightTime = 4;
	}
	else if (CurrentLevel == 4 || CurrentLevel == 14)
	{
		FrightTime = 3;
	}
	else if (CurrentLevel == 5 || CurrentLevel == 7 || CurrentLevel == 8 || CurrentLevel == 11)
	{
		FrightTime = 2;
	}
	else if (CurrentLevel == 9 || CurrentLevel == 12 || CurrentLevel == 13 || CurrentLevel == 15 || CurrentLevel == 16)
	{
		FrightTime = 1;
	}
	else
	{
		FrightTime = 0;
	}

	//Faccio partire il timer 
	GetWorld()->GetTimerManager().SetTimer(PacmanEnergyModeTimer, [&]()
		{
			//Quando termina il timer i fantasmi tornano allo status appropriato 
			//Se non sono più frightened perchè sono già stati mangiati non serve risettargli lo status 
			if (GameMode->Blinky->GetGhostStatus() == FRIGHTENED)
			{
				GameMode->Blinky->SetGhostNonBlue();

				if (GameMode->ChaseScatterPeriod == CHASE)
				{
					GameMode->Blinky->SetGhostStatus(CHASE);
				}
				else
					if (GameMode->ChaseScatterPeriod == SCATTER)
					{
						GameMode->Blinky->SetGhostStatus(SCATTER);
					}
					else
					{
						//DEBUG:
						GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("Qualcosa è andato storto !! Controlla il valore di ChaseScatterPeriod ")));
					}
			}
			if (GameMode->Pinky->GetGhostStatus() == FRIGHTENED)
			{
				GameMode->Pinky->SetGhostNonBlue();

				if (GameMode->ChaseScatterPeriod == CHASE)
				{
					GameMode->Pinky->SetGhostStatus(CHASE);
				}
				else
					if (GameMode->ChaseScatterPeriod == SCATTER)
					{
						GameMode->Pinky->SetGhostStatus(SCATTER);
					}
					else
					{
						//DEBUG:
						GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("Qualcosa è andato storto !! Controlla il valore di ChaseScatterPeriod ")));
					}
			}
			if (GameMode->Inky->GetGhostStatus() == FRIGHTENED)
			{
				GameMode->Inky->SetGhostNonBlue();

				if (GameMode->ChaseScatterPeriod == CHASE)
				{
					GameMode->Inky->SetGhostStatus(CHASE);
				}
				else
					if (GameMode->ChaseScatterPeriod == SCATTER)
					{
						GameMode->Inky->SetGhostStatus(SCATTER);
					}
					else
					{
						//DEBUG:
						GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("Qualcosa è andato storto !! Controlla il valore di ChaseScatterPeriod ")));
					}
			}
			if (GameMode->Clyde->GetGhostStatus() == FRIGHTENED)
			{
				GameMode->Clyde->SetGhostNonBlue();

				if (GameMode->ChaseScatterPeriod == CHASE)
				{
					GameMode->Clyde->SetGhostStatus(CHASE);
				}
				else
					if (GameMode->ChaseScatterPeriod == SCATTER)
					{
						GameMode->Clyde->SetGhostStatus(SCATTER);
					}
					else
					{
						//DEBUG:
						GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("Qualcosa è andato storto !! Controlla il valore di ChaseScatterPeriod ")));
					}
			}
			//DEBUG:
			//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("Fine EnergizedMode")));

		}, FrightTime, false);

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