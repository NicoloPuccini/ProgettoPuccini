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

private:
	UPROPERTY(EditAnywhere)
		int32 Score = 0 ;

	UPROPERTY(EditAnywhere)
		int32 Lives = 200;

	UPROPERTY(EditAnywhere)
	int32 CurrentLives=200;

	UPROPERTY(EditAnywhere)
		int32 FoodieCounter = 239;

	UPROPERTY(EditAnywhere)
		int32 CurrentLevel = 1;

	//Il nome � indicativo
	UPROPERTY(EditAnywhere)
		bool BlockAllPawn = true;

	//La velocit� standard che uso come unit� di misura per tutti i pawn 
	UPROPERTY(EditAnywhere, Category = "Movement")
		 float StandardMovementSpeed = 400.0f;



	
public:


	float GetStandardMovementSpeed() const;
	//Funzioni per la gestione dei livelli :
	int32 GetLevel()const;
	void IncrementLevel();
	void ResetLevel();

	//Funzioni per la gestione delle vite 
	int32 GetCurrentLives() const;
	void DecrementCurrentLives();
	void ResetLives();

	//funzioni per la gestione dei FoodieCounter (Verve per decretare quando Pacman Vince il gioco )
	void SetFoodieCounter(int32 NewCount);
	int32 GetFoodieCounter() const;
	void ResetFoodieCounter();
	

	UFUNCTION(BlueprintCallable)
	bool GetBlockAllPawn() const ;

	void SetBlockAllPawn(bool YOrN);

	void AddToScore(int32 points);
	UFUNCTION(BlueprintCallable)
		int32 GetScore();
	void ResetScore();

	
};
