// Fill out your copyright notice in the Description page of Project Settings.


#include "Clyde.h"

AClyde::AClyde()
{

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));

	//Sarà il nodo che il fantasma cercherà di raggiungere inseguendo Pacman , Cambia durante la partita (Sulla Griglia)
	WhereImGoing = FVector2D(0,0);

	//La posizione del nodo che i fantasmini cercano di raggiungere quando sono in modalità scatter, è fissa , ma cambia da fantasma a fantasma (Sulla Griglia)
	SpecialSpotPosition = FVector2D(17, 15);
	//LastInputDirection = FVector(0, 0, 0);
	CurrentDirection = FVector(0, 0, 0);
	//posizione iniziale  del pawn nelle coordinate di griglia (ma non è importante )
	CurrentGridCoords = FVector2D(17, 13);
	//CurrentGridCoords = TheGridGen->GetXYPositionByRealLocation(TheGridGen->GetBlinkySpawn());

	ClydeCounter=0;
}

int32 AClyde::GetGhostCounter()
{
	return ClydeCounter;
}

void AClyde::IncrementGhostCounter()
{
	ClydeCounter = ClydeCounter + 1;
}

void AClyde::ResetGhostCounter()
{
	ClydeCounter = 0;
}


void AClyde::WhereAmIGoingUpdate()
{
	//In mod CHASE
	if (GhostStatus == CHASE)
	{
		
		//In mod CHASE 
		FVector2D PacmanGridPosition = CheckPacmanGridPosition();
		//Se la distanza tra pacman e clyde è maggiore di 8 nodi 
		if (FVector2D::Distance(CurrentGridCoords, PacmanGridPosition) > 8)
		{
			//Si comporta come Blinky
			WhereImGoing = PacmanGridPosition;
		}
		else
		{
			//Altrimenti si comporta come se fosse in Scatter
			WhereImGoing = SpecialSpotPosition;
		}
		//DEBUG:
		//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("Clyde WhereAmIGoing : X=%f, Y=%f "), WhereImGoing.X, WhereImGoing.Y));
	}
	//In mod Scatter
	else 
	{
		WhereImGoing = SpecialSpotPosition;
	}
	//In modalità Frightened il pawn non punta un punto specifico ma sceglie le direzioni casualmente 
}

void AClyde::LoadSpecialSpot()
{
	SpecialSpotPosition = TheGridGen->GetXYPositionByRealLocation(TheGridGen->GetClydeSpecialSpotLocation());
	//DEBUG:
	//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("Clyde SpecialSpot : X=%f, Y=%f "), SpecialSpotPosition.X, SpecialSpotPosition.Y));
}
