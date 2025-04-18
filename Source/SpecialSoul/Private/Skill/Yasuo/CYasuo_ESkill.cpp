// Fill out your copyright notice in the Description page of Project Settings.


#include "Skill/Yasuo/CYasuo_ESkill.h"

#include "SpecialSoul.h"
#include "Components/ArrowComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Net/UnrealNetwork.h"

UCYasuo_ESkill::UCYasuo_ESkill()
{
}

void UCYasuo_ESkill::UseSkill(ACBasePlayer* Caster)
{
	// E 키를 눌렀을때 들어오는 부분
	if (SkillChargeCount == 0 || bIsESkillActive) return;

	Yasuo = Cast<ACYasuo>(Caster);

	CRPC_CheckCanSkill();	
}

void UCYasuo_ESkill::StartUseSkill()	// 서버에서 호출했기 때문에 서버 전용 함수로 바뀜
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

			                                              FVector curLoc = WeakYasuo.Get()->GetActorLocation();
			                                              FVector newLoc = FMath::VInterpTo(
				                                              curLoc, EndPos,
				                                              WeakYasuo.Get()->GetWorld()->DeltaTimeSeconds, 5);
			                                              WeakYasuo.Get()->SetActorLocation(newLoc);
		                                              }),
	                                              Yasuo->GetWorld()->DeltaTimeSeconds, true, FireDelay
	);
}

void UCYasuo_ESkill::EndUseSkill()
{
	
	Yasuo->ESkill(false);;
	Yasuo->GetWorld()->GetTimerManager().ClearTimer(FireTimer);
	
	bIsESkillActive = false; // 스킬 사용 완료
	// StartChargeTimer();
}

void UCYasuo_ESkill::StartChargeTimer()
{
	// 누적 개수가 최대치 미만일 때만 충전
	if (SkillChargeCount < MaxChargeCount && !Yasuo->GetWorld()->GetTimerManager().IsTimerActive(ChargeTimerHandle))
	{
		float CalcChargeCooldown = Yasuo->CalcHaste(ChargeCooldown);
		Yasuo->GetWorld()->GetTimerManager().SetTimer(ChargeTimerHandle, this, &UCYasuo_ESkill::OnChargeCompleted,
		                                              CalcChargeCooldown, false);
	}
}

void UCYasuo_ESkill::OnChargeCompleted()
{
	Yasuo->GetWorld()->GetTimerManager().ClearTimer(ChargeTimerHandle);

	if (SkillChargeCount < MaxChargeCount)
	{
		SkillChargeCount++; // 누적 개수 증가
		// 추가 충전이 필요하면 타이머 재시작
		StartChargeTimer();
	}
}

void UCYasuo_ESkill::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	UObject::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UCYasuo_ESkill, SkillChargeCount);
	DOREPLIFETIME(UCYasuo_ESkill, bIsESkillActive);
}

void UCYasuo_ESkill::CRPC_CheckCanSkill_Implementation()
{
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
	// 도착 지점에 충돌체가 없으면 사용
	if (!bHit)
	{
		SRPC_UseSkill();
	}
}

void UCYasuo_ESkill::SRPC_UseSkill_Implementation()
{
	Yasuo->SetSkillUsing(ESkillKey::E, true);
	Yasuo->ActivateSkillMovement(true);
	Yasuo->GetWorld()->GetTimerManager().ClearTimer(FireTimer);
			
	DashTime=0.f;
	--SkillChargeCount;
	bIsESkillActive = true;

	StartUseSkill();
}