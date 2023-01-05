// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DataAsset_ElementList.generated.h"

class ACharacter_BaseUnit;


UENUM() enum class E_TypeElement
{
	UNIT,
	REPAIR
};

/**
 * 
 */
UCLASS()
class SOLIKIE_API UDataAsset_ElementList : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY()
	E_TypeElement type;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Building)
	UTexture2D* Icon {nullptr};
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Building)
	FString Description {""};
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Building)
	int32 Time {0};
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Building)
	int32 Cost {0};
};
