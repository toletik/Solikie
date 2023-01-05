// Fill out your copyright notice in the Description page of Project Settings.


#include "UserWidget_PlayerFund.h"
#include "../Containers/Macro.h"

void UUserWidget_PlayerFund::Init(AActor_PlayerManager* playerManager)
{
	playerManager->army.GetCurrencies().delegateOnMoneyChange.AddUniqueDynamic(this, &UUserWidget_PlayerFund::EditMoneyTextOnChange);
	playerManager->army.GetCurrencies().delegateOnWorkersChange.AddUniqueDynamic(this, &UUserWidget_PlayerFund::EditWorkersTextOnChange);


	_moneyText->SetText(FText::FromString("Money: " + FString::SanitizeFloat(playerManager->army.GetCurrencies().GetCurrentMoney())));
	_workersText->Text = FText::FromString("Population: " + FString::SanitizeFloat(playerManager->army.GetCurrencies().GetCurrFreeCitizen()));
}

void UUserWidget_PlayerFund::EditMoneyTextOnChange(float money)
{
	_moneyText->SetText(FText::FromString("Money: " + FString::SanitizeFloat(money)));
}
void UUserWidget_PlayerFund::EditWorkersTextOnChange(int workersNb)
{
	_workersText->Text =  FText::FromString("Population: " + FString::SanitizeFloat(workersNb));
}
