// Fill out your copyright notice in the Description page of Project Settings.


#include "Skill/Jinx/Jinx_RSkill.h"

#include "Projectile/Jinx/MegaRocketBullet.h"

UJinx_RSkill::UJinx_RSkill()
{
	ConstructorHelpers::FClassFinder<AMegaRocketBullet> BulletClassFinder(TEXT("/Script/Engine.Blueprint'/Game/Player/Jinx/BP_MegaRocketBullet.BP_MegaRocketBullet_C'"));
	if (BulletClassFinder.Succeeded())
	{
		BulletClass = BulletClassFinder.Class;
	}
}

void UJinx_RSkill::UseSkill(ACBasePlayer* Caster)
{
	UE_LOG(LogTemp, Warning, TEXT("Jinx_RSkill"));

	if (!Caster || !BulletClass)
	{
		UE_LOG(LogTemp, Warning, TEXT("Caster or MinigunBullet is nullptr..."));
		return;
	}
	
	// 시전시간 동안 Progressbar UI를 띄우고

	// 시전가능해지면 

	// 스킬을 발사한다
	
}
