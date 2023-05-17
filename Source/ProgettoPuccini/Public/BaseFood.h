// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseFood.generated.h"

UCLASS()
class PROGETTOPUCCINI_API ABaseFood : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ABaseFood();

	//Metodi per la posizione del Food 
	FVector2D GetFoodGridPosition();
	void SetFoodGridPosition(const double InX, const double InY);

	//Un metodo per settare se un Food è stato mangiato
	void SetFoodEaten(bool YesNo);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	//ATTRIBUTI di BaseFood :

	//Il punteggio che si guadagna mangiando il Food 
	UPROPERTY(EditAnywhere)
		int32 Points;
	//Un attributo che ti dice se il Food è già stato mangiato 
	UPROPERTY(EditAnywhere)
		bool Eaten;
	//Un attributo per tener conto della posizione del Food sulla griglia di gioco 
	UPROPERTY(EditAnywhere)
		FVector2D FoodGridPosition;
	//Un attributo per tener conto della posizione del food nel mondo di gioco 
	UPROPERTY(EditAnywhere)
		FVector FoodRealPosition;




	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
