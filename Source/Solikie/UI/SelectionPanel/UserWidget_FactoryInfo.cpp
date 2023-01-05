// Fill out your copyright notice in the Description page of Project Settings.


#include "UserWidget_FactoryInfo.h"

#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Solikie/Containers/DataAsset_UnitData.h"

void UUserWidget_FactoryInfo::UpdateWithNewFactory(AActor_BaseFactory* factory)
{
	_icon->SetBrushFromTexture(factory->GetBaseEntityComponent()->icon);

	//Life
	_textLifeCurrent->SetText(FText::FromString(FString::SanitizeFloat(factory->GetBaseEntityComponent()->_lifePoints)));
	_textLifeMax->SetText(FText::FromString(FString::SanitizeFloat(factory->GetBaseEntityComponent()->_lifePointMax)));

	//Init delegate
	factory->delegateUpdateLoadingWidget.AddDynamic(this,
			&UUserWidget_FactoryInfo::UpgradeProgressBar);
	factory->delegateUpdateSelectionProgressBar.AddUniqueDynamic(this,
			&UUserWidget_FactoryInfo::UpgradeElementQueue);
	factory->GetBaseEntityComponent()->delegateUpdateHealth.AddDynamic(this,
			&UUserWidget_FactoryInfo::UpdateLifePoint);
	
	_baseFactory = factory;
	_progressBar->SetPercent(0.0f);

	UpgradeElementQueue();
}

void UUserWidget_FactoryInfo::UpgradeProgressBar(float newValue)
{
	_progressBar->SetPercent(newValue);
}

void UUserWidget_FactoryInfo::UpdateLifePoint(float newLife)
{
	_textLifeCurrent->SetText(FText::FromString(FString::SanitizeFloat(_baseFactory->GetBaseEntityComponent()->_lifePoints)));
}

void UUserWidget_FactoryInfo::UpgradeElementQueue()
{
	//Queue
	if (_baseFactory->GetElementInQueue(0))
	{
		_first->SetBrushFromTexture(_baseFactory->GetElementInQueue(0)->Icon);
		_firstButton->OnClicked.AddUniqueDynamic(this, &UUserWidget_FactoryInfo::FirstButton);
		_firstButton->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		_first->SetBrushFromTexture(textureDefaultQueue);
		_firstButton->SetVisibility(ESlateVisibility::Hidden);
	}

	if (_baseFactory->GetElementInQueue(1))
	{
		_second->SetBrushFromTexture(_baseFactory->GetElementInQueue(1)->Icon);
		_secondButton->OnClicked.AddUniqueDynamic(this, &UUserWidget_FactoryInfo::SecondButton);
		_secondButton->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		_second->SetBrushFromTexture(textureDefaultQueue);
		_secondButton->SetVisibility(ESlateVisibility::Hidden);
	}

	if (_baseFactory->GetElementInQueue(2))
	{
		_third->SetBrushFromTexture(_baseFactory->GetElementInQueue(2)->Icon);
		_thirdButton->OnClicked.AddUniqueDynamic(this, &UUserWidget_FactoryInfo::ThirdButton);
		_thirdButton->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		_third->SetBrushFromTexture(textureDefaultQueue);
		_thirdButton->SetVisibility(ESlateVisibility::Hidden);
	}

	if (_baseFactory->GetElementInQueue(3))
	{
		_fourth->SetBrushFromTexture(_baseFactory->GetElementInQueue(3)->Icon);
		_fourthButton->OnClicked.AddUniqueDynamic(this, &UUserWidget_FactoryInfo::FourthButton);
		_fourthButton->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		_fourth->SetBrushFromTexture(textureDefaultQueue);
		_fourthButton->SetVisibility(ESlateVisibility::Hidden);
	}

	if (_baseFactory->GetElementInQueue(4))
	{
		_fifth->SetBrushFromTexture(_baseFactory->GetElementInQueue(4)->Icon);
		_fifthButton->OnClicked.AddUniqueDynamic(this, &UUserWidget_FactoryInfo::FifthButton);
		_fifthButton->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		_fifth->SetBrushFromTexture(textureDefaultQueue);
		_fifthButton->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UUserWidget_FactoryInfo::RemoveRefDelegate()
{//Call when selection change
	if (!_baseFactory)
		return;
	_baseFactory->delegateUpdateLoadingWidget.RemoveDynamic(this, &UUserWidget_FactoryInfo::UpgradeProgressBar);
	_baseFactory->delegateUpdateSelectionProgressBar.RemoveDynamic(this, &UUserWidget_FactoryInfo::UpgradeElementQueue);
	_baseFactory->GetBaseEntityComponent()->delegateUpdateHealth.RemoveDynamic(this, &UUserWidget_FactoryInfo::UpdateLifePoint);
	/*_firstButton->OnClicked.Clear();
	_secondButton->OnClicked.Clear();
	_thirdButton->OnClicked.Clear();
	_fourthButton->OnClicked.Clear();
	_fifthButton->OnClicked.Clear();*/
	_baseFactory = nullptr;
}

void UUserWidget_FactoryInfo::FirstButton()
{
	_baseFactory->RemoveUnitFromQueue(0);
}

void UUserWidget_FactoryInfo::SecondButton()
{
	_baseFactory->RemoveUnitFromQueue(1);
}

void UUserWidget_FactoryInfo::ThirdButton()
{
	_baseFactory->RemoveUnitFromQueue(2);
}

void UUserWidget_FactoryInfo::FourthButton()
{
	_baseFactory->RemoveUnitFromQueue(3);
}

void UUserWidget_FactoryInfo::FifthButton()
{
	_baseFactory->RemoveUnitFromQueue(4);
}