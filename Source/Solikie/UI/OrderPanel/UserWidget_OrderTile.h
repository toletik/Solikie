// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UserWidget_OrderTile.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOrderTile_OnButtonClick_DynamicDelegate, int32, indexInGrid);

class UImage;
class UButton;

UCLASS()
class SOLIKIE_API UUserWidget_OrderTile : public UUserWidget
{
	GENERATED_BODY()

public:
	void Init(UTexture2D* texture, int32 indexInGrid);

	FOrderTile_OnButtonClick_DynamicDelegate DelegateOnButtonClick;

private:
	virtual void NativeOnInitialized() override;

	UFUNCTION()
	void OnButtonClick();
	
	UPROPERTY(meta = (BindWidget))
	UImage* _tileImage {nullptr};
	UPROPERTY(meta = (BindWidget))
	UButton* _button {nullptr};

	int32 _indexInGrid {0};
};
