// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/CSelectUpgradeWidget.h"

#include "Components/HorizontalBox.h"
#include "Game/CPlayerState.h"
#include "Player/CYasuo.h"
#include "UI/CUpgradeWidget.h"

void UCSelectUpgradeWidget::SetCardData(TArray<FString> Data)
{
	ACBasePlayer* Player = Cast<ACBasePlayer>(GetWorld()->GetFirstPlayerController()->GetPawn());
	int32 WeaponGrade, DamageGrade, AbilityHasteGrade, ProjectilesGrade, CritChanceGrade;
	Player->GetCurGrade(WeaponGrade, DamageGrade, AbilityHasteGrade, ProjectilesGrade, CritChanceGrade);

	for (auto& data : Data)
	{
		UCUpgradeWidget* cardWidget = Cast<UCUpgradeWidget>(CreateWidget(GetWorld(), UpgradeWidgetFactory));

		cardWidget->CardName = data;
		if (data == "Weapon")
		{
			int32 curDamage, nextDamage;
			FString strLevel, strTitle, strDesc, strStat;

			cardWidget->SetBg(UIDataArray[0].TypeBG["Skill"]);

			if (Player->IsA(ACYasuo::StaticClass()))
			{
				Player->GetCurDamageNextDamage(WeaponGrade, curDamage, nextDamage, true);
				// 다음 업글 단계가 Final인지 확인
				if (Player->IsNextWeaponFinal(true))
				{
					strLevel = FString::Printf(TEXT("진화"));
					strTitle = FString::Printf(TEXT("떠도는 폭풍"));
					strDesc = FString::Printf(TEXT(
						"야스오가 기력을 연마해 수는\n적지만 훨씬 큰 소용돌이를\n방출합니다. 소용돌이는 포물선을\n그리며 날아가며 목표 지점에\n피해를 입히는 폭풍을 남깁니다."));
					cardWidget->SetTarget(strLevel, UIDataArray[0].UpgradeTextures["YasuoFinal"], strTitle, strDesc,
					                      strStat);
				}
				else
				{
					strLevel = FString::Printf(TEXT("%d레벨"), WeaponGrade + 1);
					strTitle = FString::Printf(TEXT("강철 폭풍"));
					strDesc = FString::Printf(TEXT("피해량"));
					strStat = FString::Printf(TEXT("%d > %d"), curDamage, nextDamage);
					cardWidget->SetTarget(strLevel, UIDataArray[0].UpgradeTextures["YasuoNormal"], strTitle,
					                      strDesc, strStat);
				}
			}
			else
			{
				int32 curProjectile, nextProjectile;
				Player->GetCurDamageNextDamage(WeaponGrade, curDamage, nextDamage, false);
				Player->GetCurProjectileNextProjectile(WeaponGrade, curProjectile, nextProjectile);
				// 다음 업글 단계가 Final인지 확인
				if (Player->IsNextWeaponFinal(false))
				{
					strLevel = FString::Printf(TEXT("진화"));
					strTitle = FString::Printf(TEXT("전투 고양이 총알\n세례"));
					strDesc = FString::Printf(
						TEXT("총알이 대상을 관통합니다. 처음\n적중한 ㅏ이후에는 피해량이\n감소합니다. 재사용 대기시간이\n크게 감소합니다."));
					cardWidget->SetTarget(strLevel, UIDataArray[0].UpgradeTextures["JinxFinal"], strTitle,
					                      strDesc, "");
				}
				else
				{
					strLevel = FString::Printf(TEXT("%d레벨"), WeaponGrade + 1);
					strTitle = FString::Printf(TEXT("야옹 야옹"));
					strDesc = FString::Printf(TEXT("피해량"));
					strStat = FString::Printf(TEXT("%d > %d"), curDamage, nextDamage);
					FString strDesc2 = FString::Printf(TEXT("투사체"));
					FString strStat2 = FString::Printf(TEXT("%d > %d"), curProjectile, nextProjectile);
					cardWidget->SetTarget(strLevel, UIDataArray[0].UpgradeTextures["JinxNormal"], strTitle,
					                      strDesc, strStat, strDesc2, strStat2);
				}
			}
		}
		else
		{
			cardWidget->SetBg(UIDataArray[0].TypeBG["Stat"]);

			FString strDesc, strUpgradeStat, strTitle;
			strUpgradeStat = Player->GetUpgradeData(data, strDesc, strTitle);
			int32 nextLevel = 0;
			UTexture* upgradeTexture;
			if (data == "Damage")
			{
				nextLevel = DamageGrade + 1;
				upgradeTexture = UIDataArray[0].UpgradeTextures["Damage"];
			}
			else if (data == "AbilityHaste")
			{
				nextLevel = AbilityHasteGrade + 1;
				upgradeTexture = UIDataArray[0].UpgradeTextures["CoolTime"];
			}
			else if (data == "Projectiles")
			{
				nextLevel = ProjectilesGrade + 1;
				upgradeTexture = UIDataArray[0].UpgradeTextures["ProjectileCount"];
			}
			else if (data == "CritChance")
			{
				nextLevel = CritChanceGrade + 1;
				upgradeTexture = UIDataArray[0].UpgradeTextures["Critical"];
			}

			FString strLevel = FString::Printf(TEXT("%d레벨"), nextLevel);

			cardWidget->SetTarget(strLevel, upgradeTexture, strTitle, strDesc,
			                      strUpgradeStat);
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
