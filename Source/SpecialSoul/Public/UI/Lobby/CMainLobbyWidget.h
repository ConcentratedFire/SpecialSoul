// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Game/CGameInstance.h"
#include "CMainLobbyWidget.generated.h"

/**
 * 
 */
UCLASS()
class SPECIALSOUL_API UCMainLobbyWidget : public UUserWidget
{
	GENERATED_BODY()

private:
	virtual void NativeConstruct() override;

private:
	UPROPERTY(EditDefaultsOnly, meta=(BindWidget))
	class UCheckBox* Chk_CreateRoom;
	UPROPERTY(EditDefaultsOnly, meta=(BindWidget))
	class UCheckBox* Chk_JoinRoom;
	UPROPERTY(EditDefaultsOnly, meta=(BindWidget))
	class UWidgetSwitcher* WidgetSwitcher;
	UPROPERTY(EditDefaultsOnly, meta=(BindWidget))
	class UEditableText* Txt_RoomName;
	UPROPERTY(EditDefaultsOnly, meta=(BindWidget))
	class UButton* Btn_Enter;

	UPROPERTY(EditDefaultsOnly, meta=(BindWidget))
	class UScrollBox* Scroll_RoomList;
	UPROPERTY(EditDefaultsOnly, meta=(BindWidget))
	class UTextBlock* Txt_FindingMsg;
	UPROPERTY(EditDefaultsOnly, meta=(BindWidget))
	class UButton* Btn_Find;

	UFUNCTION()
	void OnCreateRoomChecked(bool bIsChecked);
	UFUNCTION()
	void OnJoinRoomChecked(bool bIsChecked);
	UFUNCTION()
	void OnEnterClicked();

private:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class UCSessionSlotWidget> SessionInfoWidget;

	void AddSlotWidget(const struct FSessionInfo& sessionInfo);

public:
	int32 SessionNumber;
};
