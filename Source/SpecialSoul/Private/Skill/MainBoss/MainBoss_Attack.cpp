// Fill out your copyright notice in the Description page of Project Settings.


#include "Skill/MainBoss/MainBoss_Attack.h"

#include "Enemy/MainBoss/MainBoss.h"

void UMainBoss_Attack::UseSkill(ACharacter* Caster)
{
	if (!Caster)
	{
		return;
	}
	
	AMainBoss* mainBoss = Cast<AMainBoss>(Caster);
	if (mainBoss)
	{
		mainBoss->HandleAttack();
	}
}
