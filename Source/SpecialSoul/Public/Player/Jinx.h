// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CBasePlayer.h"
#include "Jinx.generated.h"

class ACPlayerController;
class USkillComponent;
class UCDataSheetUtility;
class UJinxAnim;
class ISkillStrategy;

// 스킬 사용 중에는 방향 회전 막기


UCLASS()
class SPECIALSOUL_API AJinx : public ACBasePlayer
{
	GENERATED_BODY()

public:
	AJinx();

	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	virtual void Attack() override;
	void UseESkill();
	void UseRSkill();
	
	virtual void UpdatePlayerData(const int32 PlayerLevel) override;

	UPROPERTY()
	class UAnimInstance* AnimInstance;
	
public:
	bool bUsingSkill {false};
	
	void ActivateSkillMovement(bool bActive, bool bAttack); // 스킬 사용 중 캐릭터 회전 관련
	
	// void StartAttack();

	void RotateToMouseCursor();
	
	UFUNCTION(Server, Reliable)
	void SRPC_UseSkill(ESkillKey Key);
	
	UFUNCTION(NetMulticast, Reliable)
	void MRPC_PlaySkillMontage(ESkillKey Key);

private:
	void UpdateJinxAttackStat(int32 PlayerLevel);
	
	FTimerHandle AttackTimer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Anim", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UAnimMontage> AttackMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Anim", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UAnimMontage> ESkillMontage;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Anim", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UAnimMontage> RSkillMontage;

public:
	virtual void SetLocalInit(class ACPlayerController* InPC) override;

	float GetDamage(bool& OutbIsCri) const;

	virtual void DieProcess() override;
};

