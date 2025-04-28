#include "ObjectPool/CObjectPoolManager.h"

#include "SpecialSoul.h"
#include "Game/CGameState.h"
#include "Game/SpecialSoulGameMode.h"
#include "Projectile/Enemy/RangedEnemyProjectile.h"

ACObjectPoolManager::ACObjectPoolManager()
{
	PrimaryActorTick.bCanEverTick = false;
}

void ACObjectPoolManager::BeginPlay()
{
	Super::BeginPlay();

	if (auto GM = Cast<ASpecialSoulGameMode>(GetWorld()->GetAuthGameMode()))
	{
		if (auto GS = GM->GetGameState<ACGameState>())
		{
			GS->OnNextStage.AddUObject(this, &ACObjectPoolManager::PlaceItemBox);
			GS->OnNextStage.Broadcast();
		}
	}
}

void ACObjectPoolManager::InitSettings()
{
	InitPool(ItemBoxPool, AppendItemBoxSize, ItemBoxActor);
	InitPool(ExpPool, AppendExpPoolSize, ExpActor);
	InitPool(ItemMagnetPool, AppendItemMagnetSize, ItemMagnetActor);

	InitPool(MeleePool, AppendEnemyPoolSize, MeleeEnemy);
	InitPool(RangePool, AppendEnemyPoolSize, RangedEnemy);

	// InitPool(MiddleBossPool, AppendMiddleBossSize, MiddleBossActor);
	// InitPool(MiddleBossBulletPool, AppendMiddleBossBulletSize, MiddleBossBulletActor);

	InitPool(RangedEnemyProjectilePool, AppendRangedMinionProjectileSize, RangedEnemyProjectileActor);
}

void ACObjectPoolManager::ReturnEnemy(ACMeleeEnemy* Enemy)
{
	Enemy->SetActorEnableCollision(false);
	Enemy->SetActorHiddenInGame(true);
	Enemy->SetActorTickEnabled(false);
	// LOG_S(Warning, TEXT("Enemy Add : %p\tName : %s"), Enemy, Enemy==nullptr?TEXT("nullptr"):*Enemy->GetName());
	MeleePool.Push(Enemy);
}

void ACObjectPoolManager::ReturnEnemy(ARangedEnemy* Enemy)
{
	Enemy->SetActorEnableCollision(false);
	Enemy->SetActorHiddenInGame(true);
	Enemy->SetActorTickEnabled(false);
	
	RangePool.Push(Enemy);
}

void ACObjectPoolManager::ReturnTornado(ACTornado* Tornado)
{
	Tornado->SetActorEnableCollision(false);
	Tornado->SetActorHiddenInGame(true);
	Tornado->SetActorTickEnabled(false);

	TornadoPool.Push(Tornado);
}

void ACObjectPoolManager::ReturnTornadoE(ACTornado_E* Tornado)
{
	Tornado->SetActorEnableCollision(false);
	Tornado->SetActorHiddenInGame(true);
	Tornado->SetActorTickEnabled(false);

	TornadoEPool.Push(Tornado);
}

void ACObjectPoolManager::ReturnWindWall(ACWindWall* WindWall)
{
	WindWall->SetActorEnableCollision(false);
	WindWall->SetActorHiddenInGame(true);
	WindWall->SetActorTickEnabled(false);

	WindWallPool.Push(WindWall);
}

void ACObjectPoolManager::ReturnExp(ACExp* EXP)
{
	EXP->SetActorEnableCollision(false);
	EXP->SetActorHiddenInGame(true);
	EXP->SetActorTickEnabled(false);

	ExpPool.Push(EXP);
}

void ACObjectPoolManager::ReturnItemBox(ACItemBox* ItemBox)
{
	ItemBox->SetActorEnableCollision(false);
	ItemBox->SetActorHiddenInGame(true);
	ItemBox->SetActorTickEnabled(false);
	ItemBox->SetActorLocation(PoolLocation);

	ItemBoxPool.Push(ItemBox);
}

void ACObjectPoolManager::ReturnExpMagnet(ACExpMagnet* ExpMagnet)
{
	ExpMagnet->SetActorEnableCollision(false);
	ExpMagnet->SetActorHiddenInGame(true);
	ExpMagnet->SetActorTickEnabled(false);
	ExpMagnet->SetActorLocation(PoolLocation);

	ItemMagnetPool.Push(ExpMagnet);
}

void ACObjectPoolManager::ReturnMiddleBossBullet(ACMiddleBossBullet* MiddleBossBullet)
{
	MiddleBossBullet->SetActorEnableCollision(false);
	MiddleBossBullet->SetActorHiddenInGame(true);
	MiddleBossBullet->SetActorTickEnabled(false);
	MiddleBossBullet->SetActorLocation(PoolLocation);

	MiddleBossBulletPool.Push(MiddleBossBullet);
}

void ACObjectPoolManager::ReturnRangedEnemyProjectile(ARangedEnemyProjectile* Projectile)
{
	Projectile->SetActorEnableCollision(false);
	Projectile->SetActorHiddenInGame(true);
	Projectile->SetActorTickEnabled(false);
	Projectile->SetActorLocation(PoolLocation);

	RangedEnemyProjectilePool.Push(Projectile);
}

void ACObjectPoolManager::MakeTornadoPool(AActor* NewOwner)
{
	InitPool(TornadoPool, AppendTornadoPoolSize, TornadoActor, NewOwner);
	InitPool(TornadoEPool, AppendTornadoEPoolSize, TornadoEActor, NewOwner);
	InitPool(WindWallPool, AppendWindWallPoolSize, WindWallActor, NewOwner);
}

void ACObjectPoolManager::TornadoSpawn(const FTransform SpawnTransform, AActor* NewOwner)
{
	PlaceActorSetPlace(TornadoPool, AppendTornadoPoolSize, TornadoActor, SpawnTransform, NewOwner);
}

void ACObjectPoolManager::TornadoESpawn(FTransform SpawnTransform, AActor* NewOwner)
{
	PlaceActorSetPlace(TornadoEPool, AppendTornadoEPoolSize, TornadoEActor, SpawnTransform, NewOwner);
}

void ACObjectPoolManager::WindWallSpawn(FTransform SpawnTransform)
{
	PlaceActorSetPlace(WindWallPool, AppendWindWallPoolSize, WindWallActor, SpawnTransform);
}


void ACObjectPoolManager::ExpSpawn(FTransform SpawnTransform)
{
	PlaceActorSetPlace(ExpPool, AppendExpPoolSize, ExpActor, SpawnTransform);
}

void ACObjectPoolManager::MagnetSpawn(FTransform SpawnTransform)
{
	PlaceActorSetPlace(ItemMagnetPool, AppendItemMagnetSize, ItemMagnetActor, SpawnTransform);
}

void ACObjectPoolManager::RangedEnemyProjectileSpawn(FTransform SpawnTransform)
{
	//LOG_S(Error, TEXT("RangedEnemyProjectileSpawn"));
	PlaceActorSetPlace(RangedEnemyProjectilePool, AppendRangedMinionProjectileSize, RangedEnemyProjectileActor,
	                   SpawnTransform);
}

void ACObjectPoolManager::PlaceItemBox()
{
	for (int i = 0; i < ItemBoxLocation.Num(); ++i)
	{
		if (FMath::RandRange(1, 100) > 40)
		{
			// LOG_S(Warning, TEXT("%d번째 아이템 생성 패스"), i);
			continue;
		}

		TArray<FHitResult> OutHits;
		FVector Start = ItemBoxLocation[i];
		FVector End = Start;
		FCollisionObjectQueryParams ObjectQueryParams;
		ObjectQueryParams.AddObjectTypesToQuery(ECC_GameTraceChannel4);

		bool bHit = GetWorld()->SweepMultiByObjectType(
			OutHits, // 충돌 결과를 저장할 변수
			Start, // 레이의 시작 지점
			End, // 레이의 끝 지점
			FQuat::Identity, // 회전 (기본값 사용)
			ObjectQueryParams, // 충돌 채널 (여기서는 Visibility 채널 사용)
			FCollisionShape::MakeSphere(TraceRadius) // 구의 충돌 형태
		);

		if (bHit)
		{
			bool bIsNonExp = false;

			for (auto OutHit : OutHits)
			{
				if (!OutHit.GetActor()->GetActorNameOrLabel().Contains(TEXT("EXP")))
				{
					bIsNonExp = true;
					// LOG_S(Error, TEXT("%d번째 위치에 다른 아이템이 존재"), i);
					break;
				}
			}

			if (bIsNonExp) continue; // 해당 위치에 경험치가 아닌 다른 아이템이 있으면 생성하지 않는다
		}

		// LOG_S(Log, TEXT("%d번째 아이템 생성"), i);
		FTransform Transform(FRotator::ZeroRotator, ItemBoxLocation[i], FVector(1));
		PlaceActorSetPlace(ItemBoxPool, AppendItemBoxSize, ItemBoxActor, Transform);
	}
	// LOG_S(Warning, TEXT("===========아이템=========="));
}

template <>
void ACObjectPoolManager::InitPool(TArray<ABaseEnemy*>& PoolArray, const int32& AddPoolSize,
                                   const TSubclassOf<ABaseEnemy>& Class, AActor* NewOwner)
{
	if (!Class) return;
	PoolArray.Reserve(PoolArray.Max() + AddPoolSize);

	for (int i = 0; i < AddPoolSize; ++i)
	{
		FTransform Transform(FRotator::ZeroRotator, FVector(-1000, 2000, 500), FVector(1));
		FActorSpawnParameters SpawnParam;
		SpawnParam.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		ABaseEnemy* PoolObj = GetWorld()->SpawnActorDeferred<ABaseEnemy>(
			Class, Transform, nullptr, nullptr, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
		PoolObj->SetManager(this);
		PoolObj->SetActorEnableCollision(false);
		PoolObj->SetActorHiddenInGame(true);
		PoolObj->SetActorTickEnabled(false);
		UGameplayStatics::FinishSpawningActor(PoolObj, Transform);

		// LOG_S(Warning, TEXT("Enemy Add : %p\tName : %s"), PoolObj, PoolObj==nullptr?TEXT("nullptr"):*PoolObj->GetName());
		PoolArray.Push(PoolObj);
	}
}

void ACObjectPoolManager::EnemySpawn(bool bIsMelee)
{
	if (bIsMelee)
		PlaceEnemyRandomPlace(MeleePool, AppendEnemyPoolSize, MeleeEnemy);
	else
		PlaceEnemyRandomPlace(RangePool, AppendEnemyPoolSize, RangedEnemy);
}

void ACObjectPoolManager::MiddleBossSpawn()
{
	PlaceEnemyRandomPlace(MiddleBossPool, AppendMiddleBossSize, MiddleBossActor);
}

void ACObjectPoolManager::MiddleBossBulletSpawn(FTransform SpawnTransform, AActor* NewOwner)
{
	PlaceActorSetPlace(MiddleBossBulletPool, AppendMiddleBossBulletSize, MiddleBossBulletActor, SpawnTransform,
	                   NewOwner);
}
