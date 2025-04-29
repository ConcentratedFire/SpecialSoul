// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/CMeleeEnemy.h"

#include "SpecialSoul.h"
#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"
#include "ObjectPool/CObjectPoolManager.h"

class ACBasePlayer;

ACMeleeEnemy::ACMeleeEnemy()
{
	GetCapsuleComponent()->SetCapsuleHalfHeight(70.f);

	GetMesh()->SetRelativeLocation(FVector(0, 0, -60));
	GetMesh()->SetRelativeRotation(FRotator(0, 90, 0));
	GetMesh()->SetRelativeScale3D(FVector(0.008));

	ConstructorHelpers::FObjectFinder<UAnimMontage> tmpAttackMontage(
		TEXT("/Script/Engine.AnimMontage'/Game/Asset/MeleeMinion/Anim/AM_MeleeAttack.AM_MeleeAttack'"));
	if (tmpAttackMontage.Succeeded())
		AttackMontage = tmpAttackMontage.Object;

	ConstructorHelpers::FObjectFinder<UAnimMontage> tmpDieMontage(
		TEXT("/Script/Engine.AnimMontage'/Game/Asset/MeleeMinion/Anim/AM_MeleeDeath.AM_MeleeDeath'"));
	if (tmpDieMontage.Succeeded())
		DieMontage = tmpDieMontage.Object;

	AttackComp = CreateDefaultSubobject<UBoxComponent>(TEXT("AttackComp"));
	AttackComp->SetupAttachment(GetMesh(), FName("AttackSocket"));
	AttackComp->SetCollisionProfileName(FName("Enemy"));
	AttackComp->SetCollisionResponseToChannel(ECC_GameTraceChannel1, ECR_Overlap);
	AttackComp->SetCollisionResponseToChannel(ECC_GameTraceChannel2, ECR_Ignore);
	AttackComp->SetCollisionResponseToChannel(ECC_GameTraceChannel7, ECR_Ignore);
	AttackComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	AttackComp->OnComponentBeginOverlap.AddDynamic(this, &ACMeleeEnemy::OnMyOverlap);
}

void ACMeleeEnemy::BeginPlay()
{
	Super::BeginPlay();
}

void ACMeleeEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ACMeleeEnemy::DieEndAction()
{
	if (!ObjectPoolManager) return;
	ObjectPoolManager->ReturnEnemy(this);
}

void ACMeleeEnemy::OnMyOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (auto player = Cast<ACBasePlayer>(OtherActor))
	{
		player->DamageProcess(Damage);
	}
}

void ACMeleeEnemy::SetAttackCollision(bool bInEnable)
{
	AttackComp->SetCollisionEnabled(bInEnable ? ECollisionEnabled::QueryOnly : ECollisionEnabled::NoCollision);
}

void ACMeleeEnemy::SetActorHiddenInGame(bool bNewHidden)
{
	Super::SetActorHiddenInGame(bNewHidden);

	if (!bNewHidden)
	{
		AttackComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}
