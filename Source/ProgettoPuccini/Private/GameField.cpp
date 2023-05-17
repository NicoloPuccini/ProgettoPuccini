// Fill out your copyright notice in the Description page of Project Settings.


#include "GameField.h"

// COSTRUTTORE del GameField
AGameField::AGameField()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Nel Costruttore settiamo i valori degli attributi :
	NodeSize = 100.0f;
	//Setto tutte le componenti XYZ di NodeSizeVector a NodeSize = 100.0f 
	NodeSizeVector = FVector(NodeSize);

	//La mappa di char che guider� il metodo GenerateGrid su quali nodi far spawnare e dove :
	//Fisso le dimensioni della mappa
	MapSizeX = 30;
	// la y contiene 28 elementi (+ il carattere di fine stringa)
	MapSizeY = 29;

}

// Called when the game starts or when spawned
void AGameField::BeginPlay()
{
	Super::BeginPlay();
	//Quando spawna il GameField chiamiamo la funzione che genera la Grid e fa spawnare tutti i nodi 
	GenerateGrid();
	GenerateFood();
}

// Called every frame
void AGameField::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


//Definiamo i METODI :



//Ritorna la coppia di Nodi e posizioni X,Y 
TMap<FVector2D, ABaseNode*> AGameField::GetTileMAp()
{
	return TileMap;
}

TMap<ABaseNode*, ABaseFood*> AGameField::GetFoodMAp()
{
	return FoodMap;
}


//La mappa di char che guider� il metodo GenerateGrid su quali nodi far spawnare e dove :
//Fisso le dimensioni della mappa
constexpr int MapSizeX = 30;
// la y contiene 28 elementi (+ il carattere di fine stringa)
constexpr int MapSizeY = 29;

// # - WallNode
// space - WalkNode

static char Map[MapSizeX][MapSizeY] = {
	"############################",
	"#            ##            #",
	"# #### ##### ## ##### #### #",
	"# #### ##### ## ##### #### #",
	"# #### ##### ## ##### #### #",
	"#             P            #",
	"# #### ## ######## ## #### #",
	"# #### ## ######## ## #### #",
	"#      ##    ##    ##      #",
	"###### ##### ## ##### ######",
	"###### ##### ## ##### ######",
	"###### ##          ## ######",
	"###### ## ######## ## ######",
	"###### ## #      # ## ######",
	"          #      #          ",
	"###### ## #      # ## ######",
	"###### ## ###  ### ## ######",
	"###### ##          ## ######",
	"###### ## ######## ## ######",
	"###### ## ######## ## ######",
	"#            ##            #",
	"# #### ##### ## ##### #### #",
	"# #### ##### ## ##### #### #",
	"#   ##                ##   #",
	"### ## ## ######## ## ## ###",
	"#      ## ######## ##      #",
	"# #######    ##    ####### #",
	"# ########## ## ########## #",
	"#                          #",
	"############################",
};
static char FMap[MapSizeX][MapSizeY] = {
	"############################",
	"#............##............#",
	"#.####.#####.##.#####.####.#",
	"#E####.#####.##.#####.####E#",
	"#.####.#####.##.#####.####.#",
	"#.............P............#",
	"#.####.##.########.##.####.#",
	"#.####.##.########.##.####.#",
	"#......##....##....##......#",
	"######.##### ## #####.######",
	"######.##### ## #####.######",
	"######.##          ##.######",
	"######.## ######## ##.######",
	"######.## #      # ##.######",
	"      .   #      #   .      ",
	"######.## #      # ##.######",
	"######.## ###  ### ##.######",
	"######.##          ##.######",
	"######.## ######## ##.######",
	"######.## ######## ##.######",
	"#............##............#",
	"#.####.#####.##.#####.####.#",
	"#.####.#####.##.#####.####.#",
	"#...##................##...#",
	"###.##.##.########.##.##.###",
	"#......##.########.##......#",
	"#.#######....##....#######.#",
	"#E##########.##.##########E#",
	"#..........................#",
	"############################",
};


//Definiamo la funzione che Genera la Grid a tutti gli effetti 
void AGameField::GenerateGrid()
{
	for (int x = 0; x < MapSizeX; x++)
	{
		for (int y = 0; y < MapSizeY - 1; y++)
		{
			const char MapTile = Map[x][y];
			//
			//La Vera posizione di un nodo sul campo da gioco si ottiene come somma :       location del game field +la RelativeLocation = Real location del nodo 
			// Avremo quindi tre dati che dicono la posizione di un nodo :
			// 
			// NodeGridPosition = E' un FVector2D (X,Y) Indica le cordinate in termini di tile sulla griglia 
			// RelativeNodeLocation = E' un FVector (X,Y,Z) indica le coordinate del nodo sul GameField (Non � quindi reale)
			// NodeRealPosition =FVector (X,Y,Z) Indica la posizione Reale del nodo dentro al mondo di gioco  
			//
			FVector RelativeNodeLocation(x * NodeSizeVector.X, y * NodeSizeVector.Y, 0);
			// imposto le coordinate spaziali per la funzione di spawn della tile (Non capisco a che diavolo serve GetActorLocation e di cosa lo fa (del GameField ?))
			const FVector CurrentSpawnPosition = GetActorLocation() + RelativeNodeLocation;
			// questa funzione spawna una nuova tile
			const auto SpawnedNode = SpawnNodeActorById(MapTile, CurrentSpawnPosition);
			// assegna le coordinate di griglia alla tile (Setto l'attributo NodeGridPosition del nodo con X,Y)
			SpawnedNode->NodeGridPosition = (FVector2D(x, y));
			// assegna le coordinate spaziali alla tile
			SpawnedNode->NodeRealPosition = CurrentSpawnPosition;
			// aggiungo alle strutture dati il riferimento alla tile creata
			Grid.Add(SpawnedNode);
			TileMap.Add(FVector2D(x, y), SpawnedNode);
		}
	}
}

void AGameField::GenerateFood()
{
	for (int x = 0; x < MapSizeX; x++)
	{
		for (int y = 0; y < MapSizeY - 1; y++)
		{
			const char MapFood = FMap[x][y];
			//
			//La Vera posizione di un nodo sul campo da gioco si ottiene come somma :       location del game field +la RelativeLocation = Real location del nodo 
			// Avremo quindi tre dati che dicono la posizione di un nodo :
			// 
			// NodeGridPosition = E' un FVector2D (X,Y) Indica le cordinate in termini di tile sulla griglia 
			// RelativeNodeLocation = E' un FVector (X,Y,Z) indica le coordinate del nodo sul GameField (Non � quindi reale)
			// NodeRealPosition =FVector (X,Y,Z) Indica la posizione Reale del nodo dentro al mondo di gioco  
			//
			FVector RelativeNodeLocation(x * NodeSizeVector.X, y * NodeSizeVector.Y, 20);
			// imposto le coordinate spaziali per la funzione di spawn del food 
			const FVector CurrentSpawnPosition = GetActorLocation() + RelativeNodeLocation;
			// questa funzione spawna un nuovo food
			const auto SpawnedFood = SpawnFoodActorById(MapFood, CurrentSpawnPosition);
			// assegna le coordinate di griglia alla tile (Setto l'attributo NodeGridPosition del nodo con X,Y)
			SpawnedFood->FoodGridPosition = (FVector2D(x, y));
			// assegna le coordinate spaziali alla tile
			SpawnedFood->FoodRealPosition = CurrentSpawnPosition;
			// aggiungo alle strutture dati il riferimento alla tile creata
			FoodGrid.Add(SpawnedFood);
			//Vado a prendermi il nodo su cui faccio spawnare la Tile 
			//GetTileMap ritona le coppie nodo, posizione X,Y corrispondenti a quelle generate nel GameField
			ABaseNode* Node = GetTileMAp()[FVector2D(x, y)];
			FoodMap.Add(Node, SpawnedFood);
		}
	}
}

ABaseNode* AGameField::SpawnNodeActorById(char CharId, FVector Position) const
{
	ABaseNode* Node;
	TSubclassOf<ABaseNode> ClassToSpawn = ABaseNode::StaticClass();

	if (CharId == '#')
	{
		ClassToSpawn = WallNode;
	}
	else
	{
		//Altrimenti � un WalkNode
		ClassToSpawn = WalkNode;
	}
	//Qua viene spawnata a tutti gli effetti la Tile del tipo richiesto .
	Node = GetWorld()->SpawnActor<ABaseNode>(ClassToSpawn, Position, FRotator::ZeroRotator);
	return Node;
}

ABaseFood* AGameField::SpawnFoodActorById(char CharId, FVector Position) const
{
	ABaseFood* Food;
	TSubclassOf<ABaseFood> ClassToSpawn = ABaseFood::StaticClass();

	if (CharId == '.')
	{
		ClassToSpawn = Foodie;
	}
	else
		if (CharId == 'E')
		{
			ClassToSpawn = EnergyFood;
		}
		else
		{
			//Altrimenti � un WalkNode
			ClassToSpawn = NoFood;
		}
	//Qua viene spawnata a tutti gli effetti la Tile del tipo richiesto .
	Food = GetWorld()->SpawnActor<ABaseFood>(ClassToSpawn, Position, FRotator::ZeroRotator);
	return Food;
}



//Definiamo i metodi per :

//Restituisce la RelativeNodeLocation data la posizione X,Y
FVector AGameField::GetRelativeLocationByXYPosition(const int32 InX, const int32 InY)
{
	return  NodeSize * FVector(InX, InY, 0);
}

//Restituisce la posizione X,Y data la RelativeNodeLocation
FVector2D AGameField::GetXYPositionByRelativeLocation(const FVector& Location)
{
	double x = floor(Location[0] / (NodeSize));
	double y = floor(Location[1] / (NodeSize));
	// GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, FString::Printf(TEXT("x=%f,y=%f"), x, y));
	return FVector2D(x, y);
}


bool AGameField::IsNodeValidForWalk(ABaseNode* Node)
{
	if (Node == nullptr) return false;
	if (Node->CanWalk == NOTWALKABLE) return false;

	return true;
}

//Serve a trovare Il prossimo nodo sul percorso di un Pawn conoscendo le coordinate X,Y del Pawn e la sua direzione 
ABaseNode* AGameField::GetNextNode(const FVector2D StartCoords, FVector InputDir)
{
	const float RequestedX = StartCoords.X + InputDir.X;
	const float RequestedY = StartCoords.Y + InputDir.Y;
	// la funzione clamp ritorna un numero compreso tra min e max se nel range
	//se pi� piccolo del min ritorna il min
	//se pi� grande del max ritorna il max
	//Evita cos� che possano essere forniti nodi al di fuori della mappa come possibili target
	const float ClampedX = FMath::Clamp(RequestedX, 0.f, MapSizeX - 1);
	const float ClampedY = FMath::Clamp(RequestedY, 0.f, MapSizeY - 2);
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, FString::Printf(TEXT("x=%f,y=%f"), ClampedX, ClampedY));

	//GetTileMap ritona le coppie nodo, posizione X,Y corrispondenti a quelle generate nel GameField
	ABaseNode* PossibleNode = GetTileMAp()[FVector2D(ClampedX, ClampedY)];

	//Se il prossimo nodo nella direzione percorsa � fuori dalla mappa ritorna null
	if (RequestedX > ClampedX || RequestedX < 0)
	{
		//X overflow
		PossibleNode = nullptr;
	}
	if (RequestedY > ClampedY || RequestedY < 0)
	{
		//Y overflow
		PossibleNode = nullptr;
	}
	return PossibleNode;
}


//Restituisce un Fvector2D dato un Fvector
FVector2D AGameField::GetFVector2DFromFVector3D(FVector Vector3D)
{
	return FVector2D(Vector3D.X, Vector3D.Y);
}