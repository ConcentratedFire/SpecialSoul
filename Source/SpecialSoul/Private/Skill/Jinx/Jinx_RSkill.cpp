// Fill out your copyright notice in the Description page of Project Settings.


#include "Skill/Jinx/Jinx_RSkill.h"

#include "Player/Jinx.h"
#include "Player/Components/SkillComponent.h"
#include "Projectile/Jinx/MegaRocketBullet.h"

UJinx_RSkill::UJinx_RSkill()
{
	ConstructorHelpers::FClassFinder<AMegaRocketBullet> BulletClassFinder(TEXT("/Script/Engine.Blueprint'/Game/Player/Jinx/BP_MegaRocketBullet.BP_MegaRocketBullet_C'"));
	if (BulletClassFinder.Succeeded())
	{
		BulletClass = BulletClassFinder.Class;
	}
}

// 서버에서 호출된다.
void UJinx_RSkill::UseSkill(ACharacter* Caster)
{
	AJinx* Jinx = Cast<AJinx>(Caster);
	if (!Caster || !BulletClass || !Jinx)
	{
		UE_LOG(LogTemp, Warning, TEXT("Caster or MinigunBullet or Jinx is nullptr..."));
		return;
	}

	if (!bCasted)
	{
		Jinx->MRPC_PlaySkillMontage(ESkillKey::R);
		bCasted = true;
	}
	else
	{
		StartUseSkill(Jinx);
	}
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

	EndUseSkill(Jinx);
}

void UJinx_RSkill::EndUseSkill(AJinx* Jinx)
{
	bCasted = false;
	Jinx->ResetLeftCooltime(ESkillKey::R);
}
