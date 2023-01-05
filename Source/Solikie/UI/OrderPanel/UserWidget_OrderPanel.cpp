// Fill out your copyright notice in the Description page of Project Settings.


#include "UserWidget_OrderPanel.h"

#include "Solikie/Gameplay/Pawn_Player.h"
#include "UserWidget_OrderTile.h"
#include "Components/UniformGridPanel.h" 
#include "Components/WidgetSwitcher.h"
#include "Solikie/Gameplay/Actor_PlayerManager.h"
#include "Solikie/Containers/DataAsset_FactoryData.h"
#include "Solikie/Containers/DataAsset_UnitData.h"
#include "Solikie/Gameplay/Entities/ActorComponent_BaseEntity.h"
#include "Solikie/Gameplay/Entities/Factories/Actor_BaseFactory.h"
#include "Solikie/Gameplay/Entities/Units/Character_BaseUnit.h"


void UUserWidget_OrderPanel::Init(AActor_PlayerManager* playerManager)
{
	playerManager->delegateSelectionChange.AddUniqueDynamic(this, &UUserWidget_OrderPanel::OnSelectionChange);


	//Init _GridPanelDefault
	for (int i = 0; i < playerManager->_factoryDatas.Num(); ++i)
	{
		UUserWidget_OrderTile* newWidget = CreateWidget<UUserWidget_OrderTile>(this, _orderTileTemplate);
		newWidget->Init(playerManager->_factoryDatas[i]->Icon, i);
		newWidget->DelegateOnButtonClick.AddUniqueDynamic(playerManager, &AActor_PlayerManager::BuildingStart);
		_GridPanelDefault->AddChildToUniformGrid(newWidget, i / 3, i % 3);
	}
	for (int i = 0; i < playerManager->_FormationsIcons.Num(); ++i)
	{
		UUserWidget_OrderTile* newWidget = CreateWidget<UUserWidget_OrderTile>(this, _orderTileTemplate);
		newWidget->Init(playerManager->_FormationsIcons[i], i);
		newWidget->DelegateOnButtonClick.AddUniqueDynamic(playerManager, &AActor_PlayerManager::ChangeFormationType);
		int indexInGrid = i + playerManager->_factoryDatas.Num();
		_GridPanelDefault->AddChildToUniformGrid(newWidget, indexInGrid / 3, indexInGrid % 3);
	}

	//Init GridPanelUnit
	for (int i = 0; i < playerManager->_orderDatas.Num(); ++i)
	{
		UUserWidget_OrderTile* newWidget = CreateWidget<UUserWidget_OrderTile>(this, _orderTileTemplate);
		newWidget->Init(playerManager->_orderDatas[i], i);
		newWidget->DelegateOnButtonClick.AddUniqueDynamic(playerManager, &AActor_PlayerManager::OnUnitOrder);
		_GridPanelUnit->AddChildToUniformGrid(newWidget, i / 3, i % 3);
	}

	

}

void UUserWidget_OrderPanel::OnSelectionChange(TArray<UActorComponent_BaseEntity*> entities)
{

	//Default
	if (entities.Num() == 0)
	{
		_GridSwitcher->SetActiveWidgetIndex(0);

	}
	//Unit
	else if (ACharacter_BaseUnit* unit = Cast<ACharacter_BaseUnit>(entities[0]->GetOwner()) )
	{
		_GridSwitcher->SetActiveWidgetIndex(1);


	}
	//Factory
	else if (AActor_BaseFactory* factory = Cast<AActor_BaseFactory>(entities[0]->GetOwner()) )
	{
		_GridSwitcher->SetActiveWidgetIndex(2);


		//Init _GridPanelFactory
		_GridPanelFactory->ClearChildren();
		for (int i = 0; i < factory->GetStats()->UnitsToProduct.Num(); ++i)
		{
			UUserWidget_OrderTile* newWidget = CreateWidget<UUserWidget_OrderTile>(this, _orderTileTemplate);
			newWidget->Init(factory->GetStats()->UnitsToProduct[i]->Icon, i);
			newWidget->DelegateOnButtonClick.AddUniqueDynamic(factory, &AActor_BaseFactory::AddUnitToQueue);
			_GridPanelFactory->AddChildToUniformGrid(newWidget, i / 3, i % 3);
		}

	}
}

//Camera Related
void UUserWidget_OrderPanel::NativeOnInitialized()
{
	_player = Cast<APawn_Player>(GetOwningPlayer()->GetPawn());
}
void UUserWidget_OrderPanel::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	_player->canMove = false;
}
void UUserWidget_OrderPanel::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	_player->canMove = true;
}