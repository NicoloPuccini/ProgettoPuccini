// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


#include"BasePawn.h"
#include "Kismet/GameplayStatics.h"
#include "MyGameInstance.h"
#include "BaseNode.h"
#include "GameField.h"
#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "PhantomPawn.generated.h"

UENUM()
enum EPhantomStatus {
	CHASE UMETA(DisplayName = "CHASE"),
	SCATTER UMETA(DisplayName = "SCATTER"),
	FRIGHTENED UMETA(DisplayName = "FRIGHTENED"),
	INCORPOREAL  UMETA(DisplayName = "INCORPOREAL") //Lo stato in cui entra il fantasma quando viene mangiato 
};

UENUM()
enum  EMooveset {

	NORMAL UMETA(DisplayName = "NORMAL"),
	ATHOUSE UMETA(DisplayName = "ATHOUSE"),
	EXITHOUSE UMETA(DisplayName = "EXITHOUSE"),
	GOTOHOUSE  UMETA(DisplayName = "GOTOHOUSE")

};


UCLASS()
class PROGETTOPUCCINI_API APhantomPawn : public APawn
{
	GENERATED_BODY()

public:

	// Sets default values for this pawn's properties
	APhantomPawn();

	//La funzioni che gestiscono il ghost counter limit
	int32 GetGhostCounterLimit();
	void SetGhostCounterLimit(int32 Limit);


	//Funzioni che gestiscono i contatori dei fantasmini (Ogni fantasmino la impementa in modo diverso )
	virtual int32 GetGhostCounter();
	virtual void IncrementGhostCounter();
	virtual void ResetGhostCounter();
	void CheckGhostCounter();
	void ResetAllGhostCounter();

	//Funzioni sullo stato del fantasma 
	TEnumAsByte<EPhantomStatus> GetGhostStatus();
	void SetGhostStatus(const TEnumAsByte<EPhantomStatus> Status);

	// Funzioni sul Mooveset dei fantasmi
	TEnumAsByte<EMooveset> GetGhostMooveset();
	void SetGhostMooveset(const TEnumAsByte<EMooveset>  Behavior);

	//Funzione che fa diventare i fantasmi Blu
	void SetGhostBlue();
	void SetGhostNonBlue();
	//Funzione che carica alcuni attributi dentro i fantasmi 
	virtual void LoadSpecialSpot();

	//Serve a tenere nota della posizione di Pacman e a far muovere i fantasmi di conseguenza 
	FVector2D CheckPacmanGridPosition();
	FVector CheckPacmanDirection();

	//Questa è la funzione che aggiorna WhereAmIGoing tutte le volte che sono ad un incrocio (Ogni fantasma la definisce in modo diverso )
	virtual void WhereAmIGoingUpdate();
	//Questa funzione fa tornare ogni fantasma nella sua Spawn location (Ad eccezione di blinky )(Ogni fantasma la definisce in modo diverso )
	virtual void  GoToSpawnLocation();


	//Dichiariamo metodi e Attributi :

	//<------------------------------------------------------------------------------------------------------Da cancellare i fantasmi non prendono input da tastiera 
	//void SetVerticalInput(float AxisValue);
	//void SetHorizontalInput(float AxisValue);

	UFUNCTION(BlueprintCallable)
		void SetNextNodeByDir(FVector InputDir);

	//Funzioni per il movimento dei fantasmi 
	FVector From2To3SizeVector(FVector2D input);

	FVector2D GetSpecialSpotPosition();
	FVector2D GetWhereImGoing();
	FVector GetCurrentDirection() const;
	FVector2D GetGridPosition() const;
	void SetGridPosition(FVector2D Location);


protected:



	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//UPROPERTY(VisibleAnywhere, Category = "Movement")
	//	FVector LastInputDirection;
	UPROPERTY(VisibleAnywhere, Category = "Movement")
		FVector CurrentDirection;
	//La direzione che seguono i fantasmi quando sono dentro la casa 
	UPROPERTY(VisibleAnywhere, Category = "Movement")
		FVector HouseDirection;
	void SetCurrentDirection(FVector Dir);

	//Velocità dei ghost
	UPROPERTY(EditAnywhere, Category = "Movement")
		float CurrentMovementSpeed = 400.0f;
	UPROPERTY(EditAnywhere, Category = "Movement")
		float GhostTunnelSpeed;
	UPROPERTY(EditAnywhere, Category = "Movement")
		float GhostSpeed;
	UPROPERTY(EditAnywhere, Category = "Movement")
		float FrightGhostSpeed;
	UPROPERTY(EditAnywhere, Category = "Movement")
		float IncorporealGhostSpeed = 1000.0f;




	UPROPERTY(EditAnywhere)
		float AcceptedDistance = 4.f;

	UPROPERTY(VisibleAnywhere)
		FVector2D CurrentGridCoords;

	//Stato del fantasma : Scatter,Chase,Frightened
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Phantom")
		TEnumAsByte<EPhantomStatus> GhostStatus;
	//Mooveset del fantasma : NORMAL , INHOUSE , EXITHOUSE
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Phantom")
		TEnumAsByte<EMooveset> Mooveset;

	//Il limite al contatore personale di ogni fantasma , oltre il quale il fantasma esce di casa 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Phantom")
		int32 GhostCounterLimit;

	//Serve a tenere conto della prima volta che reggiugi un nodo in modalità scatter per invertire la direzione solo la prima volta 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Phantom")
		bool DirectionInvercted = false;


	UPROPERTY(VisibleAnywhere, Category = "Nodes")
		ABaseNode* NextNode;
	UPROPERTY(VisibleAnywhere, Category = "Nodes")
		ABaseNode* TargetNode;
	UPROPERTY(VisibleAnywhere, Category = "Nodes")
		ABaseNode* LastNode;

	//Sarà il nodo che il fantasma cercherà di raggiungere inseguendo Pacman , Cambia durante la partita (Sulla Griglia)
	UPROPERTY(VisibleAnywhere, Category = "Phantom")
		FVector2D WhereImGoing;

	//La posiione del nodo che i fantasmini cercano di raggiungere quando sono in modalità scatter, è fissa , ma cambia da fantasma a fantasma (Sulla Griglia)
	UPROPERTY(VisibleAnywhere, Category = "Phantom")
		FVector2D SpecialSpotPosition;

	//Serve a tener conto se il fantasma è sul terreno di casa o meno 
	UPROPERTY(VisibleAnywhere, Category = "Phantom")
		bool InGhostHome = false;

	//Le funzioni dei fantasmi :



public:

	//Funzioni sulla velocità dei fantasmi :
	void SetCurrentMovementSpeed(float NewSpeed);
	float GetCurrentMovementSpeed();
	float GetIncorporealGhostSpeed();
	float GetGhostSpeed() const;
	void SetSpeeds();

protected:

	//La funzione che dal GateEntrance riporta ogni fantasma alla sua SpawnPosition (Ogni fantasma la definisce in modo diverso )
	void EnterHouse();



	//Controlla se il fantasma si trova in uno dei due tunnel 
	void GhostSpeedCheck();
	//Controlla che il fantasma sia ad un'incrocio
	bool CrossingDetection();

	//Sceglie la nuova direzione da percorrere in base a WhereImGoing
	FVector ChoseNewDirection();

	//Sceglie la nuova direzione casualmente (Viene usato in FRIGHTENED)
	FVector ChoseNewRandomDirection();

	//Le funzioni da Pawn
	void HandleMovement();
	void OnNodeReached();
	void MoveToCurrentTargetNode();
	void SetTargetNode(ABaseNode* Node);
	void SetNextNode(ABaseNode* Node);
	void SetNodeGeneric(const FVector Dir);
	void Eat();
public:
	void SetLastNode(ABaseNode* Node);
	void ResetAllPhantomNodes();

protected:



	UPROPERTY(VisibleAnywhere)
		class AMyGameMode* GameMode;

	//Passo una reference della GameInstance
	UPROPERTY(VisibleAnywhere)
		UMyGameInstance* GameInstance;


	UPROPERTY(VisibleAnywhere)
		AGameField* TheGridGen;

	//Passo una reference del BasePawn
	UPROPERTY(VisibleAnywhere)
		ABasePawn* Pacman;


public:


	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual UStaticMeshComponent* GetStaticMeshComponent() const;
	virtual void SetStaticMeshComponent(UStaticMeshComponent* NewMesh);

private:
	static class DirectionAndDistance {
	public:
		FVector Direction;
		float Distance;
	};
};