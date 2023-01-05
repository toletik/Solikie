// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor_PlayerManager.h"

#include "Entities/Factories/Actor_BaseFactory.h"
#include "Entities/Units/Character_BaseUnit.h"
#include "Kismet/GameplayStatics.h"
#include "Solikie/UI/SelectionPanel/UserWidget_SelectionInfo.h"
#include "Solikie/Containers/Macro.h"
#include "Solikie/Containers/Define.h"
#include "Map/Actor_MapManager.h"
#include "AIController.h"
#include "DrawDebugHelpers.h"
#include "UObject/ConstructorHelpers.h"
#include "Components/DecalComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerController.h"
#include "Camera/CameraComponent_CamComp.h"
#include "Materials/Material.h"
#include "Solikie/Containers/DataAsset_FactoryData.h"
#include "Solikie/Containers/DataAsset_UnitData.h"
#include "Solikie/Gameplay/Actor_GameManager.h"

AActor_PlayerManager::AActor_PlayerManager()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;

	// Create a decal in the world to show the cursor's location
	{
		_cursorToWorld = CreateDefaultSubobject<UDecalComponent>("CursorToWorld");
		_cursorToWorld->SetupAttachment(RootComponent);

		static ConstructorHelpers::FObjectFinder<UMaterial> DecalMaterialAsset(TEXT("Material'/Game/Assets/Decals/D_Cursor'"));
		if (DecalMaterialAsset.Succeeded())
			_cursorToWorld->SetDecalMaterial(DecalMaterialAsset.Object);

		_cursorToWorld->DecalSize = FVector(16.0f, 32.0f, 32.0f);
		_cursorToWorld->SetRelativeRotation(FRotator(90.0f, 0.0f, 0.0f).Quaternion());
	}
}
void AActor_PlayerManager::BeginPlay()
{
	Super::BeginPlay(); 

	if (!_map)
		_map = Cast<AActor_MapManager>(UGameplayStatics::GetActorOfClass(GetWorld(), AActor_MapManager::StaticClass()));

	mainHud = CreateWidget<UUserWidget_MainHud>(GetGameInstance(), mainHudTemplate);
	if (mainHud)
	{
		mainHud->AddToViewport(200);
		mainHud->Init(this);
		
		FInputModeGameAndUI mode;
		mode.SetWidgetToFocus(mainHud->TakeWidget());
		mode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
		mode.SetHideCursorDuringCapture(false);

		APlayerController* playerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
		playerController->SetInputMode(mode);
		playerController->SetShowMouseCursor(true);
	}

	GetWorld()->GetTimerManager().SetTimer(_timerHandleMoney, this, &AActor_PlayerManager::TimerAddMoney, timeMoneyRate, true);
	delegateSelectionChange.AddUniqueDynamic(this, &AActor_PlayerManager::FillSelectionSprites);

}
void AActor_PlayerManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FHitResult TraceHitResult;
	UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetHitResultUnderCursorByChannel(TraceTypeQuery1, true, TraceHitResult);

	if (TraceHitResult.GetActor() && _map->IsPositionInsideMap(TraceHitResult.Location))
	{
		_cursorToWorld->SetWorldLocation(TraceHitResult.Location);
		_cursorToWorld->SetWorldRotation(TraceHitResult.ImpactNormal.Rotation());

		//print(TraceHitResult.GetComponent()->GetFullName());
		
		//Building
		if (_currentOrder == E_OrderWithValidation::BUILD)
		{
			FVector Center = _map->GetCenterOfBuilding(TraceHitResult.Location, _factoryDatas[_buildingIndex]->_TilesOccupied);
			FVector Extents = FVector(100 / 2 * _factoryDatas[_buildingIndex]->_TilesOccupied.X, 100 / 2 * _factoryDatas[_buildingIndex]->_TilesOccupied.Y, 10);
			DrawDebugBox(GetWorld(), Center, Extents, (_map->IsBuildingValid(TraceHitResult.Location, _factoryDatas[_buildingIndex]->_TilesOccupied) && IsPositionInsideBuildingsSight(TraceHitResult.Location))
				? FColor::Green : FColor::Red, false, -1.f, 0, 10);
		}

		//Selection
		if (_currentOrder == E_OrderWithValidation::SELECT)
		{
			FVector selectionEndPosition = TraceHitResult.Location;
			FVector Center = _selectionStartPos + (selectionEndPosition - _selectionStartPos) / 2;
			FVector Extents = (selectionEndPosition - _selectionStartPos) / 2;
			Extents.Z = 100;

			DrawDebugBox(GetWorld(), Center, Extents.GetAbs(), FColor::Green, false, -1.f, 0, 20);
		}
	}

	//FollowSelection
	if (PlayerPawnRef && bIsFollowingSelection && selectedEntity.Num() > 0)
	{
		//Get X and Y of first selected entity but keep same Z
		FVector camPos = selectedEntity[0]->GetOwner()->GetActorLocation();
		camPos.Z = PlayerPawnRef->GetActorLocation().Z;

		PlayerPawnRef->SetActorLocation(camPos);
	}
}


/************************************************************************/
/*                         Validation Related                           */
/************************************************************************/
void AActor_PlayerManager::ValidateOrder()
{
	switch (_currentOrder)
	{
	case E_OrderWithValidation::NONE:
		SelectionStart();
		break;
	case E_OrderWithValidation::BUILD:
		BuildingEnd();
		break;
	case E_OrderWithValidation::MVT:
		SelectedEntitiesMoveTo();
		break;
	case E_OrderWithValidation::ATTACK:
		SelectedEntitiesAttackTo();
		break;
	default:
		break;
	}


	//Set back to None if we accomplished an Order, or leave SELECTED because it will be set back in SelectionEnd
	if(_currentOrder != E_OrderWithValidation::SELECT)
		_currentOrder = E_OrderWithValidation::NONE;

}
void AActor_PlayerManager::CancelOrder()
{
	if(_currentOrder != E_OrderWithValidation::NONE)
		_currentOrder = E_OrderWithValidation::NONE;
	//If no order to cancel
	else
	{
		SelectedEntitiesMoveTo();
	}
}

/************************************************************************/
/*                     SelectedEntity Related                           */
/************************************************************************/

void AActor_PlayerManager::SelectedEntitiesPrepareMoveTo()
{
	_currentOrder = E_OrderWithValidation::MVT;
}
void AActor_PlayerManager::SelectedEntitiesPrepareAttackTo()
{
	_currentOrder = E_OrderWithValidation::ATTACK;
}
void AActor_PlayerManager::ChangeFormationType(int32 index)
{
	switch (index)
	{
	case 0:
		FormationType = E_Formation::Triangle;
		break;
	case 1:
		FormationType = E_Formation::Circle;
		break;
	case 2:
		FormationType = E_Formation::Square;
		break;
	default:
		return;
	}
}

void AActor_PlayerManager::SelectedEntitiesMoveTo()
{
	FHitResult TraceHitResult;
	UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetHitResultUnderCursor(ECC_Visibility, true, TraceHitResult);

	if (TraceHitResult.GetActor() && _map->IsPositionInsideMap(TraceHitResult.Location))
	{
		float minSpeed = TNumericLimits<float>::Max();
		TArray<ACharacter_BaseUnit*> selectedUnits {};
		for (UActorComponent_BaseEntity* currentEntity : selectedEntity)
		{
			if (ACharacter_BaseUnit* baseUnit = Cast<ACharacter_BaseUnit>(currentEntity->GetOwner()))
			{
				float currUnitSpeed = baseUnit->GetStats()->MvtSpeedLinear;
				if (currUnitSpeed < minSpeed)
					minSpeed = currUnitSpeed;
				selectedUnits.Emplace(baseUnit);
			}
		}
		
		for (ACharacter_BaseUnit* currUnit : selectedUnits)
			currUnit->GetCharacterMovement()->MaxWalkSpeed = minSpeed;

		UFormations::MoveWithFormation(TraceHitResult.Location, selectedUnits, FormationType, false);
	}

}
void AActor_PlayerManager::SelectedEntitiesAttackTo()
{
	FHitResult TraceHitResult;
	UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetHitResultUnderCursor(ECC_Visibility, true, TraceHitResult);

	if (TraceHitResult.GetActor() && _map->IsPositionInsideMap(TraceHitResult.Location))
	{
		float minSpeed = TNumericLimits<float>::Max();
		TArray<ACharacter_BaseUnit*> selectedUnits{};
		for (UActorComponent_BaseEntity* currentEntity : selectedEntity)
		{
			if (ACharacter_BaseUnit* baseUnit = Cast<ACharacter_BaseUnit>(currentEntity->GetOwner()))
			{
				float currUnitSpeed = baseUnit->GetStats()->MvtSpeedLinear;
				if (currUnitSpeed < minSpeed)
					minSpeed = currUnitSpeed;
				selectedUnits.Emplace(baseUnit);
			}
		}

		for (ACharacter_BaseUnit* currUnit : selectedUnits)
			currUnit->GetCharacterMovement()->MaxWalkSpeed = minSpeed;

		UFormations::MoveWithFormation(TraceHitResult.Location, selectedUnits, FormationType, true);
	}
}
void AActor_PlayerManager::SelectedEntitiesHoldPosition()
{
	for (UActorComponent_BaseEntity* currentEntity : selectedEntity)
		if (ACharacter_BaseUnit* baseUnit = Cast<ACharacter_BaseUnit>(currentEntity->GetOwner()))
			baseUnit->HoldPosition();
}
void AActor_PlayerManager::SelectedEntitiesCeaseFire()
{
	for(UActorComponent_BaseEntity* currentEntity  : selectedEntity)
		if(ACharacter_BaseUnit* baseUnit = Cast<ACharacter_BaseUnit>(currentEntity->GetOwner()) )
			baseUnit->CeaseFire();
}

void AActor_PlayerManager::SelectedEntitiesHeal()
{
	for(UActorComponent_BaseEntity* currentEntity  : selectedEntity)
		if(ACharacter_BaseUnit* baseUnit = Cast<ACharacter_BaseUnit>(currentEntity->GetOwner()) )
			baseUnit->Heal();
}


/************************************************************************/
/*                       Selection Related                              */
/************************************************************************/
void AActor_PlayerManager::SelectionStart()
{
	EmptySelectionSprites();

	FHitResult TraceHitResult;
	UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetHitResultUnderCursor(ECC_Visibility, true, TraceHitResult);
	
	if (TraceHitResult.GetActor() && _map->IsPositionInsideMap(TraceHitResult.Location))
	{
		_currentOrder = E_OrderWithValidation::SELECT;
		_selectionStartPos = TraceHitResult.Location;
	}

}
void AActor_PlayerManager::SelectionEnd()
{
	if (_currentOrder != E_OrderWithValidation::SELECT)
		return;

	_currentOrder = E_OrderWithValidation::NONE;

	
	FHitResult TraceHitResult;
	UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetHitResultUnderCursor(ECC_Visibility, true, TraceHitResult);
	if (TraceHitResult.GetActor() && _map->IsPositionInsideMap(TraceHitResult.Location))
	{
		FVector selectionEndPosition = TraceHitResult.Location;
		FVector Center = _selectionStartPos + (selectionEndPosition - _selectionStartPos) / 2;
		FVector Extents = (selectionEndPosition - _selectionStartPos) / 2;
		Extents.Z = OFFSET_Z_FOR_FLYING * 2;

		TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes = TArray<TEnumAsByte<EObjectTypeQuery>>();
		TArray<AActor*> ActorsToIgnore = TArray<AActor*>();
		TArray<AActor*> OutActors = TArray<AActor*>();

		UKismetSystemLibrary::BoxOverlapActors(GetWorld(), Center, Extents.GetAbs(), ObjectTypes, AActor::StaticClass(), ActorsToIgnore, OutActors);
		
		selectedEntity.Empty();
		int currentPrio = 0;
		for (AActor* currentActor : OutActors)
		{
			if (currentActor->ActorHasTag("Unit"))
			{
				ACharacter_BaseUnit* unit = Cast<ACharacter_BaseUnit>(currentActor);
				UActorComponent_BaseEntity* baseEntity = unit->GetBaseEntityComponent();
				if (team != baseEntity->team)
					continue;
				if (baseEntity->prio > currentPrio)
				{
					currentPrio = baseEntity->prio;
					
					selectedEntity.Empty();
				}
				if (baseEntity->prio == currentPrio)
					selectedEntity.Add(baseEntity);
			}
			else if (currentActor->ActorHasTag("Factory"))
			{
				AActor_BaseFactory* factory = Cast<AActor_BaseFactory>(currentActor);
				UActorComponent_BaseEntity* baseEntity = factory->GetBaseEntityComponent();
				if (team != baseEntity->team)
					continue;
				if (baseEntity->prio > currentPrio)
				{
					currentPrio = baseEntity->prio;
					selectedEntity.Empty();
				}
				if (baseEntity->prio == currentPrio)
					selectedEntity.Add(factory->GetBaseEntityComponent());
			}
		}
		delegateSelectionChange.Broadcast(selectedEntity);

	}
}
void AActor_PlayerManager::SelectAll()
{
	EmptySelectionSprites();
	selectedEntity.Empty();

	for (ACharacter_BaseUnit* currentUnit : army.GetUnits())
		if(currentUnit)
			selectedEntity.Add(currentUnit->GetBaseEntityComponent());
	
	delegateSelectionChange.Broadcast(selectedEntity);
}

void AActor_PlayerManager::EmptySelectionSprites()
{
	for (UActorComponent_BaseEntity* entityBase : selectedEntity)
		entityBase->SetUnselected();
}

void AActor_PlayerManager::FillSelectionSprites(TArray<UActorComponent_BaseEntity*> comp)
{
	for (UActorComponent_BaseEntity* entityBase : comp)
	{
		entityBase->SetSelected();
	}
}

void AActor_PlayerManager::FollowSelectionStart()
{
	bIsFollowingSelection = true;
}
void AActor_PlayerManager::FollowSelectionEnd()
{
	bIsFollowingSelection = false;
}

/************************************************************************/
/*                       Building Related                              */
/************************************************************************/

void AActor_PlayerManager::BuildingStart(int32 index)
{
	if(index > _factoryDatas.Num())
		return;

	if (!army.GetCurrencies().HasEnoughMoney(_factoryDatas[index]->Cost))
	{
		print("Not Enough Minerals");
		return;
	}
	
	_currentOrder = E_OrderWithValidation::BUILD;
	_buildingIndex = index;
}
void AActor_PlayerManager::BuildingEnd()
{
	if (_currentOrder != E_OrderWithValidation::BUILD)
		return;

	FHitResult TraceHitResult;
	UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetHitResultUnderCursor(ECC_Visibility, true, TraceHitResult);

	if (TraceHitResult.GetActor() && IsPositionInsideBuildingsSight(TraceHitResult.Location) && _map->IsPositionInsideMap(TraceHitResult.Location) && _map->IsBuildingValid(TraceHitResult.Location, _factoryDatas[_buildingIndex]->_TilesOccupied))
	{
		//Spawn
		AActor_BaseFactory* Factory = GetWorld()->SpawnActor<AActor_BaseFactory>(_factoryDatas[_buildingIndex]->Template, FVector::ZeroVector, FRotator::ZeroRotator);
		Factory->SetActorScale3D(FVector(_factoryDatas[_buildingIndex]->_TilesOccupied.X, _factoryDatas[_buildingIndex]->_TilesOccupied.Y, 1.0));

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
		
		//Set Location
		Factory->SetActorLocation(_map->GetCenterOfBuilding(TraceHitResult.Location, _factoryDatas[_buildingIndex]->_TilesOccupied));

		//Register
		Factory->SetTeam(team);
		AActor_GameManager::GetInstance()->RemoveMoney(team, _factoryDatas[_buildingIndex]->Cost);

	}
}

bool AActor_PlayerManager::IsPositionInsideBuildingsSight(FVector position)
{
	for(AActor_BaseFactory* currentFactory : army.GetFactories())
		if(currentFactory)
			if( (currentFactory->GetActorLocation() - position).Size2D() <= currentFactory->GetStats()->radiusSight * 2) //For unknown reason we need to double the sight
				return true;

	return false;
}


/************************************************************************/
/*                     Order Panel Related                              */
/************************************************************************/

void AActor_PlayerManager::OnUnitOrder(int32 index)
{
	switch (index)
	{
	case 0:
		SelectedEntitiesPrepareAttackTo();
		break;
	case 1:
		SelectedEntitiesPrepareMoveTo();
		break;
	case 2:
		SelectedEntitiesHoldPosition();
		break;
	case 3:
		SelectedEntitiesCeaseFire();
		break;
	case 4:
		SelectedEntitiesHeal();
		break;
	default:
		break;
	}
}

void AActor_PlayerManager::UpdateSelectionAfterClickButton(UActorComponent_BaseEntity* comp)
{
	EmptySelectionSprites();
	selectedEntity.Empty();
	selectedEntity.Add(comp);
	comp->SetSelected();
}

/************************************************************************/
/*                               Money                                  */
/************************************************************************/


void AActor_PlayerManager::TimerAddMoney()
{
	army.GetCurrencies().AddMoney(moneyToAdd);
}