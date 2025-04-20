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
    if (!Jinx)
        return;

    ShotCount = 0;
    ShotBulletCount = 0;
    UWorld* World = Caster->GetWorld();

    // 기존 타이머를 먼저 clear
    World->GetTimerManager().ClearTimer(ShotTimer);
    World->GetTimerManager().ClearTimer(OneShotTimer);

    // ShotTimer를 설정
    World->GetTimerManager().SetTimer(ShotTimer,
        FTimerDelegate::CreateUObject(this, &UJinx_Attack::HandleShot, Caster, Jinx),
        ShotDelay, true, 0.f);

    UE_LOG(LogTemp, Warning, TEXT("Shot Timer started"));
}

void UJinx_Attack::HandleShot(ACharacter* Caster, AJinx* Jinx)
{
    // ShotCount가 TotalShot 이상이면 종료
    if (ShotCount >= TotalShot)
    {
        UE_LOG(LogTemp, Warning, TEXT("All shots fired! TotalShot reached."));

        UWorld* World = Caster->GetWorld();
        World->GetTimerManager().ClearTimer(ShotTimer);
        World->GetTimerManager().ClearTimer(OneShotTimer);

        // if (Jinx)
        // {
        //     Jinx->MRPC_ActivateSkillMovement(false);
        // }
        return;
    }

    // 발사 방향 계산
    FVector FireDir = Caster->GetActorForwardVector();
    FRotator SpawnRot = FRotationMatrix::MakeFromX(FireDir).Rotator();

    // OneShot 타이머가 이미 설정되어 있다면 다시 설정하지 않음
    if (!Caster->GetWorld()->GetTimerManager().IsTimerActive(OneShotTimer))
    {
        // OneShot 타이머 시작
        Caster->GetWorld()->GetTimerManager().SetTimer(OneShotTimer,
            FTimerDelegate::CreateUObject(this, &UJinx_Attack::HandleOneShot, Caster, SpawnRot),
            OneShotDelay, true, 0.f);

        //UE_LOG(LogTemp, Warning, TEXT("OneShot Timer started"));
    }
}

// 한번에 3개씩 쏘기
void UJinx_Attack::HandleOneShot(ACharacter* Caster, FRotator SpawnRot)
{
    // 총알 개수가 OneShotBullet을 초과하면 다음 Shot으로 넘어감
    if (ShotBulletCount >= OneShotBullet)
    {
        //UE_LOG(LogTemp, Warning, TEXT("ShotBulletCount exceeded. Moving to next shot"));

        // OneShot 타이머를 초기화하고 ShotCount 증가
        Caster->GetWorld()->GetTimerManager().ClearTimer(OneShotTimer);
        ShotCount++;
        ShotBulletCount = 0;

        // 다음 샷을 위해 타이머 재설정
        UWorld* World = Caster->GetWorld();
        World->GetTimerManager().SetTimer(OneShotTimer,
            FTimerDelegate::CreateUObject(this, &UJinx_Attack::HandleOneShot, Caster, SpawnRot),
            OneShotDelay, true, 0.f);

        return;
    }

    // 총알 스폰
    FString SocketName = FString::Printf(TEXT("Minigun_FirePos%d"), ShotBulletCount);
    FVector SpawnPos = Caster->GetMesh()->GetSocketLocation(FName(*SocketName));

    FActorSpawnParameters SpawnParams;
    SpawnParams.Owner = Caster;
    SpawnParams.Instigator = Caster;

    auto Bullet = Caster->GetWorld()->SpawnActor<AMinigunBullet>(BulletClass,
        SpawnPos, SpawnRot, SpawnParams);

    if (Bullet)
    {
        Bullet->ApplyCasterStat(Caster); // 스탯 적용
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("Failed to spawn Bullet"));
    }
    
    ShotBulletCount++;
}
