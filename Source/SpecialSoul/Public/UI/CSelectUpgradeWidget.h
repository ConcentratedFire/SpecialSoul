// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CSelectUpgradeWidget.generated.h"

USTRUCT(BlueprintType)
struct FUIData // 야스오 이동거리 기류 획득 데이터
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, category = "Widget")
	TMap<FString, class UTexture*> TypeBG;
	UPROPERTY(EditDefaultsOnly, category = "Widget")
	TMap<FString, class UTexture*> UpgradeTextures;

	FUIData()
	{
	};
};

/**
 * 
 */
UCLASS()
class SPECIALSOUL_API UCSelectUpgradeWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void SetCardData(TArray<FString> Data, const TArray<struct FCardStruct>& CardData);
	void ClearCardData();

private:
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	class UHorizontalBox* HorizonCard;

	UPROPERTY(EditDefaultsOnly, category = "Widget")
	TSubclassOf<UUserWidget> UpgradeWidgetFactory;

	UPROPERTY(EditAnywhere, Category = "Widget", Replicated)
	TArray<FUIData> UIDataArray;

private:
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
};
