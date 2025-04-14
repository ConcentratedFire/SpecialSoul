// Fill out your copyright notice in the Description page of Project Settings.


#include "Skill/Jinx/Jinx_RSkill.h"

#include "Projectile/Jinx/MegaRocketBullet.h"

UJinx_RSkill::UJinx_RSkill()
{
	ConstructorHelpers::FClassFinder<AMegaRocketBullet> BulletClassFinder(TEXT("/Game/Player/Jinx/BP_MinigunBullet.BP_MinigunBullet_C"));
	if (BulletClassFinder.Succeeded())
	{
		BulletClass = BulletClassFinder.Class;
	}
}

void UJinx_RSkill::UseSkill(ACBasePlayer* Caster)
{
	// 시전시간 동안 Progressbar UI를 띄우고

	// 시전가능해지면 

	// 스킬을 발사한다
	
}
