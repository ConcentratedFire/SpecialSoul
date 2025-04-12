#include "Enemy/Ranged/RangedEnemy.h"

#include "Components/CapsuleComponent.h"
#include "Enemy/Components/EnemyFSMComponent.h"
#include "Enemy/Ranged/RangedEnemyAIController.h"

ARangedEnemy::ARangedEnemy()
{
	PrimaryActorTick.bCanEverTick = true;

	ConstructorHelpers::FClassFinder<ARangedEnemyAIController> tempController(
	TEXT("/Script/Engine.Blueprint'/Game/Enemy/Ranged/BP_RangedEnemyAIController.BP_RangedEnemyAIController_C'"));
	if (tempController.Succeeded())
		AIControllerClass = tempController.Class;
	
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

	if (FSMComponent)
	{
		//FSMComponent->OnStateChange.AddDynamic(this, &ARangedEnemy::OnFSMStateChanged);
		//FSMComponent->OnAttack.AddDynamic(this, &ARangedEnemy::HandleAttack);
		//FSMComponent->OnDie.AddDynamic(this, &ARangedEnemy::HandleDie);
	}
}

void ARangedEnemy::FindTarget()
{
	Super::FindTarget();

	if (Target)
	{
		FSMComponent->TargetLocation = Target->GetActorLocation();
	}
}

void ARangedEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// void ARangedEnemy::OnFSMStateChanged(EEnemyState NewState)
// {
// 	if (!AnimInstance) return;
// 	
// 	auto REAnimInst = Cast<URangedEnemyAnimInstance>(AnimInstance);
// 	if (!REAnimInst) return;
// 	
// 	REAnimInst->CurrentState = NewState;
// }
