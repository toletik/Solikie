// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "Solikie/Gameplay/Entities/Units/Character_BaseUnit.h"
#include "UserWidget_UnitInfo.generated.h"

class UImage;
/**
 * 
 */
UCLASS()
class SOLIKIE_API UUserWidget_UnitInfo : public UUserWidget
{
	GENERATED_BODY()

	UPROPERTY(meta = (BindWidget))
	UImage* _icon {nullptr};

	//Life
	UPROPERTY(meta = (BindWidget))
	UTextBlock* _textLifeCurrent {nullptr};
	UPROPERTY(meta = (BindWidget))
	UTextBlock* _textLifeMax {nullptr};
	UPROPERTY(meta = (BindWidget))
	UTextBlock* _textArmor {nullptr};

	//Attack
	UPROPERTY(meta = (BindWidget))
	UTextBlock* _textAttackDamage {nullptr};
	UPROPERTY(meta = (BindWidget))
	UTextBlock* _textAttackSpeed {nullptr};
	UPROPERTY(meta = (BindWidget))
	UTextBlock* _textAttackRange {nullptr};

	//Repair
	UPROPERTY(meta = (BindWidget))
	UTextBlock* _textRepairPerSecond {nullptr};
	UPROPERTY(meta = (BindWidget))
	UTextBlock* _textRepairSpeed {nullptr};
	UPROPERTY(meta = (BindWidget))
	UTextBlock* _textRepairRange {nullptr};

	//Heal
	UPROPERTY(meta = (BindWidget))
	UTextBlock* _textHealPower {nullptr};
	UPROPERTY(meta = (BindWidget))
	UTextBlock* _textHealSpeed {nullptr};
	
	//Others
	UPROPERTY(meta = (BindWidget))
	UTextBlock* _textSpeed {nullptr};
	UPROPERTY(meta = (BindWidget))
	UTextBlock* _textCapturePower {nullptr};

	UPROPERTY()
	ACharacter_BaseUnit* baseUnit {nullptr};
	
	
public:
	UFUNCTION()
	void UpdateWithNewUnit(ACharacter_BaseUnit* unit);

	UFUNCTION()
	void UpdateLifePoint(float newLife);

	void RemoveRefDelegate();
};
