// Fill out your copyright notice in the Description page of Project Settings.


#include "Skill/Jinx/Jinx_RSkill.h"

#include "Player/Jinx.h"
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

	AJinx* Jinx = Cast<AJinx>(Caster);
	if (Jinx)
	{
		Jinx->ActivateSkillMovement(true);
		Jinx->GetWorld()->GetTimerManager().ClearTimer(CastingTimer);
	}
	
	// TODO : 시전시간 동안 Progressbar UI를 띄우고
	
	Jinx->GetWorld()->GetTimerManager().SetTimer(CastingTimer, FTimerDelegate::CreateLambda(
		[this,Jinx]()
		{
			this->StartUseSkill(Jinx);
		}),
		CastingTime, false, CastingTime);
}

void UJinx_RSkill::StartUseSkill(AJinx* Jinx)
{
	if (!Jinx) return;
	
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = Jinx;
	SpawnParams.Instigator = Jinx;

	FVector SpawnPos = Jinx->GetMesh()->GetSocketLocation(FName(TEXT("Minigun_FirePos0")));
	FRotator SpawnRot = Jinx->GetActorRotation();
			
	auto Bullet = Jinx->GetWorld()->SpawnActor<AMegaRocketBullet>(BulletClass,
			SpawnPos, SpawnRot, SpawnParams);

	if (Bullet)
	{
		Bullet->ApplyCasterStat(Jinx); // 공격 데이터 세팅
	}
}

void UJinx_RSkill::EndUseSkill(AJinx* Jinx)
{
	Jinx->GetWorld()->GetTimerManager().ClearTimer(CastingTimer);
	Jinx->ActivateSkillMovement(false);
}
