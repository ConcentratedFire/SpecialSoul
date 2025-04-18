// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CTimeWidget.generated.h"

/**
 * 
 */
UCLASS()
class SPECIALSOUL_API UCTimeWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void SetTime(float Time);
	
private:
	UPROPERTY(EditDefaultsOnly, meta=(BindWidget))
	class UTextBlock* TxtTime;
};
