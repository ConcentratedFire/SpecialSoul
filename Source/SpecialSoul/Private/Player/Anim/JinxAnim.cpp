// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/Anim/JinxAnim.h"
#include "Player/Jinx.h"
#include "Player/Components/SkillComponent.h"

void UJinxAnim::NativeBeginPlay()
{
	Super::NativeBeginPlay();

	APawn* OwnerPawn = TryGetPawnOwner();
	if (!OwnerPawn) return;
	Jinx = Cast<AJinx>(OwnerPawn);
}

void UJinxAnim::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (!Jinx) return;
	Speed = Jinx->GetVelocity().Size();
}

void UJinxAnim::AnimNotify_Attack()
{
	if (Jinx->HasAuthority())
	{
	// 	Jinx->Attack();
	}
}

void UJinxAnim::AnimNotify_ESkill()
{
	if (Jinx->HasAuthority())
	{
		Jinx->UseESkill();
	}
}

void UJinxAnim::AnimNotify_RSkill()
{
	if (Jinx->HasAuthority())
	{
		Jinx->UseRSkill();
	}
}


void UJinxAnim::AnimNotify_AttackBegin()
{
	//LOG_S(Error, TEXT("AnimNotify_AttackBegin"));
	Jinx->ActivateSkillMovement(true, true);
}

void UJinxAnim::AnimNotify_AttackEnd()
{
	//LOG_S(Error, TEXT("AnimNotify_AttackEnd"));
	Jinx->ActivateSkillMovement(false, true);
}

void UJinxAnim::AnimNotify_SkillBegin()
{
	//LOG_S(Error, TEXT("AnimNotify_SkillBegin"));
	Jinx->ActivateSkillMovement(true, false);
}

void UJinxAnim::AnimNotify_SkillEnd()
{
	//LOG_S(Error, TEXT("AnimNotify_SkillEnd"));
	Jinx->ActivateSkillMovement(false, false);
	Jinx->bUsingSkill = false;
}
