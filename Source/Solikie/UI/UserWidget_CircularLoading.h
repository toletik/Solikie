// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UserWidget_CircularLoading.generated.h"

class UImage;
/**
 * 
 */
UCLASS()
class SOLIKIE_API UUserWidget_CircularLoading : public UUserWidget
{
	GENERATED_BODY()
	UMaterialInstanceDynamic* _matLoading;
public:
	void NativeOnInitialized() override;
	
	UPROPERTY(meta = (BindWidget))
	UImage* circularLoading {nullptr};

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UMaterialInterface* materialInterface;
	
	UFUNCTION()
	void UpdateMaterial(float newValue);
};
