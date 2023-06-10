// Fill out your copyright notice in the Description page of Project Settings.


#include "Pinky.h"

APinky::APinky()
{

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));

	//Sarà il nodo che il fantasma cercherà di raggiungere inseguendo Pacman , Cambia durante la partita (Sulla Griglia)
	WhereImGoing = FVector2D(13, 15);

	//La posizione del nodo che i fantasmini cercano di raggiungere quando sono in modalità scatter, è fissa , ma cambia da fantasma a fantasma (Sulla Griglia)
	SpecialSpotPosition = FVector2D(17, 15);
	//LastInputDirection = FVector(0, 0, 0);
	CurrentDirection = FVector(0, 0, 0);
	//posizione iniziale  del pawn nelle coordinate di griglia (ma non è importante )
	CurrentGridCoords = FVector2D(17, 13);
	//CurrentGridCoords = TheGridGen->GetXYPositionByRealLocation(TheGridGen->GetBlinkySpawn());


	PinkyCounter=1;
}

int32 APinky::GetGhostCounter()
{
	return PinkyCounter;
}

void APinky::IncrementGhostCounter()
{
	PinkyCounter = PinkyCounter + 1;
}

void APinky::ResetGhostCounter()
{
	PinkyCounter = 0;
}

//La funzione che definisce il comportamento individuale del fantasmino 
void APinky::WhereAmIGoingUpdate()
{
	if (GhostStatus == CHASE)
	{
		//In mod CHASE
		FVector2D PacmanGridPosition = CheckPacmanGridPosition();
		FVector PacmanDirection = CheckPacmanDirection();
		FVector2D PacmanDirection2D = FVector2D(PacmanDirection.X, PacmanDirection.Y);
		WhereImGoing = PacmanGridPosition + (PacmanDirection2D * 4);
		//DEBUG:
		//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("Pinky WhereAmIGoing : X=%f, Y=%f "), WhereImGoing.X, WhereImGoing.Y));
	}
	//In mod SCATTER
	else 
	{
		WhereImGoing = SpecialSpotPosition;
	}

	}

void APinky::LoadSpecialSpot()
{
	SpecialSpotPosition = TheGridGen->GetXYPositionByRealLocation(TheGridGen->GetPinkySpecialSpotLocation());
}
