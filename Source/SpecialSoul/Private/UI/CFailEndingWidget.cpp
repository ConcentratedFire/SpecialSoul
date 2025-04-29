// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/CFailEndingWidget.h"

#include "Components/Button.h"

void UCFailEndingWidget::NativeConstruct()
{
	Super::NativeConstruct();

	Btn_Exit->OnClicked.AddDynamic(this, &UCFailEndingWidget::OnExitClicked);
}

void UCFailEndingWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (DeadAnim && !bIsPlayDeadAnim)
	{
		PlayAnimation(DeadAnim);
		bIsPlayDeadAnim = true;
	}
}

void UCFailEndingWidget::OnExitClicked()
{
}
