// Fill out your copyright notice in the Description page of Project Settings.


#include "Blinky.h"

ABlinky::ABlinky()
{
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	//Sar� il nodo che il fantasma cercher� di raggiungere inseguendo Pacman , Cambia durante la partita (Sulla Griglia)
		 WhereImGoing=FVector2D(1,1);

	//La posizione in cui spawna il fantasmino � fissa ma cambia da fantasma a fantasma  (Sulla Griglia)
		 SpawnPosition=FVector(550,1450,5);

	//La posiione del nodo che i fantasmini cercano di raggiungere quando sono in modalit� scatter, � fissa , ma cambia da fantasma a fantasma (Sulla Griglia)
		 SpecialSpotPosition=FVector2D(1,1);

}
