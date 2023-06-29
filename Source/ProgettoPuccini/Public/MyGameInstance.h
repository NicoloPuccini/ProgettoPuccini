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
		FString TurnMessage= " ";

	UPROPERTY(EditAnywhere)
		int32 Score = 0 ;

	UPROPERTY(EditAnywhere)
		int32 Lives = 3;

	UPROPERTY(EditAnywhere)
	int32 CurrentLives=3;

	UPROPERTY(EditAnywhere)
		int32 FoodieCounter = 239;

	UPROPERTY(EditAnywhere)
		int32 CurrentLevel = 1;

	//Il nome è indicativo
	UPROPERTY(EditAnywhere)
		bool BlockAllPawn = true;

	//La velocità standard che uso come unità di misura per tutti i pawn 
	UPROPERTY(EditAnywhere, Category = "Movement")
		 float StandardMovementSpeed = 400.0f;



	
public:

	//funzioni per gestire i TurnMessage 
	UFUNCTION(BlueprintCallable)
	FString GetTurnMessage();

	void SetTurnMessage(FString Message);
	void ClearTurnMessage();

	float GetStandardMovementSpeed() const;
	//Funzioni per la gestione dei livelli :
	UFUNCTION(BlueprintCallable)
	int32 GetLevel();

	void IncrementLevel();
	void ResetLevel();

	//Funzioni per la gestione delle vite 
	UFUNCTION(BlueprintCallable)
	int32 GetCurrentLives() ;
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
