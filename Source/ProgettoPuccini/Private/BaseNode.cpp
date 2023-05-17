#include "BaseNode.h"

// Sets default values
//COSTTRUTTORE DEL BASE NODE  
ABaseNode::ABaseNode()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ABaseNode::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void ABaseNode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


//Definiso i miei metodi per BaseNode :

FVector2D ABaseNode::GetGridPosition() {
	return NodeGridPosition;
}

void ABaseNode::SetGridPosition(const double InX, const double InY) {
	NodeGridPosition.Set(InX, InY);
}

TEnumAsByte<ECanWalk> ABaseNode::GetCanWalk() {
	return CanWalk;
}

void ABaseNode::SetCanWalk(const TEnumAsByte<ECanWalk> Walk) {
	CanWalk = Walk;
}
