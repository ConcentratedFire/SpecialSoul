#include "UI/Lobby/CLoginWidget.h"

#include "Components/Button.h"
#include "Components/EditableText.h"
#include "Components/Image.h"
#include "Game/CGameInstance.h"
#include "UI/Lobby/CLobbyWidget.h"

void UCLoginWidget::NativeConstruct()
{
	Super::NativeConstruct();

	gi = Cast<UCGameInstance>(GetWorld()->GetGameInstance());

	Txt_ID->OnTextChanged.AddDynamic(this, &UCLoginWidget::OnInputID);
	Btn_LogIn->OnClicked.AddDynamic(this, &UCLoginWidget::OnLogin);

	OrgBtnStyle.Normal.TintColor = OrgBgColor;
	OrgBtnStyle.Normal.DrawAs = ESlateBrushDrawType::RoundedBox;
	OrgBtnStyle.Normal.OutlineSettings.RoundingType = ESlateBrushRoundingType::FixedRadius;
	OrgBtnStyle.Normal.OutlineSettings.CornerRadii = FVector4(10, 10, 10, 10);
	OrgBtnStyle.Hovered.TintColor = OrgBgColor;
	OrgBtnStyle.Hovered.DrawAs = ESlateBrushDrawType::RoundedBox;
	OrgBtnStyle.Hovered.OutlineSettings.RoundingType = ESlateBrushRoundingType::FixedRadius;
	OrgBtnStyle.Hovered.OutlineSettings.CornerRadii = FVector4(10, 10, 10, 10);
	OrgBtnStyle.Pressed.TintColor = OrgBgColor;
	OrgBtnStyle.Pressed.DrawAs = ESlateBrushDrawType::RoundedBox;
	OrgBtnStyle.Pressed.OutlineSettings.RoundingType = ESlateBrushRoundingType::FixedRadius;
	OrgBtnStyle.Pressed.OutlineSettings.CornerRadii = FVector4(10, 10, 10, 10);

	ChangeBtnStyle.Normal.TintColor = ChangeBgColor;
	ChangeBtnStyle.Normal.DrawAs = ESlateBrushDrawType::RoundedBox;
	ChangeBtnStyle.Normal.OutlineSettings.RoundingType = ESlateBrushRoundingType::FixedRadius;
	ChangeBtnStyle.Normal.OutlineSettings.CornerRadii = FVector4(10, 10, 10, 10);
	ChangeBtnStyle.Hovered.TintColor = ChangeBgHoverColor;
	ChangeBtnStyle.Hovered.DrawAs = ESlateBrushDrawType::RoundedBox;
	ChangeBtnStyle.Hovered.OutlineSettings.RoundingType = ESlateBrushRoundingType::FixedRadius;
	ChangeBtnStyle.Hovered.OutlineSettings.CornerRadii = FVector4(10, 10, 10, 10);
	ChangeBtnStyle.Pressed.TintColor = ChangeBgHoverColor;
	ChangeBtnStyle.Pressed.DrawAs = ESlateBrushDrawType::RoundedBox;
	ChangeBtnStyle.Pressed.OutlineSettings.RoundingType = ESlateBrushRoundingType::FixedRadius;
	ChangeBtnStyle.Pressed.OutlineSettings.CornerRadii = FVector4(10, 10, 10, 10);

	Btn_LogIn->SetStyle(OrgBtnStyle);
	Img_Login->SetBrushTintColor(OrgImgColor);
}

void UCLoginWidget::OnInputID(const FText& Text)
{
	if (Text.IsEmpty())
	{
		Btn_LogIn->SetStyle(OrgBtnStyle);
		Img_Login->SetBrushTintColor(OrgImgColor);
	}
	else
	{
		Btn_LogIn->SetStyle(ChangeBtnStyle);
		Img_Login->SetBrushTintColor(ChangeImgColor);
	}
}

void UCLoginWidget::OnLogin()
{
	if (Txt_ID->GetText().IsEmpty()) return;

	gi->MySessionName = Txt_ID->GetText().ToString();
	LobbyWidget->LoginProcess();
}
