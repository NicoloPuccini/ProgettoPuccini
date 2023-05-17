// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseFood.h"
#include "NoFood.generated.h"

/**
 *
 */
UCLASS()
class PROGETTOPUCCINI_API ANoFood : public ABaseFood
{
	GENERATED_BODY()
		ANoFood();
	//L'attributo puntatore al Blueprint
	UPROPERTY(EditAnywhere)
		UStaticMeshComponent* StaticMeshComponent;

};
