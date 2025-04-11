// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enemy/CMeleeEnemy.h"
#include "Enemy/RangedEnemy.h"
#include "GameFramework/Actor.h"
#include "Kismet/GameplayStatics.h"
#include "Player/AttackActors/CTornado.h"
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
	UPROPERTY(EditDefaultsOnly, Category = "ObjectPool")
	TSubclassOf<ABaseEnemy> RangeEnemy;
	UPROPERTY(EditDefaultsOnly, Category = "ObjectPool")
	TSubclassOf<ACTornado> TornadoActor;

private: // Object Pool
	// 한번에 스폰시킬 Enemy 마리수
	UPROPERTY(EditDefaultsOnly, Category = "ObjectPool")
	int32 AppendMeleePoolSize = 100;

	// 근거리 미니언 풀
	UPROPERTY(VisibleAnywhere, Category = "ObjectPool")
	TArray<ABaseEnemy*> MeleePool;
	// 원거리 미니언 풀
	UPROPERTY(VisibleAnywhere, Category = "ObjectPool")
	TArray<ABaseEnemy*> RangePool;

	/**
	 * 오브젝트 풀의 크기를 늘리고 UClass를 이용하여 오브젝트 생성
	 *
	 * @tparam T 스폰시킬 오브젝트 타입
	 * @param PoolArray 오브젝트 풀 배열
	 * @param AddPoolSize 추가할 배열 크기
	 * @param Class 생성할 클래스 UClass
	 *
	 */
	template <typename T>
	void InitPool(TArray<T*>& PoolArray, const int32& AddPoolSize, const TSubclassOf<T>& Class);

	// 템플릿 특수화 (Enemy 용도)
	template <>
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
		T* PoolObj = GetWorld()->SpawnActorDeferred<T>(Class, Transform, nullptr, nullptr,
		                                               ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
		PoolObj->SetActorEnableCollision(false);
		PoolObj->SetActorHiddenInGame(true);
		PoolObj->SetActorTickEnabled(false);
		PoolObj->SetManager(this);

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
	// 만약 Top이 Null이라면 요소를 제거하고 다시 수행하도록 재귀 호출
	bool bIsNullEnemy = false;
	while (!PoolArray.Top())
	{
		bIsNullEnemy = true;
		PoolArray.Pop();
	}

	if (bIsNullEnemy)
		PlaceEnemyRandomPlace(PoolArray, AddPoolSize, Class);

	T* PoolObj = PoolArray.Pop();
	PoolObj->SetActorEnableCollision(true);
	PoolObj->SetActorHiddenInGame(false);
	PoolObj->SetActorTickEnabled(true);
	PoolObj->SetActorLocation(SpawnLocation);

	EnemyOutFromPool_Dele.Broadcast();
}
