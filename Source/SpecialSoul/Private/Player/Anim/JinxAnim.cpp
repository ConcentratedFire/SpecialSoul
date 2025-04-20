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
		Jinx->Attack();
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

void UJinxAnim::AnimNotify_SkillBegin()
{
	Jinx->SkillComponent->UseSkillCount++;

	Jinx->ActivateSkillMovement(true);
}

void UJinxAnim::AnimNotify_SkillEnd()
{
	Jinx->SkillComponent->UseSkillCount--;

	if (Jinx->SkillComponent->UseSkillCount <= 0)
	{
		Jinx->SkillComponent->UseSkillCount = 0;
		Jinx->ActivateSkillMovement(false);
	}
}

void UJinxAnim::AnimNotify_ESkillBegin()
{
	Jinx->SkillComponent->UseSkillCount++;

	Jinx->ActivateSkillMovement(true);
}

void UJinxAnim::AnimNotify_ESkillEnd()
{
	Jinx->SkillComponent->UseSkillCount--;

	if (Jinx->SkillComponent->UseSkillCount <= 0)
	{
		Jinx->SkillComponent->UseSkillCount = 0;
		Jinx->ActivateSkillMovement(false);
	}
}

void UJinxAnim::AnimNotify_RSkillBegin()
{
	Jinx->SkillComponent->UseSkillCount++;

	Jinx->ActivateSkillMovement(true);
}

void UJinxAnim::AnimNotify_RSkillEnd()
{
	Jinx->SkillComponent->UseSkillCount--;

	if (Jinx->SkillComponent->UseSkillCount <= 0)
	{
		Jinx->SkillComponent->UseSkillCount = 0;
		Jinx->ActivateSkillMovement(false);
	}
}
