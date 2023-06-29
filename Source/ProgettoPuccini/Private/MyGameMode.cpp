// Fill out your copyright notice in the Description page of Project Settings.

#include "MyGameMode.h"
#include "MyPlayerController.h"
#include "EngineUtils.h"
#include "GameField.h"



AMyGameMode::AMyGameMode()
{

}


void AMyGameMode::BeginPlay()
{
	Super::BeginPlay();

	// con questa posizione la tile di coordinate griglia (0,0) ha come
	// coordinate nello spazio dello spigolo inferiore sinistro (0,0) 
	// 
	// es. coordinate nello spazio della tile di coordinate di griglia (0, 0) 
	// 
	// (100,0) -------------- (100,100)
	//         |            |
	//		   |            |
	//         |  (50, 50)  |
	//         |            |
	//         |            |
	//  (0,0)  -------------- (0, 100)
	//

	//Prendo un riferimento alla MyGameIstance
	GameInstance = Cast<UMyGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));

	//Voglio un riferimento al BasePawn che è già in gioco e posseduto dal player 
	// La funzione GetActorOfClass prende il primo oggetto di quella classe già presente nel mondo 
	//AInteractableObject* InteractableObject = Cast<AInteractableObject>(UGameplayStatics::GetActorOfClass(World, AInteractableObject::StaticClass()));
	Pacman = Cast<ABasePawn>(UGameplayStatics::GetActorOfClass(GetWorld(), ABasePawn::StaticClass()));




	//------------------------------------------------SPAWN del GameField ----------------------------------------------------------------------------------

	FVector GridPos(50.0f, 50.0f, 0.0f);
	// N.B. tutte le funzioni andrebbero normalizzate se la posizione
	// della griglia non fosse più questa ma una qualsiasi nello spazio tridimensionale
	if (GameFieldClass != nullptr)
	{
		// spawn della griglia
		GField = GetWorld()->SpawnActor<AGameField>(GameFieldClass, GridPos, FRotationMatrix::MakeFromX(FVector(0, 0, 0)).Rotator());
	}
	else
	{
		//Se il GameField non è stato creato è un errore e lo lanciamo così :
		UE_LOG(LogTemp, Error, TEXT("Game Field is null"));
	}




	//-------------------------------------------SPAWN DEI FANTASMI----------------------------------------------------------------------------------------------------

	if (BlinkyClass != nullptr)
	{
		// spawn di Blinky
		FVector BlinkySpawnPosition = GField->GetBlinkySpawn();
		Blinky = GetWorld()->SpawnActor<ABlinky>(BlinkyClass, BlinkySpawnPosition, FRotationMatrix::MakeFromX(FVector(0, 0, 0)).Rotator());
	}
	else
	{
		//Se il GameField non è stato creato è un errore e lo lanciamo così :
		UE_LOG(LogTemp, Error, TEXT("Blinky is null"));
	}
	if (PinkyClass != nullptr)
	{
		// spawn di Pinky
		FVector PinkySpawnPosition = GField->GetPinkySpawn();
		Pinky = GetWorld()->SpawnActor<APinky>(PinkyClass, PinkySpawnPosition, FRotationMatrix::MakeFromX(FVector(0, 0, 0)).Rotator());
	}
	else
	{
		//Se il GameField non è stato creato è un errore e lo lanciamo così :
		UE_LOG(LogTemp, Error, TEXT("Pinky is null"));
	}
	if (InkyClass != nullptr)
	{
		// spawn di Inky
		FVector InkySpawnPosition = GField->GetInkySpawn();
		Inky = GetWorld()->SpawnActor<AInky>(InkyClass, InkySpawnPosition, FRotationMatrix::MakeFromX(FVector(0, 0, 0)).Rotator());
	}
	else
	{
		//Se il GameField non è stato creato è un errore e lo lanciamo così :
		UE_LOG(LogTemp, Error, TEXT("Inky is null"));
	}
	if (ClydeClass != nullptr)
	{
		// spawn di Clyde
		FVector ClydeSpawnPosition = GField->GetClydeSpawn();
		Clyde = GetWorld()->SpawnActor<AClyde>(ClydeClass, ClydeSpawnPosition, FRotationMatrix::MakeFromX(FVector(0, 0, 0)).Rotator());
	}
	else
	{
		//Se il GameField non è stato creato è un errore e lo lanciamo così :
		UE_LOG(LogTemp, Error, TEXT("Clyde is null"));
	}




	//-------------------------------------------SPAWN DELLA CAMERA--------------------------------------------------------------------------------------------------
	//La cameraComponent è dentro il Blueprint del GameField 

	//-------------------------------------------SPAWN DI PACMANPAN---------------------------------------------------------------------------------------------------
	//Avviene in automatico , essendo il Pawn posseduto dal player , la location di spawn è indicata nel PlayerStart
	//PlayerStart si trova in : PlaceActor->Basic->PlayerStart



	//------------------------------------------FACCIAMO INIZIARE IL GIOCO ---------------------------------------------------------------------------------
	//Setto le velocità dei fantasmi nelle varie condizioni (in base al livello )

	//Carico negli attributi alcune informazioni come le varie velocità che i pawn possono usare 
	LoadNewLevel();

	//Game start è la funzione che fa iniziare il gioco sia che pacman sia stato mangiato , sia ad inizio partita 
	GameStart();
}



void AMyGameMode::GameStart()
{

	Blinky->SetCurrentMovementSpeed(Blinky->GetGhostSpeed());
	Pinky->SetCurrentMovementSpeed(Pinky->GetGhostSpeed());
	Inky->SetCurrentMovementSpeed(Inky->GetGhostSpeed());
	Clyde->SetCurrentMovementSpeed(Clyde->GetGhostSpeed());

	//READY :
	GameInstance->SetTurnMessage("READY");

	//Attendi 2 second1 e poi sblocca tutti i pawn 
	//TIMER:
	FTimerHandle GameStartTimer;
	GetWorld()->GetTimerManager().SetTimer(GameStartTimer, [&]()
		{
			GameInstance->SetBlockAllPawn(false);
			GameInstance->ClearTurnMessage();
		}, 2, false);

	//Setto a tutti i ghost status SCATTER , (Per evitare il Bug del farsi mangiare mentre un fantasma è in status Incorporeal ) 
	Blinky->SetGhostStatus(SCATTER);
	Pinky->SetGhostStatus(SCATTER);
	Inky->SetGhostStatus(SCATTER);
	Clyde->SetGhostStatus(SCATTER);


	SetInitialBehavior();
	LoadChaseScatterTimes();
	StartChaseScatterTimers();


	//DEBUG:
	//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("ClydeCounterLimit= %d"), Clyde->GetGhostCounterLimit()));

	//ESPERIMENTO :
	//Dovrebbe sostituire l'asset di blinky con quello di blinky2
	/*
	static ConstructorHelpers::FObjectFinder<UStaticMesh>MeshAsset(TEXT("Blueprint'/Game/Blueprints/BP_BasePawn.BP_BasePawn'"));
	UStaticMesh* Asset = MeshAsset.Object;
	Blinky->StaticMeshComponent->SetStaticMesh(Asset);
	*/

	//Tentativo 2 :
	/*
	UStaticMeshComponent* ClydeMeshComponent = Clyde->GetStaticMeshComponent();
	UStaticMeshComponent* BlinkyMeshComponent = Blinky->GetStaticMeshComponent();

	Blinky->SetStaticMeshComponent(ClydeMeshComponent);
	*/

	//Tentativo 3 :
	/*

	// Spawn di Blinky
ABlinky* Blinky = GetWorld()->SpawnActor<ABlinky>(BlinkyClass, BlinkySpawnPosition, FRotationMatrix::MakeFromX(FVector(0, 0, 0)).Rotator());

// Creazione di un nuovo componente StaticMeshComponent per Clyde
UStaticMeshComponent* ClydeMeshComponent = NewObject<UStaticMeshComponent>(Blinky);
ClydeMeshComponent->RegisterComponent();

// Impostazione della mesh di Clyde sul componente
ClydeMeshComponent->SetStaticMesh(ClydeMesh);
ClydeMeshComponent->AttachToComponent(Blinky->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);

// Rimozione del componente StaticMeshComponent preesistente
Blinky->GetStaticMeshComponent()->DestroyComponent();

// Assegnazione del nuovo componente StaticMeshComponent
Blinky->SetRootComponent(ClydeMeshComponent);
*/



}

void AMyGameMode::ReturnAllPawnToSpawnLocations()
{
	//Riporto pacman e i fantasmi alla loro SpawnPosition
	Pacman->SetActorLocation(GField->GetPacmanSpawn());
	Pacman->SetCurrentGridCoords(GField->GetXYPositionByRealLocation(GField->GetPacmanSpawn()));
	//Pulisco tutti gli attributi nodo 
	Pacman->ResetPacmanNodes();
	//Risetto il lastNode
	ABaseNode* Node = GField->TileMap[GField->GetXYPositionByRealLocation(GField->GetPacmanSpawn())];
	Pacman->SetLastNode(Node);



	Blinky->SetActorLocation(GField->GetBlinkySpawn());
	Blinky->SetGhostNonBlue();
	Blinky->SetGridPosition(GField->GetXYPositionByRealLocation(GField->GetBlinkySpawn()));
	//Pulisco tutti gli attributi nodo 
	Blinky->ResetAllPhantomNodes();
	//Risetto il lastNode
	Node = GField->TileMap[GField->GetXYPositionByRealLocation(GField->GetBlinkySpawn())];
	Blinky->SetLastNode(Node);


	Inky->SetActorLocation(GField->GetInkySpawn());
	Inky->SetGhostNonBlue();
	Inky->SetGridPosition(GField->GetXYPositionByRealLocation(GField->GetInkySpawn()));
	//Pulisco tutti gli attributi nodo 
	Inky->ResetAllPhantomNodes();
	//Risetto il lastNode
	Node = GField->TileMap[GField->GetXYPositionByRealLocation(GField->GetInkySpawn())];
	Inky->SetLastNode(Node);

	Pinky->SetActorLocation(GField->GetPinkySpawn());
	Pinky->SetGhostNonBlue();
	Pinky->SetGridPosition(GField->GetXYPositionByRealLocation(GField->GetPinkySpawn()));
	//Pulisco tutti gli attributi nodo 
	Pinky->ResetAllPhantomNodes();
	//Risetto il lastNode
	Node = GField->TileMap[GField->GetXYPositionByRealLocation(GField->GetPinkySpawn())];
	Pinky->SetLastNode(Node);

	Clyde->SetActorLocation(GField->GetClydeSpawn());
	Clyde->SetGhostNonBlue();
	Clyde->SetGridPosition(GField->GetXYPositionByRealLocation(GField->GetClydeSpawn()));
	//Pulisco tutti gli attributi nodo 
	Clyde->ResetAllPhantomNodes();
	//Risetto il lastNode
	Node = GField->TileMap[GField->GetXYPositionByRealLocation(GField->GetClydeSpawn())];
	Clyde->SetLastNode(Node);

	GameInstance->SetBlockAllPawn(true);
}


void AMyGameMode::SetInitialBehavior()
{
	Blinky->SetGhostMooveset(NORMAL);
	Pinky->SetGhostMooveset(ATHOUSE);
	Inky->SetGhostMooveset(ATHOUSE);
	Clyde->SetGhostMooveset(ATHOUSE);
}

void AMyGameMode::LoadInitialGhostCounterLimit()
{
	Blinky->SetGhostCounterLimit(-1);
	Pinky->SetGhostCounterLimit(0);
	Inky->SetGhostCounterLimit(30);
	Clyde->SetGhostCounterLimit(60);
}

void AMyGameMode::LoadChaseScatterTimes()
{//Definisco la durata di scatter e chase in base al livello :
	int32 CurrentLevel = GameInstance->GetLevel();



	if (CurrentLevel == 1)
	{
		Scatter_1Time = 7;
		Scatter_2Time = 7;
		Scatter_3Time = 5;
		Scatter_4Time = 5;
		Chase_1Time = 20;
		Chase_2Time = 20;
		Chase_3Time = 20;
	}
	else if (CurrentLevel > 1 && CurrentLevel < 5)
	{
		Scatter_1Time = 7;
		Scatter_2Time = 7;
		Scatter_3Time = 5;
		Scatter_4Time = 0;
		Chase_1Time = 20;
		Chase_2Time = 20;
		Chase_3Time = 1033;
	}
	else if (CurrentLevel > 4)
	{
		Scatter_1Time = 5;
		Scatter_2Time = 5;
		Scatter_3Time = 5;
		Scatter_4Time = 0;
		Chase_1Time = 20;
		Chase_2Time = 20;
		Chase_3Time = 1037;
	}

}

void AMyGameMode::LoadGhostCounterLimit()
{
	Blinky->SetGhostCounterLimit(-1);
	Pinky->SetGhostCounterLimit(7);
	Inky->SetGhostCounterLimit(10);
	Clyde->SetGhostCounterLimit(22);
}





//Viene chiamata quando Pacman ha mangiato tutti i pallini 
void AMyGameMode::OnWin()
{
	//DEBUG:
	//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("OnWin chiamata !!!!!!!!!!")));


	//Faccio salire il livello 
	GameInstance->IncrementLevel();



	//Riporto i pawn alle condizioni iniziali :
	ReturnAllPawnToSpawnLocations();
	//Faccio il restore dei punti già mangiati e reset del foodieCounter 
	GameInstance->ResetFoodieCounter();
	Blinky->ResetGhostCounter();
	Pinky->ResetGhostCounter();
	Inky->ResetGhostCounter();
	Clyde->ResetGhostCounter();

	//Restore dei foodie mangiati 
	GField->RestoreAllEatenFood();




	LoadNewLevel();

	//Azzero i Timers Chase scatter
	ClearAllChaseScatterTimers();


	//Clear del timer dell'energyMode;
	GetWorld()->GetTimerManager().ClearTimer(Pacman->PacmanEnergyModeTimer);
	//Clear di pacmanEatTimer
	GetWorld()->GetTimerManager().ClearTimer(Pacman->PacmanEatTimer);
	Pacman->PacmanEatTimerEnd = true;

	GameStart();


}

//Viene chiamata quando Pacman finisce le vite 
void AMyGameMode::OnGameOver()
{
	GameInstance->SetBlockAllPawn(true);
	GameInstance->SetTurnMessage("GAME OVER");

	FTimerHandle GameOverTimer;
	GetWorld()->GetTimerManager().SetTimer(GameOverTimer, [&]()
		{
			GameInstance->SetBlockAllPawn(false);
			GameInstance->ClearTurnMessage();


			//Riporto a zero lo score , il giocatore perde tutti i punti 
			GameInstance->ResetScore();
			//Riporto le vite a quelle standard ad inizio gioco 
			GameInstance->ResetLives();
			//Il livello torna a 0
			GameInstance->ResetLevel();

			//Riporto i pawn alle condizioni iniziali :
			ReturnAllPawnToSpawnLocations();
			//Faccio il restore dei punti già mangiati e reset del foodieCounter 
			GameInstance->ResetFoodieCounter();
			Blinky->ResetGhostCounter();
			Pinky->ResetGhostCounter();
			Inky->ResetGhostCounter();
			Clyde->ResetGhostCounter();

			//Restore dei foodie mangiati 
			GField->RestoreAllEatenFood();




			LoadNewLevel();
			//Clear del timer dell'energyMode;
			GetWorld()->GetTimerManager().ClearTimer(Pacman->PacmanEnergyModeTimer);
			//Azzero i Timers Chase scatter
			ClearAllChaseScatterTimers();

			//Clear di pacmanEatTimer
			GetWorld()->GetTimerManager().ClearTimer(Pacman->PacmanEatTimer);
			Pacman->PacmanEatTimerEnd = true;

			GameStart();
		}, 2, false);




}

//Viene chiamata quando pacman viene mangiato da un fantasma 
void AMyGameMode::OnPacmanLoseLife()
{

	ReturnAllPawnToSpawnLocations();
	Blinky->ResetGhostCounter();
	Pinky->ResetGhostCounter();
	Inky->ResetGhostCounter();
	Clyde->ResetGhostCounter();
	//Cambio i ghost counter limit (sono diversi da quelli che ci sono ad inizio partita )
	LoadGhostCounterLimit();

	//Azzero i Timers Chase scatter
	ClearAllChaseScatterTimers();
	//Clear del timer dell'energyMode;
	GetWorld()->GetTimerManager().ClearTimer(Pacman->PacmanEnergyModeTimer);
	//Clear di pacmanEatTimer
	GetWorld()->GetTimerManager().ClearTimer(Pacman->PacmanEatTimer);
	Pacman->PacmanEatTimerEnd = true;

	//Faccio iniziare il gioco
	GameStart();

}

void AMyGameMode::LoadNewLevel()
{

	Blinky->SetSpeeds();
	Inky->SetSpeeds();
	Pinky->SetSpeeds();
	Clyde->SetSpeeds();

	LoadInitialGhostCounterLimit();
}





void AMyGameMode::StartChaseScatterTimers()
{


	//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT(" tempi di scatter chase :  1=%d 2=%d 3=%d 4=%d 1=%d 2=%d 3=%d"), Scatter_1Time, Scatter_2Time, Scatter_3Time,
	//	Scatter_4Time, Chase_1Time, Chase_2Time, Chase_3Time));

	//Il gioco inizia con i fantasmi in scatter_1 :
											// DEBUG:
	//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT(" inizio di Scatter_1!")));

	ChaseScatterPeriod = SCATTER;

	if (Blinky->GetGhostStatus() != FRIGHTENED && Blinky->GetGhostStatus() != INCORPOREAL) {
		Blinky->SetGhostStatus(SCATTER);
	}
	if (Pinky->GetGhostStatus() != FRIGHTENED && Pinky->GetGhostStatus() != INCORPOREAL) {
		Pinky->SetGhostStatus(SCATTER);
	}
	if (Inky->GetGhostStatus() != FRIGHTENED && Inky->GetGhostStatus() != INCORPOREAL) {
		Inky->SetGhostStatus(SCATTER);
	}
	if (Clyde->GetGhostStatus() != FRIGHTENED && Clyde->GetGhostStatus() != INCORPOREAL) {
		Clyde->SetGhostStatus(SCATTER);
	}

	GetWorld()->GetTimerManager().SetTimer(Scatter_1, [&]()
		{
			//Al termine di Scatter_1 parte il timer Chase_1 
			// DEBUG:
			//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT(" Fine di Scatter_1 inizio di chase_1!")));

			ChaseScatterPeriod = CHASE;

			if (Blinky->GetGhostStatus() != FRIGHTENED && Blinky->GetGhostStatus() != INCORPOREAL) {
				Blinky->SetGhostStatus(CHASE);
			}
			if (Pinky->GetGhostStatus() != FRIGHTENED && Pinky->GetGhostStatus() != INCORPOREAL) {
				Pinky->SetGhostStatus(CHASE);
			}
			if (Inky->GetGhostStatus() != FRIGHTENED && Inky->GetGhostStatus() != INCORPOREAL) {
				Inky->SetGhostStatus(CHASE);
			}
			if (Clyde->GetGhostStatus() != FRIGHTENED && Clyde->GetGhostStatus() != INCORPOREAL) {
				Clyde->SetGhostStatus(CHASE);
			}

			GetWorld()->GetTimerManager().SetTimer(Chase_1, [&]()
				{
					//Al termine di chase_1 parte il timer scatter_2 
					// DEBUG:
					//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("Fine di chase_1 inizio di Scatter_2!")));

					ChaseScatterPeriod = SCATTER;

					if (Blinky->GetGhostStatus() != FRIGHTENED && Blinky->GetGhostStatus() != INCORPOREAL) {
						Blinky->SetGhostStatus(SCATTER);
					}
					if (Pinky->GetGhostStatus() != FRIGHTENED && Pinky->GetGhostStatus() != INCORPOREAL) {
						Pinky->SetGhostStatus(SCATTER);
					}
					if (Inky->GetGhostStatus() != FRIGHTENED && Inky->GetGhostStatus() != INCORPOREAL) {
						Inky->SetGhostStatus(SCATTER);
					}
					if (Clyde->GetGhostStatus() != FRIGHTENED && Clyde->GetGhostStatus() != INCORPOREAL) {
						Clyde->SetGhostStatus(SCATTER);
					}

					GetWorld()->GetTimerManager().SetTimer(Scatter_2, [&]()
						{

							//Al termine di scatter_2 parte il timer chase_2 
					// DEBUG:
							//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("Fine di scatter_1 inizio di chase_2!")));

							ChaseScatterPeriod = CHASE;

							if (Blinky->GetGhostStatus() != FRIGHTENED && Blinky->GetGhostStatus() != INCORPOREAL) {
								Blinky->SetGhostStatus(CHASE);
							}
							if (Pinky->GetGhostStatus() != FRIGHTENED && Pinky->GetGhostStatus() != INCORPOREAL) {
								Pinky->SetGhostStatus(CHASE);
							}
							if (Inky->GetGhostStatus() != FRIGHTENED && Inky->GetGhostStatus() != INCORPOREAL) {
								Inky->SetGhostStatus(CHASE);
							}
							if (Clyde->GetGhostStatus() != FRIGHTENED && Clyde->GetGhostStatus() != INCORPOREAL) {
								Clyde->SetGhostStatus(CHASE);
							}

							GetWorld()->GetTimerManager().SetTimer(Chase_2, [&]()
								{

									//Al termine di chase_2 parte il timer scatter_3
					// DEBUG:
									//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("Fine di chase_2 inizio di scatter_3!")));

									ChaseScatterPeriod = SCATTER;

									if (Blinky->GetGhostStatus() != FRIGHTENED && Blinky->GetGhostStatus() != INCORPOREAL) {
										Blinky->SetGhostStatus(SCATTER);
									}
									if (Pinky->GetGhostStatus() != FRIGHTENED && Pinky->GetGhostStatus() != INCORPOREAL) {
										Pinky->SetGhostStatus(SCATTER);
									}
									if (Inky->GetGhostStatus() != FRIGHTENED && Inky->GetGhostStatus() != INCORPOREAL) {
										Inky->SetGhostStatus(SCATTER);
									}
									if (Clyde->GetGhostStatus() != FRIGHTENED && Clyde->GetGhostStatus() != INCORPOREAL) {
										Clyde->SetGhostStatus(SCATTER);
									}

									GetWorld()->GetTimerManager().SetTimer(Scatter_3, [&]()
										{

											//Al termine di Scatter_3 parte il timer Chase_3
											// DEBUG:
											//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("Fine di  scatter_3 inizio di Chase_3!")));

											ChaseScatterPeriod = CHASE;

											if (Blinky->GetGhostStatus() != FRIGHTENED && Blinky->GetGhostStatus() != INCORPOREAL) {
												Blinky->SetGhostStatus(CHASE);
											}
											if (Pinky->GetGhostStatus() != FRIGHTENED && Pinky->GetGhostStatus() != INCORPOREAL) {
												Pinky->SetGhostStatus(CHASE);
											}
											if (Inky->GetGhostStatus() != FRIGHTENED && Inky->GetGhostStatus() != INCORPOREAL) {
												Inky->SetGhostStatus(CHASE);
											}
											if (Clyde->GetGhostStatus() != FRIGHTENED && Clyde->GetGhostStatus() != INCORPOREAL) {
												Clyde->SetGhostStatus(CHASE);
											}

											GetWorld()->GetTimerManager().SetTimer(Chase_3, [&]()
												{

													//Al termine di Chase_3 parte il timer Scatter_4
													// DEBUG:
													//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("Fine di  Chase_3 inizio di Scatter_4!")));

													ChaseScatterPeriod = SCATTER;

													if (Blinky->GetGhostStatus() != FRIGHTENED && Blinky->GetGhostStatus() != INCORPOREAL) {
														Blinky->SetGhostStatus(SCATTER);
													}
													if (Pinky->GetGhostStatus() != FRIGHTENED && Pinky->GetGhostStatus() != INCORPOREAL) {
														Pinky->SetGhostStatus(SCATTER);
													}
													if (Inky->GetGhostStatus() != FRIGHTENED && Inky->GetGhostStatus() != INCORPOREAL) {
														Inky->SetGhostStatus(SCATTER);
													}
													if (Clyde->GetGhostStatus() != FRIGHTENED && Clyde->GetGhostStatus() != INCORPOREAL) {
														Clyde->SetGhostStatus(SCATTER);
													}

													GetWorld()->GetTimerManager().SetTimer(Scatter_4, [&]()
														{
															//Al termine di Scatter_4 ho finito perchè Chase_4 ha durata illimitata 

															ChaseScatterPeriod = CHASE;

															if (Blinky->GetGhostStatus() != FRIGHTENED && Blinky->GetGhostStatus() != INCORPOREAL) {
																Blinky->SetGhostStatus(CHASE);
															}
															if (Pinky->GetGhostStatus() != FRIGHTENED && Pinky->GetGhostStatus() != INCORPOREAL) {
																Pinky->SetGhostStatus(CHASE);
															}
															if (Inky->GetGhostStatus() != FRIGHTENED && Inky->GetGhostStatus() != INCORPOREAL) {
																Inky->SetGhostStatus(CHASE);
															}
															if (Clyde->GetGhostStatus() != FRIGHTENED && Clyde->GetGhostStatus() != INCORPOREAL) {
																Clyde->SetGhostStatus(CHASE);
															}

														}, Scatter_4Time, false);
												}, Chase_3Time, false);

										}, Scatter_3Time, false);
								}, Chase_2Time, false);
						}, Scatter_2Time, false);
				}, Chase_1Time, false);
		}, Scatter_1Time, false);
}

/*
		 Scatter_1Time = 7;
		 Scatter_2Time = 7;
		 Scatter_3Time = 5;
		 Scatter_4Time = 5;
		 Chase_1Time = 20;
		 Chase_2Time = 20;
		 Chase_3Time = 20;
*/
void AMyGameMode::ClearAllChaseScatterTimers()
{
	GetWorld()->GetTimerManager().ClearTimer(Scatter_1);
	GetWorld()->GetTimerManager().ClearTimer(Scatter_2);
	GetWorld()->GetTimerManager().ClearTimer(Scatter_3);
	GetWorld()->GetTimerManager().ClearTimer(Scatter_4);
	GetWorld()->GetTimerManager().ClearTimer(Chase_1);
	GetWorld()->GetTimerManager().ClearTimer(Chase_2);
	GetWorld()->GetTimerManager().ClearTimer(Chase_3);
}


