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
	//Fa spawnare un actor all'interno del mondo , in questo caso Camera è la nostra telecamera
	//Setto dove posiziono la telecamera 
	/*float CameraPosX = ((GField->NodeSize * GField->MapSizeX) / 2) - (GField->NodeSize / 2);
	float CameraPosY = ((GField->NodeSize * GField->MapSizeY ) / 2) - (GField->NodeSize / 2);
	*/


	/*
		FVector CameraPos(100.0f, 100.0f, 1000.0f);

		Cam = GetWorld()->SpawnActor<ACamera>(CameraClass, CameraPos, FRotationMatrix::MakeFromX(FVector(0, 0, -1)).Rotator());*/

		//-------------------------------------------SPAWN DI PACMANPAN---------------------------------------------------------------------------------------------------



}