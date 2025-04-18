// Fill out your copyright notice in the Description page of Project Settings.


#include "Skill/Yasuo/CYasuo_RSkill.h"

#include "Player/CPlayerController.h"
#include "Player/CYasuo.h"

void UCYasuo_RSkill::UseSkill(ACBasePlayer* Caster)
{
	if (!bCanSkillActive) return;

	// E 키를 눌렀을때 들어오는 부분
	Yasuo = Cast<ACYasuo>(Caster);

	if (Yasuo)
	{
		Yasuo->ActivateSkillMovement(true);
		Yasuo->GetWorld()->GetTimerManager().ClearTimer(FireTimer);

		bCanSkillActive = false;
	}

	StartUseSkill();
}

void UCYasuo_RSkill::StartUseSkill()
{
	Yasuo->SetSkillUsing(ESkillKey::R, true);
	Yasuo->RSkill();

	float CalcChargeCooldown = Yasuo->PC->CalcHaste(ChargeCooldown);
	Yasuo->GetWorld()->GetTimerManager().SetTimer(ChargeTimerHandle, this, &UCYasuo_RSkill::OnChargeCompleted,
											   CalcChargeCooldown, false);
}

void UCYasuo_RSkill::OnChargeCompleted()
{
	Yasuo->GetWorld()->GetTimerManager().ClearTimer(ChargeTimerHandle);
	bCanSkillActive = true;
}
