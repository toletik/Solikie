// Fill out your copyright notice in the Description page of Project Settings.


#include "UserWidget_SelectionInfo.h"

#include "UserWidget_SelectionButton.h"
#include "Solikie/Gameplay/Pawn_Player.h"
#include "Solikie/Gameplay/Entities/Units/Character_BaseUnit.h"
#include "Solikie/Gameplay/Actor_PlayerManager.h"

void UUserWidget_SelectionInfo::Init(AActor_PlayerManager* playerManager)
{
	playerManager->delegateSelectionChange.AddUniqueDynamic(this, &UUserWidget_SelectionInfo::UpdateSelectionInfo);
	delegateUpdateWhenWeSelectionOneEntity.AddUniqueDynamic(playerManager, &AActor_PlayerManager::UpdateSelectionAfterClickButton);
}

void UUserWidget_SelectionInfo::NativeOnInitialized()
{
	player = Cast<APawn_Player>(GetOwningPlayer()->GetPawn());
	_scrollBox->AddChild(_grid);
}
void UUserWidget_SelectionInfo::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	player->canMove = false;
}
void UUserWidget_SelectionInfo::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	player->canMove = true;
}

void UUserWidget_SelectionInfo::UpdateSelectionInfo(TArray<UActorComponent_BaseEntity*> entities)
{
	_factoryInfo->RemoveRefDelegate();
	_unitInfo->RemoveRefDelegate();
	if (entities.Num() == 1)
	{
		UpdateToHaveMoreInfo(entities[0]);
		return;
	}
	
	_grid->ClearChildren();
    _widgetSwitcher->SetActiveWidgetIndex(0);
    int number = 0;
    for (UActorComponent_BaseEntity* entity : entities)
    {
    	UUserWidget_SelectionButton* newButton = CreateWidget<UUserWidget_SelectionButton>(this, button);
    	newButton->SetValue(entity);
    	newButton->delegateUpdateSelectionInfoToHaveEntityInfo.AddDynamic(this, &UUserWidget_SelectionInfo::UpdateToHaveMoreInfo);
    	_grid->AddChildToGrid(newButton, number / 10, number % 10);
    	number++;
    }
}

void UUserWidget_SelectionInfo::UpdateToHaveMoreInfo(UActorComponent_BaseEntity* compEntity)
{
	AActor* entity = compEntity->GetOwner();
	if (entity->ActorHasTag("Unit"))
	{
		_widgetSwitcher->SetActiveWidgetIndex(2);
		_unitInfo->UpdateWithNewUnit(Cast<ACharacter_BaseUnit>(entity));
	}
	else if (entity->ActorHasTag("Factory"))
	{
		_widgetSwitcher->SetActiveWidgetIndex(1);
		_factoryInfo->UpdateWithNewFactory(Cast<AActor_BaseFactory>(entity));
	}
	delegateUpdateWhenWeSelectionOneEntity.Broadcast(compEntity);
}