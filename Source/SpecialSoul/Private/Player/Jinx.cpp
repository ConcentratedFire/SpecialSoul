// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/Jinx.h"

#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Player/CPlayerController.h"
#include "Player/Anim/JinxAnim.h"
#include "Player/Components/CMovementComponent.h"
#include "Player/Components/SkillComponent.h"
#include "Skill/Jinx/Jinx_Attack.h"
#include "Skill/Jinx/Jinx_ESkill.h"
#include "Skill/Jinx/Jinx_Passive.h"
#include "Skill/Jinx/Jinx_RSkill.h"
#include "Utility/CDataSheetUtility.h"

AJinx::AJinx()
{
	// 스켈레탈 메시 세팅
	ConstructorHelpers::FObjectFinder<USkeletalMesh> TempSkMesh(TEXT("/Script/Engine.SkeletalMesh'/Game/Asset/Jinx/jinx__tft_set_13_.jinx__tft_set_13_'"));
	if (TempSkMesh.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(TempSkMesh.Object);
	}
	GetMesh()->SetRelativeLocation(FVector(0.f, 0.f, -68.f));
	GetMesh()->SetRelativeRotation(FRotator(0.f, -90.f, 0.f));
	GetMesh()->SetCollisionProfileName(TEXT("Player"));

	GetCapsuleComponent()->SetCapsuleHalfHeight(68.f);
	GetCapsuleComponent()->SetCapsuleRadius(28.f);
	
	GetCharacterMovement()->bOrientRotationToMovement = true;
	MoveComp->SetActive(false);
}

void AJinx::BeginPlay()
{
	Super::BeginPlay();
	Anim = Cast<UJinxAnim>(GetMesh()->GetAnimInstance());

	SkillComponent->BindSkill(ESkillKey::Attack, NewObject<UJinx_Attack>());
	SkillComponent->BindSkill(ESkillKey::Passive, NewObject<UJinx_Passive>());
	SkillComponent->BindSkill(ESkillKey::E, NewObject<UJinx_ESkill>());
	SkillComponent->BindSkill(ESkillKey::R, NewObject<UJinx_RSkill>());
}

void AJinx::Attack()
{
	SkillComponent->Attack(); // 기본공격
}

void AJinx::UpdatePlayerData(const int32 PlayerLevel)
{
	if (PS->JinxAttackDataMap.Contains(PlayerLevel))
	{
		UpdateJinxAttackStat(PlayerLevel);
	}

	//Super::UpdatePlayerData(PlayerLevel);
}

void AJinx::UpdateJinxAttackStat(int32 PlayerLevel)
{
	// AttackData = PS->JinxAttackDataMap.FindRef(PlayerLevel);
	PC->UpgradeWeapon(PlayerLevel);
	
	 // 업데이트된 데이터로 공격 시작
	SRPC_StartAttack_Implementation();
}

void AJinx::SRPC_StartAttack_Implementation()
{
	float remainingTime = GetWorld()->GetTimerManager().GetTimerRemaining(AttackTimer);
	GetWorld()->GetTimerManager().ClearTimer(AttackTimer);
	
	GetWorld()->GetTimerManager().SetTimer(AttackTimer, FTimerDelegate::CreateLambda([this]()
	{
		MRPC_PlayAttackMontage();
		
	}), JinxAttackData.Cooltime, true, JinxAttackData.Cooltime); // -remainingTime
}


void AJinx::MRPC_PlayAttackMontage_Implementation()
{
	PlayAnimMontage(AttackMontage);
}


void AJinx::ActivateSkillMovement(bool bActive)
{
	if (bActive)
	{
		SkillComponent->UseSkillCount++;
		GetCharacterMovement()->bOrientRotationToMovement = false; 
		MoveComp->SetActive(true);
		RotateToMouseCursor();
		return;
	}

	SkillComponent->UseSkillCount--;
	if (SkillComponent->UseSkillCount == 0)
	{
		GetCharacterMovement()->bOrientRotationToMovement = true; 
		MoveComp->SetActive(false);
	}
}

void AJinx::StartAttack()
{
	if (HasAuthority())
	{
		SRPC_StartAttack();
	}
}

void AJinx::RotateToMouseCursor()
{
	FHitResult HitResult;
	bool bHit = PC->GetHitResultUnderCursor(ECC_Visibility, true, HitResult);
	if (bHit)
	{
		FVector directionToMouseCursor = HitResult.Location -GetActorLocation();
		directionToMouseCursor.Z = 0;
		directionToMouseCursor.Normalize();

		// 목표 회전값
		FRotator targetRot = FRotationMatrix::MakeFromX(directionToMouseCursor).Rotator();

		// 회전 적용
		SetActorRotation(targetRot);
	}
}

