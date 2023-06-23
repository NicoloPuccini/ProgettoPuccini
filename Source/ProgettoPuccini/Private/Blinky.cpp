// Fill out your copyright notice in the Description page of Project Settings.


#include "Blinky.h"
#include"MyGameMode.h"


ABlinky::ABlinky()
{

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	
	//Sarà il nodo che il fantasma cercherà di raggiungere inseguendo Pacman , Cambia durante la partita (Sulla Griglia)
	 WhereImGoing = FVector2D(0,0);

	//La posizione del nodo che i fantasmini cercano di raggiungere quando sono in modalità scatter, è fissa , ma cambia da fantasma a fantasma (Sulla Griglia)
	 //Problema non prendere lo special spot da thegridg 
	 SpecialSpotPosition = FVector2D(0, 0);
	//LastInputDirection = FVector(0, 0, 0);
	CurrentDirection = FVector(0, 0, 0);

	BlinkyCounter = 0;
	
}

//per Blinky  non è necessario implementare queste funzioni 
int32 ABlinky::GetGhostCounter()
{
	return 0;
}

void ABlinky::IncrementGhostCounter()
{

}

void ABlinky::ResetGhostCounter()
{
	BlinkyCounter = 0;
}

//La funzione che definisce il comportamento individuale del fantasmino 
void ABlinky::WhereAmIGoingUpdate()
{
	//In mod CHASE
	if (GhostStatus == CHASE)
	{
		FVector2D PacmanGridPosition = CheckPacmanGridPosition();
		WhereImGoing = PacmanGridPosition;
	}
	//In mod SCATTER
	else 
	{
		WhereImGoing = SpecialSpotPosition;
	}
	//In modalità Frightened il pawn non punta un punto specifico ma sceglie le direzioni casualmente 
}

void ABlinky::LoadSpecialSpot()
{
	//Setto GameMode e SpecialSpotLocation
	TheGameMode = (AMyGameMode*)(GetWorld()->GetAuthGameMode());
	SpecialSpotPosition = TheGridGen->GetXYPositionByRealLocation(TheGridGen->GetBlinkySpecialSpotLocation());
}

void ABlinky::GoToSpawnLocation()
{
	FVector2D HomeLocation = TheGameMode->GField->GetXYPositionByRealLocation(TheGameMode->GField->GetPinkySpawn())+ FVector2D(0,-2);

	//Se non sei nella Home location Raggiungila 
	if (CurrentGridCoords != HomeLocation) {
		if (CrossingDetection())
		{

			WhereImGoing = HomeLocation;
			FVector NewDirection = ChoseNewDirection();
			SetCurrentDirection(NewDirection);

		}

		SetNodeGeneric(CurrentDirection);
	}
	else
	{
		
		//Sei Arrivato a casa e da ora assume status atHome
		if (TheGameMode->ChaseScatterPeriod == CHASE)
		{
			SetGhostStatus(CHASE);
		}
		else
		{
			SetGhostStatus(SCATTER);
		}

		SetGhostMooveset(ATHOUSE);
		SetCurrentMovementSpeed(GhostSpeed);

	
	}
}

UStaticMeshComponent* ABlinky::GetStaticMeshComponent() const
{
	return StaticMeshComponent;
}

void ABlinky::SetStaticMeshComponent(UStaticMeshComponent* NewMesh)
{
	StaticMeshComponent = NewMesh;
}
