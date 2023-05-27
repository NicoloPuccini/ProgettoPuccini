// Fill out your copyright notice in the Description page of Project Settings.
#include "Blinky.h"
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

	//-------------------------------------------SPAWN DELLA CAMERA--------------------------------------------------------------------------------------------------
//La camera Component è contenuta nel Blueprint del GameMode

		//-------------------------------------------SPAWN DI PACMANPAN---------------------------------------------------------------------------------------------------
//	Viene Spawnato in automatico essendo il Pawn posseduto dal Player , la sua posizione di spawn è dettata dal PlayerStart (PlaceActor-->Basic-->PlayerStart) 
	//-----------------------------------------------SPAWN DEI FANTASMI-----------------------------------------------------------------------------------------------------

	//Spawn di Blinky 
	// auto* Blinky = GetWorld()->SpawnActor<ABlinky>(FVector(), FRotator());
	//Blinky->SetActorLocationAndRotation(Blinky->SpawnPosition, FRotationMatrix::MakeFromX(FVector(0, 1, 0)).Rotator());
	
	//Node = GetWorld()->SpawnActor<ABaseNode>(ClassToSpawn, Position, FRotator::ZeroRotator);
	if (BlinkyClass != nullptr) {

		Blinky = GetWorld()->SpawnActor<ABlinky>(BlinkyClass, FVector(550, 1450, 5), FRotationMatrix::MakeFromX(FVector(0, 0, 0)).Rotator());
	}
	else
	{
		//Se il GameField non è stato creato è un errore e lo lanciamo così :
		UE_LOG(LogTemp, Error, TEXT("Blinky is null"));
	}
}