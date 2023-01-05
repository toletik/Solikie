// Fill out your copyright notice in the Description page of Project Settings.


#include "UserWidget_ProgressObjectifPoint.h"

#include "Components/ProgressBar.h"

void UUserWidget_ProgressObjectifPoint::UpdateProgressBar(float newValue)
{
	_progressBar->SetPercent(abs(newValue) / 100);
	_progressBar->SetFillColorAndOpacity(newValue < 0 ? FLinearColor::Red : newValue > 0 ? FLinearColor::Blue : FLinearColor::Gray);
}
