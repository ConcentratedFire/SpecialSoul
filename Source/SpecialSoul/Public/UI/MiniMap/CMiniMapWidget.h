// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CMiniMapWidget.generated.h"

/**
 * 
 */
UCLASS()
class SPECIALSOUL_API UCMiniMapWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void SetMiniMapTexture(UTextureRenderTarget2D* TX_MiniMap);

private:
	UPROPERTY(EditDefaultsOnly, meta=(BindWidget))
	class UImage* IMG_MiniMap;
};
