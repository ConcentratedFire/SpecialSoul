#include "Enemy/MainBoss/MainBoss.h"

#include "BehaviorTree/BehaviorTreeComponent.h"
#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"
#include "Enemy/MainBoss/MainBossController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Player/CBasePlayer.h"
#include "Player/CPlayerController.h"
#include "Player/Components/SkillComponent.h"
#include "Skill/MainBoss/MainBoss_Attack.h"
#include "Skill/MainBoss/MainBoss_DarkinBlade.h"
#include "UI/HUD/GameHUD.h"

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
	GetCapsuleComponent()->SetCapsuleRadius(67.f);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_GameTraceChannel7, ECR_Ignore); // WindWall

	// 칼 히트박스
	BladeHitbox = CreateDefaultSubobject<UBoxComponent>(TEXT("BladeHitbox"));
	BladeHitbox->SetRelativeLocation(FVector(-2000, 0, 0));
	BladeHitbox->SetBoxExtent(FVector(7000, 4000, 2000));
	BladeHitbox->SetCollisionProfileName("Enemy");
	BladeHitbox->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel1, ECR_Overlap);
	BladeHitbox->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel2, ECR_Ignore); 
	if (HasAuthority()) 
		BladeHitbox->OnComponentBeginOverlap.AddDynamic(this, &AMainBoss::OnBladeHitboxOverlap);
	BladeHitbox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AMainBoss::BeginPlay()
{
	Super::BeginPlay();
	
	BladeHitbox->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, TEXT("Weapon_Blade5"));
	
	SkillComponent->BindSkill(ESkillKey::Attack, NewObject<UMainBoss_Attack>());
	SkillComponent->BindSkill(ESkillKey::Q, NewObject<UMainBoss_DarkinBlade>());

	SkillComponent->CoolTimeMap.Add(ESkillKey::Attack, FSkillCooltime(1.5f, 0.f));
	SkillComponent->CoolTimeMap.Add(ESkillKey::Q, FSkillCooltime(8.f, 0.f));
	
	SkillComponent->SkillRangeMap.Add(ESkillKey::Attack, 500.f);
	SkillComponent->SkillRangeMap.Add(ESkillKey::Q, 500.f);

	GetCharacterMovement()->GravityScale = 1;
	HP = MaxHP;

	StartFindingTarget();
}

void AMainBoss::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	MyController = Cast<AMainBossController>(NewController);
}

void AMainBoss::HandleDie() // MRPC에서 처리됨
{
	if (MyController)
	{
		MyController->SetActorTickEnabled(false);

		 // BT 중단
		auto btComp = Cast<UBehaviorTreeComponent>(MyController->GetComponentByClass(UBehaviorTreeComponent::StaticClass()));
		btComp->StopTree(EBTStopMode::Safe);

		 // 이동 중단
		AMainBossController* mbController = Cast<AMainBossController>(GetOwner());
		if (mbController)
			mbController->StopMovement();

		 // 충돌 설정
		GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_GameTraceChannel2, ECR_Ignore); // PlayerAttack
	}

	OnMainBossDie.Broadcast();
	Super::HandleDie();
}

void AMainBoss::OnBladeHitboxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                     UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Log, TEXT("%s"), *(OtherActor->GetName()));
	if (auto player = Cast<ACBasePlayer>(OtherActor))
	{
		player->DamageProcess(Damage);
	}
}

void AMainBoss::SRPC_PlayDarkinBladeMontage_Implementation(float InPlayRate, FName SectionName)
{
	// Attack0 .. Attack2
	MRPC_PlayDarkinBladeMontage(InPlayRate, SectionName);
}

void AMainBoss::MRPC_PlayDarkinBladeMontage_Implementation(float InPlayRate, FName SectionName)
{
	PlayAnimMontage(DarkinBlade_Montage, InPlayRate, SectionName);
}

void AMainBoss::MRPC_UpdateMainBossHPBar_Implementation()
{
	auto pc = GetWorld()->GetFirstPlayerController();
	if (!pc) return;
	if (!pc->IsLocalController()) return;

	if (auto myPC = Cast<ACPlayerController>(pc))
	{
		//LOG_S(Log, TEXT("MRPC_UpdateMainBossHPBar %d/%d"), HP, MaxHP);
		myPC->SetBossHPPercent(static_cast<float>(HP)/MaxHP);
	}
}

void AMainBoss::MyDamage(int32 DamageAmount)
{
	SRPC_Damage(DamageAmount);  // 서버에서 데미지 처리
}

void AMainBoss::SRPC_Damage(int32 DamageAmount)
{
	HP -= DamageAmount;
	MRPC_UpdateMainBossHPBar();
	
	if (HP <= 0)
	{
		HP = 0.f;
		
		bIsDead = true;
		MRPC_Die(); 
	}
}

void AMainBoss::MRPC_Die()
{
	Super::MRPC_Die();
}

// void AMainBoss::ChangePhase()
// {
// 	// 부활 + 궁
// 	bIsUlt = true;
// 	// HP = 1;
// }

void AMainBoss::ResetLeftCooltime_DarkinBlade()
{
	SkillComponent->ResetLeftCooltime(ESkillKey::Q);
}
