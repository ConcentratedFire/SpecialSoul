// Fill out your copyright notice in the Description page of Project Settings.


#include "Skill/Jinx/Jinx_Attack.h"
#include "Player/Jinx.h"
#include "Projectile/Jinx/MinigunBullet.h"

UJinx_Attack::UJinx_Attack()
{
	ConstructorHelpers::FClassFinder<AMinigunBullet> BulletClassFinder(TEXT("/Script/Engine.Blueprint'/Game/Player/Jinx/BP_MinigunBullet.BP_MinigunBullet_C'"));
	if (BulletClassFinder.Succeeded())
	{
		BulletClass = BulletClassFinder.Class;
	}
}


void UJinx_Attack::UseSkill(ACharacter* Caster)
{
	UE_LOG(LogTemp, Warning, TEXT("UJinx_Attack::UseSkill"));

	
	if (!Caster || !BulletClass)
	{
		UE_LOG(LogTemp, Warning, TEXT("Caster or MinigunBullet is nullptr..."));
		return;
	}
	
	AJinx* Jinx = Cast<AJinx>(Caster);
	if (Jinx)
	{
		Jinx->ActivateSkillMovement(true);
	}
	
	ShotCount = 0;
	ShotBulletCount = 0;
	UWorld* World = Caster->GetWorld();
	World->GetTimerManager().ClearTimer(ShotTimer);
	World->GetTimerManager().ClearTimer(OneShotTimer);
	
	World->GetTimerManager().SetTimer(ShotTimer,
		FTimerDelegate::CreateUObject(this, &UJinx_Attack::HandleShot, Caster, Jinx),
		ShotDelay, true, 0.f);
}

void UJinx_Attack::HandleShot(ACharacter* Caster, AJinx* Jinx)
{
	if (ShotCount >= TotalShot)
	{
		UWorld* World = Caster->GetWorld();
		World->GetTimerManager().ClearTimer(ShotTimer);
		World->GetTimerManager().ClearTimer(OneShotTimer);
	
		if (Jinx)
		{
			Jinx->ActivateSkillMovement(false);
		}
		return;
	}

	FVector FireDir = Caster->GetActorForwardVector();
	FRotator SpawnRot = FRotationMatrix::MakeFromX(FireDir).Rotator();

	// OneShot 타이머 시작
	Caster->GetWorld()->GetTimerManager().SetTimer(OneShotTimer,
		FTimerDelegate::CreateUObject(this, &UJinx_Attack::HandleOneShot, Caster, SpawnRot),
		OneShotDelay, true, 0.f);
}

// 한번에 3개씩 쏘기
void UJinx_Attack::HandleOneShot(ACharacter* Caster, FRotator SpawnRot)
{
	if (ShotBulletCount >= OneShotBullet)
	{
		Caster->GetWorld()->GetTimerManager().ClearTimer(OneShotTimer);
		ShotCount++;
		ShotBulletCount = 0;
		return;
	}
	
   FActorSpawnParameters SpawnParams;
   SpawnParams.Owner = Caster;
   SpawnParams.Instigator = Caster;

	FString SocketName = FString::Printf(TEXT("Minigun_FirePos%d"), ShotBulletCount);
	FVector SpawnPos = Caster->GetMesh()->GetSocketLocation(FName(*SocketName));

	auto Bullet = Caster->GetWorld()->SpawnActor<AMinigunBullet>(BulletClass,
		SpawnPos, SpawnRot, SpawnParams);
		
	if (Bullet)
	{
		Bullet->ApplyCasterStat(Caster); // 공격 데이터 세팅
	}
	
	ShotBulletCount++;
}
