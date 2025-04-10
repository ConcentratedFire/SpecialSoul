// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enemy/CMeleeEnemy.h"
#include "Enemy/RangedEnemy.h"
#include "GameFramework/Actor.h"
#include "Kismet/GameplayStatics.h"
#include "CObjectPoolManager.generated.h"

DECLARE_MULTICAST_DELEGATE(FEnemyGotoPool);
DECLARE_MULTICAST_DELEGATE(FEnemyOutFromPool);

UCLASS()
class SPECIALSOUL_API ACObjectPoolManager : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ACObjectPoolManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	FEnemyOutFromPool EnemyOutFromPool_Dele;
	FEnemyGotoPool EnemyGotoPool_Dele;
	
	// Return To ObjectPool
	void ReturnEnemy(class ACMeleeEnemy* Enemy);

private:
	UPROPERTY(EditDefaultsOnly, Category = "ObjectPool")
	TSubclassOf<ABaseEnemy> MeleeEnemy;

private: // Object Pool
	// 한번에 스폰시킬 Enemy 마리수
	UPROPERTY(EditDefaultsOnly, Category = "ObjectPool")
	int32 AppendMeleePoolSize = 100;

	// 근거리 미니언 풀
	UPROPERTY(VisibleAnywhere, Category = "ObjectPool")
	TArray<ABaseEnemy*> MeleePool;
	// 원거리 미니언 풀
	UPROPERTY(VisibleAnywhere, Category = "ObjectPool")
	TArray<ARangedEnemy*> RangePool;
	
	/**
	 * Initializes an object pool with a specified number of objects of a given class.
	 *
	 * @tparam T 스폰시킬 오브젝트 타입
	 * @param PoolArray A reference to the array that holds the pooled objects.
	 * @param AddPoolSize The number of new objects to add to the pool.
	 * @param Class The class type of the objects to spawn into the pool.
	 *
	 * This function spawns a specified number of objects of the given class and
	 * adds them to the pool array. The objects are initialized as invisible, with
	 * collisions and ticking disabled, and placed in a default spawn location.
	 */
	template <typename T>
	void InitPool(TArray<T*>& PoolArray, const int32& AddPoolSize, const TSubclassOf<T>& Class);

	template<>
	void InitPool(TArray<ABaseEnemy*>& PoolArray, const int32& AddPoolSize, const TSubclassOf<ABaseEnemy>& Class);

private: // Place
	UPROPERTY(EditDefaultsOnly, Category = "ObjectPool")
	float Radius = 700.f;

	template <typename T>
	void PlaceEnemyRandomPlace(TArray<T*>& PoolArray, const int32& AddPoolSize, const TSubclassOf<T>& Class);
};

template <typename T>
void ACObjectPoolManager::InitPool(TArray<T*>& PoolArray, const int32& AddPoolSize, const TSubclassOf<T>& Class)
{
	if (!Class) return;
	PoolArray.Reserve(PoolArray.Max() + AddPoolSize);

	for (int i = 0; i < AddPoolSize; ++i)
	{
		FTransform Transform(FRotator::ZeroRotator, FVector(-1000, 2000, 500), FVector(1));
		FActorSpawnParameters SpawnParam;
		SpawnParam.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		T* PoolObj = GetWorld()->SpawnActorDeferred<T>(Class,Transform,nullptr,nullptr,ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
		PoolObj->SetActorEnableCollision(false);
		PoolObj->SetActorHiddenInGame(true);
		PoolObj->SetActorTickEnabled(false);
		
		UGameplayStatics::FinishSpawningActor(PoolObj, Transform);
		PoolArray.Push(PoolObj);
	}
}

template <typename T>
void ACObjectPoolManager::PlaceEnemyRandomPlace(TArray<T*>& PoolArray, const int32& AddPoolSize,
                                                const TSubclassOf<T>& Class)
{
	FVector Center = GetActorLocation();
	float RandomAngle = FMath::RandRange(0.f, 360.f);
	float X = Radius * FMath::Cos(FMath::DegreesToRadians(RandomAngle));
	float Y = Radius * FMath::Sin(FMath::DegreesToRadians(RandomAngle));
	FVector SpawnLocation = Center + FVector(X, Y, 0.f);

	// 지면 높이 조정 (옵션)
	FHitResult Hit;
	FVector TraceStart = SpawnLocation + FVector(0, 0, 1000.f);
	FVector TraceEnd = SpawnLocation - FVector(0, 0, 1000.f);
	GetWorld()->LineTraceSingleByChannel(Hit, TraceStart, TraceEnd, ECC_Visibility);
	if (Hit.bBlockingHit)
	{
		SpawnLocation.Z = Hit.Location.Z;
	}

	if (PoolArray.Num() == 0)
		InitPool(PoolArray, AddPoolSize, Class);

	// Pool에 들어가는 시점과 추가되는 시점이 겹치면 Null값이 배열에 들어가는 경우 발생
	// 만약 Top이 Null이라면 요소를 하나 빼고 다시 수행하도록 재귀 호출
	if (!PoolArray.Top())
	{
		PoolArray.Pop();
		PlaceEnemyRandomPlace(PoolArray, AddPoolSize, Class);
		return;
	}
	
	T* PoolObj = PoolArray.Pop();
	PoolObj->SetActorEnableCollision(true);
	PoolObj->SetActorHiddenInGame(false);
	PoolObj->SetActorTickEnabled(true);
	PoolObj->SetActorLocation(SpawnLocation);
	
	EnemyOutFromPool_Dele.Broadcast();
}
