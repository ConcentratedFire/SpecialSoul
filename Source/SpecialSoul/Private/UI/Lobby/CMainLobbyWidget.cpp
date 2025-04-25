// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Lobby/CMainLobbyWidget.h"

#include "SpecialSoul.h"
#include "Components/Button.h"
#include "Components/CheckBox.h"
#include "Components/EditableText.h"
#include "Components/ScrollBox.h"
#include "Components/TextBlock.h"
#include "Components/WidgetSwitcher.h"
#include "UI/Lobby/CSessionSlotWidget.h"

void UCMainLobbyWidget::NativeConstruct()
{
	Super::NativeConstruct();

	Chk_CreateRoom->OnCheckStateChanged.AddDynamic(this,&UCMainLobbyWidget::OnCreateRoomChecked);
	Chk_JoinRoom->OnCheckStateChanged.AddDynamic(this,&UCMainLobbyWidget::OnJoinRoomChecked);
	Btn_Enter->OnClicked.AddDynamic(this, &UCMainLobbyWidget::OnEnterClicked);
	Btn_Find->OnClicked.AddDynamic(this,&UCMainLobbyWidget::OnFindClicked);

	gi = Cast<UCGameInstance>(GetWorld()->GetGameInstance());
	gi->onSearchCompleted.AddDynamic(this, &UCMainLobbyWidget::AddSlotWidget);
	gi->onSearchState.AddDynamic(this, &UCMainLobbyWidget::OnChangeButtonEnable);

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
		if (!gi) return;
		
		FString roomName = Txt_RoomName->GetText().ToString();
		 // 세션 생성
		gi->CreateMySession(roomName, 4);
	}
	else
	{
		if (!gi)
		{
			LOG_S(Error, TEXT("GameInstance is NULL"));
			return;
		}
		
		 // 선택한 세션으로 Join 요청
		gi->JoinSelectedSession(SessionNumber);
	}
}

void UCMainLobbyWidget::OnFindClicked()
{
	// 기존 슬롯 리스트를 모두 지운다
	Scroll_RoomList->ClearChildren();

	if (gi)
	{
		gi->FindOtherSession();
	}
}

void UCMainLobbyWidget::AddSlotWidget(const struct FSessionInfo& sessionInfo)
{
	auto slot = CreateWidget<UCSessionSlotWidget>(this, SessionInfoWidget);
	slot->SetMainLobbyWidget(this);
	slot->Set(sessionInfo);

	Scroll_RoomList->AddChild(slot);
}

void UCMainLobbyWidget::OnChangeButtonEnable(bool bIsSearching)
{
	Btn_Find->SetIsEnabled(!bIsSearching);

	if( bIsSearching == true )
	{
		// 검색중 보이도록 처리
		Txt_FindingMsg->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		// 검색중 사라지도록 처리
		Txt_FindingMsg->SetVisibility(ESlateVisibility::Hidden);
	}
}