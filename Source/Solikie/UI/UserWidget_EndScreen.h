// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UserWidget_EndScreen.generated.h"


class UWidgetSwitcher;

UCLASS()
class SOLIKIE_API UUserWidget_EndScreen : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION()
	void Init(bool playerWon);
	
	UPROPERTY(meta = (BindWidget))
	UWidgetSwitcher* TextSwitcher {nullptr};
	
};
