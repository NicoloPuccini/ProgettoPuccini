// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "MyGameInstance.h"
#include "BaseNode.h"
#include "GameField.h"
#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "BasePawn.generated.h"



UCLASS()
class PROGETTOPUCCINI_API ABasePawn : public APawn
{
	GENERATED_BODY()

public:

	//Passo una reference della GameInstance
	UMyGameInstance* GameInstance;

	// Sets default values for this pawn's properties
	ABasePawn();

	//Dichiariamo metodi e Attributi :

	void SetVerticalInput(float AxisValue);
	void SetHorizontalInput(float AxisValue);

	UFUNCTION(BlueprintCallable)
		void SetNextNodeByDir(FVector InputDir);

	FVector GetLastValidDirection() const;
	//<-----------------------------------------------------------------Ho cambiato da protected a private
private:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, Category = "Movement")
		FVector LastInputDirection;
	UPROPERTY(VisibleAnywhere, Category = "Movement")
		FVector LastValidInputDirection;

	void SetLastValidDirection(FVector Dir);

	UPROPERTY(EditAnywhere, Category = "Movement")
		float CurrentMovementSpeed = 400.0f;
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
	void Eat();
protected:
	UPROPERTY(VisibleAnywhere)
		class AMyGameMode* GameMode;


	UPROPERTY(VisibleAnywhere)
		AGameField* TheGridGen;

public:
	UFUNCTION()
		void OnClick();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
