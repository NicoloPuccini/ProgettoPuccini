// Fill out your copyright notice in the Description page of Project Settings.


#include "WalkNode.h"

AWalkNode::AWalkNode()
{
	//Setto CanWalk a Walkable 
	CanWalk = ECanWalk::WALKABLE;

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));

	//Per ora setto la posizione del nodo a 0,0 poi lo cambierò quando genero il campo da gioco 
	NodeGridPosition = FVector2D(0, 0);
	NodeRealPosition = FVector(0);
}
