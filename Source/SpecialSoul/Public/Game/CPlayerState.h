// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Data/CYasuoData.h"
#include "Data/JinxData.h"
#include "GameFramework/PlayerState.h"
#include "CPlayerState.generated.h"

USTRUCT(BlueprintType)
struct FUpgradeData // 종류별 등급 데이터
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AttackData")
	FString UpgradeType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AttackData")
	int32 MaxGrade;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AttackData")
	FString CaculType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AttackData")
	int32 DefaultValue;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AttackData")
	int32 AppendValue;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AttackData")
	FString Desc;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AttackData")
	FString Title;

	FUpgradeData()
		: UpgradeType("None"), MaxGrade(0), CaculType("Append"), DefaultValue(0), AppendValue(0), Desc(""), Title("")
	{
	}

	FUpgradeData(FString upgradeType, int32 maxGrade, FString caculType, int32 defaultValue, int32 appendValue,
	             FString desc, FString title)
		: UpgradeType(upgradeType), MaxGrade(maxGrade), CaculType(caculType), DefaultValue(defaultValue),
		  AppendValue(appendValue), Desc(desc), Title(title)
	{
	}
};

DECLARE_DELEGATE(FUpdatdStat);

/**
 * 
 */
UCLASS()
class SPECIALSOUL_API ACPlayerState : public APlayerState
{
	GENERATED_BODY()

public: // 캐릭터 데이터
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Data")
	TMap<int32, FYasuoAttackData> YasuoAttackDataMap;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Data")
	TMap<int32, FYasuoMoveData> YasuoMoveDataMap;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Data")
	TMap<int32, FJinxAttackData> JinxAttackDataMap;

public: // 스텟 계산 및 반환
	float CalcDamage(float CurDamage, bool& OutbIsCri);
	float CalcAbilityHaste(float CurHaste);
	int32 CalcProjectile(int32 CurProjectile);

public:
	void InitPlayerState(class UCDataSheetUtility* Utility); // 데이터 테이블 로드

	UFUNCTION()
	void UpdateGradeInfo(); // 초기 데이터 세팅용

	TArray<FString> ChooseUpgradeCardList(); // 업그레이드 카드 리스트 선택
	void UpgradeStat(const FString statName);

	// 현재 업그레이드 단계
	FORCEINLINE void GetCurrentUpgradeGrade(int32& WeaponGrade, int32& DamageGrade, int32& AbilityHasteGrade,
	                                        int32& ProjectilesGrade,
	                                        int32& CritChanceGrade)
	{
		WeaponGrade = CurWeaponGrade, DamageGrade = CurDamageGrade, AbilityHasteGrade = CurAbilityHasteGrade,
			ProjectilesGrade = CurProjectilesGrade,
			CritChanceGrade = CurCritChanceGrade;
	};

	// 다음 무기 업그레이드가 마지막 업그레이드인지 확인하는 함수 
	// 템플릿으로 어떻게 합칠지 생각 안나서 우선 따로 생성
	FORCEINLINE bool NextWeaponUpgradeIsFinal_Yasuo()
	{
		return CurWeaponGrade + 1 == YasuoAttackDataMap.Num();
	};

	FORCEINLINE bool NextWeaponUpgradeIsFinal_Jinx()
	{
		return CurWeaponGrade + 1 == JinxAttackDataMap.Num();
	};

	FORCEINLINE void GetWeaponDamage_Yasuo(int32 level, int32& outCurDamage, int32& outNextDamage)
	{
		outCurDamage = 0, outNextDamage = 0;
		if (YasuoAttackDataMap.Contains(level))
			outCurDamage = YasuoAttackDataMap[level].Damage;
		if (YasuoAttackDataMap.Contains(level + 1))
			outNextDamage = YasuoAttackDataMap[level + 1].Damage;
	}

	FORCEINLINE void GetWeaponDamage_Jinx(int32 level, int32& outCurDamage, int32& outNextDamage)
	{
		outCurDamage = 0, outNextDamage = 0;
		if (JinxAttackDataMap.Contains(level))
			outCurDamage = JinxAttackDataMap[level].Damage;
		if (JinxAttackDataMap.Contains(level + 1))
			outNextDamage = JinxAttackDataMap[level + 1].Damage;
	}

	FORCEINLINE void GetWeaponProjectile_Yasuo(int32 level, int32& outCurProjectile, int32& outNexProjectile)
	{
		outCurProjectile = 0, outNexProjectile = 0;
		if (YasuoAttackDataMap.Contains(level))
			outCurProjectile = YasuoAttackDataMap[level].ProjectileCount;
		if (YasuoAttackDataMap.Contains(level + 1))
			outNexProjectile = YasuoAttackDataMap[level + 1].ProjectileCount;
	}
	
	FORCEINLINE void GetWeaponProjectile_Jinx(int32 level, int32& outCurProjectile, int32& outNexProjectile)
	{
		outCurProjectile = 0, outNexProjectile = 0;
		if (JinxAttackDataMap.Contains(level))
			outCurProjectile = JinxAttackDataMap[level].ProjectileCount;
		if (JinxAttackDataMap.Contains(level + 1))
			outNexProjectile = JinxAttackDataMap[level + 1].ProjectileCount;
	}

	// ==========최대 업글 단계인지 확인============
	bool IsMaxUpgrade_Weapon();
	FORCEINLINE bool IsMaxUpgrade_Damage() { return CurDamageGrade == UpgradeDataMap[FString("Damage")].MaxGrade; }
	FORCEINLINE bool IsMaxUpgrade_AbilityHaste(){	return CurAbilityHasteGrade == UpgradeDataMap[FString("AbilityHaste")].MaxGrade;}
	FORCEINLINE bool IsMaxUpgrade_Projectile(){	return CurProjectilesGrade == UpgradeDataMap[FString("Projectiles")].MaxGrade;}
	FORCEINLINE bool IsMaxUpgrade_CritChance(){	return CurCritChanceGrade == UpgradeDataMap[FString("CritChance")].MaxGrade;}
	// ==========================================

	FORCEINLINE FString GetUpgradeData(FString UpgradeType, FString& OutDesc, FString& OutTitle)
	{
		FString strUpgradeData = "";
		int32 defaultValue = 0;
		int32 increaseGrade = 0;
		if (UpgradeDataMap.Contains(UpgradeType))
		{
			defaultValue = UpgradeDataMap[UpgradeType].DefaultValue;
			increaseGrade = UpgradeDataMap[UpgradeType].AppendValue;
			OutTitle = UpgradeDataMap[UpgradeType].Title;
		}

		OutDesc = UpgradeDataMap[UpgradeType].Desc;

		int32 curUpgradeLevel = 0;
		// if (UpgradeType == "Weapon")
		// 	curUpgradeLevel = CurWeaponGrade;
		if (UpgradeType == "Damage")
			curUpgradeLevel = CurDamageGrade;
		else if (UpgradeType == "AbilityHaste")
			curUpgradeLevel = CurAbilityHasteGrade;
		else if (UpgradeType == "Projectiles")
			curUpgradeLevel = CurProjectilesGrade;
		else if (UpgradeType == "CritChance")
			curUpgradeLevel = CurCritChanceGrade;

		int32 nextValue = defaultValue + increaseGrade * curUpgradeLevel;
		strUpgradeData = FString::Printf(TEXT("%d > %d"), defaultValue, nextValue);
		return strUpgradeData;
	}

private:
	UPROPERTY()
	class ACBasePlayer* Player;
	
	UPROPERTY(VisibleAnywhere, Category = "Data")
	TMap<FString, FUpgradeData> UpgradeDataMap;

	UPROPERTY(visibleAnywhere, Category = "Data")
	int32 CurWeaponGrade = 1;
	UPROPERTY(visibleAnywhere, Category = "Data")
	int32 CurDamageGrade = 0;
	UPROPERTY(visibleAnywhere, Category = "Data")
	int32 CurAbilityHasteGrade = 0;
	UPROPERTY(visibleAnywhere, Category = "Data")
	int32 CurProjectilesGrade = 0;
	UPROPERTY(visibleAnywhere, Category = "Data")
	int32 CurCritChanceGrade = 0;

	TArray<FString> UpgradeData{"Weapon", "Damage", "AbilityHaste", "Projectiles", "CritChance"};

	void ShuffleArray(TArray<FString>& Array);
	void RemoveArrayElement(const FString Element);
};
