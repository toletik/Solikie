// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Pawn_Player.h"
#include "Entities/ActorComponent_BaseEntity.h"
#include "Map/Actor_MapManager.h"
#include "Solikie/UI/UserWidget_MainHud.h"
#include "Solikie/Containers/Army.h"
#include "Solikie/Containers/Team.h"
#include "Solikie/Gameplay/Entities/Units/Formations.h"
#include "Actor_PlayerManager.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDelegateUpdateSelectionPanel, TArray<UActorComponent_BaseEntity*>, comp);


class AActor_MapManager;


UENUM()
enum class E_OrderWithValidation
{
	NONE,
	BUILD,
	ATTACK,
	MVT,
	MVT_T,
	MVT_S,
	MVT_C,
	SELECT,
};



UCLASS()
class SOLIKIE_API AActor_PlayerManager : public AActor
{
	GENERATED_BODY()

public:
	AActor_PlayerManager();
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	UPROPERTY()
	FArmy army;

	FDelegateUpdateSelectionPanel delegateSelectionChange;

	UPROPERTY()
	TArray<UActorComponent_BaseEntity*>  selectedEntity;

	TMap<ACharacter_BaseUnit*, int> unitsSighted;
	TMap<AActor_BaseFactory*, int> factoriesSighted;

	APawn_Player* PlayerPawnRef {nullptr};

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Initialization, meta = (AllowPrivateAccess = "true"))
	AActor_MapManager* _map {nullptr};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UUserWidget_MainHud> mainHudTemplate;
	UPROPERTY()
	UUserWidget_MainHud* mainHud {nullptr};
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UDecalComponent* _cursorToWorld;

	E_Team team = E_Team::PLAYER;
	E_Formation FormationType = E_Formation::Triangle;


/************************************************************************/
/*                         Validation Related                           */
/************************************************************************/
public: 
	UFUNCTION()
	void ValidateOrder();
	UFUNCTION()
	void CancelOrder();

private:
	E_OrderWithValidation _currentOrder = E_OrderWithValidation::NONE;

/************************************************************************/
/*                     SelectedEntity Related                           */
/************************************************************************/
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Initialization)
	TArray<UTexture2D*> _FormationsIcons {nullptr};

	UFUNCTION()
	void SelectedEntitiesPrepareMoveTo();
	UFUNCTION()
	void SelectedEntitiesPrepareAttackTo();
	UFUNCTION()
	void ChangeFormationType(int32 index);


	UFUNCTION()
	void SelectedEntitiesMoveTo();
	UFUNCTION()
	void SelectedEntitiesAttackTo();
	UFUNCTION()
	void SelectedEntitiesHoldPosition();
	UFUNCTION()
	void SelectedEntitiesCeaseFire();
	UFUNCTION()
	void SelectedEntitiesHeal();

	

/************************************************************************/
/*                       Selection Related                              */
/************************************************************************/
public:

	UFUNCTION()
	void SelectionStart();
	UFUNCTION()
	void SelectionEnd();
	UFUNCTION()
	void SelectAll();

	UFUNCTION()
	void EmptySelectionSprites();
	UFUNCTION()
	void FillSelectionSprites(TArray<UActorComponent_BaseEntity*> comp);
	
	UFUNCTION()
	void FollowSelectionStart();
	UFUNCTION()
	void FollowSelectionEnd();

private:
	FVector _selectionStartPos{};

	bool bIsFollowingSelection {false};

/************************************************************************/
/*                       Building Related                              */
/************************************************************************/
public:
	UFUNCTION()
	void BuildingStart(int32 index);
	UFUNCTION()
	void BuildingEnd();

	bool IsPositionInsideBuildingsSight(FVector position);

private:

	int32 _buildingIndex {0};

/************************************************************************/
/*                     Order Panel Related                              */
/************************************************************************/
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Initialization, meta = (AllowPrivateAccess = "true"))
	TArray<UDataAsset_BuildableFactoryData*> _factoryDatas{ nullptr };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Initialization, meta = (AllowPrivateAccess = "true"))
	TArray<UTexture2D*> _orderDatas{ nullptr };

	UFUNCTION()
	void OnUnitOrder(int32 index);

	UFUNCTION()
	void UpdateSelectionAfterClickButton(UActorComponent_BaseEntity* comp);

/************************************************************************/
/*                              Money                                   */
/************************************************************************/
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Money, meta = (AllowPrivateAccess = "true"))
	int32 moneyToAdd = 5;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Money, meta = (AllowPrivateAccess = "true"))
	float timeMoneyRate = 3.0f;

	FTimerHandle _timerHandleMoney;

	UFUNCTION()
	void TimerAddMoney();

};
