// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor_GameManager.h"

#include "Kismet/GameplayStatics.h"
#include "Solikie/Gameplay/Entities/Factories/Actor_BaseFactory.h"
#include "Solikie/Gameplay/Entities/Units/Character_BaseUnit.h"
#include "AIController.h"
#include "Solikie/Gameplay/Actor_PlayerManager.h"
#include "Solikie/Containers/Currencies.h"
#include "Solikie/Containers/Macro.h"
#include "Solikie/Gameplay/Actor_AIManager.h"
#include "Solikie/UI//UserWidget_EndScreen.h"

AActor_GameManager* AActor_GameManager::instance;

AActor_GameManager::AActor_GameManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}
void AActor_GameManager::BeginPlay()
{
	Super::BeginPlay();
	SetInstance(this);

	//Try to set up Ref if there are not valid
	{
		if (!_playerManagerRef)
			_playerManagerRef = Cast<AActor_PlayerManager>(UGameplayStatics::GetActorOfClass(GetWorld(), AActor_PlayerManager::StaticClass()));
		
		if (!_AiManagerRef)
			_AiManagerRef = Cast<AActor_AIManager>(UGameplayStatics::GetActorOfClass(GetWorld(), AActor_AIManager::StaticClass()));
	}

	
}
void AActor_GameManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AActor_GameManager::SetInstance(AActor_GameManager* newInstance)
{
	instance = newInstance;
}
AActor_GameManager* AActor_GameManager::GetInstance()
{
	return instance;
}

//Currency 
void AActor_GameManager::AddMoney(E_Team team, float amount)
{
	if (team == E_Team::PLAYER)
		_playerManagerRef->army.GetCurrencies().AddMoney(amount);
	else if (team == E_Team::IA1)
		_AiManagerRef->army.GetCurrencies().AddMoney(amount);
}
void AActor_GameManager::RemoveMoney(E_Team team, float amount)
{
	if (team == E_Team::PLAYER)
		_playerManagerRef->army.GetCurrencies().RemoveMoney(amount);
	else if (team == E_Team::IA1)
		_AiManagerRef->army.GetCurrencies().RemoveMoney(amount);
}
bool AActor_GameManager::HasEnoughMoney(E_Team team, float amount)
{
	if (team == E_Team::PLAYER)
		return _playerManagerRef->army.GetCurrencies().HasEnoughMoney(amount);
	else if (team == E_Team::IA1)
		return _AiManagerRef->army.GetCurrencies().HasEnoughMoney(amount);

	return false; 
}

//Armies
void AActor_GameManager::RegisterUnit(ACharacter_BaseUnit* newUnit)
{
	if (newUnit->GetBaseEntityComponent()->team == E_Team::PLAYER)
		_playerManagerRef->army.AddUnit(newUnit);
	else if (newUnit->GetBaseEntityComponent()->team == E_Team::IA1)
		_AiManagerRef->army.AddUnit(newUnit);
	
		
}
void AActor_GameManager::RegisterFactory(AActor_BaseFactory* newFactory)
{
	if (newFactory->GetBaseEntityComponent()->team == E_Team::PLAYER)
	{
		_playerManagerRef->army.AddFactory(newFactory);
		_AiManagerRef->playersFactories.Add(newFactory);
	}
	else if (newFactory->GetBaseEntityComponent()->team == E_Team::IA1)
		_AiManagerRef->army.AddFactory(newFactory);
	
}
void AActor_GameManager::RegisterObjectifPoint(AActor_ObjectifPoint* objectifPoint)
{
	_CapturePoints.Add(objectifPoint);	
}
void AActor_GameManager::UnRegisterUnit(ACharacter_BaseUnit* newUnit)
{
	if (newUnit->GetBaseEntityComponent()->team == E_Team::PLAYER)
	{
		_playerManagerRef->army.RemoveUnit(newUnit);

		_playerManagerRef->selectedEntity.Remove(newUnit->GetBaseEntityComponent());
		_playerManagerRef->delegateSelectionChange.Broadcast(_playerManagerRef->selectedEntity);
		_AiManagerRef->unitsSighted.Remove(newUnit);
		_AiManagerRef->BaseAggressors.Remove(newUnit);
	}
	else if (newUnit->GetBaseEntityComponent()->team == E_Team::IA1)
	{
		_AiManagerRef->army.RemoveUnit(newUnit);

	}
		

}
void AActor_GameManager::UnRegisterFactory(AActor_BaseFactory* newFactory)
{
	if (newFactory->GetBaseEntityComponent()->team == E_Team::PLAYER)
	{
		_playerManagerRef->army.RemoveFactory(newFactory);

		_playerManagerRef->selectedEntity.Remove(newFactory->GetBaseEntityComponent());
		_playerManagerRef->delegateSelectionChange.Broadcast(_playerManagerRef->selectedEntity);

		_AiManagerRef->factoriesSighted.Remove(newFactory);
		_AiManagerRef->playersFactories.Remove(newFactory);

		if(_playerManagerRef->army.GetFactoriesNb() == 0)
			EndGame(false);
	}
	else if (newFactory->GetBaseEntityComponent()->team == E_Team::IA1)
	{
		_AiManagerRef->army.RemoveFactory(newFactory);
		
		if (_AiManagerRef->army.GetFactoriesNb() == 0)
			EndGame(true);
	}
}

//AI
void AActor_GameManager::RegisterUnitSighted(E_Team teamWhoSaw, ACharacter_BaseUnit* newUnit)
{
	if (teamWhoSaw == E_Team::IA1)
	{
		if(_AiManagerRef->unitsSighted.Contains(newUnit))
			_AiManagerRef->unitsSighted[newUnit] += 1;
		else
		{
			_AiManagerRef->unitsSighted.Add(TPair<ACharacter_BaseUnit*, int>{newUnit, 1});
			_AiManagerRef->BiggestNbOfPlayerUnitsSighted = FMath::Max(_AiManagerRef->BiggestNbOfPlayerUnitsSighted, _AiManagerRef->unitsSighted.Num());
		}
	}
	else if (teamWhoSaw == E_Team::PLAYER)
	{
		if(_playerManagerRef->unitsSighted.Contains(newUnit))
			_playerManagerRef->unitsSighted[newUnit] += 1;
		else
		{
			_playerManagerRef->unitsSighted.Add(TPair<ACharacter_BaseUnit*, int>{newUnit, 1});
			newUnit->SetActorHiddenInGame(false);
		}
	}
}
void AActor_GameManager::RegisterFactorySighted(E_Team teamWhoSaw, AActor_BaseFactory* newFactory)
{
	if (teamWhoSaw == E_Team::IA1)
	{
		if (_AiManagerRef->factoriesSighted.Contains(newFactory))
			_AiManagerRef->factoriesSighted[newFactory] += 1;
		else
			_AiManagerRef->factoriesSighted.Add(TPair<AActor_BaseFactory*, int>{ newFactory, 1 });
	}
	else if (teamWhoSaw == E_Team::PLAYER)
	{
		if(_playerManagerRef->factoriesSighted.Contains(newFactory))
			_playerManagerRef->factoriesSighted[newFactory] += 1;
		else
		{
			_playerManagerRef->factoriesSighted.Add(TPair<AActor_BaseFactory*, int>{newFactory, 1});
			newFactory->SetActorHiddenInGame(false);
		}
	}
}
void AActor_GameManager::UnRegisterUnitSighted(E_Team teamWhoSaw, ACharacter_BaseUnit* newUnit)
{
	if (teamWhoSaw == E_Team::IA1)
	{
		if (_AiManagerRef->unitsSighted.Contains(newUnit))
		{
			_AiManagerRef->unitsSighted[newUnit] -= 1;

			if(_AiManagerRef->unitsSighted[newUnit] <= 0)
				_AiManagerRef->unitsSighted.Remove(newUnit);
		}
	}
	else if (teamWhoSaw == E_Team::PLAYER)
	{
		if (_playerManagerRef->unitsSighted.Contains(newUnit))
		{
			_playerManagerRef->unitsSighted[newUnit] -= 1;

			if(_playerManagerRef->unitsSighted[newUnit] <= 0)
			{
				_playerManagerRef->unitsSighted.Remove(newUnit);
				newUnit->SetActorHiddenInGame(true);
			}
		}
	}
}
void AActor_GameManager::UnRegisterFactorySighted(E_Team teamWhoSaw, AActor_BaseFactory* newFactory)
{
	if (teamWhoSaw == E_Team::IA1)
	{
		if (_AiManagerRef->factoriesSighted.Contains(newFactory))
		{
			_AiManagerRef->factoriesSighted[newFactory] -= 1;

			if (_AiManagerRef->factoriesSighted[newFactory] <= 0)
				_AiManagerRef->factoriesSighted.Remove(newFactory);
		}
	}
	else if (teamWhoSaw == E_Team::PLAYER)
	{
		if (_playerManagerRef->factoriesSighted.Contains(newFactory))
		{
			_playerManagerRef->factoriesSighted[newFactory] -= 1;

			if(_playerManagerRef->factoriesSighted[newFactory] <= 0)
			{
				_playerManagerRef->factoriesSighted.Remove(newFactory);
				newFactory->SetActorHiddenInGame(true);
			}
		}
	}
}
void AActor_GameManager::RegisterBaseAgressor(ACharacter_BaseUnit* aggressor)
{
	_AiManagerRef->BaseAggressors.AddUnique(aggressor);
}

//CapturePoints
bool AActor_GameManager::HasTeamAllCapturePoints(E_Team team)
{
	for (AActor_ObjectifPoint* currentPoint : _CapturePoints)
		if(currentPoint->_team != team)
			return false;

	return true;
}
AActor_ObjectifPoint* AActor_GameManager::GetFirstEnemyCapturePoint(E_Team team)
{
	for (AActor_ObjectifPoint* currentPoint : _CapturePoints)
		if (currentPoint->_team != team)
			return currentPoint;

	return nullptr;
}

//Endgame
void AActor_GameManager::EndGame(bool playerWon)
{
	//Freeze Time
	UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 0.0f);

	//Display endScreen UI
	UUserWidget_EndScreen* endScreen = CreateWidget<UUserWidget_EndScreen>(GetGameInstance(), EndScreenTemplate);
	if (endScreen)
	{
		endScreen->AddToViewport(200);
		endScreen->Init(playerWon);
		
		FInputModeUIOnly mode;
		mode.SetWidgetToFocus(endScreen->TakeWidget());
		mode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);

		APlayerController* playerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
		playerController->SetInputMode(mode);
		playerController->SetShowMouseCursor(true);
	}
}

