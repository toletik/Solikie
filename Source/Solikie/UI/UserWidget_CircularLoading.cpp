// Fill out your copyright notice in the Description page of Project Settings.


#include "UserWidget_CircularLoading.h"

#include "Components/Image.h"
#include "Solikie/Containers/Macro.h"

void UUserWidget_CircularLoading::NativeOnInitialized()
{
	if (materialInterface)
	{
		_matLoading = UMaterialInstanceDynamic::Create(materialInterface, this);
		_matLoading->SetScalarParameterValue("Angle", 0.f);
		circularLoading->SetBrushFromMaterial(_matLoading);
	}
}

void UUserWidget_CircularLoading::UpdateMaterial(float newValue)
{
	_matLoading->SetScalarParameterValue("Angle", newValue);
}