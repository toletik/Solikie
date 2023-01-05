// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Solikie/Gameplay/Entities/Factories/Actor_BaseFactory.h"
#include "UserWidget_FactoryInfo.generated.h"

class UTextBlock;
class ACharacter_BaseUnit;
/**
 * 
 */
UCLASS()
class SOLIKIE_API UUserWidget_FactoryInfo : public UUserWidget
{
	GENERATED_BODY()

	UPROPERTY(meta = (BindWidget))
	UImage* _icon {nullptr};

	//Life
	UPROPERTY(meta = (BindWidget))
	UTextBlock* _textLifeCurrent {nullptr};
	UPROPERTY(meta = (BindWidget))
	UTextBlock* _textLifeMax {nullptr};

	//Queue
	UPROPERTY(meta = (BindWidget))
	UImage* _first {nullptr};
	UPROPERTY(meta = (BindWidget))
	UButton* _firstButton {nullptr};
	
	UPROPERTY(meta = (BindWidget))
	UProgressBar* _progressBar;

	UPROPERTY(meta = (BindWidget))
	UImage* _second {nullptr};
	UPROPERTY(meta = (BindWidget))
	UButton* _secondButton {nullptr};
	UPROPERTY(meta = (BindWidget))
	UImage* _third {nullptr};
	UPROPERTY(meta = (BindWidget))
	UButton* _thirdButton {nullptr};
	UPROPERTY(meta = (BindWidget))
	UImage* _fourth {nullptr};
	UPROPERTY(meta = (BindWidget))
	UButton* _fourthButton {nullptr};
	UPROPERTY(meta = (BindWidget))
	UImage* _fifth {nullptr};
	UPROPERTY(meta = (BindWidget))
	UButton* _fifthButton {nullptr};

	AActor_BaseFactory* _baseFactory {nullptr};

	UFUNCTION()
	void FirstButton();
	UFUNCTION()
	void SecondButton();
	UFUNCTION()
	void ThirdButton();
	UFUNCTION()
	void FourthButton();
	UFUNCTION()
	void FifthButton();
	
public:
	UFUNCTION()
	void UpdateWithNewFactory(AActor_BaseFactory* factory);

	UFUNCTION()
	void UpgradeProgressBar(float newValue);

	UFUNCTION()
	void UpgradeElementQueue();

	UFUNCTION()
	void UpdateLifePoint(float newLife);

	void RemoveRefDelegate();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* textureDefaultQueue;
};
