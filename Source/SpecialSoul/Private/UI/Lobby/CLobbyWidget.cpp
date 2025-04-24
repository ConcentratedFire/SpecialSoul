// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Lobby/CLobbyWidget.h"

#include "Components/WidgetSwitcher.h"
#include "UI/Lobby/CLoginWidget.h"


void UCLobbyWidget::NativeConstruct()
{
	Super::NativeConstruct();
	WBP_Login->SetLobbyWidget(this);
}

void UCLobbyWidget::LoginProcess()
{
	WidgetSwitcher->SetActiveWidgetIndex(1);
}
