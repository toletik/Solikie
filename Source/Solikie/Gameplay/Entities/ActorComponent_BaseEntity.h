// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MinimapIconPaperSprite.h"
#include "PaperSpriteComponent.h"

#include "Components/ActorComponent.h"
#include "Solikie/Interface/Interface_Damageable.h"
#include "Solikie/Containers/Team.h"
#include "Solikie/UI/UserWidget_HealthBar.h"
#include "Solikie/Gameplay/Entities/SelectionPaperSprite.h"
#include "ActorComponent_BaseEntity.generated.h"


class UNiagaraSystem;
class UWidgetComponent;
class ACharacter_BaseUnit;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnLifePointChange_DynamicDelegate, AActor*, damager, float, lifePoints);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDelegateUpdateHealth, float, newValue);


UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class SOLIKIE_API UActorComponent_BaseEntity : public UActorComponent, public IInterface_Damageable
{
	GENERATED_BODY()

	UPROPERTY()
	UWidgetComponent* _widgetComponent {nullptr};
public:
	UActorComponent_BaseEntity();
	virtual void BeginPlay() override;
	virtual float TakeDamage(ACharacter_BaseUnit* damager, float damageToTake) override;
	void Heal(float healingPoint, bool healMax = false);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stats)
	E_Team team = E_Team::NONE;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stats)
	UTexture2D* icon {nullptr};

	/*
	 * Selection's circle sprite under entities
	 */
	
	UPROPERTY()
	USelectionPaperSprite* selectionCircle {nullptr};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Visibility)
	TSubclassOf<USelectionPaperSprite> selectionClassComp {nullptr};

	UFUNCTION()
	void TriggerSelectionChange();
	UFUNCTION()
	void SetUnselected();
	UFUNCTION()
	void SetSelected();
	UFUNCTION()
	void SetupSelectionSprite();
	UFUNCTION()
	void SetupSelectionScale();

	/*
	 * Minimap icons
	 */
	
	UPROPERTY()
	UMinimapIconPaperSprite* minimapIcon {nullptr};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Visibility)
	TSubclassOf<UMinimapIconPaperSprite> minimapIconClassComp {nullptr};
	
	UFUNCTION()
	void SetupMinimapIcons();
	UFUNCTION()
	void SetupMinimapScale();

	/*
	 *
	 */
	
	FOnLifePointChange_DynamicDelegate delegateOnLifePointChange;
	FDelegateUpdateHealth delegateUpdateHealth;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UWidgetComponent> classComp;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Stats, meta = (AllowPrivateAccess = "true"))
	int prio = 0;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Stats, meta = (AllowPrivateAccess = "true"))
	float _lifePointMax = 10.0f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Stats, meta = (AllowPrivateAccess = "true"))
	float _armor = 1.0f;
	
	float _lifePoints = 10.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "VFX")
	UNiagaraSystem* particlesDeath = nullptr;
};
