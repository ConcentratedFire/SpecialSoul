// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SpecialSoul.h"
#include "Enemy/CMeleeEnemy.h"
#include "Enemy/CMiddleBoss.h"
#include "Enemy/AttackActors/CMiddleBossBullet.h"
#include "Enemy/Ranged/RangedEnemy.h"
#include "GameFramework/Actor.h"
#include "Item/CExp.h"
#include "Item/CExpMagnet.h"
#include "Item/CItemBox.h"
#include "Kismet/GameplayStatics.h"
#include "Player/AttackActors/CTornado.h"
#include "Player/AttackActors/CTornado_E.h"
#include "Player/AttackActors/CWindWall.h"
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
	void InitSettings();

public:
	FEnemyOutFromPool EnemyOutFromPool_Dele;
	FEnemyGotoPool EnemyGotoPool_Dele;

	// Return To ObjectPool
	void ReturnEnemy(ACMeleeEnemy* Enemy);
	void ReturnTornado(ACTornado* Tornado);
	void ReturnTornadoE(ACTornado_E* Tornado);
	void ReturnWindWall(ACWindWall* WindWall);
	void ReturnExp(ACExp* EXP);
	void ReturnItemBox(ACItemBox* ItemBox);
	void ReturnExpMagnet(ACExpMagnet* ExpMagnet);
	void ReturnMiddleBossBullet(ACMiddleBossBullet* MiddleBossBullet);

	// Tornado
	void MakeTornadoPool(AActor* NewOwner);
	void TornadoSpawn(FTransform SpawnTransform);
	void TornadoESpawn(FTransform SpawnTransform);

	// Wind Wall
	void WindWallSpawn(FTransform SpawnTransform);

	// Object Setting
	void ExpSpawn(FTransform SpawnTransform);
	void MagnetSpawn(FTransform SpawnTransform);

	// Enemy Setting
	void EnemySpawn(bool bIsMelee);
	void MiddleBossSpawn();
	void MiddleBossBulletSpawn(FTransform SpawnTransform);

private:
	UPROPERTY(EditDefaultsOnly, Category = "ObjectPool")
	TSubclassOf<ABaseEnemy> MeleeEnemy;
	UPROPERTY(EditDefaultsOnly, Category = "ObjectPool")
	TSubclassOf<ABaseEnemy> RangeEnemy;
	UPROPERTY(EditDefaultsOnly, Category = "ObjectPool")
	TSubclassOf<ACTornado> TornadoActor;
	UPROPERTY(EditDefaultsOnly, Category = "ObjectPool")
	TSubclassOf<ACTornado_E> TornadoEActor;
	UPROPERTY(EditDefaultsOnly, Category = "ObjectPool")
	TSubclassOf<ACWindWall> WindWallActor;
	UPROPERTY(EditDefaultsOnly, Category = "ObjectPool")
	TSubclassOf<ACExp> ExpActor;
	UPROPERTY(EditDefaultsOnly, Category = "ObjectPool")
	TSubclassOf<ACItemBox> ItemBoxActor;
	UPROPERTY(EditDefaultsOnly, Category = "ObjectPool")
	TSubclassOf<ACExpMagnet> ItemMagnetActor;

	UPROPERTY(EditDefaultsOnly, Category = "ObjectPool")
	TSubclassOf<ACMiddleBoss> MiddleBossActor;
	UPROPERTY(EditDefaultsOnly, Category = "ObjectPool")
	TSubclassOf<ACMiddleBossBullet> MiddleBossBulletActor;

private: // Object Pool
	UPROPERTY(EditDefaultsOnly, Category="ObjectPool")
	FVector PoolLocation = FVector(-1000, 2000, 500);

	// 한번에 스폰시킬 Enemy 마리수
	UPROPERTY(EditDefaultsOnly, Category = "ObjectPool")
	int32 AppendEnemyPoolSize = 100;
	// 한번에 스폰시킬 회오리수
	UPROPERTY(EditDefaultsOnly, Category = "ObjectPool")
	int32 AppendTornadoPoolSize = 100;
	// 한번에 스폰시킬 EQ회오리수
	UPROPERTY(EditDefaultsOnly, Category = "ObjectPool")
	int32 AppendTornadoEPoolSize = 10;
	// 한번에 스폰시킬 장막
	UPROPERTY(EditDefaultsOnly, Category = "ObjectPool")
	int32 AppendWindWallPoolSize = 3;
	// 한번에 스폰시킬 경험치수
	UPROPERTY(EditDefaultsOnly, Category = "ObjectPool")
	int32 AppendExpPoolSize = 100;
	// 한번에 스폰시킬 아이템박스 개수
	UPROPERTY(EditDefaultsOnly, Category = "ObjectPool")
	int32 AppendItemBoxSize = 10;
	// 한번에 스폰시킬 자석 아이템 개수
	UPROPERTY(EditDefaultsOnly, Category = "ObjectPool")
	int32 AppendItemMagnetSize = 10;

	// 한번에 스폰시킬 중간보스 개수
	UPROPERTY(EditDefaultsOnly, Category = "ObjectPool")
	int32 AppendMiddleBossSize = 2;
	// 한번에 스폰시킬 중간보스 투사체 개수
	UPROPERTY(EditDefaultsOnly, Category = "ObjectPool")
	int32 AppendMiddleBossBulletSize = 10;
	
	// 근거리 미니언 풀
	UPROPERTY(VisibleAnywhere, Category = "ObjectPool")
	TArray<ABaseEnemy*> MeleePool;
	// 원거리 미니언 풀
	UPROPERTY(VisibleAnywhere, Category = "ObjectPool")
	TArray<ABaseEnemy*> RangePool;
	// 회오리 풀
	UPROPERTY(VisibleAnywhere, Category = "ObjectPool")
	TArray<ACTornado*> TornadoPool;
	// EQ 회오리 풀
	UPROPERTY(VisibleAnywhere, Category = "ObjectPool")
	TArray<ACTornado_E*> TornadoEPool;
	// 장막 풀
	UPROPERTY(VisibleAnywhere, Category = "ObjectPool")
	TArray<ACWindWall*> WindWallPool;
	// 경험치 풀
	UPROPERTY(VisibleAnywhere, Category = "ObjectPool")
	TArray<ACExp*> ExpPool;
	// 아이템박스 풀
	UPROPERTY(VisibleAnywhere, Category = "ObjectPool")
	TArray<ACItemBox*> ItemBoxPool;
	// 자석 아이템 풀
	UPROPERTY(VisibleAnywhere, Category = "ObjectPool")
	TArray<ACExpMagnet*> ItemMagnetPool;

	// 중간보스 풀
	UPROPERTY(VisibleAnywhere, Category = "ObjectPool")
	TArray<ACMiddleBoss*> MiddleBossPool;
	// 중간보스 투사체 풀
	UPROPERTY(VisibleAnywhere, Category = "ObjectPool")
	TArray<ACMiddleBossBullet*> MiddleBossBulletPool;

private: // ItemBox	
	UPROPERTY(EditAnywhere, Category = "ObjectPool|ItemBox")
	TArray<FVector> ItemBoxLocation;

	void PlaceItemBox();

private: // Function
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
	void InitPool(TArray<T*>& PoolArray, const int32& AddPoolSize, const TSubclassOf<T>& Class,
	              AActor* NewOwner = nullptr);

	// 템플릿 특수화 (Enemy 용도)
	template <>
	void InitPool(TArray<ABaseEnemy*>& PoolArray, const int32& AddPoolSize, const TSubclassOf<ABaseEnemy>& Class,
	              AActor* NewOwner);

private: // Place
	UPROPERTY(EditDefaultsOnly, Category = "ObjectPool")
	float Radius = 700.f;
	UPROPERTY(EditDefaultsOnly, Category = "ObjectPool")
	float TraceRadius = 100.f;

	template <typename T>
	void PlaceEnemyRandomPlace(TArray<T*>& PoolArray, const int32& AddPoolSize, const TSubclassOf<T>& Class);

	template <typename T>
	void PlaceActorSetPlace(TArray<T*>& PoolArray, const int32& AddPoolSize, const TSubclassOf<T>& Class,
	                        const FTransform SpawnTransform);
};

template <typename T>
void ACObjectPoolManager::InitPool(TArray<T*>& PoolArray, const int32& AddPoolSize, const TSubclassOf<T>& Class,
                                   AActor* NewOwner)
{
	if (!Class) return;
	PoolArray.Reserve(PoolArray.Max() + AddPoolSize);

	for (int i = 0; i < AddPoolSize; ++i)
	{
		FTransform Transform(FRotator::ZeroRotator, PoolLocation, FVector(1));
		FActorSpawnParameters SpawnParam;
		SpawnParam.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		T* PoolObj = GetWorld()->SpawnActorDeferred<T>(Class, Transform, nullptr, nullptr,
		                                               ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
		PoolObj->SetActorEnableCollision(false);
		PoolObj->SetActorHiddenInGame(true);
		PoolObj->SetActorTickEnabled(false);
		PoolObj->SetManager(this);
		if (NewOwner != nullptr)
			PoolObj->SetOwner(NewOwner);

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
	FVector TraceStart = SpawnLocation + FVector(0, 0, 300);
	FVector TraceEnd = SpawnLocation - FVector(0, 0, 300);
	GetWorld()->LineTraceSingleByChannel(Hit, TraceStart, TraceEnd, ECC_Visibility);
	if (Hit.bBlockingHit)
	{
		// SpawnLocation.Z = Hit.Location.Z;
		SpawnLocation.Z = GetActorLocation().Z;
	}

	if (PoolArray.Num() == 0)
		InitPool(PoolArray, AddPoolSize, Class);

	// Pool에 들어가는 시점과 추가되는 시점이 겹치면 Null값이 배열에 들어가는 경우 발생
	// 만약 Top이 Null이라면 요소를 제거하고 다시 수행하도록 재귀 호출
	bool bIsNullEnemy = false;
	while (PoolArray.Num() > 0 && !PoolArray.Top())
	{
		bIsNullEnemy = true;
		PoolArray.Pop();
	}

	if (bIsNullEnemy)
		PlaceEnemyRandomPlace(PoolArray, AddPoolSize, Class);

	T* PoolObj = PoolArray.Pop();
	PoolObj->ResetEnemy();
	PoolObj->SetActorEnableCollision(true);
	PoolObj->SetActorHiddenInGame(false);
	PoolObj->SetActorTickEnabled(true);
	// PoolObj->SetActorLocation(GetActorLocation());
	PoolObj->SetActorLocation(SpawnLocation);

	//DrawDebugLine(GetWorld(), GetActorLocation(), GetActorLocation()+(SpawnLocation-GetActorLocation()), FColor::Red, false, 1.f, 0, 2.f);
	// LOG_S(Warning, TEXT("-------Succeed-------"));
	EnemyOutFromPool_Dele.Broadcast();
}

template <typename T>
void ACObjectPoolManager::PlaceActorSetPlace(TArray<T*>& PoolArray, const int32& AddPoolSize,
                                             const TSubclassOf<T>& Class, const FTransform SpawnTransform)
{
	if (PoolArray.Num() == 0)
		InitPool(PoolArray, AddPoolSize, Class);

	// Pool에 들어가는 시점과 추가되는 시점이 겹치면 Null값이 배열에 들어가는 경우 발생
	// 만약 Top이 Null이라면 요소를 제거하고 다시 수행하도록 재귀 호출
	bool bIsNullActor = false;
	while (!PoolArray.Top())
	{
		bIsNullActor = true;
		PoolArray.Pop();
	}

	if (bIsNullActor)
		PlaceActorSetPlace(PoolArray, AddPoolSize, Class, SpawnTransform);

	T* PoolObj = PoolArray.Pop();
	// LOG_S(Warning, TEXT("%s"), PoolObj?*PoolObj->GetName():TEXT("Nullptr"));
	PoolObj->SetActorTransform(SpawnTransform);
	PoolObj->SetActorEnableCollision(true);
	PoolObj->SetActorHiddenInGame(false);
	// PoolObj->SetActorTickEnabled(true);
}
