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

	int32 GetGhostCounter() override;
	void IncrementGhostCounter() override;
	void ResetGhostCounter() override;

public:
	//L'attributo puntatore al Blueprint
	UPROPERTY(EditAnywhere)
		UStaticMeshComponent* StaticMeshComponent;

	UPROPERTY(EditAnywhere)
		int32 BlinkyCounter;

	void WhereAmIGoingUpdate() override;
    void LoadSpecialSpot() override ;



};
