// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Currencies.h"

#include "Army.generated.h"

class AActor_BaseFactory;
class ACharacter_BaseUnit;


USTRUCT() struct FArmy
{
	GENERATED_BODY()

public:

	//Units
	void AddUnit(ACharacter_BaseUnit* unit);
	void RemoveUnit(ACharacter_BaseUnit* unit);
	int GetUnitsNb();
	void ClearUnits();
	TArray<ACharacter_BaseUnit*> GetUnits();

	//Factories
	void AddFactory(AActor_BaseFactory* factory);
	void RemoveFactory(AActor_BaseFactory* factory);
	int GetFactoriesNb();
	void ClearFactories();
	TArray<AActor_BaseFactory*> GetFactories();

	//Currency
	void SetCurrencies(FCurrencies currency);
	FCurrencies& GetCurrencies();

	float powerLevel = 0.0f;
	
private:

	//Units
	UPROPERTY()
	TArray<ACharacter_BaseUnit*> _unitList;
	//Factories
	UPROPERTY()
	TArray<AActor_BaseFactory*>  _factoryList;

	//Currency
	UPROPERTY()
	FCurrencies _currencies;
};