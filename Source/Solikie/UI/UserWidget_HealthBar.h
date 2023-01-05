// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/ProgressBar.h"
#include "UserWidget_HealthBar.generated.h"

/**
 * 
 */
UCLASS()
class SOLIKIE_API UUserWidget_HealthBar : public UUserWidget
{
	GENERATED_BODY()

	UPROPERTY(meta = (BindWidget))
	UProgressBar* _progressBar;

public:

	UFUNCTION()
	void UpdateProgressBar(float newValue);
};
