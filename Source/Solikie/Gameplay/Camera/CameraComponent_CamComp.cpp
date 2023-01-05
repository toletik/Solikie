// Fill out your copyright notice in the Description page of Project Settings.


#include "CameraComponent_CamComp.h"

#include "Solikie/Gameplay/Pawn_Player.h"


UCameraComponent_CamComp::UCameraComponent_CamComp()
{
	PrimaryComponentTick.bCanEverTick = true;
	FDelegateHandle delegate = FViewport::ViewportResizedEvent.AddLambda([this](FViewport* Viewport, uint32)
	{
		UpdateSizeEdgeThickness(Viewport);
	});
}
void UCameraComponent_CamComp::BeginPlay()
{
	Super::BeginPlay();
	UpdateSizeEdgeThickness();
	_owner = GetOwner();
	_player = Cast<APawn_Player>(_owner);
	if (!_player)
		UE_LOG(LogTemp, Fatal, TEXT("IS NOT A PLAYER MANAGER"));
	_playerController = Cast<APlayerController>(_player->Controller);
	if (!_playerController)
		UE_LOG(LogTemp, Fatal, TEXT("IS NOT A PLAYER CONTROLLER"));
}
void UCameraComponent_CamComp::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	if (CalculateVectorDirectionIfMousePositionIsOnEdge() && _player->canMove)
	{
		_owner->SetActorLocation(_owner->GetActorLocation() + _direction * speedCamera * DeltaTime);
		ClampXY();
	}
}
void UCameraComponent_CamComp::Zoom(float Value)
{
	FVector newPlayerLocation = _owner->GetActorLocation();
	newPlayerLocation += (newPlayerLocation.Z < zMax && newPlayerLocation.Z > zMin) ?
			GetForwardVector() * Value * speedZoom : FVector(0, 0, GetForwardVector().Z * Value * speedZoom);
	newPlayerLocation.Z = FMath::Clamp(newPlayerLocation.Z, zMin, zMax);
	_owner->SetActorLocation(newPlayerLocation);
}

void UCameraComponent_CamComp::UpdateSizeEdgeThickness(FViewport* Viewport)
{
	FIntPoint sizeViewport = Viewport->GetSizeXY();
	_edgeThickness.X = sizeViewport.X;
	_edgeThickness.Y = sizeViewport.Y;
}
bool UCameraComponent_CamComp::CalculateVectorDirectionIfMousePositionIsOnEdge()
{
	_playerController->GetMousePosition(_mousePosition.X, _mousePosition.Y);
	_direction.X = (_mousePosition.Y <= edgeThicknessWidthScreen) ? 1 :
		(_mousePosition.Y >= _edgeThickness.Y - edgeThicknessWidthScreen) ? -1 : 0;
	
	_direction.Y = (_mousePosition.X <= edgeThicknessHeightScreen) ? -1 :
		(_mousePosition.X >= _edgeThickness.X - edgeThicknessHeightScreen) ? 1 : 0;

	return _direction.X != 0 || _direction.Y != 0;
}
void UCameraComponent_CamComp::ClampXY()
{
	FVector newPlayerLocation = _owner->GetActorLocation();
	newPlayerLocation.X = FMath::Clamp(newPlayerLocation.X, xMin, xMax);
	newPlayerLocation.Y = FMath::Clamp(newPlayerLocation.Y, yMin, yMax);
	_owner->SetActorLocation(newPlayerLocation);
}