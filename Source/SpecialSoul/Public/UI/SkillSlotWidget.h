// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SkillSlotWidget.generated.h"

enum class ESkillKey : uint8;
/**
 * 
 */
UCLASS()
class SPECIALSOUL_API USkillSlotWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(meta = (BindWidget))
	class UButton* btn_SkillSlot; // TODO : 버튼의 이미지로 스킬 표시해주기

	UPROPERTY(meta = (BindWidget))
	class UImage* img_Blur;

	UPROPERTY(meta = (BindWidget))
	class UImage* img_Cover;

	UPROPERTY(meta = (BindWidget))
    class UTextBlock* txt_SkillKey;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* txt_Cooltime;

	UFUNCTION()
	void SetSkillSlotVisuals(ESkillKey skillKey, UObject* InResoueceObject); // 버튼 이미지 세팅 함수

	UFUNCTION()
	void UpdateCoolTime(float leftTime, float totalTime);


private:
	UFUNCTION()
	void UpdateImages_LeftCooltimePercent(float leftPercent);
	
	UFUNCTION()
	void UpdateText_LeftCooltime(float leftTime);
	
	UFUNCTION()
	void SetBlurMaterialScalarParam(FName paramName, float value);

	UFUNCTION()
	void SetCoverMaterialScalarParam(FName paramName, float value);

	FString KeyToString(ESkillKey Key);
};
