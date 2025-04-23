// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/MiniMap/CMiniMapWidget.h"

#include "Components/Image.h"
#include "Engine/TextureRenderTarget2D.h"

void UCMiniMapWidget::SetMiniMapTexture(UTextureRenderTarget2D* TX_MiniMap)
{
	FSlateBrush NewBrush;
	NewBrush.SetResourceObject(TX_MiniMap); // RenderTarget을 브러시로 설정
	NewBrush.ImageSize = FVector2D(1920, 1080); // 이미지 크기 설정
	IMG_MiniMap->SetBrush(NewBrush); // UImage에 브러시 적용
}
