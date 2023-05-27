// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "MyGameInstance.generated.h"

/**
 *
 */
UCLASS()
class PROGETTOPUCCINI_API UMyGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
		int32 Score = 0 ;

	UPROPERTY(EditAnywhere)
	int32 Lives=3;

	UPROPERTY(EditAnywhere)
		int32 Level = 0;

	void AddToScore(int32 points);

	UFUNCTION(BlueprintCallable)
		int32 GetScore();
	
};
