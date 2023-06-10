// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseFood.h"
#include "EnergyFood.generated.h"

/**
 *
 */
UCLASS()
class PROGETTOPUCCINI_API AEnergyFood : public ABaseFood
{
	GENERATED_BODY()
public:
		AEnergyFood();

		//Funzione che decrementa il contatore dei foodie e delle EnergyFood ancora da mangiare 
		 void DecrementFoodieCounter() override;
	//L'attributo puntatore al Blueprint
	UPROPERTY(EditAnywhere)
		UStaticMeshComponent* StaticMeshComponent;
};
