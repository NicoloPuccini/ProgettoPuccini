// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseNode.h"
#include "WalkNode.generated.h"

/**
 *
 */
UCLASS()
class PROGETTOPUCCINI_API AWalkNode : public ABaseNode
{
	GENERATED_BODY()
public:
	AWalkNode();
protected:

	//Attributi di Walk Node oltre a quelli ereditati dal BaseNode :

	//L'attributo puntatore al Blueprint
	UPROPERTY(EditAnywhere)
		UStaticMeshComponent* StaticMeshComponent;

};
