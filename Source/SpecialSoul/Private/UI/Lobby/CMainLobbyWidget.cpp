// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Lobby/CMainLobbyWidget.h"

#include "Components/Button.h"
#include "Components/CheckBox.h"
#include "Components/EditableText.h"
#include "Components/ScrollBox.h"
#include "Components/WidgetSwitcher.h"
#include "UI/Lobby/CSessionSlotWidget.h"

void UCMainLobbyWidget::NativeConstruct()
{
	Super::NativeConstruct();

	Chk_CreateRoom->OnCheckStateChanged.AddDynamic(this,&UCMainLobbyWidget::OnCreateRoomChecked);
	Chk_JoinRoom->OnCheckStateChanged.AddDynamic(this,&UCMainLobbyWidget::OnJoinRoomChecked);
	Btn_Enter->OnClicked.AddDynamic(this, &UCMainLobbyWidget::OnEnterClicked);
}

void UCMainLobbyWidget::OnCreateRoomChecked(bool bIsChecked)
{
	if (bIsChecked)
	{
		Chk_JoinRoom->SetCheckedState(ECheckBoxState::Unchecked);
		WidgetSwitcher->SetActiveWidgetIndex(0);
	}
}

void UCMainLobbyWidget::OnJoinRoomChecked(bool bIsChecked)
{
	if (bIsChecked)
	{
		Chk_CreateRoom->SetCheckedState(ECheckBoxState::Unchecked);
		WidgetSwitcher->SetActiveWidgetIndex(1);
	}
}

void UCMainLobbyWidget::OnEnterClicked()
{
	if (Chk_CreateRoom->IsChecked())
	{
		if (Txt_RoomName->GetText().IsEmpty()) return;

		FString roomName = Txt_RoomName->GetText().ToString();
		// 세션 생성 함수 호출
	}
	else
	{
		// 선택한 세션으로 Join 요청
		// 인덱스는 SessionNumber로 사용하면 됌
	}
}

void UCMainLobbyWidget::AddSlotWidget(const struct FSessionInfo& sessionInfo)
{
	auto slot = CreateWidget<UCSessionSlotWidget>(this, SessionInfoWidget);
	slot->SetMainLobbyWidget(this);
	slot->Set(sessionInfo);

	Scroll_RoomList->AddChild(slot);
}