// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Entities/ActorComponent_BaseEntity.h"
#include "Solikie/Containers/Army.h"
#include "Solikie/Containers/Team.h"
#include "Actor_AIManager.generated.h"

class AActor_BaseFactory;
class AActor_MapManager;
class ACharacter_BaseUnit;
class UDataAsset_AIData;


UCLASS()
class SOLIKIE_API AActor_AIManager : public AActor
{
	GENERATED_BODY()

public:
	UPROPERTY()
	FArmy army;
	
	TMap<ACharacter_BaseUnit*, int> unitsSighted;
	TMap<AActor_BaseFactory*, int> factoriesSighted;

	TArray<AActor_BaseFactory*> playersFactories;	
	TArray<ACharacter_BaseUnit*> BaseAggressors;

	int BiggestNbOfPlayerUnitsSighted {0};

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Planner, meta = (AllowPrivateAccess = "true"))
	UDataAsset_AIData* AIPersonality {nullptr};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Initialization, meta = (AllowPrivateAccess = "true"))
	AActor_MapManager* _map {nullptr};

	E_Team team = E_Team::IA1;

	//To prevent AI army to flee when one unit is down after it finished gathering forces
	int BiggestNbOfPlayerUnitsSightedOvercomed {0};


	virtual void BeginPlay() override;

	//Helpers
	void AllUnitsAttackTo(FVector destination);
	void AllUnitsMoveTo(FVector destination);
	float UnitsSightedPowerLevel();

/************************************************************************/
/*                      HTN Planner Related                             */
/************************************************************************/

	FTimerHandle _TimerRefreshStatus;

	UFUNCTION()
	void StartBehaviorTreesLoop();
	UFUNCTION()
	void RunBehaviorTrees();

	//Units Allocation
	void RunBehaviorTreeUnits();
	//Units Tasks
	void DefendBase();
	void GatherForces();
	void CapturePoint();
	void AttackLocation();
	void Scout();
	//Units Decorators
	bool NeedToDefendBase();
	bool NeedToGatherForces();
	bool NeedToCapturePoint();
	bool CanAttackPlayer();
	bool NeedToScout();


	//Money Allocation
	void RunBehaviorTreeMoney();
	//Money Tasks
	void DevelopArmy();
	void DevelopBase();
	//Money Decorators
	bool NeedToDevelopArmy();
	bool NeedToDevelopBase();

/************************************************************************/
/*                              Money                                   */
/************************************************************************/
	FTimerHandle _timerHandleMoney;

	UFUNCTION()
	void TimerAddMoney();
};
