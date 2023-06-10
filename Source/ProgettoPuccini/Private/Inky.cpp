// Fill out your copyright notice in the Description page of Project Settings.


#include "Inky.h"

AInky::AInky()
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

	InkyCounter;
}

int32 AInky::GetGhostCounter()
{
	return InkyCounter;
}

void AInky::IncrementGhostCounter()
{
	InkyCounter = InkyCounter + 1;
}

void AInky::ResetGhostCounter()
{
	InkyCounter = 0;
}


void AInky::WhereAmIGoingUpdate()
{
	if (GhostStatus == CHASE)
	{
		//In mod CHASE
		FVector2D PacmanGridPosition = CheckPacmanGridPosition();
		FVector PacmanDirection = CheckPacmanDirection();
		FVector2D PacmanDirection2D = FVector2D(PacmanDirection.X, PacmanDirection.Y);
		//Calcolo il Green spot , che è due nodi davanti alla direzione di pacman  dalla posizione attuale di pacman
		FVector2D GreenSpot = PacmanGridPosition + (PacmanDirection2D * 2);

		FVector2D BlinkyGridPosition = Blinky->GetGridPosition();
		//Calcolo il vettore distanza fra il greenspot e Blinky
		FVector2D DistanceVector = GreenSpot - BlinkyGridPosition;
		//Calcolo il punto simmetrico a Blinky rispetto al green spot 
		WhereImGoing = GreenSpot + DistanceVector;
		//DEBUG:
		//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("Inky WhereAmIGoing : X=%f, Y=%f "), WhereImGoing.X, WhereImGoing.Y));
	}
	//In mod SCATTER
	else 
	{
		WhereImGoing = SpecialSpotPosition;
	}
}

void AInky::LoadSpecialSpot()
{
	SpecialSpotPosition = TheGridGen->GetXYPositionByRealLocation(TheGridGen->GetInkySpecialSpotLocation());
	Blinky = Cast<ABlinky>(UGameplayStatics::GetActorOfClass(GetWorld(), ABlinky::StaticClass()));
	
}
