// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "BaseFood.h"
#include "BaseNode.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameField.generated.h"

UCLASS()
class PROGETTOPUCCINI_API AGameField : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AGameField();

	//Dichiariamo un pò di ATTRIBUTI pubblici :
	// Dichiariamo un'attributo per tener traccia della dimensione dei Nodi 
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		float NodeSize;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		int MapSizeX;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		int MapSizeY;

	//Dichiariamo un'Array di puntatori ai BaseNode che formeranno la nostra griglia di gioco GRID
	UPROPERTY(VisibleAnywhere)
		TArray<ABaseNode*> Grid;
	UPROPERTY(VisibleAnywhere)
		TArray<ABaseFood*> FoodGrid;

	//Dichiaro Un array di coppie (FVector2D , ABaseNode ) per tener conto della posizione relativa (x,y) di ogni nodo sulla griglia
	TMap<FVector2D, ABaseNode*> TileMap;
	TMap<ABaseNode*, ABaseFood*> FoodMap;


	//METODI public :
	//Dichiaro un metodo che restituisce la posizione reale nel campo da gioco data quella relativa (x,y)
	FVector GetRelativeLocationByXYPosition(const int32 InX, const int32 InY);

	// return (x,y) position given a relative position
	FVector2D GetXYPositionByRelativeLocation(const FVector& Location);

	TMap<FVector2D, ABaseNode*> GetTileMAp();
	TMap<ABaseNode*, ABaseFood*> GetFoodMAp();

	//Controlla se il nodo è NOTWALKABLE , o se il nodo è vuoto , in tal caso ritorna false , altrimenti ritorna true
	static bool IsNodeValidForWalk(class ABaseNode* Node);

	//Restituise il prossimo nodo data una posizione X,Y e una direzione , ritorna null se il prossimo nodo è fuori dalla mappa 
	ABaseNode* GetNextNode(const FVector2D StartCoords, FVector InputDir);

	//Restituisce un Fvector2D dato un Fvector
	FVector2D GetFVector2DFromFVector3D(FVector Vector3D);

private:
	//Dichiariamo gli ATTRIBUTI private :

	//Questo servirà per generare i WallNode
	UPROPERTY(EditDefaultsOnly, Category = "Nodes")
		TSubclassOf<ABaseNode> WallNode;
	//Questo servirà per generare i WalkNode
	UPROPERTY(EditDefaultsOnly, Category = "Nodes")
		TSubclassOf<ABaseNode> WalkNode;

	//Questo servirà per generare i Foodie
	UPROPERTY(EditDefaultsOnly, Category = "Foods")
		TSubclassOf<ABaseFood> Foodie;
	//Questo servirà per generare i NoFood
	UPROPERTY(EditDefaultsOnly, Category = "Foods")
		TSubclassOf<ABaseFood> NoFood;
	//Questo servirà per generare i EnergyFood
	UPROPERTY(EditDefaultsOnly, Category = "Foods")
		TSubclassOf<ABaseFood> EnergyFood;
	//Questo servirà per generare i EnergyFood
	UPROPERTY(EditDefaultsOnly, Category = "Foods")
		TSubclassOf<ABaseFood> SuperFood;

	//Dichiaro un attributo , è un vettore (X,Y,Z) che conterrà in ogni sua componente la NodeSize
	FVector NodeSizeVector;



	//METODI private :
	//Dichiariamo un metodo per spawnare i Nodi da usare dentro GenerateGrid
	ABaseNode* SpawnNodeActorById(char CharId, FVector Position) const;
	//Dichiaro un metodo per far spawnare i Food da usare dentro GenerateFood
	ABaseFood* SpawnFoodActorById(char CharId, FVector Position) const;
	//Dichiaro un metodo per generare la griglia di gioco Grid
	void GenerateGrid();
	void GenerateFood();
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;


};

