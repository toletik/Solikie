// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraComponent.h"
#include "Windows/AllowWindowsPlatformTypes.h"
#include "CameraComponent_CamComp.generated.h"

class APawn_Player;
/**
 * 
 */
UCLASS()
class SOLIKIE_API UCameraComponent_CamComp : public UCameraComponent
{
	GENERATED_BODY()

	
public:

	UCameraComponent_CamComp();
	virtual void BeginPlay();
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction);

	void Zoom(float Value);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float edgeThicknessWidthScreen = 100.0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float edgeThicknessHeightScreen = 100.0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Speed)
	float speedCamera = 200.0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Speed)
	float speedZoom = 10.0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Coordinates)
	float xMin = -300.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Coordinates)
	float xMax = 300.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Coordinates)
	float yMin = -300.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Coordinates)
	float yMax = 300.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Coordinates)
	float zMin = 100.0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Coordinates)
	float zMax = 800.0;
	

private:

	FVector2D _edgeThickness;
	FVector2D _mousePosition;
	FVector _direction;

	AActor* _owner;
	APawn_Player* _player;
	APlayerController* _playerController;

	void UpdateSizeEdgeThickness(OPTIONAL FViewport* Viewport = GEngine->GameViewport->Viewport);

	bool CalculateVectorDirectionIfMousePositionIsOnEdge();

	void ClampXY();
};
