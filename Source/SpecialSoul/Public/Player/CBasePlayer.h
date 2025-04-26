#pragma once

#include "CoreMinimal.h"
#include "Data/CYasuoData.h"
#include "Data/JinxData.h"
#include "Game/CPlayerState.h"
#include "GameFramework/Character.h"
#include "CBasePlayer.generated.h"

struct FSkillCooltime;
class USkillComponent;

UENUM(BlueprintType)
enum class ESkillKey : uint8
{
	Attack, Passive, E, R, Q, W
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
	void PrintNetLog();

	UPROPERTY()
	class ASpecialSoulGameMode* GM;
	UPROPERTY()
	class ACGameState* GS;
	UPROPERTY()
	class ACPlayerState* PS;
	UPROPERTY()
	class ACObjectPoolManager* ObjectPoolManager;

public:
	UPROPERTY()
	class ACPlayerController* PC;

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
	UPROPERTY(EditDefaultsOnly, Category="Components")
	class USceneCaptureComponent2D* MiniMapCam;
	UPROPERTY(EditDefaultsOnly)
	class UMaterialInterface* MiniMapSceneMat;

protected: // Actor Component
	UPROPERTY(EditDefaultsOnly)
	class UCMovementComponent* MoveComp;

protected: // Get Player Data
	UPROPERTY()
	class UCDataSheetUtility* DataSheetUtility;

protected: // MoveSpeed
	UPROPERTY(EditDefaultsOnly, Category = "MoveSpeed")
	float PlayerMoveSpeed = 600;

public: // Update Info
	virtual void UpdatePlayerData(const int32 PlayerLevel);
	void SetCardData(const TArray<FString>& CardList, TArray<FCardStruct>& CardData);
	void EndUpgrade();
	UFUNCTION(NetMulticast, Reliable)
	void MRPC_EndUpgrade();
	UFUNCTION(Client, Reliable)
	void CRPC_EndUpgrade();

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
	UFUNCTION(NetMulticast, Reliable)
	void MRPC_ShowUpgradeUI(const TArray<FString>& cardList, const TArray<FCardStruct>& CardData);
	UFUNCTION(Client, Reliable)
	void CRPC_ShowUpgradeUI(const TArray<FString>& cardList, const TArray<FCardStruct>& CardData);

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
	void SetSkillUsing(ESkillKey Key, bool bUseSkill);
	void ResetLeftCooltime(ESkillKey skillKey);
	
	UFUNCTION(Client, Reliable)
	void CRPC_SetSkillChargingUI(ESkillKey skillKey, bool bIsCharging, class ACPlayerController* InPC);

	UFUNCTION()
	void OnCooltimeChanged(ESkillKey skillKey, FSkillCooltime cooltimeInfo);

public:
	void MyApplyDamage(float Damage, class ABaseEnemy* DamagedActor);
	UFUNCTION(NetMulticast, Reliable)
	void MRPC_AddKillScore();
	UFUNCTION(Client, Reliable)
	void CRPC_AddKillScore();

public:
	UPROPERTY(Replicated)
	bool bAttacking = false;

public:
	UPROPERTY(VisibleAnywhere, Category = "Data|Stat", Replicated)
	FYasuoAttackData YasuoStat;
	UPROPERTY(VisibleAnywhere, Category = "Data|Stat", Replicated)
	FYasuoMoveData YasuoMoveInfo;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Data", Replicated)
	FJinxAttackData JinxAttackData; // 기본공격 데이터

public:
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

public:
	UFUNCTION(Server, Reliable)
	void SRPC_UnPause();
	UFUNCTION(NetMulticast, Reliable)
	void MRPC_UnPause();
	UFUNCTION(Client, Reliable)
	void CRPC_UnPause();

private:
	UFUNCTION(Client, Reliable)
	void CRPC_SetMinimap(class ACPlayerController* InPC);

public:
	virtual void SetLocalInit(class ACPlayerController* InPC);
	bool bIsLocalInit = false;

public:
	// HP 체력
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = HP)
	float MaxHP {1000.f};
	
	UPROPERTY(ReplicatedUsing=OnRep_HP, EditAnywhere, BlueprintReadWrite, Category = HP)
	float hp {MaxHP};

	UFUNCTION()
	void OnRep_HP();

	 // get, set 프로퍼티
	__declspec(property(get = GetHP, put = SetHP)) float HP;
	float GetHP();
	void SetHP(float value);

	void DamageProcess(float damage);
	bool bIsDead {false};

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = UI)
	class UWidgetComponent* OverheadUIComp;
};
