// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UserWidget_FactoryInfo.h"
#include "UserWidget_SelectionButton.h"
#include "UserWidget_UnitInfo.h"

#include "Blueprint/UserWidget.h"
#include "Components/GridPanel.h"
#include "Components/Image.h"
#include "Components/ScrollBox.h"
#include "Components/WidgetSwitcher.h"
#include "UserWidget_SelectionInfo.generated.h"


class AActor_PlayerManager;
class APawn_Player;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDelegateUpdateWhenWeSelectionOneEntity, UActorComponent_BaseEntity*, comp);

UCLASS()
class SOLIKIE_API UUserWidget_SelectionInfo : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UUserWidget_SelectionButton> button;

	UPROPERTY()
	APawn_Player* player;

	UFUNCTION()
	void Init(AActor_PlayerManager* playerManager);
	
	virtual void NativeOnInitialized() override;
	virtual void NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent) override;

	UFUNCTION()
	void UpdateSelectionInfo(TArray<UActorComponent_BaseEntity*> entities);

	UFUNCTION()
	void UpdateToHaveMoreInfo(UActorComponent_BaseEntity* compEntity);

	FDelegateUpdateWhenWeSelectionOneEntity delegateUpdateWhenWeSelectionOneEntity;
	
private :
	UPROPERTY(meta = (BindWidget))
	UImage* _background {nullptr};
	UPROPERTY(meta = (BindWidget))
	UWidgetSwitcher* _widgetSwitcher {nullptr};
	UPROPERTY(meta = (BindWidget))
	UScrollBox* _scrollBox {nullptr};
	UPROPERTY(meta = (BindWidget))
	UGridPanel* _grid = {nullptr};
	UPROPERTY(meta = (BindWidget))
	UUserWidget_UnitInfo* _unitInfo = {nullptr};
	UPROPERTY(meta = (BindWidget))
	UUserWidget_FactoryInfo* _factoryInfo = {nullptr};
};