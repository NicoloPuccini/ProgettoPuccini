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

	//Passo una reference della GameInstance
	UMyGameInstance* GameInstance;

	//Passo una reference del BasePawn
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		ABasePawn* Pacman;





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

	//Serve a tener conto degli intervalli di SCATTER e CHASE 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Chase Scatter ")
		TEnumAsByte<EPhantomStatus> ChaseScatterPeriod;

	int32 Scatter_1Time = 0;
	int32 Scatter_2Time = 0;
	int32 Scatter_3Time = 0;
	int32 Scatter_4Time = 0;
	int32 Chase_1Time = 0;
	int32 Chase_2Time = 0;
	int32 Chase_3Time = 0;


	FTimerHandle Scatter_1;
	FTimerHandle Chase_1;
	FTimerHandle Scatter_2;
	FTimerHandle Chase_2;
	FTimerHandle Scatter_3;
	FTimerHandle Chase_3;
	FTimerHandle Scatter_4;


	//Dichiarazione del metodo costruttore della MyGameMode
	AMyGameMode();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	void GameStart();
	void ReturnAllPawnToSpawnLocations();

private:
	void StartChaseScatterTimers();
	void ClearAllChaseScatterTimers();
	void SetInitialBehavior();
	void LoadInitialGhostCounterLimit();
	void LoadChaseScatterTimes();
public:
	void LoadGhostCounterLimit();
	void OnWin();
	void OnGameOver();
	void OnPacmanLoseLife();
	void LoadNewLevel();

};
