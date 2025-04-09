// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CObjectPoolManager.generated.h"

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

private:
	UPROPERTY(EditDefaultsOnly, Category = "ObjectPool")
	TSubclassOf<class ACMeleeEnemy> MeleeEnemy;

private:	// Object Pool
	UPROPERTY(EditDefaultsOnly, Category = "ObjectPool")
	int32 AppendMeleePoolSize = 100;
	
	UPROPERTY(VisibleAnywhere, Category = "ObjectPool")
	TArray<class ACMeleeEnemy*> MeleePool;

	template<typename T>
	void InitPool(TArray<T*>& PoolArray, const int32& AddPoolSize, const TSubclassOf<T>& Class);
};

