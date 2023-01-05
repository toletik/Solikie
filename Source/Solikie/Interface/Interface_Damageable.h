// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Interface_Damageable.generated.h"

class ACharacter_BaseUnit;

UINTERFACE(MinimalAPI)
class UInterface_Damageable : public UInterface
{
	GENERATED_BODY()
};

class SOLIKIE_API IInterface_Damageable
{
	GENERATED_BODY()

public:
	virtual float TakeDamage(ACharacter_BaseUnit* damager, float damageToTake) { return 0.0f; }

};
