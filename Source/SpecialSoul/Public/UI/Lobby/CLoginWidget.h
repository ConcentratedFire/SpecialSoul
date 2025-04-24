// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CLoginWidget.generated.h"

/**
 * 
 */
UCLASS()
class SPECIALSOUL_API UCLoginWidget : public UUserWidget
{
	GENERATED_BODY()

private:
	virtual void NativeConstruct() override;

public:
	void SetLobbyWidget(class UCLobbyWidget* InLobbyWidget) { LobbyWidget = InLobbyWidget; }

	UFUNCTION()
	void OnLogin();
	UFUNCTION()
	void OnInputID(const FText& Text);

private:
	UPROPERTY(EditDefaultsOnly, meta=(BindWidget))
	class UEditableText* Txt_ID;
	UPROPERTY(EditDefaultsOnly, meta=(BindWidget))
	class UButton* Btn_LogIn;
	UPROPERTY(EditDefaultsOnly, meta=(BindWidget))
	class UImage* Img_Login;

	FLinearColor OrgBgColor = FLinearColor(.5, .5, .5, 1);
	FLinearColor OrgImgColor = FLinearColor(.1, .1, .1, 1);
	FLinearColor ChangeBgColor = FLinearColor(1, 0, 0, 1);
	FLinearColor ChangeBgHoverColor = FLinearColor(.7, 0, 0, 1);
	FLinearColor ChangeImgColor = FLinearColor(1, 1, 1, 1);

	FButtonStyle OrgBtnStyle;
	FButtonStyle ChangeBtnStyle;

private:
	UPROPERTY()
	class UCGameInstance* gi;
	UPROPERTY()
	class UCLobbyWidget* LobbyWidget;
};
