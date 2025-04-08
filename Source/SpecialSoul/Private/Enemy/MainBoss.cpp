// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/MainBoss.h"

AMainBoss::AMainBoss()
{
	PrimaryActorTick.bCanEverTick = true;
	ConstructorHelpers::FObjectFinder<USkeletalMesh> TempSkMesh (TEXT("/Script/Engine.SkeletalMesh'/Game/Asset/Aatrox/prestige_blood_moon_aatrox__2022_.prestige_blood_moon_aatrox__2022_'"));
	if (TempSkMesh.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(TempSkMesh.Object);
	}
	GetMesh()->SetRelativeLocation(FVector(0.f, 0.f, -100.f));
	GetMesh()->SetRelativeRotation(FRotator(0.f, -90.f, 0.f));
	GetMesh()->SetRelativeScale3D(FVector(0.015f));


}

void AMainBoss::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AMainBoss::BeginPlay()
{
	Super::BeginPlay();
}
