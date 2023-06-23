// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PhantomPawn.h"
#include "Clyde.generated.h"

/**
 * 
 */
UCLASS()
class PROGETTOPUCCINI_API AClyde : public APhantomPawn
{
	GENERATED_BODY()
public:
	AClyde();

	int32 GetGhostCounter() override;
	void IncrementGhostCounter() override;
	void ResetGhostCounter() override;

protected:
	//L'attributo puntatore al Blueprint
	UPROPERTY(EditAnywhere)
		UStaticMeshComponent* StaticMeshComponent;

	UPROPERTY(EditAnywhere)
		class AMyGameMode* TheGameMode;

	UPROPERTY(EditAnywhere)
	int32 ClydeCounter;

	void WhereAmIGoingUpdate() override;
	void LoadSpecialSpot() override;
	void GoToSpawnLocation() override;
public:
    UStaticMeshComponent* GetStaticMeshComponent() const override;
	void SetStaticMeshComponent(UStaticMeshComponent* NewMesh) override;
};
