// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/Components/SkillComponent.h"

#include "EnhancedInputComponent.h"
#include "Interface/SkillStrategy.h"
#include "Player/CBasePlayer.h"

// Sets default values for this component's properties
USkillComponent::USkillComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	bWantsInitializeComponent = true;

	ConstructorHelpers::FObjectFinder<UInputAction> tempESkill(
		TEXT("/Script/EnhancedInput.InputAction'/Game/Player/Input/IA_ESkill.IA_ESkill'"));
	if (tempESkill.Succeeded())
	{
		IA_ESkill = tempESkill.Object;
	}
	ConstructorHelpers::FObjectFinder<UInputAction> tempRSkill(
		TEXT("/Script/EnhancedInput.InputAction'/Game/Player/Input/IA_RSkill.IA_RSkill'"));
	if (tempRSkill.Succeeded())
	{
		IA_RSkill = tempRSkill.Object;
	}
}

void USkillComponent::BeginPlay()
{
	Super::BeginPlay();
}

void USkillComponent::InitializeComponent()
{
	Super::InitializeComponent();

	if (GetOwner())
	{
		OwnerCharacter = Cast<ACharacter>(GetOwner());
		if (auto player = Cast<ACBasePlayer>(OwnerCharacter))
		{
			player->OnInputBindingDel.AddUObject(this, &USkillComponent::SetInputBinding);
		}
	}
}

void USkillComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// 서버에서만 쿨타임 업데이트!
	if (OwnerCharacter->HasAuthority())
		UpdateCooltime(DeltaTime);
}


void USkillComponent::UpdateCooltime(float deltaTime)
{
	for (auto& timer : CoolTimeMap)
	{
		FSkillCooltime& cooldownInfo = timer.Value;
		if (cooldownInfo.LeftCooltime > 0.f)
		{
			cooldownInfo.LeftCooltime = FMath::Max(0.f, cooldownInfo.LeftCooltime - deltaTime);
		}

		CRPC_UpdateSkillCooltime(timer.Key, cooldownInfo);
	}
}
void USkillComponent::CRPC_UpdateSkillCooltime_Implementation(ESkillKey skillKey, FSkillCooltime cooltimeInfo)
{
	if (auto controller = OwnerCharacter->GetController())
	{
		if (controller->IsLocalController())
		{
			OnCooltimeUpdated.Broadcast(skillKey, cooltimeInfo);
		}
	}
}


bool USkillComponent::ResetLeftCooltime(ESkillKey key)
{
	if (!CoolTimeMap.Contains(key) || !SkillMap.Contains(key))
	{
		return false;
	}
	
	CoolTimeMap[key].LeftCooltime = CoolTimeMap[key].TotalCooltime;
	return true;
}

bool USkillComponent::CanUseSkill(ESkillKey key)
{
	if (!CoolTimeMap.Contains(key) || !SkillMap.Contains(key))
	{
		UE_LOG(LogTemp, Warning, TEXT("Can not Use Skill... "));
		return true; // TODO : false로 바꿔주기!!!
	}

	return CoolTimeMap[key].LeftCooltime <= 0.f;
}


void USkillComponent::CastSkill(ESkillKey Key)
{
	if (!OwnerCharacter)
	{
		UE_LOG(LogTemp, Error, TEXT("!OwnerCharacter"));
		return;
	}

	if ((Key == ESkillKey::E || Key == ESkillKey::R) && (bUseESkill || bUseRSkill)) return; // 스킬 사용중에는 다른 스킬 사용 방지
	
	SkillMap[Key]->UseSkill(OwnerCharacter); // OwnerCharacter데이터를 반영해서 스킬 사용
}

void USkillComponent::SRPC_CastSkill_Implementation(ESkillKey Key)
{
	if (!SkillMap.Contains(Key))
	{
		UE_LOG(LogTemp, Error, TEXT("!SkillMap.Contains(Key) "));
		return;
	}
	if (!OwnerCharacter)
	{
		UE_LOG(LogTemp, Error, TEXT("!OwnerCharacter"));
		return;
	}
	
	if (!CanUseSkill(Key))
	{
		return;
	}
	

	if ((Key == ESkillKey::E || Key == ESkillKey::R) && (bUseESkill || bUseRSkill)) return; // 스킬 사용중에는 다른 스킬 사용 방지

	SkillMap[Key]->UseSkill(OwnerCharacter); // OwnerCharacter데이터를 반영해서 스킬 사용
}

void USkillComponent::BindSkill(ESkillKey Key, const TScriptInterface<ISkillStrategy>& Skill)
{
	SkillMap.Add(Key, Skill);
}

void USkillComponent::SetInputBinding(class UEnhancedInputComponent* Input)
{
	if (IA_ESkill)
		Input->BindAction(IA_ESkill, ETriggerEvent::Started, this, &USkillComponent::OnESkillPressed);
	if (IA_RSkill)
		Input->BindAction(IA_RSkill, ETriggerEvent::Started, this, &USkillComponent::OnRSkillPressed);
}

void USkillComponent::Attack()
{
	CastSkill(ESkillKey::Attack);
}

void USkillComponent::Passive()
{
	CastSkill(ESkillKey::Passive);
}

void USkillComponent::OnESkillPressed()
{
	SRPC_CastSkill(ESkillKey::E);
}

void USkillComponent::OnRSkillPressed()
{
	SRPC_CastSkill(ESkillKey::R);
}
