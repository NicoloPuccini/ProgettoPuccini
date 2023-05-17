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
		AEnergyFood();
	//L'attributo puntatore al Blueprint
	UPROPERTY(EditAnywhere)
		UStaticMeshComponent* StaticMeshComponent;
};
