// Fill out your copyright notice in the Description page of Project Settings.


#include "GateNode.h"
//Il Gate node è di fatto identico al WallNode , la gestione di uscita dei fantasmidalla loro casa la faccio nel PwantomPawn

AGateNode::AGateNode()
{
	//Setto CanWalk a NotWalkable 
	CanWalk = ECanWalk::NOTWALKABLE;

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));

	//Per ora setto la posizione del nodo a 0,0 poi lo cambierò quando genero il campo da gioco 
	NodeGridPosition = FVector2D(0, 0);
	NodeRealPosition = FVector(0);
}