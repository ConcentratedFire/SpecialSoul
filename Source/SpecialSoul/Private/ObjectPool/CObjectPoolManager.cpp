// Fill out your copyright notice in the Description page of Project Settings.


#include "ObjectPool/CObjectPoolManager.h"

#include "SpecialSoul.h"
#include "Enemy/CMeleeEnemy.h"

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
	InitPool(MeleePool, AppendMeleePoolSize, MeleeEnemy);
}

template <typename T>
void ACObjectPoolManager::InitPool(TArray<T*>& PoolArray, const int32& AddPoolSize, const TSubclassOf<T>& Class)
{
	if (!Class) return;
	PoolArray.Reserve(PoolArray.Max() + AddPoolSize);

	for (int i = 0; i < AddPoolSize; ++i)
	{
		FActorSpawnParameters SpawnParam;
		SpawnParam.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		T* PoolObj = GetWorld()->SpawnActor<T>(Class, FVector(-1000, 2000, 500), FRotator::ZeroRotator, SpawnParam);
		PoolObj->SetActorEnableCollision(false);
		PoolObj->SetActorHiddenInGame(true);
		PoolObj->SetActorTickEnabled(false);
		PoolArray.Push(PoolObj);
	}
}
