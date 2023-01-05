// Fill out your copyright notice in the Description page of Project Settings.


#include "UserWidget_Minimap.h"

#include "Blueprint/SlateBlueprintLibrary.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Kismet/GameplayStatics.h"
#include "Solikie/Containers/Macro.h"
#include "Solikie/Gameplay/Pawn_Player.h"
#include "Solikie/Gameplay/Map/Actor_MapManager.h"


void UUserWidget_Minimap::NativeOnInitialized()
{
	_player = Cast<APawn_Player>(GetOwningPlayer()->GetPawn());
	_mapManager = Cast<AActor_MapManager>(UGameplayStatics::GetActorOfClass(GetWorld(), AActor_MapManager::StaticClass()));
	
	_button->OnClicked.AddUniqueDynamic(this, &UUserWidget_Minimap::MouseButtonDown);

}
void UUserWidget_Minimap::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	_player->canMove = false;
	focusMinimap = true;
	
}
void UUserWidget_Minimap::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	_player->canMove = true;
	focusMinimap = false;
}

void UUserWidget_Minimap::SetTexture(UTexture2D* tex)
{
	if (!tex)
		return;
	_minimap->SetBrushFromTexture(tex);
}

void UUserWidget_Minimap::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}

void UUserWidget_Minimap::MouseButtonDown()
{
	if (focusMinimap)
	{
		FVector2D pixelPosition;
		FVector2D viewportMinPosition;
		FVector2D viewportMaxPosition;

		USlateBlueprintLibrary::LocalToViewport(GetWorld(), _button->GetTickSpaceGeometry(),
			FVector2D( 0, 0 ), pixelPosition, viewportMinPosition);
		USlateBlueprintLibrary::LocalToViewport(GetWorld(), _button->GetTickSpaceGeometry(),
			_button->GetTickSpaceGeometry().GetLocalSize(), pixelPosition, viewportMaxPosition);

		FVector2D mousePosition = UWidgetLayoutLibrary::GetMousePositionOnViewport(GetWorld());
		mousePosition = ( mousePosition - viewportMinPosition ) / ( viewportMaxPosition - viewportMinPosition );
		
		FVector newPosition = _mapManager->GetPositionInWorld(mousePosition);
		newPosition.Z = _player->GetActorLocation().Z;
		_player->SetActorLocation(newPosition);
	}
}