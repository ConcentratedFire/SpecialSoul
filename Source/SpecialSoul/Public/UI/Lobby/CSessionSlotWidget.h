// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CSessionSlotWidget.generated.h"

/**
 * 
 */
UCLASS()
class SPECIALSOUL_API UCSessionSlotWidget : public UUserWidget
{
	GENERATED_BODY()

private:
	virtual void NativeConstruct() override;

private:
	UPROPERTY(EditDefaultsOnly, meta=(BindWidget))
	class UBorder* Border;
	UPROPERTY(EditDefaultsOnly, meta=(BindWidget))
	class UTextBlock* Txt_RoomName;
	UPROPERTY(EditDefaultsOnly, meta=(BindWidget))
	class UTextBlock* Txt_HostName;
	UPROPERTY(EditDefaultsOnly, meta=(BindWidget))
	class UTextBlock* Txt_PlayerCount;
	UPROPERTY(EditDefaultsOnly, meta=(BindWidget))
	class UTextBlock* Txt_PingSpeed;

	int32 SessionNumber;

	UPROPERTY()
	class UCMainLobbyWidget* MainLobbyWidget;

	UFUNCTION()
	void SessionSlotClick(FGeometry MyGeometry, const FPointerEvent& MouseEvent);

public:
	void Set(const struct FSessionInfo& sessionInfo);

	void SetMainLobbyWidget(class UCMainLobbyWidget* NewMainLobbyWidget) { this->MainLobbyWidget = NewMainLobbyWidget; };
};
