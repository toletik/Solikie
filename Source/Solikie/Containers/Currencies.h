// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Currencies.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDelegateOnMoneyChange, float, money);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDelegateOnWorkersChange, int, workersNb);


//Currencies the AI and the player will possess 
USTRUCT() struct FCurrencies
{
	GENERATED_BODY()

public:

	//Money helpers
	bool HasEnoughMoney(float cost);
	float GetCurrentMoney();
	void SetCurrentMoney(float value);
	void AddMoney(float value);
	void RemoveMoney(float value);

	//Citizen helpers
	bool HaveEnoughCitizen(int nb);
	int GetCurrFreeCitizen();
	void AddFreeCitizen(int nb);
	void OccupyCitizen(int nb);


	FDelegateOnMoneyChange delegateOnMoneyChange;
	FDelegateOnWorkersChange delegateOnWorkersChange;


private:
	UPROPERTY()
	float _money {100.0f};   // main currency
	UPROPERTY()
	int   _freeCitizen {0}; // prevents the player from being able to create infinite units
};
