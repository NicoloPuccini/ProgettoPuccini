// Fill out your copyright notice in the Description page of Project Settings.


#include "EnergyFood.h"
#include"MyGameMode.h"
#include "BasePawn.h"


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
void AEnergyFood::TestDelegate()
{
	//DEBUG:
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("TESTDELEGATE viene lanciata ")));
}
void AEnergyFood::BeginPlay()
{
	
	GameMode = (AMyGameMode*)(GetWorld()->GetAuthGameMode());
	GameInstance = GameMode->GameInstance;
	PacmanPawn = GameMode->Pacman;
	
}

void AEnergyFood::HandleFood()
{


	//Decremento di 1 il foodieCounter
	GameInstance->SetFoodieCounter(GameInstance->GetFoodieCounter() - 1);

	//Do inizio alla EnergizedMode dove pacman è più veloce e può mangiare i fantasmi Frightened 
	PacmanPawn->BeginEnergizedMode();

	//Registro questo ffod per essere resumato 
	//GameMode->GField->OnRestoreFoodEvent.AddDynamic(this, &AEnergyFood::TestDelegate);
}



