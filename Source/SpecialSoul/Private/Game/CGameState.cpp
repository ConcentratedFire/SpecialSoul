// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/CGameState.h"

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
		UpdateYasuoAttackStat(1);
}


void ACGameState::UpdateYasuoAttackStat(const int32 Level)
{
	const auto& StatData = EXPDataMap[Level];
	ExpInfo.ID = StatData.ID;
	ExpInfo.XP = StatData.XP;
}