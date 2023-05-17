// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseFood.h"

// Sets default values
ABaseFood::ABaseFood()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

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

// Called when the game starts or when spawned
void ABaseFood::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void ABaseFood::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

