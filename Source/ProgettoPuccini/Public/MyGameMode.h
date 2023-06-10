// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "MyGameInstance.h"
#include "Clyde.h"
#include "Inky.h"
#include "Pinky.h"
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

	FTimerHandle PacmanEatTimer;

	//Passo una reference della GameInstance
	UMyGameInstance* GameInstance;

	//Serve per spawnare Blinky
	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<ABlinky> BlinkyClass;
	// reference to a GameField object
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		ABlinky* Blinky;

	//Serve per spawnare Pinky
	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<APinky> PinkyClass;
	// reference to a GameField object
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		APinky* Pinky;

	//Serve per spawnare Inky
	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<AInky> InkyClass;
	// reference to a GameField object
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		AInky* Inky;

	//Serve per spawnare Clyde
	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<AClyde> ClydeClass;
	// reference to a GameField object
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		AClyde* Clyde;

			//Dichiarazione del metodo costruttore della MyGameMode
	AMyGameMode();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	void GameStart();
	
private:
	void StartChaseScatterTimers();
	void SetInitialBehavior();
 	void LoadGhostCounterLimit();
};
