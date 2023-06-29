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
	// 
	//Questa funzione serve a ottenere la posizione sulla griglia data la posizione dell'attore nel mondo 
	FVector2D GetXYPositionByRealLocation(FVector Location) const ;
	
	//Dichiaro un metodo che restituisce la posizione reale nel campo da gioco data quella relativa (x,y)
	FVector GetRelativeLocationByXYPosition(const int32 InX, const int32 InY);

	// return (x,y) position given a relative position
	FVector2D GetXYPositionByRelativeLocation(const FVector& Location);

	TMap<FVector2D, ABaseNode*> GetTileMAp();
	TMap<ABaseNode*, ABaseFood*> GetFoodMAp();

	//Controlla se il nodo è NOTWALKABLE , o se il nodo è vuoto , in tal caso ritorna false , altrimenti ritorna true
	static bool IsNodeValidForWalk(class ABaseNode* Node);

	//Controlla se il food è Eatable 
	static bool IsEatable(class ABaseFood* Food);

	//La funzione che uso per nascondere l'asset del food una volta mangiato 
	void HideFood(class ABaseFood* Food);
	void RestoreFood(class ABaseFood* Food);
	//Restituise il prossimo nodo data una posizione X,Y e una direzione , ritorna null se il prossimo nodo è fuori dalla mappa 
	ABaseNode* GetNextNode(const FVector2D StartCoords, FVector InputDir);
	ABaseFood* GetFood(ABaseNode* Node);
	//Restituisce un Fvector2D dato un Fvector
	FVector2D GetFVector2DFromFVector3D(FVector Vector3D);

	//Funzioni per recuperare la SpawnLocation di Pacman
	FVector GetPacmanSpawn()const;

	//Funzioni per recuperare le SpawnLocation dei fantasmi 
	FVector GetBlinkySpawn()const;
	FVector GetInkySpawn()const;
	FVector GetPinkySpawn()const;
	FVector GetClydeSpawn()const;

	//La funzione che ritorna la location di ExitNode (Serve ai fantasmi per uscire di casa )
	FVector GetExitNodeLocation()const;

	//Le funzioni che ritornano la location del SpecialSpot
	FVector GetBlinkySpecialSpotLocation()const;
	FVector GetPinkySpecialSpotLocation()const;
	FVector GetInkySpecialSpotLocation()const;
	FVector GetClydeSpecialSpotLocation()const;

	//Funzioni per recuperare le location dei teleport 
	FVector GetRightTeleportLocation()const;
	FVector GetLeftTeleportLocation()const;
	
	//Funzione per recuperare le locatiotion dei TunnelNode
	TArray<FVector> GetTunnelNodeLocations() const;

private:
	//Dichiariamo gli ATTRIBUTI private :
	// 
	//Contiene la spawn location di Pacman
	UPROPERTY(EditAnywhere, Category = "Pacman")
		FVector PacmanSpawnLocation;

	//Contiene la spawn location di Blinky(red)
	UPROPERTY(EditAnywhere, Category = "Ghosts")
		 FVector BlinkySpawnLocation;
	//Contiene la location del SpecialSpot di Blinky(il punto che cerca di raggiungere in modalità scatter )
	UPROPERTY(EditAnywhere, Category = "Ghosts")
	FVector BlinkySpecialSpotLocation;


	//Contiene la spawn location di Inky(blue)
	UPROPERTY(EditAnywhere, Category = "Ghosts")
		 FVector InkySpawnLocation;
	//Contiene la location del SpecialSpot di Inky(il punto che cerca di raggiungere in modalità scatter )
	UPROPERTY(EditAnywhere, Category = "Ghosts")
		FVector InkySpecialSpotLocation;

	//Contiene la spawn location di Pinky(pink)
	UPROPERTY(EditAnywhere, Category = "Ghosts")
		 FVector PinkySpawnLocation;
	//Contiene la location del SpecialSpot di Pinky(il punto che cerca di raggiungere in modalità scatter )
	UPROPERTY(EditAnywhere, Category = "Ghosts")
		FVector PinkySpecialSpotLocation;

	//Contiene la spawn location di Clyde(Orange)
	UPROPERTY(EditAnywhere, Category = "Ghosts")
		 FVector ClydeSpawnLocation;
	//Contiene la location del SpecialSpot di Clyde(il punto che cerca di raggiungere in modalità scatter )
	UPROPERTY(EditAnywhere, Category = "Ghosts")
		FVector ClydeSpecialSpotLocation;

	//Contiene le location di tutti i tunnel Node (Usato nel PhantomPawn per rallentare i fantasmi )
	UPROPERTY(EditAnywhere, Category = "Tunnel")
	TArray<FVector> TunnelNodeLocations;
	//Contiene la location del Teleport destro
	UPROPERTY(EditAnywhere, Category = "Teleport")
		FVector RightTeleportLocation;
	//Contiene la location del Teleport Sinistro
	UPROPERTY(EditAnywhere, Category = "Teleport")
		FVector LeftTeleportLocation;
	//E' la casella prima del Gate viene usata dai fantsmi per uscire di casa 
	UPROPERTY(EditAnywhere, Category = "Ghosts")
		FVector ExitNodeLocation;
	
	//Questo servirà per generare i WallNode
	UPROPERTY(EditDefaultsOnly, Category = "Nodes")
		TSubclassOf<ABaseNode> WallNode;
	//Questo servirà per generare i WalkNode
	UPROPERTY(EditDefaultsOnly, Category = "Nodes")
		TSubclassOf<ABaseNode> WalkNode;
	//Questo servirà per generare i WalkNode
	UPROPERTY(EditDefaultsOnly, Category = "Nodes")
		TSubclassOf<ABaseNode> GateNode;
	//Questo servirà per generare i Foodie
	UPROPERTY(EditDefaultsOnly, Category = "Foods")
		TSubclassOf<ABaseFood> Foodie;
	//Questo servirà per generare i NoFood
	UPROPERTY(EditDefaultsOnly, Category = "Foods")
		TSubclassOf<ABaseFood> NoFood;
	//Questo servirà per generare i EnergyFood
	UPROPERTY(EditDefaultsOnly, Category = "Foods")
		TSubclassOf<ABaseFood> EnergyFood;
	//Questo servirà per generare i Fruit
	UPROPERTY(EditDefaultsOnly, Category = "Foods")
		TSubclassOf<ABaseFood> Fruit;

	FVector FruitLocation;

	public:
		FVector GetFruitLocation() const;

		void RestoreAllEatenFood();

		private:
	//Dichiaro un attributo , è un vettore (X,Y,Z) che conterrà in ogni sua componente la NodeSize
	FVector NodeSizeVector;



	//METODI private :
	//Dichiariamo un metodo per spawnare i Nodi da usare dentro GenerateGrid
	ABaseNode* SpawnNodeActorById(char CharId, FVector Position) ;
	//Dichiaro un metodo per far spawnare i Food da usare dentro GenerateFood
	ABaseFood* SpawnFoodActorById(char CharId, FVector Position) ;
	//Dichiaro un metodo per generare la griglia di gioco Grid
	void GenerateGrid();
	void GenerateFood();
private:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;


};

