// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


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

	/*//Serve per creare la Camera
	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<ACamera> CameraClass;
	// reference to a GameField object
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		ACamera* Cam;*/

		/*	UPROPERTY(EditDefaultsOnly)
				TSubclassOf<APacmanPawn> PacmanClass;
			// reference to a GameField object
			UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
				APacmanPawn* Pacman;
			*/

			//Dichiarazione del metodo costruttore della MyGameMode
	AMyGameMode();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

};
