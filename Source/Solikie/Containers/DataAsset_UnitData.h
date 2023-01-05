#pragma once

#include "CoreMinimal.h"
#include "DataAsset_ElementList.h"
#include "Engine/DataAsset.h"
#include "NiagaraSystem.h"
#include "DataAsset_UnitData.generated.h"

class ACharacter_BaseUnit;

UCLASS(BlueprintType, Blueprintable)
class SOLIKIE_API UDataAsset_UnitData : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	
	//Attack
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attack")
	bool CanHitGround = true;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attack")
	bool CanHitAir = false;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attack")
	float AttackDamage = 10.0f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attack")
	float AttackSpeed = 1.0f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attack")
	float AttackRange = 10.0f;

	//Mvt
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Mvt")
	bool IsFlying = false;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Mvt")
	float MvtSpeedLinear = 600.0f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mvt")
	float TempMvtSpeedLinear = 600.0f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Mvt")
	float MvtSpeedAngular = 200.0f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Mvt")
	float MvtAcceleration = 20.0f;

	//Capture
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Capture")
	float CapturePower = 10.0f;

	//Repair
	/*UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Repair")
	bool CanRepair = false;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Repair")
	float RepairPerSecond = 10.0f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Repair")
	float RepairSpeed = 1.0f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Repair")
	float RepairRange = 10.0f;*/

	//Heal
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Repair")
	float healPower = 10.0f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Repair")
	float HealSpeed = 1.0f;

	//AI
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI")
	float PowerLevel = 1.0f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI")
	float radiusSight = 1.0f;

	//VFX
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "VFX")
	UNiagaraSystem* particlesDeath;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "VFX")
	UNiagaraSystem* particlesAttack;
};

UCLASS(BlueprintType, Blueprintable)
class SOLIKIE_API  UDataAsset_BuildableUnitData : public UDataAsset_ElementList
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Building)
	TSubclassOf<ACharacter_BaseUnit> Template {nullptr};

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI")
	float PowerLevel = 1.0f;

	UDataAsset_BuildableUnitData();
};