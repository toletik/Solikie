// Fill out your copyright notice in the Description page of Project Settings.


#include "Pawn_Player.h"

#include "AIController.h"
#include "DrawDebugHelpers.h"
#include "UObject/ConstructorHelpers.h"
#include "Components/DecalComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerController.h"
#include "Camera/CameraComponent_CamComp.h"
#include "Entities/Units/Character_BaseUnit.h"
#include "Materials/Material.h"
#include "Kismet/GameplayStatics.h"
#include "Solikie/Containers/DataAsset_FactoryData.h"
#include "Solikie/Gameplay/Entities/Factories/Actor_BaseFactory.h"
#include "Solikie/Containers/Macro.h"
#include "Solikie/Gameplay/Actor_PlayerManager.h"


APawn_Player::APawn_Player()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;

	// Don't rotate character to camera direction
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Create a camera...
	_topDownCameraComponent = CreateDefaultSubobject<UCameraComponent_CamComp>(TEXT("TopDownCamera"));
	_topDownCameraComponent->SetRelativeRotation(FRotator(-90.f, 0.f, 0.f));
	_topDownCameraComponent->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

}
void APawn_Player::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	check(PlayerInputComponent);
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("Zoom", this->_topDownCameraComponent, &UCameraComponent_CamComp::Zoom);

	AActor_PlayerManager* playerManagerRef = Cast<AActor_PlayerManager>(UGameplayStatics::GetActorOfClass(GetWorld(), AActor_PlayerManager::StaticClass()));
	if(playerManagerRef)
	{
		playerManagerRef->PlayerPawnRef = this;

		PlayerInputComponent->BindAction("Order_MoveTo", IE_Pressed, playerManagerRef, &AActor_PlayerManager::SelectedEntitiesPrepareMoveTo);
		PlayerInputComponent->BindAction("Order_AttackTo", IE_Pressed, playerManagerRef, &AActor_PlayerManager::SelectedEntitiesPrepareAttackTo);
		PlayerInputComponent->BindAction("Order_HoldPosition", IE_Pressed, playerManagerRef, &AActor_PlayerManager::SelectedEntitiesHoldPosition);
		PlayerInputComponent->BindAction("Order_CeaseFire", IE_Pressed, playerManagerRef, &AActor_PlayerManager::SelectedEntitiesCeaseFire);

		PlayerInputComponent->BindAction("Validate", IE_Pressed, playerManagerRef, &AActor_PlayerManager::ValidateOrder);
		PlayerInputComponent->BindAction("Validate", IE_Released, playerManagerRef, &AActor_PlayerManager::SelectionEnd);

		PlayerInputComponent->BindAction("Cancel", IE_Pressed, playerManagerRef, &AActor_PlayerManager::CancelOrder);

		PlayerInputComponent->BindAction("SelectAll", IE_Pressed, playerManagerRef, &AActor_PlayerManager::SelectAll);

		PlayerInputComponent->BindAction("FollowSelection", IE_Pressed, playerManagerRef, &AActor_PlayerManager::FollowSelectionStart);
		PlayerInputComponent->BindAction("FollowSelection", IE_Released, playerManagerRef, &AActor_PlayerManager::FollowSelectionEnd);
	}

	
}
