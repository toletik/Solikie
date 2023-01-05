// Fill out your copyright notice in the Description page of Project Settings.


#include "UserWidget_MainHud.h"

#include "UserWidget_PlayerFund.h"
#include "SelectionPanel/UserWidget_SelectionInfo.h"
#include "OrderPanel/UserWidget_OrderPanel.h"



void UUserWidget_MainHud::Init(AActor_PlayerManager* playerManager)
{
	_PlayerFund->Init(playerManager);
	_SelectionInfo->Init(playerManager);
	_OrderPanel->Init(playerManager);

}
