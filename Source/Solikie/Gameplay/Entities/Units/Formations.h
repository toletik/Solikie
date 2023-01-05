// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character_BaseUnit.h"
#include "UObject/NoExportTypes.h"
#include "Formations.generated.h"


UENUM()
enum class E_Formation
{
	Circle,
	Square,
	Triangle
};


UCLASS()
class SOLIKIE_API UFormations : public UObject
{
	GENERATED_BODY()

public:

	UFUNCTION()
	static void MoveWithFormation(FVector destination, TArray<ACharacter_BaseUnit*> units, E_Formation formationType, bool IsAttacking);


	UFUNCTION(BlueprintCallable)
	static TArray<FVector> GetTrianglePositions(FVector targetPos, FVector centroid, int size);

	UFUNCTION(BlueprintCallable)
	static TArray<FVector> GetSquarePositions(FVector targetPos, FVector centroid, int size);

	UFUNCTION(BlueprintCallable)
	static TArray<FVector> GetCirclePositions(FVector targetPos, FVector centroid, int size);
	
	UFUNCTION()
	static int GetNearestPos(FVector position, TArray<bool>& beenChosen, TArray<ACharacter_BaseUnit*> units); //returns the index in the array

	UFUNCTION()
	static FVector GetCentroid(TArray<ACharacter_BaseUnit*> units);

	UFUNCTION()
	static TMap<FVector, float> GetAllPointsDist(TArray<FVector> positions, FVector centroid);
	
	
};
