// Fill out your copyright notice in the Description page of Project Settings.


#include "MinimapIconPaperSprite.h"

void UMinimapIconPaperSprite::SetAsUnit()
{
	if (unitSprite)
		SetSprite(unitSprite);
}

void UMinimapIconPaperSprite::SetAsFactory()
{
	if (factorySprite)
		SetSprite(factorySprite);
	SetRelativeLocation(FVector(0,0,200));
}

void UMinimapIconPaperSprite::SetAsObjective()
{
	//UPaperSprite* paperSprite = NewObject<UPaperSprite>(objectiveSprite);
	if (objectiveSprite)
		SetSprite(objectiveSprite);
}

void UMinimapIconPaperSprite::SetAsBlue()
{
	FLinearColor col = GetSpriteColor();
	col.R = 0;
	col.G = 0;
	col.B = 1;
	SetSpriteColor(col);
}

void UMinimapIconPaperSprite::SetAsRed()
{
	FLinearColor col = GetSpriteColor();
	col.R = 1;
	col.G = 0;
	col.B = 0;
	SetSpriteColor(col);
}
