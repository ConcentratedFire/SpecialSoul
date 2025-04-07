#include "Player/Anim/CYasuoAnim.h"

#include "Player/CYasuo.h"

void UCYasuoAnim::NativeBeginPlay()
{
	Super::NativeBeginPlay();

	Owner = Cast<ACYasuo>(TryGetPawnOwner());
}

void UCYasuoAnim::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (!Owner) return;
	Speed = Owner->GetVelocity().Size();
}

void UCYasuoAnim::PlayAttackMontage()
{
	if (!AttackMontage) return;
	Montage_Play(AttackMontage, 1.f);
}

void UCYasuoAnim::AnimNotify_YasuoDefaultAttack()
{
	Owner->Attack();
}