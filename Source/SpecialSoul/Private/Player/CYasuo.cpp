// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/CYasuo.h"

#include "SpecialSoul.h"
#include "Components/CapsuleComponent.h"
#include "Components/WidgetComponent.h"
#include "Game/CGameState.h"
#include "Game/CPlayerState.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Net/UnrealNetwork.h"
#include "ObjectPool/CObjectPoolManager.h"
#include "Player/CPlayerController.h"
#include "Player/Anim/CYasuoAnim.h"
#include "Player/AttackActors/CTornado.h"
#include "Player/Components/CMovementComponent.h"
#include "Player/Components/SkillComponent.h"
#include "Skill/Yasuo/CYasuo_ESkill.h"
#include "Skill/Yasuo/CYasuo_RSkill.h"
#include "Utility/CDataSheetUtility.h"

ACYasuo::ACYasuo()
{
}

void ACYasuo::BeginPlay()
{
	Super::BeginPlay();

	// SkillComponent->BindSkill(ESkillKey::E, NewObject<UCYasuo_ESkill>());
	// SkillComponent->BindSkill(ESkillKey::R, NewObject<UCYasuo_RSkill>());

	GetWorldTimerManager().SetTimer(ChargePassiveEnergyTimer, this, &ACYasuo::SRPC_ChargePassiveEnergy_Timer, 1.f, true);

	if (HasAuthority() && IsLocallyControlled() && ObjectPoolManager)
	{
		ObjectPoolManager->MakeTornadoPool(this);
	}
}

void ACYasuo::PrintNetLog()
{
	FString logStr = TEXT("PassiveEnergy : 0"); // 기본값 설정

	int32 dataCount = 0;
	dataCount = PassiveEnergy;

	logStr = FString::Printf(TEXT("PassiveEnergy : %d"), dataCount);

	DrawDebugString(GetWorld(), GetActorLocation() + FVector::UpVector * 200.0f, logStr, nullptr, FColor::Red, 0, true,
	                1);
}

void ACYasuo::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!Anim && GetMesh() && GetMesh()->GetAnimInstance())
	{
		Anim = Cast<UCYasuoAnim>(GetMesh()->GetAnimInstance());
	}

	if (IsLocallyControlled())
		CRPC_SetAttackFrontVector();

	PrintNetLog();

	// 기본 공격
	if (YasuoMoveInfo.ID > 0)
	{
		// 이동 거리가 충분하면 기류를 충전
		SRPC_ChargePassiveEnergy();

		// if (!SkillComponent->bUseESkill && !SkillComponent->bUseRSkill)
		// {
		// 기류가 100이 되면 회오리 발사
		if (!bAttacking && PassiveEnergy >= 100)
		{
			SRPC_PlayAttackAnim();
		}
		// }
	}
}

void ACYasuo::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ACYasuo, AttackFrontVector);
	DOREPLIFETIME(ACYasuo, MoveDistance);
}

float ACYasuo::GetDamage(bool& OutbIsCri) const
{
	float CalcDamage = PS->CalcDamage(YasuoStat.Damage, OutbIsCri);
	return CalcDamage;
}

void ACYasuo::Attack()
{
	if (!HasAuthority()) return;
	CRPC_GetAttackVectors();
}

void ACYasuo::WindWall()
{
	FTransform Transform;
	FVector curLocation = GetActorLocation();
	curLocation.Z -= GetCapsuleComponent()->GetScaledCapsuleHalfHeight();
	Transform.SetLocation(curLocation);
	Transform.SetRotation(GetActorRotation().Quaternion());
	Transform.SetScale3D(FVector(1.f));
	ObjectPoolManager->WindWallSpawn(Transform);
}

void ACYasuo::CRPC_SetAttackFrontVector_Implementation()
{
	FVector forwardVec = GetActorForwardVector();
	FVector Velocity = GetVelocity().GetSafeNormal();

	FVector Dir = forwardVec;
	if (Velocity != FVector::ZeroVector)
	{
		Dir = (forwardVec + Velocity) * 0.5f;
	}
	Dir.Normalize();

	SRPC_ReplicateAttackVector(Dir);
}

void ACYasuo::SRPC_ReplicateAttackVector_Implementation(FVector AttackDir)
{
	AttackFrontVector = AttackDir;

	if (HasAuthority())
		OnRep_RotateArrow();
}

void ACYasuo::OnRep_RotateArrow()
{
	// LOG_S(Warning, TEXT("AttackFrontVector : %.2f, %.2f, %.2f"), AttackFrontVector.X, AttackFrontVector.Y,
	// AttackFrontVector.Z);
	float rotateValue = UKismetMathLibrary::MakeRotFromX(AttackFrontVector).Yaw * -1;
	FRotator newRot = ArrowRotation;
	newRot.Roll += rotateValue;
	ArrowWidgetComp->SetWorldRotation(newRot);
}

void ACYasuo::CRPC_GetAttackVectors_Implementation()
{
	int AttackCnt = YasuoStat.ProjectileCount;
	AttackCnt = PS->CalcProjectile(AttackCnt);
	float AngleStep = 360.f / static_cast<float>(AttackCnt);
	TArray<FVector> AttackVectors;

	for (int32 i = 0; i < AttackCnt; ++i)
	{
		float AngleOffset = -AngleStep * (AttackCnt - 1) / 2 + AngleStep * i;
		FRotator RotationOffset(0.0f, AngleOffset, 0.0f);
		FVector RotatedVector = RotationOffset.RotateVector(AttackFrontVector);
		AttackVectors.Add(RotatedVector);
	}

	SRPC_TryDefaultAttack(AttackVectors);
}

void ACYasuo::SRPC_TryDefaultAttack_Implementation(const TArray<FVector>& AttackVectors)
{
	for (const FVector& Vector : AttackVectors)
	{
		FTransform Transform;
		FVector curLocation = GetActorLocation();
		curLocation.Z -= GetCapsuleComponent()->GetScaledCapsuleHalfHeight();
		Transform.SetLocation(curLocation);
		Transform.SetRotation(Vector.Rotation().Quaternion());
		Transform.SetScale3D(FVector(1.f));
		ObjectPoolManager->TornadoSpawn(Transform);
	}

	MRPC_EndDefaultAttack();
	bAttacking = false;
}

void ACYasuo::MRPC_EndDefaultAttack_Implementation()
{
	// 회오리가 나가고 기류를 깍아주기 위해 옮김
	PassiveEnergy -= 100;
}

void ACYasuo::SRPC_ChargePassiveEnergy_Implementation()
{
	if (!PC) return;

	float calcDistance = PC->CalcHaste(YasuoMoveInfo.StackDistance);
	if (MoveDistance >= calcDistance)
	{
		// 4의 기류를 획득
		SRPC_ChargePassiveEnergy_Timer();
		MoveDistance -= calcDistance;
	}
}

void ACYasuo::SRPC_ChargePassiveEnergy_Timer_Implementation()
{
	int32 NewEnergy = FMath::Clamp(PassiveEnergy + PassiveEnergyRegen, 0.f, 100.f);
	MRPC_ChargePassiveEnergy(NewEnergy);
}

void ACYasuo::MRPC_ChargePassiveEnergy_Implementation(const int32 NewEnergy)
{
	PassiveEnergy = NewEnergy;
}

void ACYasuo::ESkill(const bool bAnimStart)
{
	// LOG_S(Warning, TEXT("ESkill"));
	Anim->PlayESkillMontage(bAnimStart);
	GetCharacterMovement()->GravityScale = bAnimStart ? 0.f : 1.f;
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_GameTraceChannel5, bAnimStart ? ECR_Ignore : ECR_Block);

	if (bAnimStart) return;

	FTransform Transform;
	FVector curLocation = GetActorLocation();
	curLocation.Z -= GetCapsuleComponent()->GetScaledCapsuleHalfHeight();
	Transform.SetLocation(curLocation);
	Transform.SetRotation(FQuat::Identity);
	Transform.SetScale3D(FVector(1, 1, 1));
	ObjectPoolManager->TornadoESpawn(Transform);
}

void ACYasuo::RSkill()
{
	Anim->PlayRSkillMontage();
}

void ACYasuo::ActivateSkillMovement(bool bActivate)
{
	MoveComp->bCanMove = !bActivate;
}

void ACYasuo::UpdatePlayerData(const int32 PlayerLevel)
{
	// 레벨업 후 정보 갱신 처리
	// MoveData는 키값 체크 후 넘기기
	if (PS->YasuoMoveDataMap.Contains(PlayerLevel))
		PC->UpdateYasuoMoveStat(PlayerLevel);
}

void ACYasuo::SRPC_PlayAttackAnim_Implementation()
{
	MRPC_PlayAttackAnim();
	bAttacking = true;
}

void ACYasuo::MRPC_PlayAttackAnim_Implementation()
{
	Anim->PlayAttackMontage();
}
