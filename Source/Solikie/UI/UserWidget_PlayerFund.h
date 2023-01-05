// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "Solikie/Gameplay/Actor_PlayerManager.h"
#include "UserWidget_PlayerFund.generated.h"


UCLASS()
class SOLIKIE_API UUserWidget_PlayerFund : public UUserWidget
{
	GENERATED_BODY()


public:
	UFUNCTION()
	void Init(AActor_PlayerManager* playerManager); 

private:
	UFUNCTION()
	void EditMoneyTextOnChange(float money);
	UFUNCTION()
	void EditWorkersTextOnChange(int workersNb);

	UPROPERTY(meta = (BindWidget))
	UTextBlock* _moneyText {nullptr};
	UPROPERTY(meta = (BindWidget))
	UTextBlock* _workersText {nullptr};

};
