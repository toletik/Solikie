// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Solikie/Gameplay/Entities/Units/Character_BaseUnit.h"
#include "UserWidget_SelectionButton.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDelegateUpdateSelectionInfoToHaveEntityInfo, UActorComponent_BaseEntity*, compEntity);

class ACharacter_BaseUnit;
/**
 * 
 */
UCLASS()
class SOLIKIE_API UUserWidget_SelectionButton : public UUserWidget 
{
	GENERATED_BODY()

public :

	virtual void NativeOnInitialized() override;
	
	void SetValue(UActorComponent_BaseEntity* entity);

	UFUNCTION()
	void OnClick();

	FDelegateUpdateSelectionInfoToHaveEntityInfo delegateUpdateSelectionInfoToHaveEntityInfo;
	
private:
	UActorComponent_BaseEntity* _actor;
	
	UPROPERTY(meta = (BindWidget))
	UImage* _image = {nullptr};

	UPROPERTY(meta = (BindWidget))
	UButton* _button = {nullptr};
};
