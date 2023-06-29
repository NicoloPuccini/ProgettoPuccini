// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseFood.h"
#include "Fruit.generated.h"

/**
 * 
 */
UCLASS()
class PROGETTOPUCCINI_API AFruit : public ABaseFood
{
	GENERATED_BODY()
		AFruit();
protected:
	//L'attributo puntatore al Blueprint
	UPROPERTY(EditAnywhere)
		UStaticMeshComponent* StaticMeshComponent;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
public:
	void ShowFruit() override ;
private:
	void RestoreFruit();
};
