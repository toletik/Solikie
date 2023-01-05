// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor_MapManager.h"

#include "Engine/StaticMeshActor.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Solikie/Gameplay/Camera/Actor_SceneCaptureCamera.h"

float AActor_MapManager::maxRange;

AActor_MapManager::AActor_MapManager()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}
void AActor_MapManager::BeginPlay()
{
	Super::BeginPlay();
	
	_OriginBottomLeft = _Origin - FVector(_Height * _TileSize / 2, _Length * _TileSize / 2, 0);
	_OriginTopRight = _Origin + FVector(_Height * _TileSize / 2, _Length * _TileSize / 2, 0);

	for(int i = 0; i < _Height; ++i)
		for(int j = 0; j < _Length; ++j)
			_Tiles.Add(FMapTile{});

	AActor_SceneCaptureCamera* sceneCapture = Cast<AActor_SceneCaptureCamera>
		(UGameplayStatics::GetActorOfClass(GetWorld(), AActor_SceneCaptureCamera::StaticClass()));
	if (sceneCapture)
		sceneCapture->InitSceneCaptureWithMap(_Origin + FVector(0,0,1000.f), _Height > _Length ? _Height * _TileSize : _Length * _TileSize);

	maxRange =  _Height > _Length ? _Height * _TileSize / 2 : _Length * _TileSize / 2;
	
	AStaticMeshActor* planeFogOfWar = nullptr;
	TArray<AActor*> outActor;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AStaticMeshActor::StaticClass(), outActor);
	for (AActor* actor : outActor)
	{
		if (actor->ActorHasTag("FogOfWar"))
		{
			planeFogOfWar = Cast<AStaticMeshActor>(actor);
			break;
		}
	}
	
	if (planeFogOfWar)
		planeFogOfWar->SetActorScale3D(FVector(_Height, _Length, 1));
}

FMapTile& AActor_MapManager::GetTile(const FVector& position)
{	
	return _Tiles[GetTileIndex(position)];
}
int32 AActor_MapManager::GetTileIndex(const FVector& position)
{
	FVector positionInRepere = position - _OriginBottomLeft;

	return FMath::Floor(positionInRepere.Y / _TileSize) + FMath::Floor(positionInRepere.X / _TileSize) * _Length;
}
FVector AActor_MapManager::GetTileCenter(const int32 index)
{
	int32 indexHeight = index / _Length;
	int32 indexLength = index % _Length;

	return _OriginBottomLeft +  FVector(indexHeight * _TileSize + _TileSize / 2, indexLength * _TileSize + _TileSize / 2, 0);
}
FVector AActor_MapManager::GetTileCenter(const FVector& position)
{
	return GetTileCenter(GetTileIndex(position));
}

FVector AActor_MapManager::GetPositionInWorld(const FVector2D& positionOnMinimap)
{
	FVector position;  
	position.Y = _Origin.X + (positionOnMinimap.X * 2 - 1) * _Length * (_TileSize / 2);
	position.X = _Origin.Y - (positionOnMinimap.Y * 2 - 1) * _Height * (_TileSize / 2);
	return position;
}

bool AActor_MapManager::IsPositionInsideMap(const FVector& position)
{
	return ( _OriginBottomLeft.X <= position.X && position.X <= _OriginTopRight.X &&
			 _OriginBottomLeft.Y <= position.Y && position.Y <= _OriginTopRight.Y);
}
FVector AActor_MapManager::GetCenterOfBuilding(const FVector& position, FIntPoint buildingNbOfTiles)
{
	//if buildingNbOfTiles has peer values add half tileSize for each peer value
	FVector CenterOfBuilding =  GetTileCenter(position) + FVector((int)(buildingNbOfTiles.X + 1) % 2 * _TileSize / 2, (int)(buildingNbOfTiles.Y + 1) % 2 * _TileSize / 2, 0);

	//Clamp so the building don't exceed map borders
	return FVector(FMath::Clamp(CenterOfBuilding.X, _OriginBottomLeft.X + buildingNbOfTiles.X * _TileSize / 2, _OriginTopRight.X - buildingNbOfTiles.X * _TileSize / 2),
				   FMath::Clamp(CenterOfBuilding.Y, _OriginBottomLeft.Y + buildingNbOfTiles.Y * _TileSize / 2, _OriginTopRight.Y - buildingNbOfTiles.Y * _TileSize / 2),
				   0);
}
bool AActor_MapManager::IsBuildingValid(const FVector& position, FIntPoint buildingNbOfTiles)
{
	FVector Center = GetCenterOfBuilding(position, buildingNbOfTiles);
	FVector Extents = FVector(_TileSize / 2 * buildingNbOfTiles.X, _TileSize / 2 * buildingNbOfTiles.Y, 10) * 0.9f; //We multiply by 0.9 to don't touch sides tiles

	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes = TArray<TEnumAsByte<EObjectTypeQuery>>();
	UClass* ActorClassFilter = nullptr;
	TArray<AActor*> ActorsToIgnore = TArray<AActor*>();
	TArray<AActor*> OutActors = TArray<AActor*>();

	return !(UKismetSystemLibrary::BoxOverlapActors(GetWorld(), Center, Extents, ObjectTypes, ActorClassFilter, ActorsToIgnore, OutActors));
}





