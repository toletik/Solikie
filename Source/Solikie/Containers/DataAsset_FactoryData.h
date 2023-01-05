#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "DataAsset_FactoryData.generated.h"

class AActor_BaseFactory;
class UDataAsset_BuildableUnitData;
class UDataAsset_ElementList;

UCLASS(BlueprintType, Blueprintable)
class SOLIKIE_API UDataAsset_FactoryData : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Units)
	float radiusSight = 300;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Units)
	uint8  QueueMaxSize {5};
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Units)
	TArray<UDataAsset_ElementList*> UnitsToProduct = TArray<UDataAsset_ElementList*>();
};

UCLASS(BlueprintType, Blueprintable)
class SOLIKIE_API UDataAsset_BuildableFactoryData : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Tiles)
	FIntPoint _TilesOccupied = FIntPoint::ZeroValue;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Building)
	TSubclassOf<AActor_BaseFactory> Template {nullptr};
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Building)
	UTexture2D* Icon {nullptr};
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Building)
	FString Description {""};
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Building)
	int32 Cost {0};
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Building)
	int32 Time {0};
};

