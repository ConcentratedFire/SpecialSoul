// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SkillComponent.h"
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
	void PrintNetLog();
	virtual void Tick(float DeltaTime) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

public: // Using Charge Enemy
	UPROPERTY(VisibleAnywhere, Category = "Data|MoveDistance", Replicated)
	float MoveDistance = 0.0f;

	float GetDamage(bool& OutbIsCri) const;
	int32 GetRange() const { return YasuoStat.ProjectileRange; }

public:
	virtual void Attack() override;
	virtual void UpdatePlayerData(const int32 PlayerLevel) override;
	void WindWall();

private:
	UPROPERTY(VisibleAnywhere, Category = "Data|Stat", Replicated)
	int32 PassiveEnergy = 0;

private: // Attack
	UPROPERTY(EditDefaultsOnly, category=Attack)
	TSubclassOf<class ACTornado> TornadoFactory;

public:
	UPROPERTY(BlueprintReadOnly, Category=Attck, ReplicatedUsing=OnRep_RotateArrow)
	FVector AttackFrontVector;

	UFUNCTION(Client, Unreliable)
	void CRPC_SetAttackFrontVector();
	UFUNCTION(Server, Unreliable)
	void SRPC_ReplicateAttackVector(FVector AttackDir);
	UFUNCTION()
	void OnRep_RotateArrow();

	TArray<FVector> GetAttackVector();

private: // Anim
	UPROPERTY()
	class UCYasuoAnim* Anim;

private: // Passive Energy
	const int32 PassiveEnergyMax = 100;
	const int32 PassiveEnergyRegen = 4;
	FTimerHandle ChargePassiveEnergyTimer;

	UFUNCTION(Server, Reliable)
	void SRPC_ChargePassiveEnergy();
	UFUNCTION()
	void ChargePassiveEnergy();

public: // E Skill
	void ESkill(const bool bAnimStart);
	void RSkill();
	void ActivateSkillMovement(bool bActivate);
};
