// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Standby/CStandbyWidget.h"

#include "Components/Button.h"
#include "Components/CanvasPanel.h"

void UCStandbyWidget::NativeConstruct()
{
	Super::NativeConstruct();

	Canvas_Pick->SetVisibility(ESlateVisibility::Hidden);

	Btn_Exit->OnClicked.AddDynamic(this, &UCStandbyWidget::Btn_ExitClick);
	Btn_Ready->OnClicked.AddDynamic(this, &UCStandbyWidget::Btn_ReadyClick);
	Btn_Champion->OnClicked.AddDynamic(this, &UCStandbyWidget::Btn_ChampionClick);
	Btn_Yasuo->OnClicked.AddDynamic(this, &UCStandbyWidget::Btn_YasuoPick);
	Btn_Jinx->OnClicked.AddDynamic(this, &UCStandbyWidget::Btn_JinxPick);

	bHasAuthority = GetWorld()->GetFirstPlayerController()->HasAuthority();
	if (bHasAuthority)
	{
		FButtonStyle Style;
		Style.Normal.SetResourceObject(ReadyImage[0]);
		Style.Normal.SetImageSize(FVector2D(252, 66));
		Style.Normal.DrawAs = ESlateBrushDrawType::Image;
		Style.Hovered.SetResourceObject(ReadyImage[1]);
		Style.Hovered.SetImageSize(FVector2D(252, 66));
		Style.Hovered.DrawAs = ESlateBrushDrawType::Image;
		Style.Pressed.SetResourceObject(ReadyImage[2]);
		Style.Pressed.SetImageSize(FVector2D(252, 66));
		Style.Pressed.DrawAs = ESlateBrushDrawType::Image;
		
		Btn_Ready->SetStyle(Style);
	}
}

void UCStandbyWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}

void UCStandbyWidget::Btn_ExitClick()
{
}

void UCStandbyWidget::Btn_ReadyClick()
{
	if (!bHasAuthority) return;
		
}

void UCStandbyWidget::Btn_ChampionClick()
{
	bShowChampion = !bShowChampion;
	Canvas_Pick->SetVisibility(bShowChampion ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Hidden);
}

void UCStandbyWidget::Btn_YasuoPick()
{
}

void UCStandbyWidget::Btn_JinxPick()
{
}
