// Fill out your copyright notice in the Description page of Project Settings.


#include "UserWidget_OrderTile.h"
#include "Components/Image.h"
#include "Components/Button.h"


void UUserWidget_OrderTile::Init(UTexture2D* texture, int32 indexInGrid)
{
	if(texture)
	_tileImage->SetBrushFromTexture(texture);

	_indexInGrid = indexInGrid;
}


void UUserWidget_OrderTile::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_button->OnClicked.AddUniqueDynamic(this, &UUserWidget_OrderTile::OnButtonClick);
}

void UUserWidget_OrderTile::OnButtonClick()
{
	DelegateOnButtonClick.Broadcast(_indexInGrid);
}