// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/CSelectUpgradeWidget.h"
#include "Components/HorizontalBox.h"
#include "Game/CPlayerState.h"
#include "Net/UnrealNetwork.h"
#include "Player/CYasuo.h"
#include "UI/CUpgradeWidget.h"
#include "Data/CYasuoData.h"

void UCSelectUpgradeWidget::SetCardData(TArray<FString> Data, const TArray<FCardStruct>& CardData)
{
	for (int i = 0; i < Data.Num(); ++i)
	{
		UCUpgradeWidget* cardWidget = Cast<UCUpgradeWidget>(CreateWidget(GetWorld(), UpgradeWidgetFactory));

		cardWidget->CardName = Data[i];
		if (Data[i] == "Weapon")
		{
			cardWidget->SetBg(UIDataArray[0].TypeBG["Skill"]);

			if (CardData[i].bIsYasuo)
			{
				// 다음 업글 단계가 Final인지 확인
				if (CardData[i].bIsNextWeaponFinal)
				{
					cardWidget->SetTarget(CardData[i].strLevel, UIDataArray[0].UpgradeTextures["YasuoFinal"],
					                      CardData[i].strTitle, CardData[i].strDesc,
					                      CardData[i].strStat);
				}
				else
				{
					cardWidget->SetTarget(CardData[i].strLevel, UIDataArray[0].UpgradeTextures["YasuoNormal"],
					                      CardData[i].strTitle,
					                      CardData[i].strDesc, CardData[i].strStat, CardData[i].strDesc2,
					                      CardData[i].strStat2);
				}
			}
			else
			{
				// 다음 업글 단계가 Final인지 확인
				if (CardData[i].bIsNextWeaponFinal)
				{
					cardWidget->SetTarget(CardData[i].strLevel, UIDataArray[0].UpgradeTextures["JinxFinal"],
					                      CardData[i].strTitle,
					                      CardData[i].strDesc, "");
				}
				else
				{
					cardWidget->SetTarget(CardData[i].strLevel, UIDataArray[0].UpgradeTextures["JinxNormal"],
					                      CardData[i].strTitle,
					                      CardData[i].strDesc, CardData[i].strStat, CardData[i].strDesc2,
					                      CardData[i].strStat2);
				}
			}
		}
		else
		{
			cardWidget->SetBg(UIDataArray[0].TypeBG["Stat"]);

			UTexture* upgradeTexture;
			if (Data[i] == "Damage")
			{
				upgradeTexture = UIDataArray[0].UpgradeTextures["Damage"];
			}
			else if (Data[i] == "AbilityHaste")
			{
				upgradeTexture = UIDataArray[0].UpgradeTextures["CoolTime"];
			}
			else if (Data[i] == "Projectiles")
			{
				upgradeTexture = UIDataArray[0].UpgradeTextures["ProjectileCount"];
			}
			else if (Data[i] == "CritChance")
			{
				upgradeTexture = UIDataArray[0].UpgradeTextures["Critical"];
			}

			cardWidget->SetTarget(CardData[i].strLevel, upgradeTexture, CardData[i].strTitle, CardData[i].strDesc,
			                      CardData[i].strUpgradeStat);
		}

		// Horizontal Box에 넣음
		HorizonCard->AddChild(cardWidget);
		// 좌우 패딩 10씩 설정
		cardWidget->SetPadding(FMargin(10.0f, 0.0f, 10.0f, 0.0f));
	}
}

void UCSelectUpgradeWidget::ClearCardData()
{
	HorizonCard->ClearChildren();
	SetVisibility(ESlateVisibility::Hidden);
}

void UCSelectUpgradeWidget::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UCSelectUpgradeWidget, UIDataArray);
}
