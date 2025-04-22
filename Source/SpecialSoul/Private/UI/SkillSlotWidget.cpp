// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/SkillSlotWidget.h"

#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Player/CBasePlayer.h"

void USkillSlotWidget::SetSkillSlotVisuals(ESkillKey skillKey, UObject* InResoueceObject) 
{
	if (txt_SkillKey)
	{
		txt_SkillKey->SetText(FText::FromString(KeyToString(skillKey)));
	}
	if (btn_SkillSlot && InResoueceObject)
	{
		FButtonStyle buttonStyle = btn_SkillSlot->GetStyle();

		FSlateBrush newBrush;
		newBrush.SetResourceObject(InResoueceObject);
		newBrush.DrawAs = ESlateBrushDrawType::Type::Image;
		buttonStyle.SetNormal(newBrush);
		buttonStyle.SetHovered(newBrush);
		buttonStyle.SetPressed(newBrush);

		// 버튼 스타일 적용 (Setter 사용)
		btn_SkillSlot->SetStyle(buttonStyle);
	}
}

void USkillSlotWidget::UpdateCoolTime(float leftTime, float totalTime)
{
	//UE_LOG(LogTemp, Warning, TEXT("USkillSlotWidget::UpdateSkillCooltime"));

	UpdateImages_LeftCooltimePercent(leftTime/totalTime);
	UpdateText_LeftCooltime(leftTime);
}

// ------ private ------

void USkillSlotWidget::UpdateImages_LeftCooltimePercent(float leftPercent)
{
	SetBlurMaterialScalarParam("Percent", 1-leftPercent); // 1-percent만큼 블러처리
	SetCoverMaterialScalarParam("Percent", leftPercent); //percent만큼 가리기

	if (leftPercent <= 0.f)
	{
		if (img_Blur)
			img_Blur->SetOpacity(0.f); // 블러X (투명하게)
	}
}

void USkillSlotWidget::UpdateText_LeftCooltime(float leftTime)
{
	FString t = FString::Printf(TEXT("%.1f"), leftTime); // 소수점 1자리까지 표시
	txt_Cooltime->SetText(FText::FromString(t));

	if (t == "0.0")
	{
		txt_Cooltime->SetVisibility(ESlateVisibility::Hidden);	
	}
	else
	{
		txt_Cooltime->SetVisibility(ESlateVisibility::Visible);
	}
}

void USkillSlotWidget::SetBlurMaterialScalarParam(FName paramName, float value)
{
	if (img_Blur)
	{
		auto dm = img_Blur->GetDynamicMaterial();
		if (dm)
			dm->SetScalarParameterValue(paramName, value);
	}
}

void USkillSlotWidget::SetCoverMaterialScalarParam(FName paramName, float value)
{
	if (img_Cover)
	{
		auto dm = img_Cover->GetDynamicMaterial();
		if (dm)
			dm->SetScalarParameterValue(paramName, value);
	}
}

FString USkillSlotWidget::KeyToString(ESkillKey Key)
{
	switch (Key)
	{
	case ESkillKey::E: return TEXT("E");
	case ESkillKey::R: return TEXT("R");
	default: return TEXT("?");
	}
}
