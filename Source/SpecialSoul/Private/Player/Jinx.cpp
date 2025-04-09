// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/Jinx.h"

#include "Components/CapsuleComponent.h"
#include "Player/Anim/JinxAnim.h"
#include "Skill/Jinx/Jinx_Attack.h"
#include "Skill/Jinx/Jinx_ESkill.h"
#include "Skill/Jinx/Jinx_Passive.h"
#include "Skill/Jinx/Jinx_RSkill.h"
#include "Utility/CDataSheetUtility.h"

AJinx::AJinx()
{
	// 스켈레탈 메시 세팅
	ConstructorHelpers::FObjectFinder<USkeletalMesh> TempSkMesh(TEXT("/Script/Engine.SkeletalMesh'/Game/Asset/Jinx/jinx__tft_set_13_.jinx__tft_set_13_'"));
	if (TempSkMesh.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(TempSkMesh.Object);
	}
	GetMesh()->SetRelativeLocation(FVector(0.f, 0.f, -68.f));
	GetMesh()->SetRelativeRotation(FRotator(0.f, -90.f, 0.f));
	GetMesh()->SetCollisionProfileName(TEXT("Player"));

	GetCapsuleComponent()->SetCapsuleHalfHeight(68.f);
	GetCapsuleComponent()->SetCapsuleRadius(28.f);
}

void AJinx::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
void AJinx::BeginPlay()
{
	Super::BeginPlay();

	Anim = Cast<UJinxAnim>(GetMesh()->GetAnimInstance());

	BindSkill(ESkillKey::Attack, NewObject<UJinx_Attack>());
	BindSkill(ESkillKey::Passive, NewObject<UJinx_Passive>());
	BindSkill(ESkillKey::E, NewObject<UJinx_ESkill>());
	BindSkill(ESkillKey::R, NewObject<UJinx_RSkill>());
	
	 // TODO Init Data Settings
	 // 캐릭터를 선택하면 GameMode에서 해당 캐릭터의 정보를 읽고
	 // Player의 BeginPlay에서 초기 데이터를 세팅해주도록 변경
	DataSheetUtility = NewObject<UCDataSheetUtility>(this); // Outer인 this를 기준으로 객체 생명주기를 관리 (Outer가 죽을 때 자동으로 GC 됨)
	DataSheetUtility->FetchGoogleSheetData<FJinxAttackData>("Jinx", "A1", "G8", AttackDataMap);
	
	DataSheetUtility->OnDataFetched.AddDynamic(this, &AJinx::PrintAttackDataMap);
}

// 플레이어의 키 입력에 따른 스킬 캐스팅
void AJinx::CastSkill(ESkillKey Key)
{
	if (!SkillMap.Contains(Key))
	{
		UE_LOG(LogTemp, Warning, TEXT("CastSkill) Skill does not exist"));
		return;
	}
	
	SkillMap[Key]->UseSkill(this); // 캐릭터(this)를 넣어줌으로써, 스킬에서 캐릭터의 데이터를 사용할 수 있다
}

void AJinx::PrintAttackDataMap()
{
	for (const auto& Pair : AttackDataMap)
	{
		UE_LOG(LogTemp, Log, TEXT("Jinx's AttackDataMap || ID: %d) ProjectileCount: %d, ProjectileRange: %f, Damage: %f, Cooltime: %f, UseAP: %s, APDamage: %f"),
			Pair.Key, Pair.Value.ProjectileCount, Pair.Value.ProjectileRange, Pair.Value.Damage, Pair.Value.Cooltime, *Pair.Value.UseAP, Pair.Value.APDamage);
	}
}

void AJinx::BindSkill(ESkillKey Key, const TScriptInterface<ISkillStrategy>& Skill)
{
	SkillMap.Add(Key, Skill);
}

void AJinx::StartAttack()
{
	//GetWorld()->GetTimerManager().SetTimer(AttackTimer, this,
	//	FTimerDelegate::CreateLambda([this]() {}))
}
