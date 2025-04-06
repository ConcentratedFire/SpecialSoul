// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/CYasuo.h"

#include "Utility/CDataSheetUtility.h"

void ACYasuo::BeginPlay()
{
	Super::BeginPlay();

	// TODO Init Data Settings
	// 캐릭터를 선택하면 GameMode에서 해당 캐릭터의 정보를 읽고
	// Player의 BeginPlay에서 초기 데이터를 세팅해주도록 변경
	UCDataSheetUtility* DataSheetUtility = NewObject<UCDataSheetUtility>();
	if (DataSheetUtility)
		DataSheetUtility->FetchGoogleSheetData("Yasuo", "B2", "H7");
	// 리소스 해제
	DataSheetUtility->ConditionalBeginDestroy();
	DataSheetUtility = nullptr;
}

void ACYasuo::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ACYasuo::Attack()
{
}
