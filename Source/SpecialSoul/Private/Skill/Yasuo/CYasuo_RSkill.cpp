// Fill out your copyright notice in the Description page of Project Settings.


#include "Skill/Yasuo/CYasuo_RSkill.h"

#include "Player/CYasuo.h"

UCYasuo_RSkill::UCYasuo_RSkill()
{
	Yasuo = Cast<ACYasuo>(GetOuter());
}

void UCYasuo_RSkill::UseSkill(ACBasePlayer* Caster)
{
	// R 키를 눌렀을때 들어오는 부분
	if (!bCanSkillActive) return;	

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

	float CalcChargeCooldown = Yasuo->CalcHaste(ChargeCooldown);
	Yasuo->GetWorld()->GetTimerManager().SetTimer(ChargeTimerHandle, this, &UCYasuo_RSkill::OnChargeCompleted,
											   CalcChargeCooldown, false);
}

void UCYasuo_RSkill::OnChargeCompleted()
{
	Yasuo->GetWorld()->GetTimerManager().ClearTimer(ChargeTimerHandle);
	bCanSkillActive = true;
}
