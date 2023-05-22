// Fill out your copyright notice in the Description page of Project Settings.


#include "EnergyFood.h"

AEnergyFood::AEnergyFood()
{
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	//Gli EnergyFood valgono 50 punti l'uno e rendono pacman invincibile
	Points = 50;
	//Per ora setto la posizione del Food a 0,0 poi lo cambierò quando genero il campo da gioco 
	FoodGridPosition = FVector2D(0, 0);
	FoodRealPosition = FVector(0);
	//E setto inizialmente Eaten a false 
	Eaten = false;
}
