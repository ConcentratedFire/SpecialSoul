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

	int32 GetDamage() const { return YasuoStat.Damage; }
	int32 GetRange() const { return YasuoStat.ProjectileRange; }

public:
	virtual void Attack() override;
	virtual void UpdatePlayerData(const int32 PlayerLevel) override;

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

public:
	UPROPERTY(BlueprintReadOnly, Category=Attck)
	FVector AttackFrontVector;
	
	void SetAttackFrontVector();
	TArray<FVector> GetAttackVector();

private: // Anim
	UPROPERTY()
	class UCYasuoAnim* Anim;

private: // Spec
	UFUNCTION()
	virtual void PrintAttackDataMap() override; // 임시

	virtual void UpgradeWeapon(const int32 Level) override;
	void UpdateYasuoMoveStat(const int32 Level);

private: // Passive Energy
	const int32 PassiveEnergyMax = 100;
	const int32 PassiveEnergyRegen = 4;
	FTimerHandle ChargePassiveEnergyTimer;

	UFUNCTION()
	void ChargePassiveEnergy();

private: // Passive Movement
	void CheckMoveData();

private:
	virtual void RotateArrow() override;
	
};
