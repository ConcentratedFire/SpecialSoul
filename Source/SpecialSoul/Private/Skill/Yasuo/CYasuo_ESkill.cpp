// Fill out your copyright notice in the Description page of Project Settings.


#include "Skill/Yasuo/CYasuo_ESkill.h"

#include "SpecialSoul.h"
#include "Components/ArrowComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Player/Components/SkillComponent.h"

UCYasuo_ESkill::UCYasuo_ESkill()
{
	Yasuo = Cast<ACYasuo>(GetOuter());
}

void UCYasuo_ESkill::UseSkill(ACharacter* Caster)
{
	// E 키를 눌렀을때 들어오는 부분
	if (SkillChargeCount == 0 || bIsESkillActive) return;

	// 야스오의 전방 100cm 앞에 벽이 있는지 확인
	TArray<FHitResult> HitResults;
	FTransform Transform = Yasuo->GetArrowComponent()->GetComponentTransform();
	FVector Start = Yasuo->GetActorLocation() + Yasuo->GetActorForwardVector() * MoveDist;
	FVector End = Start;
	TArray<TEnumAsByte<EObjectTypeQuery>> objectTypes;
	objectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_GameTraceChannel5));
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(Yasuo);

	bool bHit = UKismetSystemLibrary::SphereTraceMultiForObjects(Yasuo->GetWorld(), Start, End, 50, objectTypes, false,
	                                                             ActorsToIgnore,
	                                                             EDrawDebugTrace::ForDuration,
	                                                             HitResults,
	                                                             true);

	if (!bHit)
	{
		Yasuo->SetSkillUsing(ESkillKey::E, true);
		// 도착 지점에 충돌체가 없으면 사용
		if (Yasuo)
		{
			Yasuo->ActivateSkillMovement(true);
			Yasuo->GetWorld()->GetTimerManager().ClearTimer(FireTimer);

			DashTime = 0.f;
			--SkillChargeCount;
			Yasuo->CRPC_UpdateChargeCountUI(ESkillKey::E, SkillChargeCount);
			if (SkillChargeCount == 0)
				Yasuo->CRPC_SetSkillChargingUI(ESkillKey::E, false);
			bIsESkillActive = true;
		}

		// 스킬 시전
		StartUseSkill();
	}
}

void UCYasuo_ESkill::StartUseSkill()
{
	TWeakObjectPtr<UCYasuo_ESkill> WeakThis(this); // GC에 의해 댕글링 포인터될 때를 위해 WeakPtr 사용
	TWeakObjectPtr<ACYasuo> WeakYasuo(Yasuo);

	Yasuo->ESkill(true);

	FVector EndPos = Yasuo->GetActorLocation() + Yasuo->GetActorForwardVector() * MoveDist;

	Yasuo->GetWorld()->GetTimerManager().SetTimer(FireTimer, FTimerDelegate::CreateLambda(
		                                              [WeakThis, WeakYasuo, EndPos]()
		                                              {
			                                              // 이동 끝
			                                              if (!WeakThis.IsValid() || !WeakYasuo.IsValid() || WeakThis.
				                                              Get()->MoveThreshold > FVector::Dist(
					                                              EndPos, WeakYasuo.Get()->GetActorLocation()))
			                                              {
				                                              WeakThis->EndUseSkill();
				                                              return;
			                                              }
			                                              if (WeakThis->DashTime >= 1.f)
			                                              {
				                                              WeakThis->EndUseSkill();
				                                              return;
			                                              }

			                                              WeakThis->DashTime += WeakYasuo->GetWorldTimerManager().
				                                              GetTimerElapsed(WeakThis->FireTimer);

			                                              WeakThis.Get()->Dash(EndPos);
		                                              }),
	                                              Yasuo->GetWorld()->DeltaTimeSeconds, true, FireDelay
	);
}

void UCYasuo_ESkill::Dash(FVector EndPos)
{
	FVector curLoc = Yasuo->GetActorLocation();
	FVector newLoc = FMath::VInterpTo(curLoc, EndPos, Yasuo->GetWorld()->DeltaTimeSeconds, 10);
	Yasuo->SetActorLocation(newLoc);
}

void UCYasuo_ESkill::EndUseSkill()
{
	Yasuo->ESkill(false);
	Yasuo->GetWorld()->GetTimerManager().ClearTimer(FireTimer);

	bIsESkillActive = false; // 스킬 사용 완료
	StartChargeTimer();
}

void UCYasuo_ESkill::StartChargeTimer()
{
	// 누적 개수가 최대치 미만일 때만 충전
	if (SkillChargeCount < MaxChargeCount && !Yasuo->GetWorld()->GetTimerManager().IsTimerActive(ChargeTimerHandle))
	{
		float CalcChargeCooldown = Yasuo->CalcHaste(ChargeCooldown);
		Yasuo->GetWorld()->GetTimerManager().SetTimer(ChargeTimerHandle, this, &UCYasuo_ESkill::OnChargeCompleted,
		                                              CalcChargeCooldown, false);

		// UI
		Yasuo->SkillComponent->CoolTimeMap[ESkillKey::E].TotalCooltime = CalcChargeCooldown;
		Yasuo->ResetLeftCooltime(ESkillKey::E);
		
		LOG_S(Warning, TEXT("%d"), SkillChargeCount);
		if (SkillChargeCount == 0) // 0개면 blur와 cover이미지를 사용
		{
			Yasuo->CRPC_SetSkillChargingUI(ESkillKey::E, false, Yasuo->PC);
		}
		else // 1, 2개면 Charge이미지만 사용
		{
			Yasuo->CRPC_SetSkillChargingUI(ESkillKey::E, true, Yasuo->PC);
		}
	}
}

void UCYasuo_ESkill::OnChargeCompleted()
{
	Yasuo->GetWorld()->GetTimerManager().ClearTimer(ChargeTimerHandle);
	Yasuo->CRPC_SetSkillChargingUI(ESkillKey::E, false, Yasuo->PC);
	
	if (SkillChargeCount < MaxChargeCount)
	{
		++SkillChargeCount; // 누적 개수 증가
		
		Yasuo->SkillComponent->UpdateChargedCount(ESkillKey::E, SkillChargeCount);
		// 추가 충전이 필요하면 타이머 재시작
		StartChargeTimer();
	}
}
