// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Projectile.generated.h"

class ACBasePlayer;
class UNiagaraSystem;
class UNiagaraComponent;
class UProjectileMovementComponent;

UENUM(BlueprintType)
enum class EAttackType : uint8
{
	SingleTarget, // 단일 타겟 공격
	AreaOfEffect  // 범위 공격 
};

UCLASS()
class SPECIALSOUL_API AProjectile : public AActor
{
	GENERATED_BODY()
	
public:
	AProjectile();
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;
	
public:
	UFUNCTION(NetMulticast, Reliable)
	void MRPC_SpawnHitVFX();
	
	UFUNCTION()
	virtual void OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	void ApplyCasterStat(ACharacter* Caster);

	virtual void InitMoveComp(); // Pool에서 꺼낸 후 사용	

public:
	UPROPERTY(VisibleAnywhere, Category = "Projectile")
	TObjectPtr<USceneComponent> RootSceneComp; // 루트
	
	UPROPERTY(VisibleAnywhere, Category = "Projectile")
	TObjectPtr<UStaticMeshComponent> MeshComp; // 투사체 메시
	
	UPROPERTY(VisibleAnywhere, Category = "Projectile")
	TObjectPtr<UProjectileMovementComponent> ProjectileMovementComp;

	UPROPERTY(EditDefaultsOnly, Category="Projectile")
	TObjectPtr<UNiagaraSystem> TailVfxAsset; // NiagaraComponent는 ActorComponent이므로, 생성해서 넣어줘야한다.

	UPROPERTY(VisibleAnywhere, Category = "Projectile")
	TObjectPtr<UNiagaraComponent> TailVfx;

	UPROPERTY(EditDefaultsOnly, Category="Projectile")
	TObjectPtr<UNiagaraSystem> HitVfxAsset;
	
	UPROPERTY(VisibleAnywhere, Category = "Projectile")
	FVector StartLocation;

protected:
	virtual void OnDestroy(); // Destory OR Pool->Push

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Projectile")
	EAttackType AttackType = EAttackType::SingleTarget;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Projectile")
	int32 Penetration {1}; // 사거리

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Projectile")
	float Damage {30.f}; // 데미지

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Projectile")
	float AttackRange {1500.f}; // 사거리
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Projectile")
	float CurrentDistance;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Projectile", meta = (EditCondition = "AttackType == EAttackType::AreaOfEffect"))
	float ExplosionRadius = 100.0f;
};

//NS_FireArrow & NS_FireArrow_Hit=> 징키 평타 꼬리VFX(크기 줄이기) & E공격 꼬리 VFX
//NS_BigFire => 징키 궁 꼬리VFX
//NS_Projectile_03 + NS_Projectile_03_Hit => 원거리적 공격 