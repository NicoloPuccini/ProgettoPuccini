// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PhantomPawn.h"
#include "Blinky.generated.h"

/**
 * 
 */
UCLASS()
class PROGETTOPUCCINI_API ABlinky : public APhantomPawn
{
	GENERATED_BODY()
public:
	ABlinky();

protected:
	//L'attributo puntatore al Blueprint
	UPROPERTY(EditAnywhere)
		UStaticMeshComponent* StaticMeshComponent;

};
