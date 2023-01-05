// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SphereComponent.h"
#include "GameFramework/Actor.h"
#include "Actor_SceneCaptureCamera.generated.h"

UCLASS()
class SOLIKIE_API AActor_SceneCaptureCamera : public AActor
{
	GENERATED_BODY()

	USphereComponent* sphereComponent;
	
public:	

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SceneCapture", meta = (AllowPrivateAccess = "true"))
	USceneCaptureComponent2D* _sceneCapture2D  {nullptr};

	// Sets default values for this actor's properties
	AActor_SceneCaptureCamera();

	void InitSceneCaptureWithMap(FVector position, float orthoWidth);
};
