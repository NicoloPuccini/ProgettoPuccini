// // Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseNode.h"
#include "WallNode.generated.h"

/**
 *
 */
UCLASS()
class PROGETTOPUCCINI_API AWallNode : public ABaseNode
{
	GENERATED_BODY()
public:
	AWallNode();
protected:

	//Attributi di Wall Node oltre a quelli ereditati dal BaseNode :
	//L'attributo puntatore al Blueprint
	UPROPERTY(EditAnywhere)
		UStaticMeshComponent* StaticMeshComponent;

};
