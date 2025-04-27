// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/UpgradeSlotWidget.h"

#include "Components/Image.h"
#include "Components/TextBlock.h"

void UUpgradeSlotWidget::SetUpgradeCount(int count)
{
	Text_UpgradeCount->SetText(FText::FromString(FString::FromInt(count)));
}

void UUpgradeSlotWidget::SetImageIcon(UTexture* ImageTexture)
{
	Image_Icon->SetBrushFromTexture(Cast<UTexture2D>(ImageTexture), false);
	Image_Icon->SetColorAndOpacity(FLinearColor(1,1,1,1)); 
}
