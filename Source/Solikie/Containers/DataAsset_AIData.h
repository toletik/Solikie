#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "DataAsset_FactoryData.h"
#include "DataAsset_AIData.generated.h"


UENUM()
enum class E_ScoutType
{
	FULL_VISIOM,
	PLAYER,
	IA1
};


UCLASS(BlueprintType, Blueprintable)
class SOLIKIE_API UDataAsset_AIData : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:

	//Planning
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Planning)
	float RefreshRate{ 1.0f };
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Planning)
	float StartUpDelay{ 0.0f };

	//Money
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Money)
	float MoneyAtStart { 50.0f };
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Money)
	float MoneyAddRate { 2.0f };
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Money)
	float MoneyAddAmmount { 10.0f };

	//Scout
	//E_ScoutType

	//PreRequisites
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = PreRequisites)
	uint8  UnitsMinNb_CapturePoint {3};
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = PreRequisites)
	uint8  UnitsMinNb_AttackPlayer {8};

	//Limitations
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Limitations)
	uint8  UnitsMaxNb {10};
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Limitations)
	uint8  FactoriesMaxNb {3};

	//Build
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Build, meta = (AllowPrivateAccess = "true"))
	TArray<UDataAsset_BuildableFactoryData*> PossibleFactories {nullptr};

};

