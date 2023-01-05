// Fill out your copyright notice in the Description page of Project Settings.


#include "UserWidget_HealthBar.h"

void UUserWidget_HealthBar::UpdateProgressBar(float newValue)
{
	_progressBar->SetPercent(newValue);
}