// Fill out your copyright notice in the Description page of Project Settings.


#include "Skill/Jinx/Jinx_ESkill.h"

#include "Player/CBasePlayer.h"
#include "Player/Jinx.h"
#include "Projectile/Jinx/RocketLauncherBullet.h"

UJinx_ESkill::UJinx_ESkill()
{
	ConstructorHelpers::FClassFinder<ARocketLauncherBullet> BulletClassFinder(TEXT("/Script/Engine.Blueprint'/Game/Player/Jinx/BP_RocketLauncherBullet.BP_RocketLauncherBullet_C'"));
	if (BulletClassFinder.Succeeded())
	{
		BulletClass = BulletClassFinder.Class;
	}
}

void UJinx_ESkill::UseSkill(ACBasePlayer* Caster)
{
	UE_LOG(LogTemp, Warning, TEXT("Jinx_ESkill"));

	if (!Caster || !BulletClass)
	{
		UE_LOG(LogTemp, Warning, TEXT("Caster or MinigunBullet is nullptr..."));
		return;
	}

	AJinx* Jinx = Cast<AJinx>(Caster);
	if (Jinx)
	{
		Jinx->UseOrientationToMovement(false);
		Jinx->UseMoveCompRotation(true);
	}
	
	// 시전시간 동안 Progressbar UI를 띄우고

	// 시전가능해지면 

	// Caster Transform에 맞춰서 스킬을 발사한다
	FVector SpawnLoc = Caster->GetActorLocation() + Caster->GetActorForwardVector() * 50.f;

	//FRotator SpawnRot =

	if (Jinx)
	{
		Jinx->UseOrientationToMovement(true);
		Jinx->UseMoveCompRotation(false);
	}
}
