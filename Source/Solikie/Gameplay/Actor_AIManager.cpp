// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor_AIManager.h"

#include "Solikie/Gameplay/Actor_GameManager.h"
#include "Solikie/Gameplay/Actor_PlayerManager.h"
#include "Entities/Factories/Actor_BaseFactory.h"
#include "Entities/Units/Character_BaseUnit.h"
#include "Solikie/Gameplay/Map/Actor_MapManager.h"
#include "Solikie/Containers/DataAsset_AIData.h"
#include "Solikie/Containers/DataAsset_UnitData.h"
#include "Solikie/Containers/DataAsset_FactoryData.h"
#include "Solikie/Containers/Macro.h"
#include "NavigationSystem.h"
#include "Kismet/GameplayStatics.h"

void AActor_AIManager::BeginPlay()
{
	Super::BeginPlay();

	ensure(AIPersonality);

	if (!_map)
		_map = Cast<AActor_MapManager>(UGameplayStatics::GetActorOfClass(GetWorld(), AActor_MapManager::StaticClass()));

	//Money
	army.GetCurrencies().SetCurrentMoney(AIPersonality->MoneyAtStart);
	GetWorld()->GetTimerManager().SetTimer(_timerHandleMoney, this, &AActor_AIManager::TimerAddMoney, AIPersonality->MoneyAddRate, true);

	//Behaviors
	if(AIPersonality->StartUpDelay == 0.0f)
		StartBehaviorTreesLoop();
	else
		GetWorld()->GetTimerManager().SetTimer(_TimerRefreshStatus, this, &AActor_AIManager::StartBehaviorTreesLoop, AIPersonality->StartUpDelay, false);
		
}

//Helpers
void AActor_AIManager::AllUnitsAttackTo(FVector destination)
{
	for(ACharacter_BaseUnit* currentUnit : army.GetUnits())
		if(currentUnit)
			currentUnit->AttackTo(destination);
}
void AActor_AIManager::AllUnitsMoveTo(FVector destination)
{
	for (ACharacter_BaseUnit* currentUnit : army.GetUnits())
		if (currentUnit)
			currentUnit->MoveTo(destination);
}
float AActor_AIManager::UnitsSightedPowerLevel()
{
	float total = 0.0f;

	for(TPair<ACharacter_BaseUnit*, int>& entry : unitsSighted)
	{
		if (entry.Key && entry.Key->GetStats())
			total += entry.Key->GetStats()->PowerLevel;
	}
			

	return total;
}

void AActor_AIManager::StartBehaviorTreesLoop()
{
	GetWorld()->GetTimerManager().SetTimer(_TimerRefreshStatus, this, &AActor_AIManager::RunBehaviorTrees, AIPersonality->RefreshRate, true);
}
void AActor_AIManager::RunBehaviorTrees()
{
	RunBehaviorTreeUnits();
	RunBehaviorTreeMoney();
}

//Units Allocation
void AActor_AIManager::RunBehaviorTreeUnits()
{
	//Main tree
	if (NeedToDefendBase())
	{
		DefendBase();
	}
	else if (NeedToGatherForces())
	{
		GatherForces();
	}
	else if (NeedToCapturePoint())
	{
		CapturePoint();
	}
	else if (CanAttackPlayer())
	{
		AttackLocation();
	}

	//Parallel Task
	/*if (NeedToScout())
	{
		Scout();
	}*/


}
//Units Tasks
void AActor_AIManager::DefendBase()
{
	print("Task DefendBase");

	AllUnitsAttackTo(BaseAggressors[0]->GetActorLocation());
}
void AActor_AIManager::GatherForces()
{
	print("Task GatherForces");

	AllUnitsMoveTo(army.GetFactories()[0]->GetActorLocation());
}
void AActor_AIManager::CapturePoint()
{
	print("Task CapturePoint");

	AllUnitsAttackTo(AActor_GameManager::GetInstance()->GetFirstEnemyCapturePoint(E_Team::IA1)->GetActorLocation());
}
void AActor_AIManager::AttackLocation()
{
	print("Task AttackLocation");
	//AllUnitsAttackTo(factoriesSighted.begin()->Key->GetActorLocation());

	AllUnitsAttackTo(playersFactories[0]->GetActorLocation());
}
void AActor_AIManager::Scout()
{
	print("Task Scout");

	//if(army.GetUnits().Num() > 0)
	//	army.GetUnits()[0]->MoveTo(_playerBasePossiblePosition);
}
//Units Decorators
bool AActor_AIManager::NeedToDefendBase()
{
	//Clean Aggressors if not in sight anymore
	for (int i = BaseAggressors.Num() - 1; i >= 0; --i)
		if (BaseAggressors[i] && !unitsSighted.Contains(BaseAggressors[i]))
			BaseAggressors.RemoveAt(i);

	return (BaseAggressors.Num() > 0);
}
bool AActor_AIManager::NeedToGatherForces()
{
	//To prevent AI army to flee when one unit is down after it finished gathering forces
	if(army.GetUnitsNb() >= BiggestNbOfPlayerUnitsSighted)
		BiggestNbOfPlayerUnitsSightedOvercomed = BiggestNbOfPlayerUnitsSighted;

	return (army.GetUnitsNb() < BiggestNbOfPlayerUnitsSighted && BiggestNbOfPlayerUnitsSighted != BiggestNbOfPlayerUnitsSightedOvercomed);
}
bool AActor_AIManager::NeedToCapturePoint()
{
	return (!AActor_GameManager::GetInstance()->HasTeamAllCapturePoints(E_Team::IA1) && army.GetUnits().Num() >= AIPersonality->UnitsMinNb_CapturePoint);
}
bool AActor_AIManager::CanAttackPlayer()
{
	//Use personality for decision
	//return (army.powerLevel >= UnitsSightedPowerLevel() &&	factoriesSighted.Num() > 0);
	return (playersFactories.Num() > 0 && army.GetUnits().Num() >= AIPersonality->UnitsMinNb_AttackPlayer);
}
bool AActor_AIManager::NeedToScout()
{
	return false;
	//return (factoriesSighted.Num() == 0);
}


//Money Allocation
void AActor_AIManager::RunBehaviorTreeMoney()
{
	//Main Tree
	if (NeedToDevelopArmy())
	{
		DevelopArmy();
	}
	else if (NeedToDevelopBase())
	{
		DevelopBase();
	}
}
//Money Tasks
void AActor_AIManager::DevelopArmy()
{
	print("Task DevelopArmy");

	//iterate on all factories, add to the one with less queue
	AActor_BaseFactory* bestFactoryInWhichAddToQueue = nullptr;

	for (AActor_BaseFactory* currentFactory : army.GetFactories())
	{
		if(!bestFactoryInWhichAddToQueue)
			bestFactoryInWhichAddToQueue = currentFactory;
		else if(currentFactory->GetQueueSize() < bestFactoryInWhichAddToQueue->GetQueueSize())
			bestFactoryInWhichAddToQueue = currentFactory;
	}

	//Choose Unit to build at random
	if(bestFactoryInWhichAddToQueue)
		bestFactoryInWhichAddToQueue->AddUnitToQueue(FMath::RandRange(0, bestFactoryInWhichAddToQueue->GetStats()->UnitsToProduct.Num() - 1 ));
	
	
}
void AActor_AIManager::DevelopBase()
{
	print("Task DevelopBase");

	//Choose Factory to build at random
	UDataAsset_BuildableFactoryData* factoryToBuild = AIPersonality->PossibleFactories[FMath::RandRange(0, AIPersonality->PossibleFactories.Num() - 1)];

	if (!army.GetCurrencies().HasEnoughMoney(factoryToBuild->Cost))
	{
		print("Not Enough Minerals");
		return;
	}


	AActor_BaseFactory* lastFactory = army.GetFactories().Last();
	FVector spawnLocation = UNavigationSystemV1::GetRandomPointInNavigableRadius(GetWorld(), lastFactory->GetActorLocation(), lastFactory->GetStats()->radiusSight);

	if (_map->IsPositionInsideMap(spawnLocation) && _map->IsBuildingValid(spawnLocation, factoryToBuild->_TilesOccupied))
	{
		//Spawn
		AActor_BaseFactory* Factory = GetWorld()->SpawnActor<AActor_BaseFactory>(factoryToBuild->Template, FVector::ZeroVector, FRotator::ZeroRotator);
		Factory->SetActorScale3D(FVector(factoryToBuild->_TilesOccupied.X, factoryToBuild->_TilesOccupied.Y, 1.0));

		//Set Location
		Factory->SetActorLocation(_map->GetCenterOfBuilding(spawnLocation, factoryToBuild->_TilesOccupied));

		//Register 
		Factory->SetTeam(team);
		AActor_GameManager::GetInstance()->RemoveMoney(team, factoryToBuild->Cost);

		//Minimap Icons Scaling Setup
		UMinimapIconPaperSprite* minimapIcon = Factory->FindComponentByClass<UMinimapIconPaperSprite>();
		USelectionPaperSprite* selectionSprite = Factory->FindComponentByClass<USelectionPaperSprite>();
		UMeshComponent* mesh = Factory->FindComponentByClass<UMeshComponent>();
		if (mesh && minimapIcon)
		{
			float scale = (mesh->Bounds.BoxExtent.X + mesh->Bounds.BoxExtent.Y) / 80.f;
			if (minimapIcon)
			{
				minimapIcon->SetWorldScale3D(FVector(scale, 1, scale));
				minimapIcon->SetRelativeLocation(FVector(0, 0, 200));
				if (team == E_Team::PLAYER)
					minimapIcon->SetAsBlue();
				else
					minimapIcon->SetAsRed();
			}
			if (selectionSprite)
				selectionSprite->SetWorldScale3D(FVector(scale, 1, scale));
		}
	}

}
//Money Decorators
bool AActor_AIManager::NeedToDevelopArmy()
{
	//Check units in factories queue
	int trueNbOfUnits = army.GetUnitsNb();
	for (AActor_BaseFactory* factory : army.GetFactories())
		if (factory)
			trueNbOfUnits += factory->GetQueueSize();

	int MinNbOfUnitWanted = (!AActor_GameManager::GetInstance()->HasTeamAllCapturePoints(E_Team::IA1) ? AIPersonality->UnitsMinNb_CapturePoint : AIPersonality->UnitsMinNb_AttackPlayer);
	MinNbOfUnitWanted = FMath::Max(MinNbOfUnitWanted, BiggestNbOfPlayerUnitsSighted);


	return (trueNbOfUnits < MinNbOfUnitWanted && trueNbOfUnits < AIPersonality->UnitsMaxNb);
}
bool AActor_AIManager::NeedToDevelopBase()
{
	return (army.GetFactoriesNb() < AIPersonality->FactoriesMaxNb);
}

void AActor_AIManager::TimerAddMoney()
{
	army.GetCurrencies().AddMoney(AIPersonality->MoneyAddAmmount);
}