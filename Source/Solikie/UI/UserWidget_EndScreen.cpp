// Fill out your copyright notice in the Description page of Project Settings.


#include "UserWidget_EndScreen.h"

#include "Components/WidgetSwitcher.h"


void UUserWidget_EndScreen::Init(bool playerWon)
{
	TextSwitcher->SetActiveWidgetIndex(playerWon);
}
