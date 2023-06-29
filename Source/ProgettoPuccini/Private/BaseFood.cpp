// Fill out your copyright notice in the Description page of Project Settings.

#include "BaseFood.h"
#include "MyGameMode.h"


// Sets default values
ABaseFood::ABaseFood()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	
}

// Called when the game starts or when spawned
void ABaseFood::BeginPlay()
{
	Super::BeginPlay();
	GameMode = (AMyGameMode*)(GetWorld()->GetAuthGameMode());
	GameInstance = GameMode->GameInstance;
	
}

void ABaseFood::ShowFruit()
{

}

void ABaseFood::HandleFood()
{

}

void ABaseFood::ResumeFood()
{
	/*
	void AGameField::RestoreFood(ABaseFood * Food)
	{

		FVector NewFoodLocation = Food->GetActorLocation();

		//Controllo se l'asset è stato nasconsto andando a controllare la sua coordinata Z 
		if (NewFoodLocation.Z == -380)
		{
			//Se è stato nascosto lo riporto sul GameField
			NewFoodLocation.Z = NewFoodLocation.Z + 400;
			Food->SetActorLocation(NewFoodLocation);
		}

	}*/

	//DEBUG:
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("RESUME FOOD Viene chiamata !")));
	GameMode->GField->RestoreFood(this);

}

FVector2D ABaseFood::GetFoodGridPosition()
{
	return FoodGridPosition;
	
}

void ABaseFood::SetFoodGridPosition(const double InX, const double InY)
{
	FoodGridPosition.Set(InX, InY);
}

void ABaseFood::SetFoodEaten(bool YesNo)
{

	Eaten = YesNo;
}


// Called every frame
void ABaseFood::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

int32 ABaseFood::GetPointsFromFood()
{
	return Points;
}