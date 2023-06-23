// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


#include "Kismet/GameplayStatics.h"
#include "MyGameInstance.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseFood.generated.h"

UCLASS()
class PROGETTOPUCCINI_API ABaseFood : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ABaseFood();

	//La funzione che verrà overload da tutti i food pe capire quali sono fodie e quali energyFood per decrementare il FoodieCounter
	virtual void HandleFood();
	
	//Riposta il food da mangiato a non mangiato e visibile 
	void ResumeFood();

	//Metodi per la posizione del Food 
	FVector2D GetFoodGridPosition();
	void SetFoodGridPosition(const double InX, const double InY);

	//Un metodo per settare se un Food è stato mangiato
	void SetFoodEaten(bool YesNo);

	//Una funzione che restituisce i punti che si guadagnano mangiando quel food 
	int32 GetPointsFromFood();
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	//ATTRIBUTI di BaseFood :

	


	//Il punteggio che si guadagna mangiando il Food 
	UPROPERTY(EditAnywhere)
		int32 Points;
	//Un attributo che ti dice se il Food è già stato mangiato 
	UPROPERTY(EditAnywhere)
		bool Eaten;
	//Un attributo per tener conto della posizione del Food sulla griglia di gioco 
	UPROPERTY(EditAnywhere)
		FVector2D FoodGridPosition;
	//Un attributo per tener conto della posizione del food nel mondo di gioco 
	UPROPERTY(EditAnywhere)
		FVector FoodRealPosition;
protected:
	//Passo una reference del GameMode
	UPROPERTY(VisibleAnywhere)
	     class AMyGameMode* GameMode;

	//Passo una reference della GameInstance
	UPROPERTY(VisibleAnywhere)
		UMyGameInstance* GameInstance;
    

	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
