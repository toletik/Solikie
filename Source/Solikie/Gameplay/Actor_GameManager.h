// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Entities/Factories/Actor_BaseFactory.h"
#include "Entities/ObjectifPoint/Actor_ObjectifPoint.h"
#include "Entities/Units/Character_BaseUnit.h"
#include "GameFramework/Actor.h"
#include "Solikie/Containers/Team.h"
#include "Actor_GameManager.generated.h"

class AActor_PlayerManager;
class AActor_AIManager;
class UDataAsset_BuildableFactoryData;
class UUserWidget_EndScreen;

UCLASS()
class SOLIKIE_API AActor_GameManager : public AActor
{
	GENERATED_BODY()

public:
	AActor_GameManager();
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	static class AActor_GameManager* instance;
	static void SetInstance(AActor_GameManager* newInstance);
	static class AActor_GameManager* GetInstance();

	//Currency 
	void AddMoney(E_Team team, float amount);
	void RemoveMoney(E_Team team, float amount);
	bool HasEnoughMoney(E_Team team, float amount);

	//Armies
	void RegisterUnit(ACharacter_BaseUnit* newUnit);
	void RegisterFactory(AActor_BaseFactory* newFactory);
	void RegisterObjectifPoint(AActor_ObjectifPoint* objectifPoint);
	void UnRegisterUnit(ACharacter_BaseUnit* newUnit);
	void UnRegisterFactory(AActor_BaseFactory* newFactory);

	//AI
	void RegisterUnitSighted(E_Team teamWhoSaw, ACharacter_BaseUnit* newUnit);
	void RegisterFactorySighted(E_Team teamWhoSaw, AActor_BaseFactory* newFactory);
	void UnRegisterUnitSighted(E_Team teamWhoSaw, ACharacter_BaseUnit* newUnit);
	void UnRegisterFactorySighted(E_Team teamWhoSaw, AActor_BaseFactory* newFactory);
	void RegisterBaseAgressor(ACharacter_BaseUnit* aggressor);

	//CapturePoints
	bool HasTeamAllCapturePoints(E_Team team);
	AActor_ObjectifPoint* GetFirstEnemyCapturePoint(E_Team team);
	
	//EndGame
	void EndGame(bool playerWon);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Refs)
	AActor_PlayerManager* _playerManagerRef {nullptr};
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Refs)
	AActor_AIManager* _AiManagerRef {nullptr};


private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UUserWidget_EndScreen> EndScreenTemplate;
	UPROPERTY()
	TArray<AActor_ObjectifPoint*> _CapturePoints;
};
