// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor_BaseFactory.h"

#include "Components/StaticMeshComponent.h"
#include "Solikie/Containers/DataAsset_FactoryData.h"
#include "Solikie/Containers/DataAsset_UnitData.h"
#include "Solikie/Gameplay/Entities/Units/Character_BaseUnit.h"
#include "NavigationSystem.h"
#include "Components/WidgetComponent.h"
#include "Engine/StaticMeshActor.h"
#include "Solikie/Containers/Macro.h"
#include "Solikie/Containers/Define.h"
#include "Solikie/Gameplay/Actor_GameManager.h"
#include "Solikie/Gameplay/Map/Actor_MapManager.h"

AActor_BaseFactory::AActor_BaseFactory()
{
	PrimaryActorTick.bCanEverTick = true;

	_mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	SetRootComponent(_mesh);

	BaseEntityComponent = CreateDefaultSubobject<UActorComponent_BaseEntity>(TEXT("BaseUnitComponent"));

	_widgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("CircularLoadingWidgetComponent"));
	_widgetComponent->AttachTo(RootComponent);
	_widgetComponent->SetRelativeLocation(FVector(0, 0, 110));

	_sightCapsule = CreateDefaultSubobject<UCapsuleComponent>(FName("_sightCapsule"));
	_sightCapsule->SetupAttachment(RootComponent);
	
	Tags.Add("Factory");
}
void AActor_BaseFactory::BeginPlay()
{
	Super::BeginPlay();

	UUserWidget_CircularLoading* circularLoading = Cast<UUserWidget_CircularLoading>(_widgetComponent->GetUserWidgetObject());
	if (circularLoading)
	{
		delegateUpdateLoadingWidget.AddDynamic(circularLoading,
			&UUserWidget_CircularLoading::UpdateMaterial);
	}

	//TEMP
	if (GetBaseEntityComponent()->team != E_Team::NONE)
	{
		AActor_GameManager::GetInstance()->RegisterFactory(this);	
		
		_sightCapsule->OnComponentBeginOverlap.AddDynamic(this, &AActor_BaseFactory::OnSightDetectionCapsuleBeginOverlap);
		_sightCapsule->OnComponentEndOverlap.AddDynamic(this, &AActor_BaseFactory::OnSightDetectionCapsuleEndOverlap);
		_sightCapsule->SetCapsuleHalfHeight(_factoryStat->radiusSight * 2 + OFFSET_Z_FOR_FLYING);
		_sightCapsule->SetCapsuleRadius(_factoryStat->radiusSight * 2);

		if (GetBaseEntityComponent()->team == E_Team::PLAYER)
		{
			_fogOfWar = NewObject<UActorComponent_SignInFogOfWar>(this, _classFogOfWar);
			if (_fogOfWar)
			{
				_fogOfWar->radius = _factoryStat->radiusSight / AActor_MapManager::maxRange;
				_fogOfWar->RegisterComponent();
			}
				
		}
		else if (GetBaseEntityComponent()->team == E_Team::IA1)
			SetActorHiddenInGame(true);
	}

}
void AActor_BaseFactory::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (GetWorld()->GetTimerManager().IsTimerActive(_timerHandle))
	{
		_timeCoolDown += DeltaTime;
		delegateUpdateLoadingWidget.Broadcast(_timeCoolDown / _unitsQueue[0]->Time);
	}
}

void AActor_BaseFactory::SetTeam(E_Team newTeam)
{
	BaseEntityComponent->team = newTeam;

	//Set SightCapsule once we have a team to avoid being none and having everyone as a treat
	if (GetBaseEntityComponent()->team != E_Team::NONE)
	{
		AActor_GameManager::GetInstance()->RegisterFactory(this);

		_sightCapsule->OnComponentBeginOverlap.AddDynamic(this, &AActor_BaseFactory::OnSightDetectionCapsuleBeginOverlap);
		_sightCapsule->OnComponentEndOverlap.AddDynamic(this, &AActor_BaseFactory::OnSightDetectionCapsuleEndOverlap);
		_sightCapsule->SetCapsuleHalfHeight(_factoryStat->radiusSight * 2 + OFFSET_Z_FOR_FLYING);
		_sightCapsule->SetCapsuleRadius(_factoryStat->radiusSight * 2);

		if (GetBaseEntityComponent()->team == E_Team::PLAYER)
		{
			_fogOfWar = NewObject<UActorComponent_SignInFogOfWar>(this, _classFogOfWar);
			if (_fogOfWar)
			{
				_fogOfWar->radius = _factoryStat->radiusSight / AActor_MapManager::maxRange;
				_fogOfWar->RegisterComponent();
				
			}
		}
		else if (GetBaseEntityComponent()->team == E_Team::IA1)
		{
			SetActorHiddenInGame(true);
			TArray<AActor*> outActor;
			_sightCapsule->GetOverlappingActors(outActor);
			for (AActor* actor : outActor)
			{
				if (ACharacter_BaseUnit* unit = Cast<ACharacter_BaseUnit>(actor))
				{
					if (unit->GetBaseEntityComponent()->team == E_Team::PLAYER)
					{
						SetActorHiddenInGame(false);
						break;
					}
				}
				else if (AActor_BaseFactory* factory = Cast<AActor_BaseFactory>(actor))
				{
					if (factory->GetBaseEntityComponent()->team == E_Team::PLAYER)
					{
						SetActorHiddenInGame(false);
						break;
					}
				}
			}
		}
	}

}


void AActor_BaseFactory::AddUnitToQueue(int32  indexBuildable)
{
	if(indexBuildable > _factoryStat->UnitsToProduct.Num() || _unitsQueue.Num() >= _factoryStat->QueueMaxSize)
		return;

	if (!AActor_GameManager::GetInstance()->HasEnoughMoney(BaseEntityComponent->team, _factoryStat->UnitsToProduct[indexBuildable]->Cost) )
	{
		print("Not Enough Minerals");
		return;
	}

	//Launch Timer if we are adding in an empty queue
	if (_unitsQueue.Num() == 0)
		GetWorld()->GetTimerManager().SetTimer(_timerHandle, this, &AActor_BaseFactory::EndTimer, _factoryStat->UnitsToProduct[indexBuildable]->Time);
	
	_unitsQueue.Add(_factoryStat->UnitsToProduct[indexBuildable]);
	if(UDataAsset_BuildableUnitData* unit = Cast<UDataAsset_BuildableUnitData>(_factoryStat->UnitsToProduct[indexBuildable]))
		_powerLevelInQueue += unit->PowerLevel;

	AActor_GameManager::GetInstance()->RemoveMoney(BaseEntityComponent->team, _factoryStat->UnitsToProduct[indexBuildable]->Cost);
	delegateUpdateSelectionProgressBar.Broadcast();
	
	//print("Unit Added");
}
void AActor_BaseFactory::RemoveUnitFromQueue(int32 indexInQueue)
{
	if (indexInQueue >= _unitsQueue.Num())
		return;
	
	AActor_GameManager::GetInstance()->AddMoney(BaseEntityComponent->team, _unitsQueue[indexInQueue]->Cost);
	if (UDataAsset_BuildableUnitData* unit = Cast<UDataAsset_BuildableUnitData>(_unitsQueue[indexInQueue]))
		_powerLevelInQueue -= unit->PowerLevel;
	_unitsQueue.RemoveAt(indexInQueue);

	//Clear Timer if we removed the first in queue
	if (indexInQueue == 0)
	{
		GetWorld()->GetTimerManager().ClearTimer(_timerHandle);
		_timeCoolDown = 0;
		delegateUpdateLoadingWidget.Broadcast(_timeCoolDown);
		//Launch new timer if there is still units in queues
		if(_unitsQueue.Num() > 0)
		{
			GetWorld()->GetTimerManager().SetTimer(_timerHandle, this, &AActor_BaseFactory::EndTimer, _unitsQueue[0]->Time);
		}
	}
	
	delegateUpdateSelectionProgressBar.Broadcast();
	//print("Unit Removed");
}


void AActor_BaseFactory::EndTimer()
{
	switch (_unitsQueue[0]->type)
	{
	case E_TypeElement::UNIT:
		SpawnUnit();
		break;
	case E_TypeElement::REPAIR:
		RepairFactory();
		break;
	default:;
	}
		
}

void AActor_BaseFactory::RepairFactory()
{
	GetBaseEntityComponent()->Heal(0, true);
	
	_unitsQueue.RemoveAt(0);

	//ReLaunch Timer if there is still units in Queue
	if (_unitsQueue.Num() > 0)
		GetWorld()->GetTimerManager().SetTimer(_timerHandle, this, &AActor_BaseFactory::EndTimer, _unitsQueue[0]->Time);

	_timeCoolDown = 0;
	delegateUpdateLoadingWidget.Broadcast(_timeCoolDown);
	delegateUpdateSelectionProgressBar.Broadcast();
	BaseEntityComponent->delegateUpdateHealth.Broadcast(BaseEntityComponent->_lifePoints / BaseEntityComponent->_lifePointMax);
	
	//print("RepairFactory");
}

void AActor_BaseFactory::SpawnUnit()
{
	if (IsValid(Cast<UDataAsset_BuildableUnitData>(_unitsQueue[0])->Template))
	{
		FVector spawnLocation = UNavigationSystemV1::GetRandomPointInNavigableRadius(GetWorld(), GetActorLocation(), _factoryStat->radiusSight);

		FRotator spawnRotation = (spawnLocation - GetActorLocation()).GetSafeNormal2D().Rotation();

		ACharacter_BaseUnit* baseUnit = GetWorld()->SpawnActor<ACharacter_BaseUnit>(Cast<UDataAsset_BuildableUnitData>(_unitsQueue[0])->Template, spawnLocation, spawnRotation);

		if (baseUnit)
		{
			//print("Unit Spawned");
			baseUnit->SetIcon(_unitsQueue[0]->Icon);
			baseUnit->SetTeam(BaseEntityComponent->team);

			UMinimapIconPaperSprite* minimapIcon = baseUnit->FindComponentByClass<UMinimapIconPaperSprite>();
			UMeshComponent* mesh = baseUnit->FindComponentByClass<UMeshComponent>();
			if (mesh && minimapIcon)
			{
				if (BaseEntityComponent->team == E_Team::PLAYER)
					minimapIcon->SetAsBlue();
				else
					minimapIcon->SetAsRed();
			}
		}

	}

	_unitsQueue.RemoveAt(0);

	//ReLaunch Timer if there is still units in Queue
	if (_unitsQueue.Num() > 0)
		GetWorld()->GetTimerManager().SetTimer(_timerHandle, this, &AActor_BaseFactory::EndTimer, _unitsQueue[0]->Time);

	_timeCoolDown = 0;
	delegateUpdateLoadingWidget.Broadcast(_timeCoolDown);

	delegateUpdateSelectionProgressBar.Broadcast();
}

void AActor_BaseFactory::OnSightDetectionCapsuleBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (ACharacter_BaseUnit* unit = Cast<ACharacter_BaseUnit>(OtherActor))
	{
		if (unit->GetBaseEntityComponent()->team != GetBaseEntityComponent()->team && unit->GetBaseEntityComponent()->team != E_Team::NONE)
			AActor_GameManager::instance->RegisterUnitSighted(GetBaseEntityComponent()->team, unit);
	}
	else if (AActor_BaseFactory* factory = Cast<AActor_BaseFactory>(OtherActor))
	{
		if (factory->GetBaseEntityComponent()->team != GetBaseEntityComponent()->team && factory->GetBaseEntityComponent()->team != E_Team::NONE)
			AActor_GameManager::instance->RegisterFactorySighted(GetBaseEntityComponent()->team, factory);
	}
}

void AActor_BaseFactory::OnSightDetectionCapsuleEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (ACharacter_BaseUnit* unit = Cast<ACharacter_BaseUnit>(OtherActor))
	{
		if (unit->GetBaseEntityComponent()->team != GetBaseEntityComponent()->team)
			AActor_GameManager::instance->UnRegisterUnitSighted(GetBaseEntityComponent()->team, unit);
	}
	else if (AActor_BaseFactory* factory = Cast<AActor_BaseFactory>(OtherActor))
	{
		if (factory->GetBaseEntityComponent()->team != GetBaseEntityComponent()->team)
			AActor_GameManager::instance->UnRegisterFactorySighted(GetBaseEntityComponent()->team, factory);
	}
}
