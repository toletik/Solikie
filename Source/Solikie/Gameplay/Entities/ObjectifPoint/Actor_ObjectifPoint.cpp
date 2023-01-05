// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor_ObjectifPoint.h"

#include <string>

#include "Components/WidgetComponent.h"
#include "Solikie/Containers/DataAsset_UnitData.h"
#include "Solikie/Containers/Macro.h"
#include "Solikie/Containers/Define.h"
#include "Solikie/Gameplay/Actor_GameManager.h"
#include "Solikie/Gameplay/Entities/Units/Character_BaseUnit.h"
#include "Solikie/Gameplay/Map/Actor_MapManager.h"
#include "Solikie/UI/UserWidget_ProgressObjectifPoint.h"

// Sets default values
AActor_ObjectifPoint::AActor_ObjectifPoint()
{
	PrimaryActorTick.bCanEverTick = true;

	mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	
	CapsuleDetection = CreateDefaultSubobject<UCapsuleComponent>("CapsuleDetection");

	RootComponent = CapsuleDetection;
	mesh->SetupAttachment(RootComponent);
	CapsuleDetection->SetCollisionEnabled(ECollisionEnabled::QueryOnly);

	_widgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("ProgressWidgetComponent"));
	_widgetComponent->AttachTo(RootComponent);
	_widgetComponent->SetRelativeLocation(FVector(0, 0, 110));

}


// Called when the game starts or when spawned
void AActor_ObjectifPoint::BeginPlay()
{
	Super::BeginPlay();


	CapsuleDetection->OnComponentBeginOverlap.AddDynamic(this, &AActor_ObjectifPoint::OnCapsuleDetectionBeginOverlap);
	CapsuleDetection->OnComponentEndOverlap.AddDynamic(this, &AActor_ObjectifPoint::OnCapsuleDetectionEndOverlap);
	CapsuleDetection->SetCapsuleHalfHeight(captureRange + OFFSET_Z_FOR_FLYING);
	CapsuleDetection->SetCapsuleRadius(captureRange);

	DynMaterial = UMaterialInstanceDynamic::Create(mesh->GetMaterial(0), this);
	DynMaterial->SetVectorParameterValue("Color", FVector(0, 0, 0));
	mesh->SetMaterial(0, DynMaterial);

	_fogOfWar = NewObject<UActorComponent_SignInFogOfWar>(this, _classFogOfWar);
	if (_fogOfWar)
	{
		_fogOfWar->radius = captureRange / 2 / AActor_MapManager::maxRange;
		_fogOfWar->RegisterComponent();
	}

	UUserWidget_ProgressObjectifPoint* circularLoading = Cast<UUserWidget_ProgressObjectifPoint>(_widgetComponent->GetUserWidgetObject());
	if (circularLoading)
	{
		delegateUpdateProgressBar.AddDynamic(circularLoading,
			&UUserWidget_ProgressObjectifPoint::UpdateProgressBar);
	}

	AActor_GameManager::instance->RegisterObjectifPoint(this);

	minimapIcon = FindComponentByClass<UMinimapIconPaperSprite>();

	SetupMinimapIcons();
	SetupMinimapScale();
}

void AActor_ObjectifPoint::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	_captureValue += _powerCapture * DeltaTime;
	UpdateCapture();
}

void AActor_ObjectifPoint::OnCapsuleDetectionBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->ActorHasTag("Unit"))
	{
		if (ACharacter_BaseUnit* unit = Cast<ACharacter_BaseUnit>(OtherActor))
		{
			if (unit->GetBaseEntityComponent()->team == E_Team::PLAYER)
			{
				_powerCapture += unit->GetStats()->CapturePower;
			}
			else
			{
				_powerCapture -= unit->GetStats()->CapturePower;
				AActor_GameManager::instance->RegisterUnitSighted(E_Team::PLAYER, unit);
			}

		}
	}
}

void AActor_ObjectifPoint::OnCapsuleDetectionEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor->ActorHasTag("Unit"))
	{
		if (ACharacter_BaseUnit* unit = Cast<ACharacter_BaseUnit>(OtherActor))
		{
			if (unit->GetBaseEntityComponent()->team == E_Team::PLAYER)
			{
				_powerCapture -= unit->GetStats()->CapturePower;
			}
			else
			{
				_powerCapture += unit->GetStats()->CapturePower;
				AActor_GameManager::instance->UnRegisterUnitSighted(E_Team::PLAYER, unit);
			}

		}
	}
}

void AActor_ObjectifPoint::AddMoney()
{
	AActor_GameManager::instance->AddMoney(_team, moneyToAdd);
}

void AActor_ObjectifPoint::UpdateCapture()
{
	if (_captureValue >= 100 && _team != E_Team::PLAYER)
	{
		_captureValue = 100;
		_team = E_Team::PLAYER;
		DynMaterial->SetVectorParameterValue("Color", FVector(0, 0, 1));
		GetWorld()->GetTimerManager().SetTimer(_timerHandleMoney, this, &AActor_ObjectifPoint::AddMoney, timeMoneyRate, true);
	}
	else if (_captureValue <= -100 && _team != E_Team::IA1)
	{
		_captureValue = -100;
		_team = E_Team::IA1;
		DynMaterial->SetVectorParameterValue("Color", FVector(1, 0, 0));
		GetWorld()->GetTimerManager().SetTimer(_timerHandleMoney, this, &AActor_ObjectifPoint::AddMoney, timeMoneyRate, true);
	}

	if (_team == E_Team::PLAYER && _captureValue <= 0)
	{
		_team = E_Team::NONE;
		DynMaterial->SetVectorParameterValue("Color", FVector::ZeroVector);
		GetWorld()->GetTimerManager().ClearTimer(_timerHandle);
	}
	else if (_team == E_Team::IA1 && _captureValue >= 0)
	{
		_team = E_Team::NONE;
		DynMaterial->SetVectorParameterValue("Color", FVector::ZeroVector);
		GetWorld()->GetTimerManager().ClearTimer(_timerHandle);
	}
	_captureValue = FMath::Clamp(_captureValue, -100.f, 100.f);
	SetupMinimapIcons();
	delegateUpdateProgressBar.Broadcast(_captureValue);
}

void AActor_ObjectifPoint::SetupMinimapScale()
{
	//UMeshComponent* mesh = GetOwner()->FindComponentByClass<UMeshComponent>();

	if (mesh)
	{
		
		float scale = mesh->Bounds.SphereRadius / 25.f;
		minimapIcon->SetWorldScale3D(FVector(scale, 1, scale));
	}
}

void AActor_ObjectifPoint::SetupMinimapIcons()
{
	minimapIcon->SetRelativeLocation(FVector(0, 0, 300));
	minimapIcon->SetAsObjective();
	if (_team == E_Team::PLAYER)
		minimapIcon->SetAsBlue();
	else if (_team == E_Team::IA1)
		minimapIcon->SetAsRed();
	if (Cast<ACharacter_BaseUnit>(GetOwner()))
		minimapIcon->SetAsUnit();
	else if (Cast<AActor_BaseFactory>(GetOwner()))
		minimapIcon->SetAsFactory();
}
