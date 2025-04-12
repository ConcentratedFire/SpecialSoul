// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Player/CBasePlayer.h"
#include "CYasuo.generated.h"

/**
 * 
 */
UCLASS()
class SPECIALSOUL_API ACYasuo : public ACBasePlayer
{
	GENERATED_BODY()

private:
	ACYasuo();

private:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

public: // Using Charge Enemy
	UPROPERTY(VisibleAnywhere, Category = "Data|MoveDistance")
	float MoveDistance = 0.0f;
	
public:
	virtual void Attack() override;
	virtual void UpdataPlayerData(const int32 PlayerLevel) override;

private:
	UPROPERTY(VisibleAnywhere, Category = "Data|Stat")
	FYasuoAttackData YasuoStat;
	UPROPERTY(VisibleAnywhere, Category = "Data|Stat")
	FYasuoMoveData YasuoMoveInfo;
	UPROPERTY(VisibleAnywhere, Category = "Data|Stat")
	int32 PassiveEnergy = 0;

private: // Attack
	UPROPERTY(EditDefaultsOnly, category=Attack)
	TSubclassOf<class ACTornado> TornadoFactory;
	UPROPERTY(VisibleAnywhere, Category=Attack)
	int32 AttackCnt = 5;

	TArray<FVector> GetAttackVector();

private: // Anim
	UPROPERTY()
	class UCYasuoAnim* Anim;

private: // Spec
	UFUNCTION()
	virtual void PrintAttackDataMap() override; // 임시

	void UpdateYasuoAttackStat(const int32 Level);
	void UpdateYasuoMoveStat(const int32 Level);

private: // Passive Energy
	const int32 PassiveEnergyMax = 100;
	const int32 PassiveEnergyRegen = 4;
	FTimerHandle ChargePassiveEnergyTimer;

	UFUNCTION()
	void ChargePassiveEnergy();

private: // Passive Movement
	void CheckMoveData();
};
