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
#include "UI/ChampionStatusWidget.h"
#include "UI/GameWidget.h"
#include "UI/HUD/GameHUD.h"

AJinx::AJinx()
{
	// 스켈레탈 메시 세팅
	ConstructorHelpers::FObjectFinder<USkeletalMesh> TempSkMesh(
		TEXT("/Script/Engine.SkeletalMesh'/Game/Asset/Jinx/jinx__tft_set_13_.jinx__tft_set_13_'"));
	if (TempSkMesh.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(TempSkMesh.Object);
	}
	GetMesh()->SetRelativeLocation(FVector(0.f, 0.f, -68.f));
	GetMesh()->SetRelativeRotation(FRotator(0.f,  -90.f, 0.f));
	GetMesh()->SetCollisionProfileName(TEXT("Player"));

	GetCapsuleComponent()->SetCapsuleHalfHeight(68.f);
	GetCapsuleComponent()->SetCapsuleRadius(28.f);

	GetCharacterMovement()->bOrientRotationToMovement = true;
	MoveComp->SetActive(false);

}

void AJinx::BeginPlay()
{
	Super::BeginPlay();
	AnimInstance = GetMesh()->GetAnimInstance();

	SkillComponent->BindSkill(ESkillKey::Attack, NewObject<UJinx_Attack>());
	SkillComponent->BindSkill(ESkillKey::Passive, NewObject<UJinx_Passive>());
	SkillComponent->BindSkill(ESkillKey::E, NewObject<UJinx_ESkill>());
	SkillComponent->BindSkill(ESkillKey::R, NewObject<UJinx_RSkill>());

	SkillComponent->CoolTimeMap.Add(ESkillKey::E, FSkillCooltime(1.f, 0.f));
	SkillComponent->CoolTimeMap.Add(ESkillKey::R, FSkillCooltime(2.f, 0.f));
}

void AJinx::SetLocalInit(ACPlayerController* InPC)
{
	Super::SetLocalInit(InPC);
	if (AGameHUD* hud = Cast<AGameHUD>(InPC->GetHUD()))
	{
		UObject* e = StaticLoadObject(UObject::StaticClass(), nullptr, TEXT("/Game/UI/textures/Fishbones.Fishbones"));
		UObject* r = StaticLoadObject(UObject::StaticClass(), nullptr, TEXT("/Game/UI/textures/MegaRocket.MegaRocket"));

		hud->SetSkillSlotVisuals(ESkillKey::E, e);
		hud->SetSkillSlotVisuals(ESkillKey::R, r);

		UObject* portrait = StaticLoadObject(UObject::StaticClass(), nullptr, TEXT("/Game/UI/textures/jinx_portrait.jinx_portrait"));
		hud->SetPortrait(portrait);

		UObject* passiveImg = StaticLoadObject(UObject::StaticClass(), nullptr, TEXT("/Game/UI/textures/Get_Excited.Get_Excited"));
		hud->SetPassiveImage(passiveImg);

		hud->SetDefaultWeaponUI(this);
	}
}

float AJinx::GetDamage(bool& OutbIsCri) const
{
	float CalcDamage = PS->CalcDamage(JinxAttackData.Damage, OutbIsCri);
	return CalcDamage;
}

void AJinx::DieProcess()
{
	Super::DieProcess();
	Cast<UJinxAnim>(AnimInstance)->PlayDieAnimation();
}

void AJinx::Attack()
{
	SkillComponent->Attack(); // 기본공격
}

void AJinx::UseESkill()
{
	// if (!SkillComponent->bUsingSkill)
	// {
	// 	SkillComponent->bUsingSkill = true;
		SkillComponent->SRPC_CastSkill_Implementation(ESkillKey::E);
	// }
}

void AJinx::UseRSkill()
{
	// if (!SkillComponent->bUsingSkill)
	// {
	// 	SkillComponent->bUsingSkill = true;
		SkillComponent->SRPC_CastSkill_Implementation(ESkillKey::R);
	// }
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
	// AttackData = PS->JinxAttackDataMap.FindRef(PlayerLevel);
	PC->UpgradeWeapon(PlayerLevel);

	// 업데이트된 데이터로 공격 시작
	SRPC_UseSkill_Implementation(ESkillKey::Attack);
}

void AJinx::SRPC_UseSkill_Implementation(ESkillKey Key)
{
	switch (Key)
	{
	case ESkillKey::Attack:
		{
			// 서버에서 타이머를 돌리며
			float remainingTime = GetWorld()->GetTimerManager().GetTimerRemaining(AttackTimer);

			GetWorld()->GetTimerManager().ClearTimer(AttackTimer);

			GetWorld()->GetTimerManager().SetTimer(AttackTimer, FTimerDelegate::CreateLambda([this, Key]()
			{
				if (!bUsingSkill
					&& !AnimInstance->Montage_IsPlaying(ESkillMontage)
					&& !AnimInstance->Montage_IsPlaying(RSkillMontage))
				{
					MRPC_PlaySkillMontage(Key); // 모든 클라에서 애니메이션을 재생한다
				}
				
				// 0.3초 후 Attack()
				FTimerHandle AttackDelayTimer;
				GetWorld()->GetTimerManager().SetTimer(AttackDelayTimer, FTimerDelegate::CreateLambda([this]()
				{
					Attack();
				}), 0.3f, false);
			}), JinxAttackData.Cooltime, true, JinxAttackData.Cooltime - remainingTime);
		}
		break;

	case ESkillKey::Passive:
		break;

	case ESkillKey::E:
		{
			bUsingSkill = true;
			MRPC_PlaySkillMontage_Implementation(ESkillKey::E);
		}
		break;

	case ESkillKey::R:
		{
			bUsingSkill = true;
			MRPC_PlaySkillMontage_Implementation(ESkillKey::R);
		}
		break;

	default:
		break;
	}
}

void AJinx::MRPC_PlaySkillMontage_Implementation(ESkillKey Key)
{
	//UE_LOG(LogTemp, Warning, TEXT("MRPC_PlaySkillMontage"));

	switch (Key)
	{
	case ESkillKey::Attack:
		{
			//UE_LOG(LogTemp, Warning, TEXT("MRPC_PlaySkillMontage - Attack"));
			PlayAnimMontage(AttackMontage);
		}
		break;
	case ESkillKey::Passive:
		break;
	case ESkillKey::E:
		//UE_LOG(LogTemp, Warning, TEXT("MRPC_PlaySkillMontage - E"));
		PlayAnimMontage(ESkillMontage);
		break;
	case ESkillKey::R:
		//UE_LOG(LogTemp, Warning, TEXT("MRPC_PlaySkillMontage - R"));
		PlayAnimMontage(RSkillMontage);
		break;

	default:
		break;
	}
}

void AJinx::ActivateSkillMovement(bool bActive, bool bAttack)
{
	if (bActive)
	{
		GetCharacterMovement()->bOrientRotationToMovement = false;
		MoveComp->SetActive(true);
		RotateToMouseCursor();
		if (!bAttack)
			MoveComp->bCanMove = false;
		return;
	}

	GetCharacterMovement()->bOrientRotationToMovement = true;
	MoveComp->SetActive(false);
	if (!bAttack)
		MoveComp->bCanMove = true;
}

void AJinx::StartAttack()
{
	if (HasAuthority())
	{
		SRPC_UseSkill(ESkillKey::Attack);
	}
}


void AJinx::RotateToMouseCursor()
{
	if (!IsLocallyControlled()) return;

	FHitResult HitResult;
	bool bHit = PC->GetHitResultUnderCursor(ECC_Visibility, true, HitResult);
	if (bHit)
	{
		FVector directionToMouseCursor = HitResult.Location - GetActorLocation();
		directionToMouseCursor.Z = 0;
		directionToMouseCursor.Normalize();

		// 목표 회전값
		FRotator targetRot = FRotationMatrix::MakeFromX(directionToMouseCursor).Rotator();

		// 회전 적용
		SetActorRotation(targetRot);
	}
}
