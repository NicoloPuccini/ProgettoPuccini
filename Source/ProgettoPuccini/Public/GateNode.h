// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseNode.h"
#include "GateNode.generated.h"

/**
 * 
 */
UCLASS()
class PROGETTOPUCCINI_API AGateNode : public ABaseNode
{
	GENERATED_BODY()
public:
	AGateNode();
protected:

	//Attributi di Wall Node oltre a quelli ereditati dal BaseNode :
	//L'attributo puntatore al Blueprint
	UPROPERTY(EditAnywhere)
		UStaticMeshComponent* StaticMeshComponent;

};
