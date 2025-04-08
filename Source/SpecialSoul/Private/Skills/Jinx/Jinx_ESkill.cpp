// Fill out your copyright notice in the Description page of Project Settings.


#include "Skills/Jinx/Jinx_ESkill.h"

void UJinx_ESkill::UseSkill(AActor* Caster)
{
	// 시전시간 동안 Progressbar UI를 띄우고

	// 시전가능해지면 

	// Caster Transform에 맞춰서 스킬을 발사한다
	FVector SpawnLoc = Caster->GetActorLocation() + Caster->GetActorForwardVector() * 50.f;
	
	
}
