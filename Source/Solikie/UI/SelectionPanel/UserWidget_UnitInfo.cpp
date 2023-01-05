// Fill out your copyright notice in the Description page of Project Settings.


#include "UserWidget_UnitInfo.h"

#include "Components/Image.h"
#include "Solikie/Containers/DataAsset_UnitData.h"

void UUserWidget_UnitInfo::UpdateWithNewUnit(ACharacter_BaseUnit* unit)
{
	_icon->SetBrushFromTexture(unit->GetBaseEntityComponent()->icon);

	//Life
	_textLifeCurrent->SetText(FText::FromString(FString::SanitizeFloat(unit->GetBaseEntityComponent()->_lifePoints)));
	_textLifeMax->SetText(FText::FromString(FString::SanitizeFloat(unit->GetBaseEntityComponent()->_lifePointMax)));
	_textArmor->SetText(FText::FromString(FString::SanitizeFloat(unit->GetBaseEntityComponent()->_armor)));

	//Attack
	_textAttackDamage->SetText(FText::FromString(FString::SanitizeFloat(unit->GetStats()->AttackDamage)));
	_textAttackSpeed->SetText(FText::FromString(FString::SanitizeFloat(unit->GetStats()->AttackSpeed)));
	_textAttackRange->SetText(FText::FromString(FString::SanitizeFloat(unit->GetStats()->AttackRange)));

	//Repair
/*	_textRepairPerSecond->SetText(FText::FromString(FString::SanitizeFloat(unit->GetStats()->RepairPerSecond)));
	_textRepairSpeed->SetText(FText::FromString(FString::SanitizeFloat(unit->GetStats()->RepairSpeed)));
	_textRepairRange->SetText(FText::FromString(FString::SanitizeFloat(unit->GetStats()->RepairRange)));*/

	//Heal
	_textHealPower->SetText(FText::FromString(FString::SanitizeFloat(unit->GetStats()->healPower)));
	_textHealPower->SetText(FText::FromString(FString::SanitizeFloat(unit->GetStats()->HealSpeed)));
	
	//Others
	_textSpeed->SetText(FText::FromString(FString::SanitizeFloat(unit->GetStats()->MvtSpeedLinear)));
	_textCapturePower->SetText(FText::FromString(FString::SanitizeFloat(unit->GetStats()->CapturePower)));

	baseUnit = unit;
	baseUnit->GetBaseEntityComponent()->delegateUpdateHealth.AddDynamic(this,
			&UUserWidget_UnitInfo::UpdateLifePoint);
}

void UUserWidget_UnitInfo::UpdateLifePoint(float newLife)
{
	_textLifeCurrent->SetText(FText::FromString(FString::SanitizeFloat(baseUnit->GetBaseEntityComponent()->_lifePoints)));
}

void UUserWidget_UnitInfo::RemoveRefDelegate()
{
	if (!baseUnit)
		return;
	baseUnit->GetBaseEntityComponent()->delegateUpdateHealth.RemoveDynamic(this,
			&UUserWidget_UnitInfo::UpdateLifePoint);
	baseUnit = nullptr;
}