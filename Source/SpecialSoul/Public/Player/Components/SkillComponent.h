#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Skill/SkillCooltime.h"
#include "SkillComponent.generated.h"


class ISkillStrategy;
enum class ESkillKey : uint8;
class UInputAction;


// 스킬 쿨타임 델리게이트
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnCooltimeUpdated, ESkillKey, skillKey, FSkillCooltime, cooltimeInfo);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnChargeCountChanged, ESkillKey, skillKey, int32, count);

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
	void UpdateChargedCount(ESkillKey skillKey, int32 count);

	FOnCooltimeUpdated OnCooltimeUpdated;
	FOnChargeCountChanged OnChargeCountChanged;

protected:
	virtual void BeginPlay() override;

	void UpdateCooltime(float deltaTime);
	
public:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<ACharacter> OwnerCharacter {nullptr};

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Skills")
	TMap<ESkillKey, TScriptInterface<ISkillStrategy>> SkillMap;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Skills")
	TMap<ESkillKey, FSkillCooltime> CoolTimeMap; // 쿨타임

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Skills")
	TMap<ESkillKey, int32> ChargedCount;
	
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

	UFUNCTION(Client, Reliable)
	void CRPC_UpdateSkillCooltime(ESkillKey skillKey, FSkillCooltime cooltimeInfo);
	
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