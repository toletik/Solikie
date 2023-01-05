// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UserWidget_OrderPanel.generated.h"

class APawn_Player;
class UUserWidget_OrderTile;
class UUniformGridPanel;
class UWidgetSwitcher;
class UActorComponent_BaseEntity;
class AActor_PlayerManager;

UCLASS()
class SOLIKIE_API UUserWidget_OrderPanel : public UUserWidget
{
	GENERATED_BODY()


public:
	void Init(AActor_PlayerManager* playerManager);

private:

	UPROPERTY(meta = (BindWidget))
	UWidgetSwitcher* _GridSwitcher {nullptr};
	
	//Unit
	UPROPERTY(meta = (BindWidget))
	UUniformGridPanel* _GridPanelUnit {nullptr};
	
	//Factory
	UPROPERTY(meta = (BindWidget))
	UUniformGridPanel* _GridPanelFactory {nullptr};

	//Default
	UPROPERTY(meta = (BindWidget))
	UUniformGridPanel* _GridPanelDefault {nullptr};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Initialization, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UUserWidget_OrderTile> _orderTileTemplate { nullptr };

	UFUNCTION()
	void OnSelectionChange(TArray<UActorComponent_BaseEntity*> entities);


//Camera Related
public:

	virtual void NativeOnInitialized() override;
	virtual void NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent) override;

private:

	APawn_Player* _player;



};
