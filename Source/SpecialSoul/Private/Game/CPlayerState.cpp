// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/CPlayerState.h"

#include "SpecialSoul.h"
#include "Player/CYasuo.h"
#include "Utility/CDataSheetUtility.h"

void ACPlayerState::InitPlayerState(class UCDataSheetUtility* Utility)
{
	// Utility->OnDataFetched.AddDynamic(this, &ACPlayerState::UpdateGradeInfo);
	Utility->FetchGoogleSheetData<FUpgradeData>("Upgrade", "A1", "G6", UpgradeDataMap);
	
	Player = Cast<ACBasePlayer>(GetPawn());
}

void ACPlayerState::UpdateGradeInfo()
{
	// 초기 데이터 세팅
	// if (UpgradeDataMap.Num() > 0)
	// {
	// 	CurDamageGrade = 1, CurAbilityHasteGrade = 1, CurProjectilesGrade = 1, CurCritChanceGrade = 1;
	// }
}

TArray<FString> ACPlayerState::ChooseUpgradeCardList()
{
	if (UpgradeData.Num() >= 3) ShuffleArray(UpgradeData);

	TArray<FString> CardList;
	int32 CardCount = 0;
	for (int i = 0; i < UpgradeData.Num() - 1; i++)
	{
		CardList.Push(UpgradeData[i]);
		if (++CardCount == 3)
			break;
	}

	return CardList;
}

void ACPlayerState::UpgradeStat(const FString statName)
{
	if (statName == "Weapon")
	{
		++CurWeaponGrade;
		Player->UpgradeWeapon(CurWeaponGrade);	// 플레이어 공통 함수를 통해서 업그레이드 처리
		if (IsMaxUpgrade_Weapon())
		{
			RemoveArrayElement(statName);
		}
	}
	else if (statName == "Damage")
	{
		++CurDamageGrade;
		if (IsMaxUpgrade_Damage())
		{
			RemoveArrayElement(statName);
		}
	}
	else if (statName == "AbilityHaste")
	{
		++CurAbilityHasteGrade;
		if (IsMaxUpgrade_AbilityHaste())
		{
			RemoveArrayElement(statName);
		}
	}
	else if (statName == "Projectiles")
	{
		++CurProjectilesGrade;
		if (IsMaxUpgrade_Projectile())
		{
			RemoveArrayElement(statName);
		}
	}
	else if (statName == "CritChance")
	{
		++CurCritChanceGrade;
		if (IsMaxUpgrade_CritChance())
		{
			RemoveArrayElement(statName);
		}
	}

	Player->EndUpgrade();
}

bool ACPlayerState::IsMaxUpgrade_Weapon()
{
	if (Player->IsA(ACYasuo::StaticClass()))
		return CurWeaponGrade == YasuoAttackDataMap.Num();
	else
		return CurWeaponGrade == JinxAttackDataMap.Num();
}

void ACPlayerState::ShuffleArray(TArray<FString>& Array)
{
	FRandomStream Random(FMath::Rand());

	for (int32 i = Array.Num() - 1; i > 0; i--)
	{
		int32 j = Random.RandRange(0, i);
		Array.Swap(i, j);
	}
}

void ACPlayerState::RemoveArrayElement(const FString Element)
{
	UpgradeData.Remove(Element);
}
