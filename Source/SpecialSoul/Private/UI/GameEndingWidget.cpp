// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/GameEndingWidget.h"

#include "Components/Button.h"
#include "Game/CGameInstance.h"
#include "Player/CPlayerController.h"

void UGameEndingWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (Btn_Exit)
	{
		Btn_Exit->OnClicked.AddDynamic(this, &UGameEndingWidget::OnExitClicked);
	}
}

void UGameEndingWidget::OnExitClicked()
{
	Cast<UCGameInstance>(GetGameInstance())->ExitRoom();
}
