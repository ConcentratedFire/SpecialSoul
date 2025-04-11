// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/CYasuo.h"

#include "Kismet/GameplayStatics.h"
#include "Player/Anim/CYasuoAnim.h"
#include "Player/AttackActors/CTornado.h"
#include "Utility/CDataSheetUtility.h"

ACYasuo::ACYasuo()
{
}

void ACYasuo::BeginPlay()
{
	Super::BeginPlay();
	DataSheetUtility_->OnDataFetched.AddDynamic(this, &ACYasuo::PrintAttackDataMap);
	
	Anim = Cast<UCYasuoAnim>(GetMesh()->GetAnimInstance());

	GetWorldTimerManager().SetTimer(ChargePassiveEnergyTimer, this, &ACYasuo::ChargePassiveEnergy, 1.f, true);
}

void ACYasuo::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (MoveDistance)
	{
		
	}

	if (PassiveEnergy >= 100)
	{
		PassiveEnergy -= 100;
		Anim->PlayAttackMontage();
	}
}

void ACYasuo::Attack()
{
	TArray<FVector> AttackVectors = GetAttackVector();
	// Test
	for (const FVector& Vector : AttackVectors)
	{
		FTransform Transform;
		FVector curLocation = GetActorLocation();
		Transform.SetLocation(FVector(curLocation.X, curLocation.Y, 0));
		Transform.SetRotation(FQuat::Identity);
		Transform.SetScale3D(FVector(1.f));
		ACTornado* Tornado = GetWorld()->SpawnActorDeferred<ACTornado>(TornadoFactory, Transform, GetOwner());
		Tornado->TornadoDirection = Vector;
		UGameplayStatics::FinishSpawningActor(Tornado, Transform);
		DrawDebugLine(GetWorld(), GetActorLocation(), GetActorLocation() + Vector * 100.f, FColor::Red, false, 0.f, 0,
		              10.f);
	}
}

TArray<FVector> ACYasuo::GetAttackVector()
{
	FVector forwardVec = GetActorForwardVector();
	FVector Velocity = GetVelocity().GetSafeNormal();

	FVector Dir = forwardVec;
	if (Velocity != FVector::ZeroVector)
	{
		Dir = (forwardVec + Velocity) * 0.5f;
	}
	Dir.Normalize();

	float AngleStep = 360.f / static_cast<float>(AttackCnt);
	TArray<FVector> AttackVectors;

	for (int32 i = 0; i < AttackCnt; ++i)
	{
		float AngleOffset = -AngleStep * (AttackCnt - 1) / 2 + AngleStep * i;
		FRotator RotationOffset(0.0f, AngleOffset, 0.0f);
		FVector RotatedVector = RotationOffset.RotateVector(Dir);
		AttackVectors.Add(RotatedVector);
	}

	return AttackVectors;
}

void ACYasuo::PrintAttackDataMap()
{
	for (const auto& Pair : YasuoAttackDataMap)
	{
		UE_LOG(LogTemp, Log,
		       TEXT(
			       "Yasuo's AttackDataMap || ID: %d) ProjectileCount: %d, ProjectileRange: %f, Damage: %f, UseAOE: %s, AOELifeTime: %f, AOEDamage: %f, AOEDamageCoolTime: %f"
		       ),
		       Pair.Key, Pair.Value.ProjectileCount, Pair.Value.ProjectileRange, Pair.Value.Damage, *Pair.Value.UseAOE,
		       Pair.Value.AOELifeTime, Pair.Value.AOEDamage, Pair.Value.AOEDamageCoolTime);
	}
	for (const auto& Pair : YasuoMoveDataMap)
	{
		UE_LOG(LogTemp, Log,
		       TEXT(
			       "Yasuo's AttackDataMap || ID: %d) ProjectileCount: %d, ProjectileRange: %d, Damage: %f"
		       ),
		       Pair.Key, Pair.Value.RangeFrom, Pair.Value.RangeTo, Pair.Value.StackDistance);
	}

	// 초기 데이터 세팅
	if (YasuoAttackDataMap.Num() > 0)
		UpdateYasuoAttackStat(1);

	if (YasuoMoveDataMap.Num() > 0)
		UpdateYasuoMoveStat(1);
}

void ACYasuo::UpdateYasuoAttackStat(const int32 Level)
{
	const auto& StatData = YasuoAttackDataMap[Level];
	YasuoStat.ID = StatData.ID;
	YasuoStat.ProjectileCount = StatData.ProjectileCount;
	YasuoStat.ProjectileRange = StatData.ProjectileRange;
	YasuoStat.Damage = StatData.Damage;
	YasuoStat.UseAOE = StatData.UseAOE;
	YasuoStat.AOELifeTime = StatData.AOELifeTime;
	YasuoStat.AOEDamage = StatData.AOEDamage;
	YasuoStat.AOEDamageCoolTime = StatData.AOEDamageCoolTime;
}

void ACYasuo::UpdateYasuoMoveStat(const int32 Level)
{
	const auto& StatData = YasuoMoveDataMap[Level];
	YasuoMoveInfo.ID = StatData.ID;
	YasuoMoveInfo.RangeFrom = StatData.RangeFrom;
	YasuoMoveInfo.RangeTo = StatData.RangeTo;
	YasuoMoveInfo.StackDistance = StatData.StackDistance;
}

void ACYasuo::ChargePassiveEnergy()
{
	// 4의 기류를 획득
	int32 NewEnergy = FMath::Clamp(PassiveEnergy + PassiveEnergyRegen, 0.f, 100.f);
	PassiveEnergy = NewEnergy;
}

void ACYasuo::CheckMoveData()
{
	
}
