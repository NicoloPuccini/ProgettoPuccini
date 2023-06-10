// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PhantomPawn.h"
#include "Pinky.generated.h"

/**
 * 
 */
UCLASS()
class PROGETTOPUCCINI_API APinky : public APhantomPawn
{
	GENERATED_BODY()
public:
	APinky();

	int32 GetGhostCounter() override;
	void IncrementGhostCounter() override;
	void ResetGhostCounter() override;

protected:
	//L'attributo puntatore al Blueprint
	UPROPERTY(EditAnywhere)
		UStaticMeshComponent* StaticMeshComponent;

	UPROPERTY(EditAnywhere)
	int32 PinkyCounter;

	void WhereAmIGoingUpdate() override;
	void LoadSpecialSpot() override;
};
