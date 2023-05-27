// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Blinky.h"
#include "GameField.h"
#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "MyGameMode.generated.h"

/**
 *
 */
UCLASS()
class PROGETTOPUCCINI_API AMyGameMode : public AGameMode
{
	GENERATED_BODY()

public:
	//Serve per creare il GameField
	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<AGameField> GameFieldClass;

	// reference to a GameField object
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		AGameField* GField;

	//Per legare Blinky al suo Blueprint
	
	UPROPERTY(EditDefaultsOnly, Category = "Phantom")
		TSubclassOf<ABlinky> BlinkyClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		ABlinky* Blinky;


			//Dichiarazione del metodo costruttore della MyGameMode
	AMyGameMode();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

};
