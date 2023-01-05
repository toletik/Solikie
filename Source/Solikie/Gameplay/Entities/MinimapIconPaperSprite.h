// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PaperSpriteComponent.h"
#include "PaperSprite.h"
#include "MinimapIconPaperSprite.generated.h"

/**
 * 
 */
UCLASS(Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class SOLIKIE_API UMinimapIconPaperSprite : public UPaperSpriteComponent
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Sprites)
	UPaperSprite* unitSprite {nullptr};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Sprites)
	UPaperSprite* factorySprite {nullptr};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Sprites)
	UPaperSprite* objectiveSprite {nullptr};

	UFUNCTION()
	void SetAsUnit();
	UFUNCTION()
	void SetAsFactory();
	UFUNCTION()
	void SetAsObjective();

	UFUNCTION()
	void SetAsBlue();
	UFUNCTION()
	void SetAsRed();
};
