// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "CYasuoAnim.generated.h"

/**
 * 
 */
UCLASS()
class SPECIALSOUL_API UCYasuoAnim : public UAnimInstance
{
	GENERATED_BODY()

public:
	void PlayAttackMontage();
	void PlayESkillMontage(bool bAnimStart);
	void PlayRSkillMontage();

private:
	virtual void NativeBeginPlay() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	UPROPERTY()
	class ACYasuo* Owner;
	UPROPERTY(BlueprintReadOnly, category = "Yasuo", meta=(allowprivateaccess = "true"))
	float Speed;

private: // Attack Anim
	UPROPERTY(EditDefaultsOnly, category = "Attack")
	class UAnimMontage* AttackMontage;

	UFUNCTION()
	void AnimNotify_YasuoDefaultAttack();

private: // Skill
	UPROPERTY(EditDefaultsOnly, category = "Attack")
	class UAnimMontage* ESkillMontage;
	UPROPERTY(EditDefaultsOnly, category = "Attack")
	class UAnimMontage* RSkillMontage;

	UFUNCTION()
	void AnimNotify_ESkillEnd();
	UFUNCTION()
	void AnimNotify_WindWallSkill();
};
