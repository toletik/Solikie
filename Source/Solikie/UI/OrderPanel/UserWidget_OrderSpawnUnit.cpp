// Fill out your copyright notice in the Description page of Project Settings.


#include "UserWidget_OrderSpawnUnit.h"
#include "Solikie/Containers/DataAsset_UnitData.h"
#include "Components/Image.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "Solikie/Gameplay/Pawn_Player.h"


void UUserWidget_OrderSpawnUnit::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (_unitData)
		_tileImage->SetBrushFromTexture(_unitData->Icon);

	_button->OnClicked.AddUniqueDynamic(this, &UUserWidget_OrderSpawnUnit::OnButtonClick);
}

void UUserWidget_OrderSpawnUnit::OnButtonClick()
{


}