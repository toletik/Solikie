// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UserWidget_MainHud.generated.h"

class AActor_PlayerManager;
class UUserWidget_Minimap;
class UUserWidget_SelectionInfo;
class UUserWidget_OrderPanel;
class UUserWidget_PlayerFund;

UCLASS()
class SOLIKIE_API UUserWidget_MainHud : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION()
	void Init(AActor_PlayerManager* playerManager);

	UPROPERTY(meta = (BindWidget))
	UUserWidget_Minimap* _Minimap {nullptr};
	UPROPERTY(meta = (BindWidget))
	UUserWidget_SelectionInfo* _SelectionInfo {nullptr};
	UPROPERTY(meta = (BindWidget))
	UUserWidget_OrderPanel* _OrderPanel {nullptr};
	UPROPERTY(meta = (BindWidget))
	UUserWidget_PlayerFund* _PlayerFund {nullptr};
	
};
