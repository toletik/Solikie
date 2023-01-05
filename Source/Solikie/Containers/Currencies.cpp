// Fill out your copyright notice in the Description page of Project Settings.


#include "Currencies.h"
#include "../Containers/Macro.h"


bool FCurrencies::HasEnoughMoney(float cost)
{
	return (_money >= cost);
}

float FCurrencies::GetCurrentMoney()
{
	return _money;
}

void FCurrencies::SetCurrentMoney(float value)
{
	_money = value;
}
void FCurrencies::AddMoney(float value)
{
	_money += value;
	delegateOnMoneyChange.Broadcast(_money);
}

void FCurrencies::RemoveMoney(float value)
{
	_money -= value;
	if (_money < 0.f)
		_money = 0.f;
	delegateOnMoneyChange.Broadcast(_money);

}

bool FCurrencies::HaveEnoughCitizen(int nb)
{
	return (_freeCitizen >= nb);
}

int FCurrencies::GetCurrFreeCitizen()
{
	return _freeCitizen;
}

void FCurrencies::AddFreeCitizen(int nb)
{
	_freeCitizen += nb;
	delegateOnWorkersChange.Broadcast(_freeCitizen);
}

void FCurrencies::OccupyCitizen(int nb)
{
	_freeCitizen -= nb;
	if (_freeCitizen < 0)
		_freeCitizen = 0;
	delegateOnWorkersChange.Broadcast(_freeCitizen);
}
