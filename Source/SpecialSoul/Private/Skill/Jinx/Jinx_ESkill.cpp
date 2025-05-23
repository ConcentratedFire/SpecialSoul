// Fill out your copyright notice in the Description page of Project Settings.


#include "Skill/Jinx/Jinx_ESkill.h"

#include "Player/CBasePlayer.h"
#include "Player/Jinx.h"
#include "Player/Components/SkillComponent.h"
#include "Projectile/Jinx/RocketLauncherBullet.h"

UJinx_ESkill::UJinx_ESkill()
{
	ConstructorHelpers::FClassFinder<ARocketLauncherBullet> BulletClassFinder(TEXT("/Script/Engine.Blueprint'/Game/Player/Jinx/BP_RocketLauncherBullet.BP_RocketLauncherBullet_C'"));
	if (BulletClassFinder.Succeeded())
	{
		BulletClass = BulletClassFinder.Class;
	}
}

// 서버에서 호출됨
void UJinx_ESkill::UseSkill(ACharacter* Caster)
{
	//UE_LOG(LogTemp, Warning, TEXT("Jinx_ESkill"));

	if (!Caster || !BulletClass)
	{
		UE_LOG(LogTemp, Warning, TEXT("Caster or MinigunBullet is nullptr..."));
		return;
	}

	AJinx* Jinx = Cast<AJinx>(Caster);
	if (!bCasted)
	{
		Jinx->MRPC_PlaySkillMontage(ESkillKey::E);
		bCasted = true;
	}
	else
	{
		Jinx->GetWorld()->GetTimerManager().ClearTimer(CastingTimer);
			
		// 스킬 발동
		StartUseSkill(Jinx);
	}
	
}

void UJinx_ESkill::StartUseSkill(AJinx* Jinx)
{
	if (!Jinx) return;
	
	TWeakObjectPtr<UJinx_ESkill> WeakThis(this); // GC에 의해 댕글링 포인터될 때를 위해 WeakPtr 사용
	TWeakObjectPtr<AJinx> WeakJinx(Jinx);
	
	// Caster Transform에 맞춰서 스킬을 발사한다
	Jinx->GetWorld()->GetTimerManager().SetTimer(CastingTimer, FTimerDelegate::CreateLambda(
		[WeakThis, WeakJinx]()
		{
			// 발사 끝
			if (!WeakThis.IsValid() || !WeakJinx.IsValid() ||
				WeakThis->FiredBulletNum >= WeakThis->BulletNum)
			{
				WeakThis->EndUseSkill(WeakJinx.Get());  
				return;
			}
			
			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = WeakJinx.Get();
			SpawnParams.Instigator = WeakJinx.Get();

			FVector SpawnPos = WeakJinx->GetMesh()->GetSocketLocation(FName(TEXT("Minigun_FirePos0")));
			FRotator SpawnRot = WeakJinx->GetActorRotation();
			
			float RandomYaw = FMath::FRandRange(-WeakThis->MaxAngle, WeakThis->MaxAngle);
			SpawnRot.Yaw += RandomYaw;
			
			auto Bullet = WeakJinx->GetWorld()->SpawnActor<ARocketLauncherBullet>(WeakThis->BulletClass,
					SpawnPos, SpawnRot, SpawnParams);

			if (Bullet)
			{
				Bullet->ApplyCasterStat(WeakJinx.Get()); // 공격 데이터 세팅
			}

			WeakThis->FiredBulletNum++;
		}),
		FireDelay, true, FireDelay
	);
}

void UJinx_ESkill::EndUseSkill(AJinx* Jinx)
{
	if (!IsValid(Jinx)) return;
	FiredBulletNum = 0;
	Jinx->GetWorld()->GetTimerManager().ClearTimer(CastingTimer);
	//Jinx->GetWorld()->GetTimerManager().ClearTimer(FireTimer);
	bCasted = false;

	Jinx->ResetLeftCooltime(ESkillKey::E);
}