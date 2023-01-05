// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor_SceneCaptureCamera.h"

#include "Kismet/GameplayStatics.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Components/SceneCaptureComponent2D.h"

// Sets default values
AActor_SceneCaptureCamera::AActor_SceneCaptureCamera()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	sphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("Root"));
	RootComponent = sphereComponent;
	
	_sceneCapture2D = CreateDefaultSubobject<USceneCaptureComponent2D>(TEXT("CameraSceneCapture"));
	_sceneCapture2D->SetWorldRotation(FRotator(-90.f, 0.f, 0.f));
	_sceneCapture2D->SetupAttachment(sphereComponent);
}

void AActor_SceneCaptureCamera::InitSceneCaptureWithMap(FVector position, float orthoWidth)
{
	_sceneCapture2D->SetWorldLocation(position);
	_sceneCapture2D->OrthoWidth = orthoWidth;
}

