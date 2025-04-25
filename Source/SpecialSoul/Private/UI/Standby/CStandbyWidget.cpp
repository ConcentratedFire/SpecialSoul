// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Standby/CStandbyWidget.h"

#include "SpecialSoul.h"
#include "Components/Button.h"
#include "Components/CanvasPanel.h"
#include "Components/TextBlock.h"
#include "Game/CPlayerState.h"
#include "GameFramework/GameState.h"
#include "GameFramework/GameStateBase.h"
#include "Player/CPlayerController.h"

void UCStandbyWidget::NativeConstruct()
{
	Super::NativeConstruct();

	Canvas_Pick->SetVisibility(ESlateVisibility::Hidden);

	Btn_Exit->OnClicked.AddDynamic(this, &UCStandbyWidget::Btn_ExitClick);
	Btn_Ready->OnClicked.AddDynamic(this, &UCStandbyWidget::Btn_ReadyClick);
	Btn_Champion->OnClicked.AddDynamic(this, &UCStandbyWidget::Btn_ChampionClick);
	Btn_Yasuo->OnClicked.AddDynamic(this, &UCStandbyWidget::Btn_YasuoPick);
	Btn_Jinx->OnClicked.AddDynamic(this, &UCStandbyWidget::Btn_JinxPick);

	PC = GetOwningPlayer<ACPlayerController>();
	Txt_PlayerReadyState->SetText(FText::FromString(""));
}

void UCStandbyWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	// 플레이어 리스트 출력
	TArray<APlayerState*> playerArr = GetWorld()->GetGameState()->PlayerArray;
	FString name;

	for (APlayerState* pState : playerArr)
	{
		if(auto ps=Cast<ACPlayerState>(pState))
			name.Append(FString::Printf(TEXT("%s : %s\n"), *ps->GetPlayerName(),
			                            ps->bIsReady ? TEXT("O") : TEXT("X")));
	}

	// LOG_S(Warning, TEXT("%s"), *name);
	Txt_PlayerReadyState->SetText(FText::FromString(name));
}

void UCStandbyWidget::Btn_ExitClick()
{
}

void UCStandbyWidget::Btn_ReadyClick()
{
	if (PC)
		PC->SRPC_ReadyToPlay();
}

void UCStandbyWidget::Btn_ChampionClick()
{
	bShowChampion = !bShowChampion;
	Canvas_Pick->SetVisibility(bShowChampion ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Hidden);
}

void UCStandbyWidget::Btn_YasuoPick()
{
	if (PC && PC->bSelectPlayer) return;
	PC->SRPC_SelectPlayer(true);
}

void UCStandbyWidget::Btn_JinxPick()
{
	if (PC && PC->bSelectPlayer) return;
	PC->SRPC_SelectPlayer(false);
}
