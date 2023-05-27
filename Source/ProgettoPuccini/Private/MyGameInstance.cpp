// Fill out your copyright notice in the Description page of Project Settings.


#include "MyGameInstance.h"

void UMyGameInstance::AddToScore(int32 points)
{
	Score = Score + points;
	//Debug Per controllare che tutto funzioni
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("NewScore=%d "), Score));
}

int32 UMyGameInstance::GetScore()
{
	return Score;
}
