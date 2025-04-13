// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/CGameState.h"

#include "EngineUtils.h"
#include "SpecialSoul.h"
#include "Player/CYasuo.h"
#include "Player/Jinx.h"

void ACGameState::PrintAttackDataMap()
{
	// for (const auto& Pair : EXPDataMap)
	// {
	// 	UE_LOG(LogTemp, Log,
	// 		   TEXT(
	// 			   "EXPDataMap || ID: %d) Exp: %d"
	// 		   ),
	// 		   Pair.Key, Pair.Value.XP);
	// }

	// 초기 데이터 세팅
	if (EXPDataMap.Num() > 0)
		UpdateExpInfo(1);
}

void ACGameState::AddExp(const int32 exp)
{
	curExp += exp;
	if (EXPDataMap.Num() == 0) return;
	if (curExp >= ExpInfo.XP)
	{
		++curLevel;
		curExp -= ExpInfo.XP;
		UpdateExpInfo(ExpInfo.ID + 1);
	}

	LOG_S(Warning, TEXT("Level: %d, curExp: %d"), curLevel, curExp);
}


void ACGameState::UpdateExpInfo(const int32 Level)
{
	if (Level >= MaxLevel) return;
	const auto& StatData = EXPDataMap[Level];
	ExpInfo.ID = StatData.ID;
	ExpInfo.XP = StatData.XP;

	if (Level == 1) return;
	// 레벨업 후 야스오 정보 갱신
	for (TActorIterator<ACYasuo> It(GetWorld(), ACYasuo::StaticClass()); It; ++It)
	{
		(*It)->UpdatePlayerData(Level);
	}

	for (TActorIterator<AJinx> It(GetWorld(), AJinx::StaticClass()); It; ++It)
	{
		(*It)->UpdatePlayerData(Level);
	}
}
