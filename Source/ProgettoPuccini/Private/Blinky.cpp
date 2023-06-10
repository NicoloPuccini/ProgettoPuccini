// Fill out your copyright notice in the Description page of Project Settings.


#include "Blinky.h"

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
	SpecialSpotPosition = TheGridGen->GetXYPositionByRealLocation(TheGridGen->GetBlinkySpecialSpotLocation());
}

