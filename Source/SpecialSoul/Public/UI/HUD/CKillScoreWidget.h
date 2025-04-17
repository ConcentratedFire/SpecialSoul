// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CKillScoreWidget.generated.h"

/**
 * 
 */
UCLASS()
class SPECIALSOUL_API UCKillScoreWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void SetKillScore(int32 KillScore);
	
private:
	UPROPERTY(EditDefaultsOnly, meta=(BindWidget))
	class UTextBlock* TxtKillScore;
};
