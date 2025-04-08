// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/CMeleeEnemy.h"

#include "Components/CapsuleComponent.h"

class ACBasePlayer;

ACMeleeEnemy::ACMeleeEnemy()
{
	GetCapsuleComponent()->SetCapsuleHalfHeight(70.f);

	GetMesh()->SetRelativeLocation(FVector(0, 0, -60));
	GetMesh()->SetRelativeRotation(FRotator(0, 90, 0));
	GetMesh()->SetRelativeScale3D(FVector(0.008));

	ConstructorHelpers::FObjectFinder<UAnimMontage> tmpAttackMontage(TEXT("/Script/Engine.AnimMontage'/Game/Asset/MeleeMinion/Anim/AM_MeleeAttack.AM_MeleeAttack'"));
	if (tmpAttackMontage.Succeeded())
		AttackMontage = tmpAttackMontage.Object;

	ConstructorHelpers::FObjectFinder<UAnimMontage> tmpDieMontage(TEXT("/Script/Engine.AnimMontage'/Game/Asset/MeleeMinion/Anim/AM_MeleeDeath.AM_MeleeDeath'"));
	if (tmpDieMontage.Succeeded())
		DieMontage = tmpDieMontage.Object;
}

void ACMeleeEnemy::BeginPlay()
{
	Super::BeginPlay();
}
