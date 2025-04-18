// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/CMiddleBoss.h"

#include <ranges>

#include "Components/CapsuleComponent.h"
#include "ObjectPool/CObjectPoolManager.h"

ACMiddleBoss::ACMiddleBoss()
{
	SetActorScale3D(FVector(5));

	GetCapsuleComponent()->SetCapsuleRadius(25.f);
	GetCapsuleComponent()->SetCapsuleHalfHeight(25.f);

	GetMesh()->SetRelativeLocation(FVector(0, 0, -17));
	GetMesh()->SetRelativeRotation(FRotator(0, -90, 0));

	ConstructorHelpers::FObjectFinder<UAnimMontage> tmpAttackMontage(
		TEXT("/Script/Engine.AnimMontage'/Game/Asset/gromp/AM_MiddleBoseAttack.AM_MiddleBoseAttack'"));
	if (tmpAttackMontage.Succeeded())
		AttackMontage = tmpAttackMontage.Object;

	ConstructorHelpers::FObjectFinder<UAnimMontage> tmpDieMontage(
		TEXT("/Script/Engine.AnimMontage'/Game/Asset/gromp/AM_MiddleBossDie.AM_MiddleBossDie'"));
	if (tmpDieMontage.Succeeded())
		DieMontage = tmpDieMontage.Object;
}

void ACMiddleBoss::Attack()
{
	FTransform transform;
	transform.SetLocation(GetActorLocation() + GetActorForwardVector() * 300);
	transform.SetRotation(GetActorRotation().Quaternion());
	transform.SetScale3D(FVector(1));
	
	ObjectPoolManager->MiddleBossBulletSpawn(transform);
}

void ACMiddleBoss::DieEndAction()
{
	Destroy();
}
