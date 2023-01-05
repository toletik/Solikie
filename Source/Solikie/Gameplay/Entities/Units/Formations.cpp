// Fill out your copyright notice in the Description page of Project Settings.


#include "Formations.h"
#include "../../../Containers/Army.h"
#include "../../../Containers/Macro.h"


void UFormations::MoveWithFormation(FVector destination, TArray<ACharacter_BaseUnit*> units, E_Formation formationType, bool IsAttacking)
{
	FVector centroid = GetCentroid(units);
	TArray<FVector> allPos;


	switch (formationType)
	{
	case E_Formation::Circle :
		allPos = GetCirclePositions(destination, centroid, units.Num());
		break;
	case E_Formation::Square:
		allPos = GetSquarePositions(destination, centroid, units.Num());
		break;
	case E_Formation::Triangle:
		allPos = GetTrianglePositions(destination, centroid, units.Num());
		break;
	default:
		return;
	}

	//TO CLEAN
	TArray<bool> beenChosen = {};
	for (int it = 0; it < units.Num(); it++)
		beenChosen.Emplace(false);


	TMap<FVector, float> posDist = GetAllPointsDist(allPos, centroid);
	for (TPair<FVector, float> pair : posDist)
	{
		int chosenIdx = GetNearestPos(pair.Key, beenChosen, units);
		beenChosen[chosenIdx] = true;

		if(IsAttacking)
			units[chosenIdx]->AttackTo(pair.Key);
		else
			units[chosenIdx]->MoveTo(pair.Key);

	}
}

TArray<FVector> UFormations::GetTrianglePositions(FVector targetPos, FVector centroid, int size)
{
	FVector fwdDir = (targetPos - centroid).GetSafeNormal();
	FVector leftDir {-fwdDir.Y, fwdDir.X, 0.0f};
	TArray<FVector> allPos{};

	int currLine = 0;
	int wantedUnitInLine = 1;
	int currentUnitInLine = 0;
	
	while (allPos.Num() != size)
	{

		allPos.Emplace(targetPos 
						-120.f * fwdDir * currLine
						+ 100.f * leftDir * (currentUnitInLine - wantedUnitInLine / 2));

		currentUnitInLine++;
		if (wantedUnitInLine == currentUnitInLine)
		{
			currLine++;
			wantedUnitInLine = 1 + currLine * 2;
			currentUnitInLine = 0;
		}
	}

	return allPos;
}

TArray<FVector> UFormations::GetSquarePositions(FVector targetPos, FVector centroid, int size)
{
	int sideSize = FMath::CeilToInt( FMath::Sqrt(size-1));
	FVector fwdDir = (targetPos - centroid).GetSafeNormal();
	FVector leftDir {-fwdDir.Y, fwdDir.X, 0.0f};
	TArray<FVector> allPos{};

	targetPos -= (sideSize / 2.f) * -120.f * fwdDir + (sideSize / 2.f) * 120.f * leftDir;

	int currLine = 0;
	int currentUnitInLine = 0;
	
	while (allPos.Num() != size)
	{

		allPos.Emplace(targetPos 
						-120.f * fwdDir * currLine
						+ 120.f * leftDir * (currentUnitInLine));

		currentUnitInLine++;
		if (currentUnitInLine == sideSize)
		{
			currLine++;
			currentUnitInLine = 0;
		}
	}
	return allPos;
}

TArray<FVector> UFormations::GetCirclePositions(FVector targetPos, FVector centroid, int size)
{
	TArray<FVector> allPos{};
	int nbCircles = FMath::Floor( FMath::Pow(size, 1.f/3.f));
	if (nbCircles == 0)
		nbCircles = 1;
	int numUnitInCircle = FMath::CeilToInt(size / nbCircles);
	int excess = size - ((nbCircles - 1) * numUnitInCircle);
	float dist = 120.f + excess * 5.f;
	if (size <= 15)
	{
		nbCircles = 1;
		excess = size;
	}

	for (int i = 0; i < numUnitInCircle; i++) //drawing all circles excepted the farthest one
	{
		float rad = 2.f * PI / numUnitInCircle * i;
		for (int recur = 1; recur < nbCircles; recur++)
		{
			FVector newPos = targetPos;

			newPos.X += FMath::Cos(rad) * recur * dist;
			newPos.Y += FMath::Sin(rad) * recur * dist;

			allPos.Emplace(newPos);
		}
	}
	for (int i = 0; i < excess; i++) //drawing the farthest circle
	{
		float rad = 2.f * PI / excess * i;
		FVector newPos = targetPos;

		newPos.X += FMath::Cos(rad) * nbCircles * dist;
		newPos.Y += FMath::Sin(rad) * nbCircles * dist;

		allPos.Emplace(newPos);
	}

	return allPos;
}

FVector UFormations::GetCentroid(TArray<ACharacter_BaseUnit*> units)
{
	FVector centroid = FVector::ZeroVector;
	for (ACharacter_BaseUnit* unit : units)
	{
		centroid += unit->GetActorLocation();
	}
	centroid /= units.Num();
	return centroid;
}

TMap<FVector, float> UFormations::GetAllPointsDist(TArray<FVector> positions, FVector centroid)
{
	TMap<FVector, float> posDist {};
	for (FVector position : positions)
	{
		float dist = (centroid - position).Size();
		posDist.Emplace(position, dist);
	}
	posDist.ValueSort([](float A, float B)
	{
		return A > B;
	});
	return posDist;
}

int UFormations::GetNearestPos(FVector position, TArray<bool>& beenChosen, TArray<ACharacter_BaseUnit*> units)
{

	if (units.Num() == 0)
	{

		return 0;
	}

	int currIdx = 0;
	float minDist = TNumericLimits<float>::Max();
	for (int idx = 0; idx < units.Num(); idx++)
	{
		
		if (!beenChosen[idx])
		{
			float currDist = (position - units[idx]->GetActorLocation()).Size();
			{
				//print (FString::SanitizeFloat(currDist));
				//print (" unitPos: " + FString::SanitizeFloat(units[idx]->GetActorLocation().X) + " Y: " + FString::SanitizeFloat(units[idx]->GetActorLocation().Y));
				//print (" pos: " + FString::SanitizeFloat(position.X) + " Y: " + FString::SanitizeFloat(position.Y));
			}
			if (currDist < minDist)
			{

				minDist = currDist;
				currIdx = idx;
			}			
		}

	}
	return currIdx;
}
