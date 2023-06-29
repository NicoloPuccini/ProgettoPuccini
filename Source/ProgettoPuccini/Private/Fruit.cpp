// Fill out your copyright notice in the Description page of Project Settings.


#include "Fruit.h"
#include "MyGameMode.h"

AFruit::AFruit()
{
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	//I Foodie valgono dieci punti l'uno 
	Points = 100;
	//Per ora setto la posizione del Food a 0,0 poi lo cambierò quando genero il campo da gioco 
	FoodGridPosition = FVector2D(0, 0);
	FoodRealPosition = FVector(0);
	//E setto inizialmente Eaten a false 
	Eaten = false;

}

void AFruit::BeginPlay()
{
	GameMode = (AMyGameMode*)(GetWorld()->GetAuthGameMode());
	GameInstance = GameMode->GameInstance;
	Points = 100;
	//Nascondo il fruit all'inizio e lo segno come eaten 
	Eaten = true;
	GameMode->GField->HideFood(this);

	//Il frutto appare a 70 foodie mangiati :
	//Il foodie counter fa il conto alla rovescia da 239 quindi a 70 foodie mangiati sarà a 169
	//Viene gestito dentro Eat del BasePawn
}

void AFruit::ShowFruit()
{ 
	//DEBUG:
	//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("ShowFruit viene chiamata !")));
	RestoreFruit();
	//Restore lo segna anche come eatable
	//Faccio partire un contatore al termine del quale il Frut torna in Hide e non può più essere mangiato (10 secondi)
	FTimerHandle FruitTimer;
	GetWorld()->GetTimerManager().SetTimer(FruitTimer, [&]()
		{
			//Se il frutto non è stato mangiato lo faccio sparire 
			if (!Eaten)
			{
				Eaten = true;
				GameMode->GField->HideFood(this);
			}
		}, 10, false);
	
}

void AFruit::RestoreFruit()
{
	FVector NewLocation = FVector(GetActorLocation().X, GetActorLocation().Y,  0  );
	SetActorLocation(NewLocation);
	Eaten = false;
}
