// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/CUpgradeWidget.h"

#include "SpecialSoul.h"
#include "Components/Button.h"
#include "Game/CPlayerState.h"
#include "Player/CBasePlayer.h"
#include "Player/CPlayerController.h"

void UCUpgradeWidget::NativeConstruct()
{
	Super::NativeConstruct();

	btn_Select->OnClicked.AddDynamic(this, &UCUpgradeWidget::OnSelect);

	PlayerState = Cast<ACPlayerState>(GetOwningPlayerState());
	PC = Cast<ACPlayerController>(PlayerState->GetPlayerController());
}

void UCUpgradeWidget::OnSelect()
{
	// LOG_S(Warning, TEXT("PC : %s"), PC?*PC->GetName():TEXT("NULL PC"));
	// LOG_S(Warning, TEXT("%s"), PlayerState?*PlayerState->GetName():TEXT("NULL PlayerState"));
	// LOG_S(Warning, TEXT("%s"), *CardName);
	// PlayerState->UpgradeStat(CardName);
	PC->SRPC_UpgradeStat(CardName);
}
