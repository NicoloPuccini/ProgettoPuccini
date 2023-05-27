// Fill out your copyright notice in the Description page of Project Settings.


#include "PhantomPawn.h"
#include "MyGameMode.h"

//COSTRUTTORE di BasePawn
APhantomPawn::APhantomPawn()
{
	// N.B il pawn viene spawnato automaticamente nella posizione del player start
	// dato che il pawn di default è stato impostato nei setting come BP_GridPawn
	//  
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	


	//// vettore di direzione inizializzato con zero (il pawn non si muove allo start del gioco fino a che non
	//   viene premuto uno dei tasti W-A-S-D )
	
	//LastInputDirection = FVector(0, 0, 0);
	LastValidInputDirection = FVector(0, 0, 0);
	////posizione iniziale  del pawn nelle coordinate di griglia 
	CurrentGridCoords = FVector2D(5, 14);
	//// nodi
	LastNode = nullptr;  //L'ultimo nodo in cui si è trovato il pawn
	TargetNode = nullptr; //Il nodo in cui il pawn sta per spostarsi 
	NextNode = nullptr; // Il prossimo nodo nella direzione in cui sta andando il Pawn


}

// Called when the game starts or when spawned
void APhantomPawn::BeginPlay()
{
	Super::BeginPlay();
	GameMode = (AMyGameMode*)(GetWorld()->GetAuthGameMode());
	TheGridGen = GameMode->GField;
	//Get the GameInstance reference 
	GameInstance = Cast<UMyGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));


	////<-------------------------------------------------------------------------------------------------------------------------------Dove come perchè Spawno il fantasmino
	FVector2D StartNode = TheGridGen->GetXYPositionByRelativeLocation(GetActorLocation());
	LastNode = TheGridGen->TileMap[StartNode];
}

// Called every frame
//Il Tick è un metodo che viene chiamato ad ogni frame e che noi sfrutteremo per far muovere gli oggetti nel campo nda gioco
void APhantomPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//Chiamiamo la funzione che definiamo qua sotto e che serve a gestire il movimento e velocità dei Pawn nel campo da gioco
	APhantomPawn::HandleMovement();
}

//Setta il TargetNode sulla base del NextNode 
void APhantomPawn::HandleMovement()
{
	MoveToCurrentTargetNode();

	//Se il campo TargetNode è vuoto (Il nodo in cui sto per muovermi)  
	if (TargetNode == nullptr)
	{
		//Se il campo NextNode è NON vuoto (Il prossimo nodo nella direzione in cui mi sto muovendo)
		if (NextNode != nullptr)
		{
			SetTargetNode(NextNode);
			SetNextNode(nullptr);
		}
	}
	// questo codice fa si che il pawn si muova autonomamente fino a che il next node è walkable

	if (!TargetNode && !NextNode)
	{
		//Se il prossimo nodo nella direzione inserita LastInputDirection è WALKABLE 
		if (TheGridGen->IsNodeValidForWalk(TheGridGen->GetNextNode(CurrentGridCoords, /*LastInputDirection*/ FVector(1,0,0)))) //<---------------Qua si mette la nuova direzione che il fantama prende 
		{
			//Setto LastValidDirection = LastInputDirection 
			//SetLastValidDirection(LastInputDirection);<------------------------------------------------------------------------------------------e anche qua 
		}
		//Se il prossimo nodo nella direzione data è WALKABLE diventa il TargetNode
		SetNodeGeneric(LastValidInputDirection);
	}
}
//--------------------------------MOVIMENTO DEL Pawn in una nuova casella -------------------------------------------------
void APhantomPawn::MoveToCurrentTargetNode()
{
	//Se TargetNode è null non faccio niente ed esco dalla funzione  
	if (TargetNode == nullptr) return;

	//Calcolo la distanza fra il TargetNode e la posizione del Pawn  
	const float Dist = FMath::Abs(FVector::Dist2D(TargetNode->GetActorLocation(), GetActorLocation()));

	//Se la distanza è minore o uguale a 4
	if (Dist <= AcceptedDistance)
	{
		OnNodeReached();
		return;
	}
	// funzione di interpolazione che fa muovere il pawn verso una nuova posizione data la posizione corrente
	const FVector2D StartVector = TheGridGen->GetFVector2DFromFVector3D(GetActorLocation());
	const FVector2D EndVector = TheGridGen->GetFVector2DFromFVector3D(TargetNode->GetActorLocation());

	//?? cosa fa la questo Pos ??
	const auto Pos = FMath::Vector2DInterpConstantTo(StartVector, EndVector, GetWorld()->GetDeltaSeconds(), CurrentMovementSpeed);
	const FVector Location(Pos.X, Pos.Y, GetActorLocation().Z);
	//Setta la posizione del Pawn ad ogni Tick
	SetActorLocation(Location);
}

//----------------------------------------------------Pacman EAT ---------------------------------------------------------------------------------
void APhantomPawn::Eat() {
	if (TargetNode == nullptr) return;
	//Prendo il food legato al nodo
	//const auto Node = GameMode->GField->GetNextNode(Coords, InputDir);
	const auto TargetFood = GameMode->GField->GetFood(TargetNode);
	//Controllo se posso mangiarlo 
	//if (TheGridGen->IsNodeValidForWalk(TheGridGen->GetNextNode(CurrentGridCoords, LastInputDirection)))
	if (TheGridGen->IsEatable(TargetFood))
	{
		//Ora che so che il food è mangiabile posso contrassegnarlo come mangiato (Settando Eaten a true ) 
		//DEBUG:
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("FoodEatenPosition : X=%f  Y=%f"), TargetFood->GetFoodGridPosition().X, TargetFood->GetFoodGridPosition().Y));
		TargetFood->SetFoodEaten(true);
		//Sommo il punteggio del food mangiato al punteggio nella GameIstance

			//DEBUG:
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("PointsEarned : =%d "), TargetFood->GetPointsFromFood()));

		GameInstance->AddToScore(TargetFood->GetPointsFromFood());

		//Ora posso "rimuoverlo" dal GameField , in realtà lo nascondo sotto il GameField 
		TheGridGen->HideFood(TargetFood);

	}

}

//-------------------------------------------------------------------------------------------------------------------------------------------------


//Quando il Pawn raggiunge il TargetNode
void APhantomPawn::OnNodeReached()
{
	//Mangia quello che si trova nel Node 
	Eat();

	//Aggiorna gli attributi del Pawn di conseguenza
	CurrentGridCoords = TargetNode->GetGridPosition();
	LastNode = TargetNode;
	SetTargetNode(nullptr);
	//---------------------------------------------------------Teleport Sinistro-------------------------------------------------------------------------------
	if (CurrentGridCoords.X == 14 && CurrentGridCoords.Y == 0 && LastValidInputDirection == FVector(0, -1, 0))
	{
		//Teletrasporto il pawn all'altro capo del teleport
		const FVector Location(GetActorLocation() + FVector(0, 27 * (TheGridGen->NodeSize), 0));
		SetActorLocation(Location);
		//Aggiorno gli attributi del Pawn di conseguenza
		CurrentGridCoords = CurrentGridCoords + FVector2D(0, 27);

	}
	//---------------------------------------------------------Teleport Destro-------------------------------------------------------------------------------
	if (CurrentGridCoords.X == 14 && CurrentGridCoords.Y == 27 && LastValidInputDirection == FVector(0, 1, 0))
	{
		//Teletrasporto il pawn all'altro capo del teleport
		const FVector Location(GetActorLocation() - FVector(0, 27 * (TheGridGen->NodeSize), 0));
		SetActorLocation(Location);
		//Aggiorno gli attributi del Pawn di conseguenza
		CurrentGridCoords = CurrentGridCoords - FVector2D(0, 27);

	}
}

void APhantomPawn::SetTargetNode(ABaseNode* Node)
{
	TargetNode = Node;
}

void APhantomPawn::SetNextNode(ABaseNode* Node)
{
	NextNode = Node;
}

//Setta in nodo con Il getNextNode se è WALKABLE
void APhantomPawn::SetNodeGeneric(const FVector Dir)
{
	const auto Node = TheGridGen->GetNextNode(CurrentGridCoords, Dir);
	if (TheGridGen->IsNodeValidForWalk(Node))
	{
		SetTargetNode(Node);
	}
}


//<-----------------------------------------------------------------------------------------------------------------------------------------Va sostituita
void APhantomPawn::SetVerticalInput(float AxisValue)
{
	/*
	if (AxisValue == 0) return;
	const FVector Dir = (GetActorForwardVector() * AxisValue).GetSafeNormal();
	LastInputDirection = Dir.GetSafeNormal();
	SetNextNodeByDir(LastInputDirection);
	*/
}
//<-------------------------------------------------------------------------------------------------------------------------------------------Va sostituita 
void APhantomPawn::SetHorizontalInput(float AxisValue)
{
	/*
	if (AxisValue == 0) return;
	const FVector Dir = (GetActorRightVector() * AxisValue).GetSafeNormal();
	LastInputDirection = Dir;
	SetNextNodeByDir(LastInputDirection);
	*/
}
//<-----------------------------------------------------------------CrossingDetection---------------------------------------------------------------------------------------------------------
bool APhantomPawn::CrossingDetection()
{
	//Questa funzione serve a controllare se il Fantasma si trova in un incrocio , da true se è ad un incrocio false altrimenti 
	int count = 0;
	FVector AllDir[] = { FVector(0,1,0),FVector(1,0,0), FVector(0,-1,0),FVector(-1,0,0) };
	for (FVector PossibleDir : AllDir) {
		//Escludo la direzione attuale del fantasma e la sua opposta (visto che non possono fare dietrofront )
		if (PossibleDir != LastValidInputDirection && PossibleDir != LastValidInputDirection * (-1))
		{
			//Prendo il prossimo nodo in quella direzione 
			auto Node = GameMode->GField->GetNextNode(CurrentGridCoords, PossibleDir);
			//Verifico che il nodo sia walkable 
			if (GameMode->GField->IsNodeValidForWalk(Node)) 
			{
				count = count + 1;
			}
		}
	}
	if (count != 0) { return true; }
	return false ;
}
//<----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------ChoseNewDirection-----------------------------------------------------------------------------------------------
FVector APhantomPawn::ChoseNewDirection()
{
	TMap<  FVector , float> DirectionAndDistanceMap;
	FVector AllDir[] = { FVector(0,1,0),FVector(1,0,0), FVector(0,-1,0),FVector(-1,0,0) };
	int32 Min = 0;//2147483647; //Ci metto il massimo numero contenibile in un int32 <-------------------------------------------------------------------Occhio qua !
	for (FVector PossibleDir : AllDir) {
		//Escludo la direzione opposta all'attuale del fantasma (visto che non possono fare dietrofront )
		if ( PossibleDir != LastValidInputDirection * (-1))
		{
			//Prendo il prossimo nodo in quella direzione 
			auto Node = GameMode->GField->GetNextNode(CurrentGridCoords, PossibleDir);
			//Verifico che il nodo sia walkable 
			if (GameMode->GField->IsNodeValidForWalk(Node))
			{
			  //Calcolo la distanza fra il nodo e WhereImGoing (mi basta il quadrato , non è necessario fare anche la radice ) 
				int32 Dist = 0; // FMath::Pow((Node->GetGridPosition() - WhereImGoing), 2);<--------------------------------------------------------Usa Dist2D che usa sopra !!!
			//Mi calcolo il minimo fra le distanze 
				if (Dist < Min) 
				{
					Min = Dist;
				}
				//Aggiungo la distanza e la possigleDir alla TMap DirectionAndDistanceMap
				DirectionAndDistanceMap.Add( PossibleDir, Dist);
			}
		}
	}
	//Rimuovo dal TMap quelli con distanza diversa dal minimo 
	for (auto& KeyValue : DirectionAndDistanceMap)
	{
		FVector dir = KeyValue.Key;
		float dist = KeyValue.Value;
		if (dist != Min) 
		{
         
		}//<---------------------------------------------------------------------------------------------------------------Da completare !
	}
	
	return FVector(0, 0, 0);
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void APhantomPawn::SetNextNodeByDir(FVector InputDir)
{
	const FVector2D Coords = TargetNode ? TargetNode->GetGridPosition() : LastNode->GetGridPosition();//<-----------------------------------------------------------Cosa significa questa sintassi ?
	const auto Node = GameMode->GField->GetNextNode(Coords, InputDir);
	if (GameMode->GField->IsNodeValidForWalk(Node))
	{
		SetNextNode(Node);
		SetLastValidDirection(InputDir);
	}
}

FVector APhantomPawn::GetLastValidDirection() const
{
	return LastValidInputDirection;
}

void APhantomPawn::SetLastValidDirection(FVector Dir)
{
	if (Dir == FVector::ZeroVector) return;
	LastValidInputDirection = Dir;
}