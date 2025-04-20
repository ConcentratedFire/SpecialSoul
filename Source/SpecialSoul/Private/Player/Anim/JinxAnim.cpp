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
