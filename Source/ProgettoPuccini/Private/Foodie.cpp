// Fill out your copyright notice in the Description page of Project Settings.

#include "MyGameMode.h"
#include "Foodie.h"

AFoodie::AFoodie()
{

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	//I Foodie valgono dieci punti l'uno 
	Points = 10;
	//Per ora setto la posizione del Food a 0,0 poi lo cambierò quando genero il campo da gioco 
	FoodGridPosition = FVector2D(0, 0);
	FoodRealPosition = FVector(0);
	//E setto inizialmente Eaten a false 
	Eaten = false;
}

void AFoodie::BeginPlay()
{

	GameMode = (AMyGameMode*)(GetWorld()->GetAuthGameMode());
	GameInstance = GameMode->GameInstance;
	

}

void AFoodie::HandleFood()
{
	//Decremento di 1 il foodieCounter
	GameInstance->SetFoodieCounter(GameInstance->GetFoodieCounter() - 1);
	//Registro questo ffod per essere resumato 
	//GameMode->GField->OnRestoreFoodEvent.AddDynamic(this, &ABaseFood::ResumeFood);
}
