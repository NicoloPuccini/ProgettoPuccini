// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseFood.h"
#include "Foodie.generated.h"

/**
 *
 */
UCLASS()
class PROGETTOPUCCINI_API AFoodie : public ABaseFood
{
	GENERATED_BODY()
public:
	AFoodie();
protected:
	//L'attributo puntatore al Blueprint
	UPROPERTY(EditAnywhere)
		UStaticMeshComponent* StaticMeshComponent;

};
