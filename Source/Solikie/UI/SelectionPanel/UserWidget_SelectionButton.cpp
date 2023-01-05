// Fill out your copyright notice in the Description page of Project Settings.


#include "UserWidget_SelectionButton.h"
#include "Solikie/Containers/Macro.h"

void UUserWidget_SelectionButton::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_button->OnClicked.AddDynamic(this, &UUserWidget_SelectionButton::OnClick);
}

void UUserWidget_SelectionButton::OnClick()
{
	if (_actor)
		delegateUpdateSelectionInfoToHaveEntityInfo.Broadcast(_actor);
}

void UUserWidget_SelectionButton::SetValue(UActorComponent_BaseEntity* entity)
{
	_image->SetBrushFromTexture(entity->icon);
	_actor = entity;
}
