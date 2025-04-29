// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CFailEndingWidget.generated.h"

/**
 * 
 */
UCLASS()
class SPECIALSOUL_API UCFailEndingWidget : public UUserWidget
{
	GENERATED_BODY()

private:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	
private:
	UPROPERTY(meta = (BindWidget))
	class UButton* Btn_Exit;
	UPROPERTY(meta=(BindWidgetAnim), Transient)
	class UWidgetAnimation* DeadAnim;

	UFUNCTION()
	void OnExitClicked();

private:
	bool bIsPlayDeadAnim;
};
