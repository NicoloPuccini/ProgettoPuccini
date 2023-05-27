// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "Kismet/GameplayStatics.h"
#include "MyGameInstance.h"
#include "BaseNode.h"
#include "GameField.h"
#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "PhantomPawn.generated.h"



UCLASS()
class PROGETTOPUCCINI_API APhantomPawn : public APawn
{
	GENERATED_BODY()

public:

	//Passo una reference della GameInstance
	UMyGameInstance* GameInstance;

	// Sets default values for this pawn's properties
	APhantomPawn();

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

	//UPROPERTY(VisibleAnywhere, Category = "Movement")
	//	FVector LastInputDirection;
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
public:
	//Sarà il nodo che il fantasma cercherà di raggiungere inseguendo Pacman , Cambia durante la partita (Sulla Griglia)
	UPROPERTY(VisibleAnywhere, Category = "Phantom")
		FVector2D WhereImGoing ;

	//La posizione in cui spawna il fantasmino è fissa ma cambia da fantasma a fantasma  (Sulla Griglia)
	UPROPERTY(VisibleAnywhere, Category = "Phantom")
		FVector SpawnPosition;

	//La posiione del nodo che i fantasmini cercano di raggiungere quando sono in modalità scatter, è fissa , ma cambia da fantasma a fantasma (Sulla Griglia)
	UPROPERTY(VisibleAnywhere, Category = "Phantom")
		 FVector2D SpecialSpotPosition;

	//Le funzioni dei fantasmi :
	
	//Controlla che il fantasma sia ad un'incrocio
	bool CrossingDetection();

	//Sceglie la nuova direzione da percorrere in base a WhereImGoing
	FVector ChoseNewDirection();

	//Le funzioni da Pawn
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
	

	// Called every frame
	virtual void Tick(float DeltaTime) override;

};