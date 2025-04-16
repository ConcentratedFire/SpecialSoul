// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Game/CPlayerState.h"
#include "GameFramework/Character.h"
#include "CBasePlayer.generated.h"

UENUM(BlueprintType)
enum class ESkillKey : uint8
{
	Attack, Passive, E, R
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
	UPROPERTY()
	class ACPlayerState* PS;
	UPROPERTY()
	class ACObjectPoolManager* ObjectPoolManager;

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

protected: // Actor Component
	UPROPERTY(EditDefaultsOnly)
	class UCMovementComponent* MoveComp;

protected: // Get Player Data
	UPROPERTY()
	class UCDataSheetUtility* DataSheetUtility;

public:	
	// Base는 virtual로만 만들고, Child에서 구현
	// Child의 BeginPlay에서 델리게이트 바인딩
	// Child에서는 override할때 UFUNCTION 붙여줘야 함.
	virtual void PrintAttackDataMap()
	{
	};

protected: // MoveSpeed
	UPROPERTY(EditDefaultsOnly, Category = "MoveSpeed")
	float PlayerMoveSpeed = 600;

public: // Update Info
	virtual void UpdatePlayerData(const int32 PlayerLevel);
	void EndUpgrade();

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	class UWidgetComponent* ArrowWidgetComp;
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Comp")
	TObjectPtr<class USkillComponent> SkillComponent;

protected: // Arrow UI
	FRotator ArrowRotation{90, 0, -45};

	virtual void RotateArrow()
	{
	};

private: // 업그레이드 UI
	void InitUpgradeUI();

protected:
	UPROPERTY()
	class UCSelectUpgradeWidget* SelectUpgradeWidget;

public:
	FORCEINLINE bool IsNextWeaponFinal(bool bYasuoType)
	{
		if (bYasuoType)
			return PS->NextWeaponUpgradeIsFinal_Yasuo();
		else
			return PS->NextWeaponUpgradeIsFinal_Jinx();
	}

	FORCEINLINE void GetCurGrade(int32& WeaponGrade, int32& DamageGrade, int32& AbilityHasteGrade,
	                             int32& ProjectilesGrade,
	                             int32& CritChanceGrade)
	{
		PS->GetCurrentUpgradeGrade(WeaponGrade, DamageGrade, AbilityHasteGrade, ProjectilesGrade, CritChanceGrade);
	}

	FORCEINLINE void GetCurDamageNextDamage(int32 level, int32& CurDamage, int32& NextDamage, bool bYasuoType)
	{
		if (bYasuoType)
			PS->GetWeaponDamage_Yasuo(level, CurDamage, NextDamage);
		else
			PS->GetWeaponDamage_Jinx(level, CurDamage, NextDamage);
	}

	FORCEINLINE void GetCurProjectileNextProjectile(int32 level, int32& CurProjectile, int32& NextProjectile,
	                                                bool bYasuoType)
	{
		if (bYasuoType)
			PS->GetWeaponProjectile_Yasuo(level, CurProjectile, NextProjectile);
		else
			PS->GetWeaponProjectile_Jinx(level, CurProjectile, NextProjectile);
	}

	FORCEINLINE FString GetUpgradeData(FString UpgradeType, FString& OutDesc, FString& OutTitle)
	{
		return PS->GetUpgradeData(UpgradeType, OutDesc, OutTitle);
	}

public:
	virtual void UpgradeWeapon(const int32 Level)
	{
	};

public:
	void SkillEnd(ESkillKey Key);
};
