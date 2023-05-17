// Fill out your copyright notice in the Description page of Project Settings.



#include "MyPlayerController.h"

AMyPlayerController::AMyPlayerController()
{
	bShowMouseCursor = true;
}

void AMyPlayerController::BeginPlay()
{
}

void AMyPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	//Bind grid movement Actions here
	InputComponent->BindAxis(TEXT("MoveForward"), this, &AMyPlayerController::SetVertical);
	InputComponent->BindAxis(TEXT("MoveRight"), this, &AMyPlayerController::SetHorizontal);
	// bind the OnClick function to InputComponent for testing purpose
	InputComponent->BindAction("Click", IE_Pressed, this, &AMyPlayerController::ClickOnGrid);
}

void AMyPlayerController::SetHorizontal(float Amount)
{
	const auto P = Cast<ABasePawn>(GetPawn());
	if (P)
	{
		P->SetHorizontalInput(Amount);
	}

}

void AMyPlayerController::SetVertical(float Amount)
{
	const auto P = Cast<ABasePawn>(GetPawn());
	if (P)
	{
		P->SetVerticalInput(Amount);
	}
}

void AMyPlayerController::ClickOnGrid()
{
	const auto P = Cast<ABasePawn>(GetPawn());
	if (P)
	{
		P->OnClick();
	}
}
