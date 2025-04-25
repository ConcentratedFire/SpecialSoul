// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CStandbyWidget.generated.h"

/**
 * 
 */
UCLASS()
class SPECIALSOUL_API UCStandbyWidget : public UUserWidget
{
	GENERATED_BODY()

private:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

private:
	UPROPERTY(EditDefaultsOnly, meta=(BindWidget))
	class UTextBlock* Txt_RoomName;
	UPROPERTY(EditDefaultsOnly, meta=(BindWidget))
	class UTextBlock* Txt_PlayerReadyState;
	UPROPERTY(EditDefaultsOnly, meta=(BindWidget))
	class UButton* Btn_Exit;
	UPROPERTY(EditDefaultsOnly, meta=(BindWidget))
	class UButton* Btn_Ready;
	UPROPERTY(EditDefaultsOnly, meta=(BindWidget))
	class UButton* Btn_Champion;

	UPROPERTY(EditDefaultsOnly, meta=(BindWidget))
	class UCanvasPanel* Canvas_Pick;
	UPROPERTY(EditDefaultsOnly, meta=(BindWidget))
	class UButton* Btn_Yasuo;
	UPROPERTY(EditDefaultsOnly, meta=(BindWidget))
	class UButton* Btn_Jinx;

	UFUNCTION()
	void Btn_ExitClick();
	UFUNCTION()
	void Btn_ReadyClick();
	UFUNCTION()
	void Btn_ChampionClick();

	UFUNCTION()
	void Btn_YasuoPick();
	UFUNCTION()
	void Btn_JinxPick();

private:
	bool bShowChampion = false;
	FString MemberName;

	UPROPERTY()
	class ACPlayerController* PC;
};
