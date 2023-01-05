// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Solikie/Gameplay/Entities/ActorComponent_BaseEntity.h"
#include "Solikie/UI/UserWidget_CircularLoading.h"
#include "Components/CapsuleComponent.h"
#include "Actor_BaseFactory.generated.h"

class UActorComponent_SignInFogOfWar;
class UDataAsset_ElementList;
class UStaticMesh;
class UDataAsset_FactoryData;
class UDataAsset_BuildableUnitData;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDelegateUpdateLoadingWidget, float, percent);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FDelegateUpdateSelectionProgressBar);

UCLASS()
class SOLIKIE_API AActor_BaseFactory : public AActor
{
	GENERATED_BODY()
	
public:
	AActor_BaseFactory();
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	void SetTeam(E_Team newTeam);

	UFUNCTION(BlueprintCallable)
	void AddUnitToQueue(int32  indexBuildable);
	void RemoveUnitFromQueue(int32  indexInQueue);

	UDataAsset_ElementList* GetElementInQueue(int index) { return index < _unitsQueue.Num() ? _unitsQueue[index] : nullptr; }
	int GetQueueSize() { return _unitsQueue.Num(); }
	float GetQueuePowerLevel() {return _powerLevelInQueue; }

	UDataAsset_FactoryData* GetStats() { return _factoryStat; }
	UActorComponent_BaseEntity* GetBaseEntityComponent() { return BaseEntityComponent; }
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UActorComponent_BaseEntity* BaseEntityComponent {nullptr};

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UUserWidget_CircularLoading> circularLoadingClass;

	FDelegateUpdateLoadingWidget delegateUpdateLoadingWidget;
	FDelegateUpdateSelectionProgressBar delegateUpdateSelectionProgressBar;

	
private:
	UFUNCTION()
	void EndTimer();
	UFUNCTION()
	void SpawnUnit();
	UFUNCTION()
	void RepairFactory();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Components, meta = (AllowPrivateAccess = "true"))
	UWidgetComponent* _widgetComponent = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Components, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* _mesh = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Stats, meta = (AllowPrivateAccess = "true"))
	UDataAsset_FactoryData* _factoryStat = nullptr;
	UPROPERTY()
	TArray<UDataAsset_ElementList*> _unitsQueue = TArray<UDataAsset_ElementList*>();

	float _powerLevelInQueue {0.0f};
	FTimerHandle _timerHandle;
	float _timeCoolDown = 0;

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

};
