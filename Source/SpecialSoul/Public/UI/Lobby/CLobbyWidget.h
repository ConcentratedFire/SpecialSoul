// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CLobbyWidget.generated.h"

/**
 * 
 */
UCLASS()
class SPECIALSOUL_API UCLobbyWidget : public UUserWidget
{
	GENERATED_BODY()

private:
	virtual void NativeConstruct() override;
	
public:
	void LoginProcess();
	
private:
	UPROPERTY(EditDefaultsOnly, meta=(BindWidget))
	class UWidgetSwitcher* WidgetSwitcher;
	UPROPERTY(EditDefaultsOnly, meta=(BindWidget))
	class UCLoginWidget* WBP_Login;
	UPROPERTY(EditDefaultsOnly, meta=(BindWidget))
	class UCMainLobbyWidget* WBP_MainLobby;
};
