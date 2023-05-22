// Fill out your copyright notice in the Description page of Project Settings.


#include "NoFood.h"

ANoFood::ANoFood()
{
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	//NoFood Non da punti 
	Points = 0;
	//Per ora setto la posizione del Food a 0,0 poi lo cambierò quando genero il campo da gioco 
	FoodGridPosition = FVector2D(0, 0);
	FoodRealPosition = FVector(0);
	//E setto inizialmente Eaten a true perchè non dovrà mai essere mangiato ne cambiare  
	Eaten = true;
}
