// Fill out your copyright notice in the Description page of Project Settings.


#include "Skill/Yasuo/CYasuo_RSkill.h"

#include "Player/CYasuo.h"
#include "Player/Components/SkillComponent.h"

UCYasuo_RSkill::UCYasuo_RSkill()
{
	Yasuo = Cast<ACYasuo>(GetOuter());
}

void UCYasuo_RSkill::UseSkill(ACharacter* Caster)
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
	EndUseSkill();
	
	float CalcChargeCooldown = Yasuo->CalcHaste(ChargeCooldown);
	Yasuo->GetWorld()->GetTimerManager().SetTimer(ChargeTimerHandle, this, &UCYasuo_RSkill::OnChargeCompleted,
											   CalcChargeCooldown, false);
}

void UCYasuo_RSkill::OnChargeCompleted()
{
	Yasuo->GetWorld()->GetTimerManager().ClearTimer(ChargeTimerHandle);
	bCanSkillActive = true;
}

void UCYasuo_RSkill::EndUseSkill()
{
	Yasuo->ResetLeftCooltime(ESkillKey::R);
	Yasuo->SkillComponent->CoolTimeMap[ESkillKey::R].TotalCooltime = Yasuo->CalcHaste(ChargeCooldown);
}
