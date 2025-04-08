// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/Jinx.h"

#include "Components/CapsuleComponent.h"
#include "Player/Anim/JinxAnim.h"
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

void AJinx::BeginPlay()
{
	Super::BeginPlay();

	Anim = Cast<UJinxAnim>(GetMesh()->GetAnimInstance());
	
	 // TODO Init Data Settings
	 // 캐릭터를 선택하면 GameMode에서 해당 캐릭터의 정보를 읽고
	 // Player의 BeginPlay에서 초기 데이터를 세팅해주도록 변경
	TObjectPtr<UCDataSheetUtility> DataSheetUtility = NewObject<UCDataSheetUtility>();
	if (DataSheetUtility)
		DataSheetUtility->FetchGoogleSheetData("Jinx", "B2", "F7");
	
	DataSheetUtility->ConditionalBeginDestroy(); // 리소스 해제
	DataSheetUtility = nullptr;
}

void AJinx::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// 플레이어의 키 입력에 따른 스킬 캐스팅
void AJinx::CastSkill(ESkillKey Key)
{
	if (!SkillMap.Contains(Key))
	{
		UE_LOG(LogTemp, Warning, TEXT("CastSkill) Skill does not exist"));
		return;
	}
	
	SkillMap[Key]->UseSkill(this);
}
