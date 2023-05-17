// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseNode.generated.h"

UENUM()
enum ECanWalk {
	WALKABLE UMETA(DisplayName = "Walkable"),
	NOTWALKABLE UMETA(DisplayName = "NotWalkable")
};

UCLASS()
class PROGETTOPUCCINI_API ABaseNode : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ABaseNode();

	//Metodi Del BaseNode

	FVector2D GetGridPosition();

	void SetGridPosition(const double InX, const double InY);

	TEnumAsByte<ECanWalk> GetCanWalk();

	void SetCanWalk(const TEnumAsByte<ECanWalk> Walk);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	//Gli ATTRIBUTI del mio BaseNode public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TEnumAsByte<ECanWalk> CanWalk;

	//Do un attributo per segnare la posizione relativa sulla griglia di gioco 
	UPROPERTY(EditAnywhere)
		FVector2D NodeGridPosition;

	// FVector Con la posizione reale del Nodo nel mondo di gioco 
	UPROPERTY(EditAnywhere)
		FVector NodeRealPosition;



	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
