// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UserWidget_ProgressObjectifPoint.generated.h"

class UProgressBar;
/**
 * 
 */
UCLASS()
class SOLIKIE_API UUserWidget_ProgressObjectifPoint : public UUserWidget
{
	GENERATED_BODY()
	
	UPROPERTY(meta = (BindWidget))
	UProgressBar* _progressBar;

public:

	UFUNCTION()
	void UpdateProgressBar(float newValue);
};
