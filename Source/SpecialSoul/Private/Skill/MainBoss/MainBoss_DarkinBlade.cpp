// Fill out your copyright notice in the Description page of Project Settings.


#include "Skill/MainBoss/MainBoss_DarkinBlade.h"

#include "Enemy/MainBoss/MainBoss.h"

void UMainBoss_DarkinBlade::UseSkill(ACharacter* Caster)
{
	UE_LOG(LogTemp, Warning, TEXT("UMainBoss_DarkinBlade::UseSkill"));

	
	if (!Caster)
	{
		return;
	}
	
	AMainBoss* mainBoss = Cast<AMainBoss>(Caster);
	if (mainBoss)
	{
		mainBoss->SRPC_PlayDarkinBladeMontage(0.7f, *FString::Printf(TEXT("Attack%d"), ComboCount));
		mainBoss->Damage = Damages[ComboCount]; 
		//UE_LOG(LogTemp, Warning,  TEXT("Attack%d"), ComboCount);

		ComboCount = (ComboCount+1) % 3;
		if (ComboCount == 0)
		{
			mainBoss->ResetLeftCooltime_DarkinBlade();
		}
	}
}
