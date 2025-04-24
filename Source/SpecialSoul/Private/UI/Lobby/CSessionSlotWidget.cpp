// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/Lobby/CSessionSlotWidget.h"
#include "Game/CGameInstance.h"

#include "Components/Border.h"
#include "Components/TextBlock.h"
#include "UI/Lobby/CMainLobbyWidget.h"

void UCSessionSlotWidget::NativeConstruct()
{
	Super::NativeConstruct();
	Border->OnMouseButtonDownEvent.BindUFunction(this,"SessionSlotClick");
}

void UCSessionSlotWidget::SessionSlotClick(FGeometry MyGeometry, const FPointerEvent& MouseEvent)
{
	MainLobbyWidget->SessionNumber = SessionNumber;
}

void UCSessionSlotWidget::Set(const FSessionInfo& sessionInfo)
{
	Txt_RoomName->SetText(FText::FromString(sessionInfo.roomName));
	Txt_HostName->SetText(FText::FromString(sessionInfo.hostName));
	Txt_PlayerCount->SetText(FText::FromString(sessionInfo.playerCount));
	Txt_PingSpeed->SetText(FText::FromString(FString::Printf(TEXT("%dms"), sessionInfo.pingSpeed)));

	SessionNumber = sessionInfo.index;
}
