// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "Blinky.h"
#include "CoreMinimal.h"
#include "PhantomPawn.h"
#include "Inky.generated.h"

/**
 * 
 */
UCLASS()
class PROGETTOPUCCINI_API AInky : public APhantomPawn
{
	GENERATED_BODY()
public:
	AInky();

	int32 GetGhostCounter() override;
	void IncrementGhostCounter() override;
	void ResetGhostCounter() override;

private:
	//L'attributo puntatore al Blueprint
	UPROPERTY(EditAnywhere)
		UStaticMeshComponent* StaticMeshComponent;

	UPROPERTY(EditAnywhere)
	int32 InkyCounter;

	//Passo una reference del BasePawn
	ABlinky* Blinky;

	UPROPERTY(EditAnywhere)
		class AMyGameMode* TheGameMode;

public:
	void WhereAmIGoingUpdate() override;
	void LoadSpecialSpot() override;
	void GoToSpawnLocation() override;

};
