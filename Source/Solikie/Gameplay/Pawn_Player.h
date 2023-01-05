// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Pawn_Player.generated.h"

class UDataAsset_BuildableFactoryData;
class AActor_PlayerManager;

UCLASS()
class SOLIKIE_API APawn_Player : public APawn
{
	GENERATED_BODY()

public:
	APawn_Player();
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	bool canMove = true;

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent_CamComp* _topDownCameraComponent;
};
