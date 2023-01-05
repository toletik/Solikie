// Fill out your copyright notice in the Description page of Project Settings.


#include "UserWidget_OrderSelectedEntities.h"
#include "Solikie/Containers/DataAsset_FactoryData.h"
#include "Components/Image.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "Solikie/Gameplay/Pawn_Player.h"


void UUserWidget_OrderSelectedEntities::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	//_tileImage->SetBrushFromTexture(_factoryData->Icon);

	_button->OnClicked.AddUniqueDynamic(this, &UUserWidget_OrderSelectedEntities::OnButtonClick);
}

void UUserWidget_OrderSelectedEntities::OnButtonClick()
{


}