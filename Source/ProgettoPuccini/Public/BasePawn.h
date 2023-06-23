// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "Kismet/GameplayStatics.h"
#include "MyGameInstance.h"
#include "BaseNode.h"
#include "GameField.h"
#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "BasePawn.generated.h"

UENUM()
enum EPacmanStatus {
	DEFAULT UMETA(DisplayName = "NORMAL"),
	ENERGIZED UMETA(DisplayName = "ENERGIZED")
};


UCLASS()
class PROGETTOPUCCINI_API ABasePawn : public APawn
{
	GENERATED_BODY()

public:


	// Sets default values for this pawn's properties
	ABasePawn();

	//Dichiariamo metodi e Attributi :

	//Serve a settare a null gli attributi LastNode NextNode TargetNode 
	void ResetPacmanNodes();

	void SetCurrentGridCoords(FVector2D Location);

	//Controllo il timer PacmanEat
	void CheckPacmanEatTimer();

	//Setta le varie velocità che Pacman può assumere , viene chiamata nella BeginPlay
	void SetPacmanSpeeds();

	//Tutto quello che accade quando si mangia l' EnergyFood
	//Viene chiamata dentro HandleFood
	void BeginEnergizedMode();

	void SetVerticalInput(float AxisValue);
	void SetHorizontalInput(float AxisValue);
	FVector2D GetGridPosition();

	UFUNCTION(BlueprintCallable)
		void SetNextNodeByDir(FVector InputDir);

	FVector GetLastValidDirection() const;

private:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;



public:
	//Il timer che conta 4 secondi da quando pacman non mangia 
	FTimerHandle PacmanEatTimer;
	bool PacmanEatTimerEnd = true ;
private:
	//Serve a non far crushare tutto quando si vince 
	bool CallOnWin = false;

	//Il timer per l'EnergyMode
	FTimerHandle PacmanEnergyModeTimer;

	//Stato di Pacman 
	UPROPERTY(EditAnywhere , Category = "Pacman Status")
		TEnumAsByte<EPacmanStatus> PacmanCurrentStatus;

	//Per il movimento 
	UPROPERTY(VisibleAnywhere, Category = "Movement")
		FVector LastInputDirection;
	UPROPERTY(VisibleAnywhere, Category = "Movement")
		FVector LastValidInputDirection;

	void SetLastValidDirection(FVector Dir);

	//Velocità di Pacman:
	UPROPERTY(EditAnywhere, Category = "Movement")
		float CurrentMovementSpeed = 400.0f;
	//La velocità di Pacman in condizioni normali 
	UPROPERTY(EditAnywhere, Category = "Movement")
		float NormalPacmanSpeed = 400.0f;
	//La velocità di pacman mentre mangia 
	UPROPERTY(EditAnywhere, Category = "Movement")
		float PacmanEatSpeed = 400.0f;
	//La velocità di pacman Quando i fantasmi sono in Fright status
	UPROPERTY(EditAnywhere, Category = "Movement")
		float PacmanFrightSpeed = 400.0f;


	UPROPERTY(EditAnywhere)
		float AcceptedDistance = 4.f;

	UPROPERTY(VisibleAnywhere)
		FVector2D CurrentGridCoords;

	UPROPERTY(VisibleAnywhere, Category = "Nodes")
		ABaseNode* NextNode;
	UPROPERTY(VisibleAnywhere, Category = "Nodes")
		ABaseNode* TargetNode;
	UPROPERTY(VisibleAnywhere, Category = "Nodes")
		ABaseNode* LastNode;

	void HandleMovement();
	void OnNodeReached();
	void MoveToCurrentTargetNode();
	void SetTargetNode(ABaseNode* Node);
	void SetNextNode(ABaseNode* Node);
	void SetNodeGeneric(const FVector Dir);
public:
	void SetLastNode(ABaseNode* Node);
private:
	void Eat();
	void EatPhantom();

	UPROPERTY(VisibleAnywhere)
		class AMyGameMode* GameMode;

	//Passo una reference della GameInstance
	UPROPERTY(VisibleAnywhere)
	UMyGameInstance* GameInstance;

	UPROPERTY(VisibleAnywhere)
		AGameField* TheGridGen;

public:
	UFUNCTION()
		void OnClick();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
