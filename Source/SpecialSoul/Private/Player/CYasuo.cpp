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

	GetWorldTimerManager().SetTimer(ChargePassiveEnergyTimer, this, &ACYasuo::ChargePassiveEnergy, 1.f, true);

	// if (ObjectPoolManager)
	// {
	// 	ObjectPoolManager->MakeTornadoPool(this);
	// }
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

	// 데이터가 들어왔는지 체크
	if (PS && PS->YasuoMoveDataMap.Num() > 0)
	{
		// 데이터 업데이트 체크
		//CheckMoveData();
	
		// 이동 거리가 충분하면 기류를 충전
		float calcDistance = CalcHaste(YasuoMoveInfo.StackDistance);
		if (MoveDistance >= calcDistance)
		{
			ChargePassiveEnergy();
			MoveDistance -= calcDistance;
		}
	}
	
	// if (!SkillComponent->bUseESkill && !SkillComponent->bUseRSkill)
	// {
	// 	// 기류가 100이 되면 회오리 발사
	// 	if (!bAttacking && PassiveEnergy >= 100)
	// 	{
	// 		//Anim->PlayAttackMontage();
	// 		bAttacking = true;
	// 	}
	// }
}

void ACYasuo::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ACYasuo, AttackFrontVector);
}

float ACYasuo::GetDamage(bool& OutbIsCri) const
{
	float CalcDamage = PS->CalcDamage(YasuoStat.Damage, OutbIsCri);
	return CalcDamage;
}

void ACYasuo::Attack()
{
	if (!HasAuthority()) return;

	TArray<FVector> AttackVectors = GetAttackVector();
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

	// 회오리가 나가고 기류를 깍아주기 위해 옮김
	PassiveEnergy -= 100;
	bAttacking = false;
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

TArray<FVector> ACYasuo::GetAttackVector()
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

	return AttackVectors;
}

void ACYasuo::ChargePassiveEnergy()
{
	// 4의 기류를 획득
	int32 NewEnergy = FMath::Clamp(PassiveEnergy + PassiveEnergyRegen, 0.f, 100.f);
	PassiveEnergy = NewEnergy;
}

void ACYasuo::UpdateCheckMoveData()
{
	PC->GetNextLevelYasuoMoveStat();
}

void ACYasuo::RotateArrow()
{
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
