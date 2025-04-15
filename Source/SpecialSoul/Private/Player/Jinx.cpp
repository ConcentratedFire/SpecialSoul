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
	
	// GetCharacterMovement()->bOrientRotationToMovement = false;
	
	SkillComponent = CreateDefaultSubobject<USkillComponent>(TEXT("SkillComponent"));
	//GetCharacterMovement()->bOrientRotationToMovement = true;

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

	if (HasAuthority() && DataSheetUtility)
	{
		DataSheetUtility->OnDataFetched.AddDynamic(this, &AJinx::InitAllData);
	}

	PC = Cast<ACPlayerController>(GetController());

}


void AJinx::Attack()
{
	SkillComponent->Attack(); // 기본공격
}

void AJinx::InitAllData()
{
	UpdatePlayerData(1);
}

void AJinx::UpdatePlayerData(const int32 PlayerLevel)
{
	if (PS->JinxAttackDataMap.Contains(PlayerLevel))
	{
		UpdateJinxAttackStat(PlayerLevel);
	}

	Super::UpdatePlayerData(PlayerLevel);
}

void AJinx::UpdateJinxAttackStat(int32 PlayerLevel)
{
	AttackData = PS->JinxAttackDataMap.FindRef(PlayerLevel);

	 // 업데이트된 데이터로 공격 시작
	GetWorld()->GetTimerManager().ClearTimer(AttackTimer);
	StartAttack();
}

void AJinx::PrintAttackDataMap() // CBasePlayer.cpp에서 바인딩됨
{
	for (const auto& Pair : PS->JinxAttackDataMap)
	{
		UE_LOG(LogTemp, Log, TEXT("Jinx's AttackDataMap || ID: %d) ProjectileCount: %d, ProjectileRange: %f, Damage: %f, Cooltime: %f, UseAP: %s, APDamage: %f"),
			Pair.Key, Pair.Value.ProjectileCount, Pair.Value.ProjectileRange, Pair.Value.Damage, Pair.Value.Cooltime, *Pair.Value.UseAP, Pair.Value.APDamage);
	}
}

void AJinx::ActivateSkillMovement(bool bActive)
{
	if (bActive)
	{
		GetCharacterMovement()->bOrientRotationToMovement = false; 
		MoveComp->SetActive(true);
		RotateToMouseCursor();
		return;
	}

	if (SkillComponent->UseSkillCount == 0)
	{
		GetCharacterMovement()->bOrientRotationToMovement = true; 
		MoveComp->SetActive(false);
	}
}

void AJinx::StartAttack()
{
	GetWorld()->GetTimerManager().SetTimer(AttackTimer, FTimerDelegate::CreateLambda([this]()
	{
		PlayAnimMontage(AttackMontage); // AttackMontage의 AnimNotify에서 애니메이션의 특정 프레임에 Attack 호출
	}), AttackData.Cooltime, true, AttackData.Cooltime);
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

