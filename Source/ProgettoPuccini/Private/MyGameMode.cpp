// Fill out your copyright notice in the Description page of Project Settings.

#include "MyGameMode.h"
#include "Blinky.h"
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
	
	//Prendo un riferimento alla MyGameIstance
		GameInstance = Cast<UMyGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));



	//-------------------------------------------SPAWN DELLA CAMERA--------------------------------------------------------------------------------------------------
	//La cameraComponent è dentro il Blueprint del GameField 

    //-------------------------------------------SPAWN DI PACMANPAN---------------------------------------------------------------------------------------------------
	//Avviene in automatico , essendo il Pawn posseduto dal player , la location di spawn è indicata nel PlayerStart
	//PlayerStart si trova in : PlaceActor->Basic->PlayerStart

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

	//Attendi un seccondo e poi sblocca tutti i pawn 
	//TIMER:
	FTimerHandle GameStartTimer;
	GetWorld()->GetTimerManager().SetTimer(GameStartTimer, [&]()
		{
			GameInstance->SetBlockAllPawn(false);
			GameStart();
		}, 2, false);

	//PacmanEatTimer , Questo timer viene resettato e fatto ripartire ogni volta che Pacman mangia
	GetWorld()->GetTimerManager().SetTimer(PacmanEatTimer, [&]()
		{
			//<------------------------------------------------------------------------Il timer si deve risettare ogni volta che termina o viene interrotto 
			//DEBUG:
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("Se c'è un fantasma in casa lo faccio uscire ")));
		}, 4, false);
}



void AMyGameMode::GameStart() 
{
	SetInitialBehavior();
	StartChaseScatterTimers();
	LoadGhostCounterLimit();
	//DEBUG:
	//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("ClydeCounterLimit= %d"), Clyde->GetGhostCounterLimit()));

	//ESPERIMENTO :
	//Dovrebbe sostituire l'asset di blinky con quello di pacman 
	/*
	static ConstructorHelpers::FObjectFinder<UStaticMesh>MeshAsset(TEXT("Blueprint'/Game/Blueprints/BP_BasePawn.BP_BasePawn'"));
	UStaticMesh* Asset = MeshAsset.Object;
	Blinky->StaticMeshComponent->SetStaticMesh(Asset);
	*/
	
}


void AMyGameMode::SetInitialBehavior() 
{
	Blinky->SetGhostMooveset(NORMAL);
	Pinky->SetGhostMooveset(ATHOUSE);
	Inky->SetGhostMooveset(ATHOUSE);
	Clyde->SetGhostMooveset(EXITHOUSE);
}

void AMyGameMode::LoadGhostCounterLimit()
{
	Blinky->SetGhostCounterLimit(-1);
	Pinky->SetGhostCounterLimit(-1);
	Inky->SetGhostCounterLimit(30);
	Clyde->SetGhostCounterLimit(60);
}

void AMyGameMode::StartChaseScatterTimers() 
{

	//Il gioco inizia con i fantasmi in scatter_1 :
											// DEBUG:
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT(" inizio di Scatter_1!")));
	Blinky->SetGhostStatus(SCATTER);
	Pinky->SetGhostStatus(SCATTER);
	Inky->SetGhostStatus(SCATTER);
	Clyde->SetGhostStatus(SCATTER);
	FTimerHandle Scatter_1;
	GetWorld()->GetTimerManager().SetTimer(Scatter_1, [&]()
		{
			//Al termine di Scatter_1 parte il timer Chase_1 
			// DEBUG:
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT(" Fine di Scatter_1 inizio di chase_1!")));
			Blinky->SetGhostStatus(CHASE);
			Pinky->SetGhostStatus(CHASE);
			Inky->SetGhostStatus(CHASE);
			Clyde->SetGhostStatus(CHASE);
	FTimerHandle Chase_1;
	GetWorld()->GetTimerManager().SetTimer(Chase_1, [&]()
		{

			//Al termine di chase_1 parte il timer scatter_2 
			// DEBUG:
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("Fine di chase_1 inizio di Scatter_2!")));
			Blinky->SetGhostStatus(SCATTER);
			Pinky->SetGhostStatus(SCATTER);
			Inky->SetGhostStatus(SCATTER);
			Clyde->SetGhostStatus(SCATTER);
			FTimerHandle Scatter_2;
			GetWorld()->GetTimerManager().SetTimer(Scatter_2, [&]()
				{

					//Al termine di scatter_2 parte il timer chase_2 
			// DEBUG:
					GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("Fine di scatter_1 inizio di chase_2!")));
					Blinky->SetGhostStatus(CHASE);
					Pinky->SetGhostStatus(CHASE);
					Inky->SetGhostStatus(CHASE);
					Clyde->SetGhostStatus(CHASE);
					FTimerHandle Chase_2;
					GetWorld()->GetTimerManager().SetTimer(Chase_2, [&]()
						{

							//Al termine di chase_2 parte il timer scatter_3
			// DEBUG:
							GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("Fine di chase_2 inizio di scatter_3!")));
							Blinky->SetGhostStatus(SCATTER);
							Pinky->SetGhostStatus(SCATTER);
							Inky->SetGhostStatus(SCATTER);
							Clyde->SetGhostStatus(SCATTER);
							FTimerHandle Scatter_3;
							GetWorld()->GetTimerManager().SetTimer(Scatter_3, [&]()
								{

									//Al termine di Scatter_3 parte il timer Chase_3
									// DEBUG:
									GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("Fine di  scatter_3 inizio di Chase_3!")));
									Blinky->SetGhostStatus(CHASE);
									Pinky->SetGhostStatus(CHASE);
									Inky->SetGhostStatus(CHASE);
									Clyde->SetGhostStatus(CHASE);
									FTimerHandle Chase_3;
									GetWorld()->GetTimerManager().SetTimer(Chase_3, [&]()
										{

											//Al termine di Chase_3 parte il timer Scatter_4
											// DEBUG:
											GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("Fine di  Chase_3 inizio di Scatter_4!")));
											Blinky->SetGhostStatus(SCATTER);
											Pinky->SetGhostStatus(SCATTER);
											Inky->SetGhostStatus(SCATTER);
											Clyde->SetGhostStatus(SCATTER);
											FTimerHandle Scatter_4;
											GetWorld()->GetTimerManager().SetTimer(Scatter_4, [&]()
												{

													//Al termine di Scatter_4 ho finito perchè Chase_4 ha durata illimitata 
												}, 5, false);
										}, 20, false);

								}, 5, false);
						}, 20, false);
				}, 7, false);
		}, 20, false);
		}, 7, false);
}


