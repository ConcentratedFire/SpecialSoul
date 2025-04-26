// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/OverheadStatusWidget.h"

#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/SpringArmComponent.h"

void UOverheadStatusWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	// Super::NativeTick(MyGeometry, InDeltaTime);
	//
	// APawn* OwnerPawn = Cast<APawn>(GetOwningPlayerPawn());
	// if (!OwnerPawn) return;
	//
	//
	// USpringArmComponent* CameraBoom = OwnerPawn->FindComponentByClass<USpringArmComponent>();
	// if (!CameraBoom) return;
	//
	// UWidgetComponent* OverheadUIComp = OwnerPawn->FindComponentByClass<UWidgetComponent>();
	// if (!OverheadUIComp) return;
	//
	// // 카메라 붐 방향 기준으로 빌보딩
	// FRotator CameraRotation = CameraBoom->GetComponentRotation();
	// FRotator NewRotation(0.f, CameraRotation.Yaw, 0.f); // 수평만 유지
	//
	// OverheadUIComp->SetWorldRotation(NewRotation);
}

void UOverheadStatusWidget::SetHP(float hp, float maxHP)
{
	HP_Bar->SetPercent(hp/maxHP);
}

void UOverheadStatusWidget::SetEnergy(float energy, float maxEnergy)
{
	Energy_Bar->SetPercent(energy/maxEnergy);
}

void UOverheadStatusWidget::SetLevel(int32 level)
{
	Text_Level->SetText(FText::AsNumber(level));
}
