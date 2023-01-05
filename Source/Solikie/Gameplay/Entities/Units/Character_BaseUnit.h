// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AIController.h"
#include "Solikie/Gameplay/Entities/ActorComponent_BaseEntity.h"
#include "Solikie/Gameplay/Entities/ActorComponent_SignInFogOfWar.h"
#include "PaperSpriteComponent.h"
#include "Components/CapsuleComponent.h"
#include "Character_BaseUnit.generated.h"

class UDataAsset_UnitData;


UENUM()
enum class E_UnitState : uint8
{
	//Static
	HoldPosition UMETA(DisplayName = "HoldPosition"), //Default
	CeaseFire    UMETA(DisplayName = "CeaseFire"),
	Heal		 UMETA(DisplayName = "Heal"),

	//Dynamic
	Mvt          UMETA(DisplayName = "Mvt"),
	MvtAttack    UMETA(DisplayName = "MvtAttack"),
};



UCLASS()
class SOLIKIE_API ACharacter_BaseUnit : public ACharacter
{
	GENERATED_BODY()

public:
	ACharacter_BaseUnit();
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

private:
	AAIController* _aIController = nullptr;

/************************************************************************/
/*                           Stats Related                              */
/************************************************************************/
public:	

	void SetTeam(E_Team newTeam);
	void SetIcon(UTexture2D* newTexture);
	UDataAsset_UnitData* GetStats() { return _unitStat; }
	UActorComponent_BaseEntity* GetBaseEntityComponent() { return BaseEntityComponent; }

private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Stats, meta = (AllowPrivateAccess = "true"))
	UDataAsset_UnitData* _unitStat = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Stats, meta = (AllowPrivateAccess = "true"))
	UActorComponent_BaseEntity* BaseEntityComponent {nullptr};

/************************************************************************/
/*                         Detection Related                            */
/************************************************************************/

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Gameplay, meta = (AllowPrivateAccess = "true"))
	class UCapsuleComponent* _targetsDetectionCapsule = nullptr;

	UFUNCTION()
	void OnTargetsDetectionCapsuleBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void OnTargetsDetectionCapsuleEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

/************************************************************************/
/*                          Sigh Related                                */
/************************************************************************/

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Gameplay, meta = (AllowPrivateAccess = "true"))
	class UCapsuleComponent* _sightCapsule = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UActorComponent_SignInFogOfWar> _classFogOfWar;
	
	UPROPERTY()
	UActorComponent_SignInFogOfWar* _fogOfWar = nullptr;

	UFUNCTION()
	void OnSightDetectionCapsuleBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void OnSightDetectionCapsuleEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	
/************************************************************************/
/*                         State Related                                */
/************************************************************************/

public:
	//Helpers
	void MoveTo(FVector destination);
	void AttackTo(FVector destination);
	void HoldPosition();
	void CeaseFire();
	void Heal();

private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Stats, meta = (AllowPrivateAccess = "true"))
	E_UnitState _state = E_UnitState::HoldPosition;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Stats, meta = (AllowPrivateAccess = "true"))
	float _flightLength = 100.0f;

	//If need to attack
	TArray<UActorComponent_BaseEntity*> _targets = TArray<UActorComponent_BaseEntity*>();
	FTimerHandle _attackTimerHandle;


	//Static State
	void SetState(E_UnitState newState);
	//Dynamic State
	void SetState(E_UnitState newState, FVector Destination);

	UFUNCTION()
	void Attack();
	UFUNCTION()
	void HealUnit();
	UFUNCTION()
	void OnReachDestination(FAIRequestID requestID, EPathFollowingResult::Type result);
	UFUNCTION()
	void OnTakeDamage(AActor* damager, float lifePoints);

	void StartAttack();
	void EndAttack();
	void PauseMvt();
	void ResumeMvt();

	void StartHeal();
	void EndHeal();
};
