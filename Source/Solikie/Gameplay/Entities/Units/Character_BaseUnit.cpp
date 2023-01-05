// Fill out your copyright notice in the Description page of Project Settings.


#include "Character_BaseUnit.h"

#include "DrawDebugHelpers.h"
#include "NiagaraFunctionLibrary.h"
#include "Solikie/Containers/DataAsset_UnitData.h"
#include "Engine/StaticMeshActor.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Solikie/Gameplay/Actor_GameManager.h"
#include "Solikie/Gameplay/Entities/ActorComponent_BaseEntity.h"
#include "Solikie/Gameplay/Map/Actor_MapManager.h"
#include "NiagaraComponent.h"
#include "Solikie/Containers/Macro.h"
#include "Solikie/Containers/Define.h"

ACharacter_BaseUnit::ACharacter_BaseUnit()
{
	PrimaryActorTick.bCanEverTick = true;

	_targetsDetectionCapsule = CreateDefaultSubobject<UCapsuleComponent>(FName("_targetsDetectionCapsule"));
	_targetsDetectionCapsule->SetupAttachment(RootComponent);

	_sightCapsule = CreateDefaultSubobject<UCapsuleComponent>(FName("_sightCapsule"));
	_sightCapsule->SetupAttachment(RootComponent);
	
	BaseEntityComponent = CreateDefaultSubobject<UActorComponent_BaseEntity>(TEXT("BaseUnitComponent"));
	
	Tags.Add("Unit");
}
void ACharacter_BaseUnit::BeginPlay()
{
	Super::BeginPlay();

	if (!GetController())
		SpawnDefaultController();

	_aIController = Cast<AAIController>(GetController());
	_aIController->ReceiveMoveCompleted.AddDynamic(this, &ACharacter_BaseUnit::OnReachDestination);
	GetCharacterMovement()->MaxWalkSpeed = GetStats()->MvtSpeedLinear;
	
	//TO CLEAN
	if (GetBaseEntityComponent()->team != E_Team::NONE)
	{
		_targetsDetectionCapsule->OnComponentBeginOverlap.AddDynamic(this, &ACharacter_BaseUnit::OnTargetsDetectionCapsuleBeginOverlap);
		_targetsDetectionCapsule->OnComponentEndOverlap.AddDynamic(this, &ACharacter_BaseUnit::OnTargetsDetectionCapsuleEndOverlap);
		_targetsDetectionCapsule->SetCapsuleHalfHeight(_unitStat->AttackRange + OFFSET_Z_FOR_FLYING);
		_targetsDetectionCapsule->SetCapsuleRadius(_unitStat->AttackRange);

		_sightCapsule->OnComponentBeginOverlap.AddDynamic(this, &ACharacter_BaseUnit::OnSightDetectionCapsuleBeginOverlap);
		_sightCapsule->OnComponentEndOverlap.AddDynamic(this, &ACharacter_BaseUnit::OnSightDetectionCapsuleEndOverlap);
		_sightCapsule->SetCapsuleHalfHeight(_unitStat->radiusSight * 2 + OFFSET_Z_FOR_FLYING);
		_sightCapsule->SetCapsuleRadius(_unitStat->radiusSight * 2);

		if (GetBaseEntityComponent()->team == E_Team::PLAYER)
		{
			_fogOfWar = NewObject<UActorComponent_SignInFogOfWar>(this, _classFogOfWar);
			if (_fogOfWar)
			{
				_fogOfWar->radius = _unitStat->radiusSight / AActor_MapManager::maxRange;
				_fogOfWar->RegisterComponent();
			}
		}
		
		if (GetBaseEntityComponent()->team == E_Team::IA1)
			SetActorHiddenInGame(true);
		
		AActor_GameManager::GetInstance()->RegisterUnit(this);
	}

	if (UActorComponent_BaseEntity* ActorComponent_BaseEntity = Cast<UActorComponent_BaseEntity>(GetComponentByClass(UActorComponent_BaseEntity::StaticClass())))
		ActorComponent_BaseEntity->delegateOnLifePointChange.AddUniqueDynamic(this, &ACharacter_BaseUnit::OnTakeDamage);

	//Flying Related
	if (_unitStat->IsFlying)
	{
		SetActorLocation(GetActorLocation() + FVector(0.0f, 0.0f, OFFSET_Z_FOR_FLYING));
		GetCharacterMovement()->SetMovementMode(MOVE_Flying);
	}
}

void ACharacter_BaseUnit::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}


/************************************************************************/
/*                           Stats Related                              */
/************************************************************************/

void ACharacter_BaseUnit::SetTeam(E_Team newTeam)
{
	BaseEntityComponent->team = newTeam;

	//Set TargetCapsule and SightCapsule once we have a team to avoid being none and having everyone as a treat
	if (GetBaseEntityComponent()->team != E_Team::NONE)
	{
		_targetsDetectionCapsule->OnComponentBeginOverlap.AddDynamic(this, &ACharacter_BaseUnit::OnTargetsDetectionCapsuleBeginOverlap);
		_targetsDetectionCapsule->OnComponentEndOverlap.AddDynamic(this, &ACharacter_BaseUnit::OnTargetsDetectionCapsuleEndOverlap);
		_targetsDetectionCapsule->SetCapsuleHalfHeight(_unitStat->AttackRange + OFFSET_Z_FOR_FLYING);
		_targetsDetectionCapsule->SetCapsuleRadius(_unitStat->AttackRange);

		_sightCapsule->OnComponentBeginOverlap.AddDynamic(this, &ACharacter_BaseUnit::OnSightDetectionCapsuleBeginOverlap);
		_sightCapsule->OnComponentEndOverlap.AddDynamic(this, &ACharacter_BaseUnit::OnSightDetectionCapsuleEndOverlap);
		_sightCapsule->SetCapsuleHalfHeight(_unitStat->radiusSight * 2 + OFFSET_Z_FOR_FLYING);
		_sightCapsule->SetCapsuleRadius(_unitStat->radiusSight * 2);
		
		if (GetBaseEntityComponent()->team == E_Team::PLAYER)
		{
			_fogOfWar = NewObject<UActorComponent_SignInFogOfWar>(this, _classFogOfWar);
			if (_fogOfWar)
			{
				_fogOfWar->radius = _unitStat->radiusSight / AActor_MapManager::maxRange;
				_fogOfWar->RegisterComponent();
			}
				
		}

		//TO CLEAN
		if (GetBaseEntityComponent()->team == E_Team::IA1)
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
			
		
		AActor_GameManager::GetInstance()->RegisterUnit(this);
	}

}

void ACharacter_BaseUnit::SetIcon(UTexture2D* newTexture)
{
	GetBaseEntityComponent()->icon = newTexture;
}


/************************************************************************/
/*                         Detection Related                            */
/************************************************************************/

void ACharacter_BaseUnit::OnTargetsDetectionCapsuleBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	
	if (UActorComponent_BaseEntity* potentialTarget = Cast<UActorComponent_BaseEntity>(OtherActor->GetComponentByClass(UActorComponent_BaseEntity::StaticClass()) )  )
	{
		//if we encounter an enemy entity
		if (potentialTarget->team != BaseEntityComponent->team && potentialTarget->team  != E_Team::NONE && !_targets.Contains(potentialTarget))
		{
			//print("New Target");
			//print("Target team");
			//print(UEnum::GetValueAsString(potentialTarget->team));
			//print("My team");
			//print(UEnum::GetValueAsString(BaseEntityComponent->team));

			//Start Attacking if we encounter our first enemy and we can start attacking
			if (_targets.Num() == 0 && (_state == E_UnitState::MvtAttack || _state == E_UnitState::HoldPosition))
				StartAttack();
			
			//print(GetFName().ToString() + " detect Target : " + OtherActor->GetFName().ToString());
			_targets.Add(potentialTarget);
		}
	}

}
void ACharacter_BaseUnit::OnTargetsDetectionCapsuleEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (UActorComponent_BaseEntity* potentialTarget = Cast<UActorComponent_BaseEntity>(OtherActor->GetComponentByClass(UActorComponent_BaseEntity::StaticClass()) ) )
	{
		//if we lost an enemy entity
		if (_targets.Contains(potentialTarget))
		{
			//print(GetFName().ToString() + " lost Target : " + OtherActor->GetFName().ToString());
			_targets.Remove(potentialTarget);

			//Stop Attacking if we lost last enemy in sight 
			if (_targets.Num() == 0 )
				EndAttack();
		}
	}
}

/************************************************************************/
/*                           Sight Related                              */
/************************************************************************/

void ACharacter_BaseUnit::OnSightDetectionCapsuleBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
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

void ACharacter_BaseUnit::OnSightDetectionCapsuleEndOverlap(UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
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

/************************************************************************/
/*                         State Related                                */
/************************************************************************/

void ACharacter_BaseUnit::MoveTo(FVector destination)
{
	SetState(E_UnitState::Mvt, destination);
}
void ACharacter_BaseUnit::AttackTo(FVector destination)
{
	SetState(E_UnitState::MvtAttack, destination);
}
void ACharacter_BaseUnit::HoldPosition()
{
	SetState(E_UnitState::HoldPosition);
}
void ACharacter_BaseUnit::CeaseFire()
{
	SetState(E_UnitState::CeaseFire);
}

void ACharacter_BaseUnit::Heal()
{
	SetState(E_UnitState::Heal);
}

void ACharacter_BaseUnit::SetState(E_UnitState newState)
{
	if (_state == E_UnitState::Heal && newState != E_UnitState::Heal)
 		EndHeal();
	
	_state = newState;

	_aIController->StopMovement();
	
	if(_targets.Num() > 0 && _state == E_UnitState::HoldPosition)
		StartAttack();
	else if (_state == E_UnitState::Heal)
		StartHeal();
}
void ACharacter_BaseUnit::SetState(E_UnitState newState, FVector Destination)
{
	if (_state == E_UnitState::Heal && newState != E_UnitState::Heal)
 		EndHeal();
	
	_state = newState;

	if (_targets.Num() > 0 && _state == E_UnitState::MvtAttack)
		StartAttack();
	else
	{
		//Keep same Z
		Destination.Z = GetActorLocation().Z;
		_aIController->MoveToLocation(Destination, -1, true, (_unitStat->IsFlying ? false : true) );
	}
}

void ACharacter_BaseUnit::Attack()
{
	//Attack first target in sight
	if (_targets.Num() > 0)
	{
		if (_unitStat->particlesAttack)
		{
			UNiagaraComponent* comp = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), _unitStat->particlesAttack, GetActorLocation());
			if (comp)
				comp->SetVectorParameter("Target", _targets[0]->GetOwner()->GetActorLocation());
		}
		_targets[0]->TakeDamage(this, _unitStat->AttackDamage);
	}
		

}

void ACharacter_BaseUnit::HealUnit()
{
	BaseEntityComponent->_lifePoints = FMath::Min(BaseEntityComponent->_lifePoints + _unitStat->healPower, BaseEntityComponent->_lifePointMax);
	BaseEntityComponent->delegateUpdateHealth.Broadcast(BaseEntityComponent->_lifePoints / BaseEntityComponent->_lifePointMax);
	if (BaseEntityComponent->_lifePoints >= BaseEntityComponent->_lifePointMax)
		EndHeal();
}

void ACharacter_BaseUnit::OnReachDestination(FAIRequestID requestID, EPathFollowingResult::Type result)
{
	if (_state != E_UnitState::CeaseFire)
		SetState(E_UnitState::HoldPosition);
}
void ACharacter_BaseUnit::OnTakeDamage(AActor* damager, float lifePoints)
{
	if (_state == E_UnitState::CeaseFire)
	{
		FVector direction = (GetActorLocation() - damager->GetActorLocation()).GetSafeNormal();
		_aIController->MoveToLocation(GetActorLocation() + direction * _flightLength);
	}
}

void ACharacter_BaseUnit::StartAttack()
{
	//print("Start Attack");

	//Set Timer only if there wasn't an attack going already
	if (!_attackTimerHandle.IsValid())
		GetWorld()->GetTimerManager().SetTimer(_attackTimerHandle, this, &ACharacter_BaseUnit::Attack, _unitStat->AttackSpeed, true);

	//Pause Mvt if needed
	if (_aIController->GetMoveStatus() == EPathFollowingStatus::Moving)
		PauseMvt();
}
void ACharacter_BaseUnit::EndAttack()
{
	//print("Stop Attack");
	GetWorld()->GetTimerManager().ClearTimer(_attackTimerHandle);

	//resume Mvt if needed
	if (_aIController->GetMoveStatus() == EPathFollowingStatus::Paused)
		ResumeMvt();

}
void ACharacter_BaseUnit::PauseMvt()	
{
	//print("Pause Mvt");
	_aIController->PauseMove(_aIController->GetCurrentMoveRequestID());
}
void ACharacter_BaseUnit::ResumeMvt()
{
	//print("Resume Mvt");
	_aIController->ResumeMove(_aIController->GetCurrentMoveRequestID());
}

void ACharacter_BaseUnit::StartHeal()
{
	//print("Start Heal");
	//Set Timer only if there wasn't an attack going already
	if (!_attackTimerHandle.IsValid())
		GetWorld()->GetTimerManager().SetTimer(_attackTimerHandle, this, &ACharacter_BaseUnit::HealUnit, _unitStat->HealSpeed, true);

	//Pause Mvt if needed
	if (_aIController->GetMoveStatus() == EPathFollowingStatus::Moving)
		PauseMvt();
}

void ACharacter_BaseUnit::EndHeal()
{
	//print("Stop Heal");
	GetWorld()->GetTimerManager().ClearTimer(_attackTimerHandle);

	//resume Mvt if needed
	if (_aIController->GetMoveStatus() == EPathFollowingStatus::Paused)
		ResumeMvt();
}