// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UserWidget_OrderSpawnUnit.generated.h"


class UDataAsset_BuildableUnitData;
class UImage;
class UButton;

UCLASS()
class SOLIKIE_API UUserWidget_OrderSpawnUnit : public UUserWidget
{
	GENERATED_BODY()


private:
	virtual void NativeOnInitialized() override;

	UFUNCTION()
	void OnButtonClick();


	UPROPERTY(meta = (BindWidget))
	UImage* _tileImage {nullptr};
	UPROPERTY(meta = (BindWidget))
	UButton* _button {nullptr};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Initialization, meta = (AllowPrivateAccess = "true"))
	UDataAsset_BuildableUnitData* _unitData;

};
