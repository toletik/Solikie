// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Actor_MapManager.generated.h"

class AStaticMeshActor;
class AActor_SceneCaptureCamera;

USTRUCT()
struct SOLIKIE_API FMapTile
{
	GENERATED_USTRUCT_BODY()

	//Heat Layer
	int32 _ScorePlayer = 0;
	int32 _ScoreAI = 0;
};

UCLASS()
class SOLIKIE_API AActor_MapManager : public AActor
{
	GENERATED_BODY()
	
public:
	AActor_MapManager();
	virtual void BeginPlay() override;

	FMapTile& GetTile(const FVector& position);
	int32 GetTileIndex(const FVector& position);
	FVector GetTileCenter(const int32 index);
	FVector GetTileCenter(const FVector& position);
	FVector GetPositionInWorld(const FVector2D& positionOnMinimap);

	bool IsPositionInsideMap(const FVector& position);
	FVector GetCenterOfBuilding(const FVector& position, FIntPoint buildingNbOfTiles);
	bool IsBuildingValid(const FVector& position, FIntPoint buildingNbOfTiles);
	
	static float maxRange;
	
private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Initialization, meta = (AllowPrivateAccess = "true"))
	FVector _Origin = FVector::ZeroVector;
	FVector _OriginBottomLeft = FVector::ZeroVector;
	FVector _OriginTopRight = FVector::ZeroVector;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Initialization, meta = (AllowPrivateAccess = "true", ClampMin = "0", ClampMax = "100", UIMin = "0", UIMax = "100"))
	int32 _Height = 1.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Initialization, meta = (AllowPrivateAccess = "true", ClampMin = "0", ClampMax = "100", UIMin = "0", UIMax = "100"))
	int32 _Length = 1.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Initialization, meta = (AllowPrivateAccess = "true", ClampMin = "0", ClampMax = "100", UIMin = "0", UIMax = "10"))
	int32 _TileSize = 1.0f;

	UPROPERTY()
	TArray<FMapTile> _Tiles {};
};
