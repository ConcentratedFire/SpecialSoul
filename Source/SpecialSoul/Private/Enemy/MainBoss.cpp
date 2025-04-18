// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/MainBoss.h"

AMainBoss::AMainBoss()
{
	//PrimaryActorTick.bCanEverTick = true;
	ConstructorHelpers::FObjectFinder<USkeletalMesh> TempSkMesh (TEXT("/Script/Engine.SkeletalMesh'/Game/Asset/Aatrox/prestige_blood_moon_aatrox__2022_.prestige_blood_moon_aatrox__2022_'"));
	if (TempSkMesh.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(TempSkMesh.Object);
	}
	GetMesh()->SetRelativeLocation(FVector(0.f, 0.f, -100.f));
	GetMesh()->SetRelativeRotation(FRotator(0.f, -90.f, 0.f));
	GetMesh()->SetRelativeScale3D(FVector(0.015f));

	ConstructorHelpers::FObjectFinder<UAnimMontage> tmpSkillQMontage(
	TEXT("/Script/Engine.AnimMontage'/Game/Enemy/MainBoss/Anim/AM_MainBossQ.AM_MainBossQ'"));
	if (tmpSkillQMontage.Succeeded())
		SkillQMontage = tmpSkillQMontage.Object;

	MoveDistance = 400.f;

	
}

void AMainBoss::BeginPlay()
{
	Super::BeginPlay();

	// 임시 코드
	MaxHP = 5000;
	HP = MaxHP;
}

void AMainBoss::DieEndAction()
{
	Destroy();
}
