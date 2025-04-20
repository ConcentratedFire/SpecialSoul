// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SkillComponent.generated.h"


class ACBasePlayer;
class ISkillStrategy;
enum class ESkillKey : uint8;
class UInputAction;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SPECIALSOUL_API USkillComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	USkillComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	virtual void InitializeComponent() override;

	bool CanUseSkill(ESkillKey key);
	bool ResetLeftCooltime(ESkillKey key);
protected:
	virtual void BeginPlay() override;

	void UpdateCooltime(float deltaTime);
	
public:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<ACharacter> OwnerCharacter {nullptr};

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Skills")
	TMap<ESkillKey, TScriptInterface<ISkillStrategy>> SkillMap;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Skills")
	TMap<ESkillKey, float> CoolTimeMap; // 쿨타임

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Skills")
	TMap<ESkillKey, float> LeftCoolTimeMap; // 남은 쿨타임

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Skills")
	TMap<ESkillKey, float> SkillRangeMap; // 스킬 사정거리

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Skills")
	int32 UseSkillCount {0};

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Skills")
	bool bUsingSkill {false};

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Skills")
	bool bUseESkill {false};

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Skills")
	bool bUseRSkill {false};
	
	void Attack(); // 기본공격

	void Passive();

	UFUNCTION()
	void OnESkillPressed(); // 키입력 이벤트
	
	UFUNCTION()
	void OnRSkillPressed(); // 키입력 이벤트

	 // 플레이어의 키 입력에 따른 스킬 캐스팅
	void BindSkill(ESkillKey Key, const TScriptInterface<ISkillStrategy>& Skill);

	void CastSkill(ESkillKey Key);
	
private:
	UFUNCTION()
	void SetInputBinding(class UEnhancedInputComponent* Input);
	
	UFUNCTION(Server, Reliable)
	void SRPC_CastSkill(ESkillKey Key);
	
	UPROPERTY(VisibleAnywhere, Category = Input)
	TObjectPtr<UInputAction> IA_ESkill;

	UPROPERTY(VisibleAnywhere, Category = Input)
	TObjectPtr<UInputAction> IA_RSkill;

};
