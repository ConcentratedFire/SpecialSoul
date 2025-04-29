// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameEndingWidget.generated.h"

/**
 * 
 */
UCLASS()
class SPECIALSOUL_API UGameEndingWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

	UPROPERTY(meta = (BindWidget))
	class UButton* Btn_Exit;
	
	UFUNCTION()
	void OnExitClicked();
};
