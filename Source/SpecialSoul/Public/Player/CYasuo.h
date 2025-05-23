// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CPlayerController.h"
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
	UPROPERTY(VisibleAnywhere, Category = "Data|MoveDistance", ReplicatedUsing=OnRep_MoveDist)
	float MoveDistance = 0.0f;
	UFUNCTION()
	void OnRep_MoveDist();

	float GetDamage(bool& OutbIsCri) const;
	int32 GetRange() const { return YasuoStat.ProjectileRange; }

public:
	virtual void Attack() override;
	virtual void UpdatePlayerData(const int32 PlayerLevel) override;
	void WindWall();

private:
	UFUNCTION(Client, Reliable)
	void CRPC_GetWindWallTransfrom();
	UFUNCTION(Server, Reliable)
	void SRPC_WindWall(const FTransform& Transform);

private:
	UPROPERTY(VisibleAnywhere, Category = "Data|Stat")
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

	void GetAttackVectors();
	void TryDefaultAttack(const TArray<FVector>& AttackVectors);
	
	UFUNCTION(Client, Reliable)
	void CRPC_GetAttackVectors();
	UFUNCTION(Server, Reliable)
	void SRPC_TryDefaultAttack(const TArray<FVector>& AttackVectors);
	UFUNCTION(NetMulticast, Reliable)
	void MRPC_EndDefaultAttack();

private: // Anim
	UPROPERTY()
	class UCYasuoAnim* Anim;

private: // Passive Energy
	const int32 PassiveEnergyMax = 100;
	const int32 PassiveEnergyRegen = 4;
	FTimerHandle ChargePassiveEnergyTimer;

	UFUNCTION(Client, Reliable)
	void SRPC_ChargePassiveEnergy();
	UFUNCTION(Server, Reliable)
	void SRPC_ChargePassiveEnergy_Timer();
	UFUNCTION(NetMulticast, Reliable)
	void MRPC_ChargePassiveEnergy(const int32 NewEnergy);

public: // E Skill
	void ESkill(const bool bAnimStart);
	UFUNCTION(NetMulticast, Reliable)
	void MRPC_PlayESkillAnim(const bool bAnimStart);
	void RSkill();
	UFUNCTION(NetMulticast, Reliable)
	void MRPC_PlayRSkillAnim();
	void ActivateSkillMovement(bool bActivate);

public:
	float CalcHaste(float CurHaste) { return PC->CalcHaste(CurHaste); }

private:
	UFUNCTION(Server, Reliable)
	void SRPC_PlayAttackAnim();
	UFUNCTION(NetMulticast, Reliable)
	void MRPC_PlayAttackAnim();

public:
	// UFUNCTION(NetMulticast, Reliable)
	// void MRPC_ChargeCountUI(ESkillKey SkillKey, int32 Count);

	UFUNCTION()
	void OnChargeCountUIChanged(ESkillKey skillKey, int32 count);
	
	UFUNCTION(Client, Reliable)
	void CRPC_UpdateChargeCountUI(ESkillKey skillKey, int32 count, class ACPlayerController* InPC=nullptr);

public:
	virtual void UpgradeWeapon(const int32 Level) override;

public:
	virtual void SetLocalInit(class ACPlayerController* InPC) override;
	virtual void DieProcess() override;
};

