// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GenericTeamAgentInterface.h"
#include "Components/SphereComponent.h"
#include "GameFramework/Actor.h"
#include "Solikie/Containers/Team.h"
#include "Components/CapsuleComponent.h"
#include "Solikie/Gameplay/Entities/MinimapIconPaperSprite.h"
#include "Actor_ObjectifPoint.generated.h"


class UActorComponent_SignInFogOfWar;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDelegateUpdateProgressBar, float, percent);

class UWidgetComponent;
UCLASS()
class SOLIKIE_API AActor_ObjectifPoint : public AActor
{
	GENERATED_BODY()
	
public:	
	AActor_ObjectifPoint();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	FDelegateUpdateProgressBar delegateUpdateProgressBar;

	E_Team _team = E_Team::NONE;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* mesh {nullptr};
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UCapsuleComponent* CapsuleDetection {nullptr};
	
	UFUNCTION()
	void OnCapsuleDetectionBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void OnCapsuleDetectionEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Capture")
	float captureRange = 700.0f;


	// DEBUG TEMP
	UMaterialInstanceDynamic* DynMaterial;

	/*
	* Minimap icons
	*/
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Visibility)
	UMinimapIconPaperSprite* minimapIcon {nullptr};
	
	UFUNCTION()
	void SetupMinimapIcons();
	UFUNCTION()
	void SetupMinimapScale();

private:

	float _captureValue = 0.0f; // -100 IA 100 Player
	float _powerCapture = 0.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Components, meta = (AllowPrivateAccess = "true"))
	UWidgetComponent* _widgetComponent = nullptr;

	FTimerHandle _timerHandle;
	
	void UpdateCapture();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UActorComponent_SignInFogOfWar> _classFogOfWar;
	
	UPROPERTY()
	UActorComponent_SignInFogOfWar* _fogOfWar = nullptr;
	
/************************************************************************/
/*                              Money                                   */
/************************************************************************/

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Money, meta = (AllowPrivateAccess = "true"))
	int32 moneyToAdd = 2;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Money, meta = (AllowPrivateAccess = "true"))
	float timeMoneyRate = 3.0f;
	
	UFUNCTION()
	void AddMoney();
private:
	FTimerHandle _timerHandleMoney;
};
