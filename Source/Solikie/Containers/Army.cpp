// Fill out your copyright notice in the Description page of Project Settings.


#include "Army.h"

#include "DataAsset_FactoryData.h"
#include "DataAsset_UnitData.h"
#include "../Gameplay/Entities/Factories/Actor_BaseFactory.h"
#include "../Gameplay/Entities/Units/Character_BaseUnit.h"
#include "Solikie/Containers/Macro.h"


//Units
void FArmy::AddUnit(ACharacter_BaseUnit* unit)
{
	_unitList.Emplace(unit);
	powerLevel += unit->GetStats()->PowerLevel;
}
void FArmy::RemoveUnit(ACharacter_BaseUnit* unit)
{
	_unitList.Remove(unit);
	powerLevel -= unit->GetStats()->PowerLevel;

	if (unit)
		unit->Destroy();
}
int FArmy::GetUnitsNb()
{
	return _unitList.Num();
}
void FArmy::ClearUnits()
{
	_unitList.Empty();
	powerLevel = 0.0f;
}
TArray<ACharacter_BaseUnit*> FArmy::GetUnits()
{
	return _unitList;
}

//Factories
void FArmy::AddFactory(AActor_BaseFactory* factory)
{
	_factoryList.Emplace(factory);
}
void FArmy::RemoveFactory(AActor_BaseFactory* factory)
{
	_factoryList.Remove(factory);

	if(factory)
		factory->Destroy();
		
}
int FArmy::GetFactoriesNb()
{
	return _factoryList.Num();
}
void FArmy::ClearFactories()
{
	_factoryList.Empty();
}
TArray<AActor_BaseFactory*> FArmy::GetFactories()
{
	return _factoryList;
}

//Currency
void FArmy::SetCurrencies(FCurrencies currency)
{
	_currencies = currency;
}
FCurrencies& FArmy::GetCurrencies()
{
	return _currencies;
}