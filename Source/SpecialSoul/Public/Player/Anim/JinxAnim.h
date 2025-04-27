// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "JinxAnim.generated.h"

class AJinx;
/**
 * 
 */
UCLASS()
class SPECIALSOUL_API UJinxAnim : public UAnimInstance
{
	GENERATED_BODY()

public:
	virtual void NativeBeginPlay() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "JinxAnim")
	TObjectPtr<AJinx> Jinx;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "JinxAnim")
	float Speed;

	// 애님 노티파이
	UFUNCTION()
	void AnimNotify_Attack();
	
	UFUNCTION()
	void AnimNotify_ESkill();

	UFUNCTION()
	void AnimNotify_RSkill();

	UFUNCTION()
	void AnimNotify_AttackBegin();
	
	UFUNCTION()
	void AnimNotify_AttackEnd();


	UFUNCTION()
	void AnimNotify_SkillBegin();

	UFUNCTION()
	void AnimNotify_SkillEnd();
};
