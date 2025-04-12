// Fill out your copyright notice in the Description page of Project Settings.


#include "Skill/Jinx/Jinx_Attack.h"

#include "Kismet/GameplayStatics.h"
#include "Player/Jinx.h"
#include "Projectile/Jinx/MinigunBullet.h"

UJinx_Attack::UJinx_Attack()
{
	ConstructorHelpers::FClassFinder<AMinigunBullet> BulletClassFinder(TEXT("/Game/Player/Jinx/BP_MinigunBullet.BP_MinigunBullet_C"));
	if (BulletClassFinder.Succeeded())
	{
		MinigunBullet = BulletClassFinder.Class;
	}
}

void UJinx_Attack::UseSkill(ACBasePlayer* Caster)
{
	if (!Caster || !MinigunBullet)
	{
		UE_LOG(LogTemp, Warning, TEXT("Caster or MinigunBullet is nullptr..."));
		return;
	}
	
	FiredBulletNum = 0;
	
	// 마우스가 가리키는 방향으로 총을 쏜다
	Caster->GetWorld()->GetTimerManager().SetTimer(FireTimer, FTimerDelegate::CreateLambda([this, Caster]()
	{
		FVector FireDir = Caster->GetActorForwardVector();
		FRotator MuzzleRot = FRotationMatrix::MakeFromX(FireDir).Rotator();
		if (FiredBulletNum < BulletNum)
		{

			 // 발사위치, 발사방향
			FVector MuzzleLoc = Caster->GetMesh()->GetSocketLocation("Minigun_FirePos0");

			//UE_LOG(LogTemp, Log, TEXT("MuzzleRot : %s"), *MuzzleRot.ToString());
			 // 총알 스폰
			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = Caster;
			SpawnParams.Instigator = Caster;

			auto Bullet = Caster->GetWorld()->SpawnActor<AMinigunBullet>(MinigunBullet, MuzzleLoc, MuzzleRot, SpawnParams);
			if (Bullet)
			{
				FiredBulletNum++;
			}
		}
		else
		{
			Caster->GetWorld()->GetTimerManager().ClearTimer(FireTimer);
		}
	}), FireDelay, true, 0.f);
}
