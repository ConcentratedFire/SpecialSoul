// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/GameDefeatWidget.h"

#include "Components/Button.h"
#include "Game/CGameInstance.h"

void UGameDefeatWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (Btn_Exit)
	{
		Btn_Exit->OnClicked.AddDynamic(this, &UGameDefeatWidget::OnExitClicked);
	}
}

void UGameDefeatWidget::OnExitClicked()
{
	Cast<UCGameInstance>(GetGameInstance())->ExitRoom();
}
