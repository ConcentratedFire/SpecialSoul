#include "Enemy/Ranged/RangedEnemy.h"

#include "Components/CapsuleComponent.h"
#include "Enemy/Components/EnemyFSMComponent.h"
#include "Enemy/Ranged/RangedEnemyAIController.h"
#include "ObjectPool/CObjectPoolManager.h"

ARangedEnemy::ARangedEnemy()
{
	PrimaryActorTick.bCanEverTick = true;

	ConstructorHelpers::FClassFinder<ARangedEnemyAIController> tempController(
	TEXT("/Script/Engine.Blueprint'/Game/Enemy/Ranged/BP_RangedEnemyAIController.BP_RangedEnemyAIController_C'"));
	if (tempController.Succeeded())
		AIControllerClass = tempController.Class;

	ConstructorHelpers::FObjectFinder<UAnimMontage> tmpAttackMontage(
		TEXT("/Script/Engine.AnimMontage'/Game/Enemy/Ranged/Anim/AM_RangedEnemy_Attack.AM_RangedEnemy_Attack'"));
	if (tmpAttackMontage.Succeeded())
		AttackMontage = tmpAttackMontage.Object;

	ConstructorHelpers::FObjectFinder<UAnimMontage> tmpDieMontage(
		TEXT("/Script/Engine.AnimMontage'/Game/Enemy/Ranged/Anim/AM_RangedEnemy_Die.AM_RangedEnemy_Die'"));
	if (tmpDieMontage.Succeeded())
		DieMontage = tmpDieMontage.Object;
	
	FSMComponent = CreateDefaultSubobject<UEnemyFSMComponent>(TEXT("FSMComponent"));

	// 스켈레탈 메시 설정
	ConstructorHelpers::FObjectFinder<USkeletalMesh> TempSkMesh (TEXT("/Script/Engine.SkeletalMesh'/Game/Asset/RangeMinion/Mesh/SM_Flying_Monster.SM_Flying_Monster'"));
	if (TempSkMesh.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(TempSkMesh.Object);
	}
	GetMesh()->SetRelativeRotation(FRotator(0.f, 90.f, 0.f));
	GetCapsuleComponent()->SetCapsuleSize(70.f, 90.f);
}

void ARangedEnemy::BeginPlay()
{
	Super::BeginPlay();
}

void ARangedEnemy::FindTarget()
{
	Super::FindTarget();

	if (Target)
	{
		FSMComponent->TargetLocation = Target->GetActorLocation();
	}
}

void ARangedEnemy::DieEndAction()
{
	if (!ObjectPoolManager) return;
	ObjectPoolManager->ReturnEnemy(this);
}

void ARangedEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
