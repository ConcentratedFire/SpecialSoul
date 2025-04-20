#include "Enemy/MainBoss/MainBoss.h"

#include "Components/CapsuleComponent.h"
#include "Enemy/MainBoss/MainBossController.h"
#include "Player/CBasePlayer.h"
#include "Player/Components/SkillComponent.h"
#include "Skill/MainBoss/MainBoss_Attack.h"
#include "Skill/MainBoss/MainBoss_DarkinBlade.h"

AMainBoss::AMainBoss()
{
	//PrimaryActorTick.bCanEverTick = true;
	ConstructorHelpers::FObjectFinder<USkeletalMesh> TempSkMesh (TEXT("/Script/Engine.SkeletalMesh'/Game/Asset/Aatrox/prestige_blood_moon_aatrox__2022_.prestige_blood_moon_aatrox__2022_'"));
	if (TempSkMesh.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(TempSkMesh.Object);
	}
	GetMesh()->SetRelativeLocation(FVector(0.f, 0.f, -100.f));
	GetMesh()->SetRelativeRotation(FRotator(0.f, -90.f, 0.f));
	GetMesh()->SetRelativeScale3D(FVector(0.015f));

	ConstructorHelpers::FObjectFinder<UAnimMontage> tmpMontage(
	TEXT("/Script/Engine.AnimMontage'/Game/Enemy/MainBoss/Anim/AM_MainBoss_DarkinBlade.AM_MainBoss_DarkinBlade'"));
	if (tmpMontage.Succeeded())
		DarkinBlade_Montage = tmpMontage.Object;

	SkillComponent = CreateDefaultSubobject<USkillComponent>(TEXT("SkillComponent"));
	MoveDistance = 400.f;
}

void AMainBoss::BeginPlay()
{
	Super::BeginPlay();
	
	SkillComponent->BindSkill(ESkillKey::Attack, NewObject<UMainBoss_Attack>());
	SkillComponent->BindSkill(ESkillKey::Q, NewObject<UMainBoss_DarkinBlade>());

	SkillComponent->CoolTimeMap.Add(ESkillKey::Attack, 1.5f);
	SkillComponent->CoolTimeMap.Add(ESkillKey::Q, 8.f);
	
	SkillComponent->LeftCoolTimeMap.Add(ESkillKey::Attack, 1.5f);
	SkillComponent->LeftCoolTimeMap.Add(ESkillKey::Q, 8.f);
	
	SkillComponent->SkillRangeMap.Add(ESkillKey::Attack, 500.f);
	SkillComponent->SkillRangeMap.Add(ESkillKey::Q, 500.f);

	// 임시 코드
	MaxHP = 5000;
	HP = MaxHP;
}

void AMainBoss::PlayDarkinBladeMontage(float InPlayRate, FName SectionName)
{
	// Attack0 .. Attack2
	PlayAnimMontage(DarkinBlade_Montage, InPlayRate, SectionName);
}

void AMainBoss::MyDamage(int32 DamageAmount)
{
	HP -= DamageAmount;
	// LOG_S(Warning, TEXT("Name:%s, HP : %d"), *GetName(), HP);
	
	if (HP <= 0)
	{
		if (!bIsUlt)
		{
			HP = 1;
			ChangePhase();

		}
		else
		{
			// 사망 + 게임종료
			bIsDead = true;
			AMainBossController* mbController = Cast<AMainBossController>(GetOwner());
			if (mbController)
				mbController->StopMovement();
			
			GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_GameTraceChannel1, ECR_Ignore);

			HandleDie(); // 사망 애니메이션 
		}
	}
}

void AMainBoss::ChangePhase()
{
	// 부활 + 궁
	bIsUlt = true;
	// HP = 1;
}

void AMainBoss::DieEndAction()
{
	// TODO : 미션 클리어 && 게임 종료
}

void AMainBoss::ResetLeftCooltime_DarkinBlade()
{
	SkillComponent->ResetLeftCooltime(ESkillKey::Q);
}
