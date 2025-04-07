// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/RangedEnemy.h"

#include "Components/CapsuleComponent.h"
#include "Enemy/EnemyAnimInstance.h"
#include "Enemy/Components/EnemyFSMComponent.h"

ARangedEnemy::ARangedEnemy()
{
	PrimaryActorTick.bCanEverTick = true;
	
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
	
	 // 델리게이트 (중재자 역할)
	 // (FSM과 AnimInstance 간의 결합도를 낮춰주기 위함)
	 // FSM과 AnimInstance가 서로의 코드를 몰라도 되도록 !!
	if (FSMComponent)
	{
		FSMComponent->OnStateChange.AddDynamic(this, &ARangedEnemy::OnFSMStateChanged);
		FSMComponent->OnAttack.AddDynamic(this, &ARangedEnemy::HandleAttack);
		//FSMComponent->OnDamaged.AddDynamic(...);
		FSMComponent->OnDie.AddDynamic(this, &ARangedEnemy::HandleDie);
	}
}

void ARangedEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ARangedEnemy::OnFSMStateChanged(EEnemyState NewState)
{
	if (AnimInstance)
	{
		AnimInstance->CurrentState = NewState;
	}
}

