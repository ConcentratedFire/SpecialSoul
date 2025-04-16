// Fill out your copyright notice in the Description page of Project Settings.


#include "Skill/Yasuo/CYasuo_ESkill.h"

#include "SpecialSoul.h"
#include "Components/ArrowComponent.h"
#include "Kismet/KismetSystemLibrary.h"

UCYasuo_ESkill::UCYasuo_ESkill()
{
}

void UCYasuo_ESkill::UseSkill(ACBasePlayer* Caster)
{
	// E 키를 눌렀을때 들어오는 부분
	ACYasuo* Yasuo = Cast<ACYasuo>(Caster);

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
		// 도착 지점에 충돌체가 없으면 사용
		if (Yasuo)
		{
			Yasuo->ActivateSkillMovement(true);
			Yasuo->GetWorld()->GetTimerManager().ClearTimer(CastingTimer);
			Yasuo->GetWorld()->GetTimerManager().ClearTimer(FireTimer);
		}

		// 2. CastingTime후에 스킬을 시작한다.
		TWeakObjectPtr<UCYasuo_ESkill> WeakThis(this); // GC에 의해 댕글링 포인터될 때를 위해 WeakPtr 사용
		TWeakObjectPtr<ACYasuo> WeakYasuo(Yasuo);


		Yasuo->GetWorld()->GetTimerManager().SetTimer(CastingTimer, FTimerDelegate::CreateLambda(
			                                              [WeakThis, WeakYasuo]()
			                                              {
				                                              if (WeakThis.IsValid() && WeakYasuo.IsValid())
				                                              {
					                                              WeakThis->StartUseSkill(WeakYasuo.Get());
				                                              }
			                                              }),
		                                              CastingTime, false, CastingTime);
	}
}

void UCYasuo_ESkill::StartUseSkill(ACYasuo* Yasuo)
{
	TWeakObjectPtr<UCYasuo_ESkill> WeakThis(this); // GC에 의해 댕글링 포인터될 때를 위해 WeakPtr 사용
	TWeakObjectPtr<ACYasuo> WeakYasuo(Yasuo);

	Yasuo->ESkill(true);

	FVector EndPos = Yasuo->GetActorLocation() + Yasuo->GetActorForwardVector() * MoveDist;

	Yasuo->GetWorld()->GetTimerManager().SetTimer(CastingTimer, FTimerDelegate::CreateLambda(
		                                              [WeakThis, WeakYasuo, EndPos]()
		                                              {
			                                              // 이동 끝
			                                              if (!WeakThis.IsValid() || !WeakYasuo.IsValid() || WeakThis.
				                                              Get()->MoveThreshold > FVector::Dist(
					                                              EndPos, WeakYasuo.Get()->GetActorLocation()))
			                                              {
				                                              WeakThis->EndUseSkill(WeakYasuo.Get());
				                                              return;
			                                              }
			                                              FVector curLoc = WeakYasuo.Get()->GetActorLocation();
			                                              FVector newLoc = FMath::VInterpTo(
				                                              curLoc, EndPos,
				                                              WeakYasuo.Get()->GetWorld()->DeltaTimeSeconds, 5);
			                                              WeakYasuo.Get()->SetActorLocation(newLoc);
		                                              }),
	                                              Yasuo->GetWorld()->DeltaTimeSeconds, true, FireDelay
	);
}

void UCYasuo_ESkill::EndUseSkill(ACYasuo* Yasuo)
{
	Yasuo->ESkill(false);;
	Yasuo->GetWorld()->GetTimerManager().ClearTimer(CastingTimer);
	Yasuo->GetWorld()->GetTimerManager().ClearTimer(FireTimer);

	Yasuo->ActivateSkillMovement(false);
}
