// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/HUD/CTimeWidget.h"

#include "Components/TextBlock.h"

void UCTimeWidget::SetTime(float Time)
{
	int32 roundTime = FMath::RoundToInt(Time);
	FString strTime = FString::Printf(TEXT("%02d"), roundTime / 60);
	FString strMin = FString::Printf(TEXT("%02d"), roundTime % 60);
	TxtTime->SetText(FText::FromString(strTime + ":" + strMin));
}
