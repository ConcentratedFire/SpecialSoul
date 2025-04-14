// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/CPlayerState.h"

#include "Utility/CDataSheetUtility.h"

void ACPlayerState::InitPlayerState(class UCDataSheetUtility* Utility)
{
	Utility->OnDataFetched.AddDynamic(this, &ACPlayerState::UpdateGradeInfo);
	Utility->FetchGoogleSheetData<FUpgradeData>("Upgrade", "A1", "E6", UpgradeDataMap);
}

void ACPlayerState::UpdateGradeInfo()
{
	// 초기 데이터 세팅
	if (UpgradeDataMap.Num() > 0)
	{
		CurDamageGrade = 1, CurAbilityHasteGrade = 1, CurProjectilesGrade = 1, CurCritChanceGrade = 1;
	}
}
