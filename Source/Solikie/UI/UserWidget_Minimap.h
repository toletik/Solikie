// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UserWidget_Minimap.generated.h"


class APawn_Player;
class AActor_MapManager;
class UImage;
class UButton;
UCLASS()
class SOLIKIE_API UUserWidget_Minimap : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeOnInitialized() override;
	virtual void NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent) override;
	
	void SetTexture(UTexture2D* tex);

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

private:
	UPROPERTY(meta = (BindWidget))
	UImage* _minimap {nullptr};
	UPROPERTY(meta = (BindWidget))
	UButton* _button {nullptr};

	APawn_Player* _player;
	AActor_MapManager* _mapManager;

	bool focusMinimap = false;

	UFUNCTION()
	void MouseButtonDown();
};
