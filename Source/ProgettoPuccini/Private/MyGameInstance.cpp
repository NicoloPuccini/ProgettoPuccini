// Fill out your copyright notice in the Description page of Project Settings.


#include "MyGameInstance.h"

void UMyGameInstance::ResetScore()
{
	Score = 0;
}








FString UMyGameInstance::GetTurnMessage()
{
	return TurnMessage;
}

void UMyGameInstance::SetTurnMessage(FString Message)
{
	TurnMessage = Message;
}

void UMyGameInstance::ClearTurnMessage()
{
	TurnMessage = " ";
}

float UMyGameInstance::GetStandardMovementSpeed() const
{
	return StandardMovementSpeed ;
}

int32 UMyGameInstance::GetLevel() 
{
	return CurrentLevel;
}

void UMyGameInstance::IncrementLevel()
{
	CurrentLevel = CurrentLevel + 1;
}

void UMyGameInstance::ResetLevel()
{
	CurrentLevel = 1;
}



int32 UMyGameInstance::GetCurrentLives() 
{
	return CurrentLives;
}

void UMyGameInstance::DecrementCurrentLives()
{
	CurrentLives = CurrentLives - 1;
}

void UMyGameInstance::ResetLives()
{
	CurrentLives = Lives;
}

void UMyGameInstance::SetFoodieCounter(int32 NewCount)
{
	
	FoodieCounter = NewCount;
	//DEBUG:
	//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("New FoodieCounterValue =%d !!"), FoodieCounter ));
}

int32 UMyGameInstance::GetFoodieCounter() const
{
	return FoodieCounter;
}

void UMyGameInstance::ResetFoodieCounter()
{
	FoodieCounter = 239;
}

bool UMyGameInstance::GetBlockAllPawn() const
{
	return BlockAllPawn ;
}

void UMyGameInstance::SetBlockAllPawn(bool YOrN)
{
	BlockAllPawn = YOrN ;
}

void UMyGameInstance::AddToScore(int32 points)
{
	Score = Score + points;
	//Debug Per controllare che tutto funzioni
	//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("NewScore=%d "), Score));
}




int32 UMyGameInstance::GetScore()
{
	return Score;
}
