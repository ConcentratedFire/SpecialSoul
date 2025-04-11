// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CBasePlayer.generated.h"

USTRUCT(BlueprintType)
struct FYasuoAttackData // 야스오 기본공격 데이터
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AttackData")
	int32 ID;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AttackData")
	int32 ProjectileCount;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AttackData")
	float ProjectileRange;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AttackData")
	float Damage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AttackData")
	FString UseAOE;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AttackData")
	float AOELifeTime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AttackData")
	float AOEDamage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AttackData")
	float AOEDamageCoolTime;

	FYasuoAttackData()
		: ID(0), ProjectileCount(0), ProjectileRange(0), Damage(0), UseAOE("N"), AOELifeTime(0.f), AOEDamage(0.f),
		  AOEDamageCoolTime(0.f)
	{
	}

	FYasuoAttackData(int32 id, int32 projectileCount, float projectileRange, float damage, FString useAOE,
	                 float aoeLifeTime, float aoeDamage, float AOEDamageCoolTime)
		: ID(id), ProjectileCount(projectileCount), ProjectileRange(projectileRange), Damage(damage),
		  UseAOE(useAOE), AOELifeTime(aoeLifeTime), AOEDamage(aoeDamage), AOEDamageCoolTime(AOEDamageCoolTime)
	{
	}
};

USTRUCT(BlueprintType)
struct FYasuoMoveData // 야스오 이동거리 기류 획득 데이터
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MoveData")
	int32 ID;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MoveData")
	int32 RangeFrom;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MoveData")
	int32 RangeTo;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MoveData")
	float StackDistance;
	
	FYasuoMoveData()
		: ID(0), RangeFrom(0), RangeTo(0), StackDistance(0)
	{
	}

	FYasuoMoveData(int32 id, int32 rangeFrom, float rangeTo, float stackDistance)
		: ID(id), RangeFrom(rangeFrom), RangeTo(rangeTo), StackDistance(stackDistance)
	{
	}
};

DECLARE_MULTICAST_DELEGATE_OneParam(FInputBindingDelegate, class UEnhancedInputComponent*)

UCLASS()
class SPECIALSOUL_API ACBasePlayer : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ACBasePlayer();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY()
	class ASpecialSoulGameMode* GM;
	UPROPERTY()
	class ACGameState* GS;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
public: // Input
	FInputBindingDelegate OnInputBindingDel;

public: // Return Component
	/** Returns TopDownCameraComponent subobject **/
	FORCEINLINE class UCameraComponent* GetTopDownCameraComponent() const { return TopDownCameraComponent; }
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }

	virtual void Attack() PURE_VIRTUAL(); // Attack 기본 함수

private: // Input
	UPROPERTY(VisibleAnywhere, Category = Input)
	class UInputMappingContext* IMC_Player;

private: // Component
	/** Top down camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* TopDownCameraComponent;

	/** Camera boom positioning the camera above the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

private: // Actor Component
	UPROPERTY(EditDefaultsOnly)
	class UCMovementComponent* MoveComp;

protected:	// Get Player Data
	UPROPERTY()
	class UCDataSheetUtility* DataSheetUtility_;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Data")
	TMap<int32, FYasuoAttackData> YasuoAttackDataMap;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Data")
	TMap<int32, FYasuoMoveData> YasuoMoveDataMap;
	
	// Base는 virtual로만 만들고, Child에서 구현
	// Child의 BeginPlay에서 델리게이트 바인딩
	// Child에서는 override할때 UFUNCTION 붙여줘야 함.
	virtual void PrintAttackDataMap();
};
