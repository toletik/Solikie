// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorComponent_BaseEntity.h"

#include "NiagaraSystem.h"
#include "NiagaraFunctionLibrary.h"
#include "Components/WidgetComponent.h"
#include "Solikie/Gameplay/Actor_GameManager.h"
#include "Solikie/Gameplay/Entities/Factories/Actor_BaseFactory.h"
#include "Solikie/Gameplay/Entities/Units/Character_BaseUnit.h"

#include "Solikie/Containers/Macro.h"

UActorComponent_BaseEntity::UActorComponent_BaseEntity()
{
}

void UActorComponent_BaseEntity::BeginPlay()
{
	Super::BeginPlay();
	
	_lifePoints = _lifePointMax;
	
	_widgetComponent = NewObject<UWidgetComponent>(this, classComp);
	_widgetComponent->RegisterComponent();
	_widgetComponent->SetWorldLocation(FVector(0, 0, 110));
	_widgetComponent->AttachTo(GetOwner()->GetRootComponent());
	UUserWidget_HealthBar* health = Cast<UUserWidget_HealthBar>(_widgetComponent->GetUserWidgetObject());
	if (health)
		delegateUpdateHealth.AddDynamic(health, &UUserWidget_HealthBar::UpdateProgressBar);
	
	delegateUpdateHealth.Broadcast(_lifePoints / _lifePointMax);

	SetupMinimapIcons();
	SetupSelectionSprite();
	
}

float UActorComponent_BaseEntity::TakeDamage(ACharacter_BaseUnit* damager, float damageToTake)
{
	_lifePoints -= FMath::Max(damageToTake - _armor, 0.0f);

	//print(GetOwner()->GetFName().ToString() + " Take " + FString::SanitizeFloat(FMath::Max(damageToTake - _armor, 0.0f)) + " Damage");

	if (_lifePoints <= 0.0f)
	{
		//print(GetOwner()->GetFName().ToString() + " Died");
		
		if (ACharacter_BaseUnit* unit = Cast<ACharacter_BaseUnit>(GetOwner()))
		{
			AActor_GameManager::GetInstance()->UnRegisterUnit(unit);
		}
		else if (AActor_BaseFactory* factory = Cast<AActor_BaseFactory>(GetOwner()))
		{
			AActor_GameManager::GetInstance()->UnRegisterFactory(factory);
		}
		if (particlesDeath)
			UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), particlesDeath, GetOwner()->GetActorLocation());
	}

	//Register aggressor if we are an AI Factory
	if (Cast<AActor_BaseFactory>(GetOwner()) && team == E_Team::IA1)
		AActor_GameManager::GetInstance()->RegisterBaseAgressor(damager);

	delegateOnLifePointChange.Broadcast(damager, _lifePoints);
	delegateUpdateHealth.Broadcast(_lifePoints / _lifePointMax);
	
	return _lifePoints;
}

void UActorComponent_BaseEntity::Heal(float healingPoint, bool healMax)
{
	_lifePoints = healMax ? _lifePointMax : FMath::Min(_lifePoints + healingPoint, _lifePointMax);
	delegateUpdateHealth.Broadcast(_lifePoints / _lifePointMax);
}

void UActorComponent_BaseEntity::TriggerSelectionChange()
{
	if (selectionCircle == nullptr)
		return;
	FLinearColor col = selectionCircle->GetSpriteColor();
	if (col.A == 0)
		col.A = 1;
	else
		col.A = 0;
	selectionCircle->SetSpriteColor(col);
}

void UActorComponent_BaseEntity::SetUnselected()
{
	if (selectionCircle == nullptr)
		return;
	FLinearColor col = selectionCircle->GetSpriteColor();
	col.A = 0;
	selectionCircle->SetSpriteColor(col);
}

void UActorComponent_BaseEntity::SetSelected()
{
	if (selectionCircle == nullptr)
		return;
	FLinearColor col = selectionCircle->GetSpriteColor();
	col.A = 1;
	selectionCircle->SetSpriteColor(col);
}

void UActorComponent_BaseEntity::SetupSelectionSprite()
{
	selectionCircle = NewObject<USelectionPaperSprite>(this, selectionClassComp);
	selectionCircle->RegisterComponent();
	selectionCircle->AttachTo(GetOwner()->GetRootComponent());
	
	selectionCircle->SetRelativeLocation(FVector(0, 0, 0));
	SetupSelectionScale();
}

void UActorComponent_BaseEntity::SetupMinimapIcons()
{
	minimapIcon = NewObject<UMinimapIconPaperSprite>(this, minimapIconClassComp);
	minimapIcon->RegisterComponent();
	minimapIcon->AttachTo(GetOwner()->GetRootComponent());
	
	minimapIcon->SetRelativeLocation(FVector(0, 0, 300));
	
	if (team == E_Team::PLAYER)
		minimapIcon->SetAsBlue();
	else
		minimapIcon->SetAsRed();
	if (Cast<ACharacter_BaseUnit>(GetOwner()))
		minimapIcon->SetAsUnit();
	else if (Cast<AActor_BaseFactory>(GetOwner()))
		minimapIcon->SetAsFactory();
}

void UActorComponent_BaseEntity::SetupSelectionScale()
{
	UMeshComponent* mesh = GetOwner()->FindComponentByClass<UMeshComponent>();
	if (mesh)
	{
		FVector pos = GetOwner()->GetActorLocation();
		float scale = (mesh->Bounds.BoxExtent.X + mesh->Bounds.BoxExtent.Y) / 80.f;
		selectionCircle->SetWorldScale3D(FVector(scale, scale, scale));
	}
}

void UActorComponent_BaseEntity::SetupMinimapScale()
{
	UMeshComponent* mesh = GetOwner()->FindComponentByClass<UMeshComponent>();

	if (mesh)
	{
		
		float scale = mesh->Bounds.SphereRadius / 100.f;
		minimapIcon->SetWorldScale3D(FVector(scale, 1, scale));
	}
}
