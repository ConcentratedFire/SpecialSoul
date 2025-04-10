// Fill out your copyright notice in the Description page of Project Settings.


#include "ObjectPool/CObjectPoolManager.h"

#include "SpecialSoul.h"

// Sets default values
ACObjectPoolManager::ACObjectPoolManager()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
}

// Called when the game starts or when spawned
void ACObjectPoolManager::BeginPlay()
{
	Super::BeginPlay();
	// InitPool(MeleePool, AppendMeleePoolSize, MeleeEnemy);

	// 오브젝트 풀링 테스트
	// FTimerHandle TimerHandle;
	// GetWorldTimerManager().SetTimer(TimerHandle, [&]()
	// {
	// 	for (int i = 0; i < 4; ++i)
	// 		PlaceEnemyRandomPlace(MeleePool, AppendMeleePoolSize, MeleeEnemy);
	// }, 1.5f, true);	
}

void ACObjectPoolManager::ReturnEnemy(class ACMeleeEnemy* Enemy)
{
	Enemy->SetActorEnableCollision(false);
	Enemy->SetActorHiddenInGame(true);
	Enemy->SetActorTickEnabled(false);

	EnemyGotoPool_Dele.Broadcast();
	// LOG_S(Warning, TEXT("Enemy Add : %p\tName : %s"), Enemy, Enemy==nullptr?TEXT("nullptr"):*Enemy->GetName());
	MeleePool.Push(Enemy);
}

template<>
void ACObjectPoolManager::InitPool(TArray<ABaseEnemy*>& PoolArray, const int32& AddPoolSize, const TSubclassOf<ABaseEnemy>& Class)
{
	if (!Class) return;
	PoolArray.Reserve(PoolArray.Max() + AddPoolSize);

	for (int i = 0; i < AddPoolSize; ++i)
	{
		FTransform Transform(FRotator::ZeroRotator, FVector(-1000, 2000, 500), FVector(1));
		FActorSpawnParameters SpawnParam;
		SpawnParam.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		ABaseEnemy* PoolObj = GetWorld()->SpawnActorDeferred<ABaseEnemy>(Class,Transform,nullptr,nullptr,ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
		PoolObj->SetManager(this);	// BaseEnemy에 존재하는 함수
		PoolObj->SetActorEnableCollision(false);
		PoolObj->SetActorHiddenInGame(true);
		PoolObj->SetActorTickEnabled(false);
		
		UGameplayStatics::FinishSpawningActor(PoolObj, Transform);
		// LOG_S(Warning, TEXT("Enemy Add : %p\tName : %s"), PoolObj, PoolObj==nullptr?TEXT("nullptr"):*PoolObj->GetName());
		PoolArray.Push(PoolObj);
	}

	EnemyGotoPool_Dele.Broadcast();
}