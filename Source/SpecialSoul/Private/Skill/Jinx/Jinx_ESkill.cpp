// Fill out your copyright notice in the Description page of Project Settings.


#include "Skill/Jinx/Jinx_ESkill.h"

#include "Player/CBasePlayer.h"

void UJinx_ESkill::UseSkill(ACBasePlayer* Caster)
{
	// 시전시간 동안 Progressbar UI를 띄우고

	// 시전가능해지면 

	// Caster Transform에 맞춰서 스킬을 발사한다
	FVector SpawnLoc = Caster->GetActorLocation() + Caster->GetActorForwardVector() * 50.f;
	
	
}
