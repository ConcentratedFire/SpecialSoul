// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/CBasePlayer.h"

#include "EngineUtils.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/WidgetComponent.h"
#include "Game/CGameState.h"
#include "Game/CPlayerState.h"
#include "Game/SpecialSoulGameMode.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "ObjectPool/CObjectPoolManager.h"
#include "Player/CPlayerController.h"
#include "Player/Components/CMovementComponent.h"
#include "UI/CSelectUpgradeWidget.h"
#include "Data/JinxData.h"
#include "Data/CYasuoData.h"
#include "Net/UnrealNetwork.h"
#include "Player/CYasuo.h"
#include "Player/Jinx.h"
#include "Player/Components/SkillComponent.h"

struct FJinxAttackData;
class UEnhancedInputLocalPlayerSubsystem;
// Sets default values
ACBasePlayer::ACBasePlayer()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;

	// Set size for player capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Don't rotate character to camera direction
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Rotate character to moving direction
	GetCharacterMovement()->RotationRate = FRotator(0.f, 640.f, 0.f);
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;
	GetCharacterMovement()->bOrientRotationToMovement = false;

	// Create a camera boom...
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->SetUsingAbsoluteRotation(true); // Don't want arm to rotate when character does
	CameraBoom->TargetArmLength = 800.f;
	CameraBoom->SetRelativeRotation(FRotator(-60.f, 0.f, 0.f));
	CameraBoom->bDoCollisionTest = false; // Don't want to pull camera in when it collides with level

	// Create a camera...
	TopDownCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("TopDownCamera"));
	TopDownCameraComponent->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	TopDownCameraComponent->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	ArrowWidgetComp = CreateDefaultSubobject<UWidgetComponent>(TEXT("ArrowWidgetComp"));
	ArrowWidgetComp->SetupAttachment(RootComponent);
	ArrowWidgetComp->SetRelativeLocationAndRotation(FVector(0, 0, -100), ArrowRotation);
	ArrowWidgetComp->SetRelativeScale3D(FVector(.2, .35, .35));

	ConstructorHelpers::FClassFinder<UUserWidget> tempArrowWidget(
		TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/UI/WBP_Arrow.WBP_Arrow_C'"));
	if (tempArrowWidget.Succeeded())
		ArrowWidgetComp->SetWidgetClass(tempArrowWidget.Class);

	MoveComp = CreateDefaultSubobject<UCMovementComponent>(TEXT("MoveComp"));

	ConstructorHelpers::FObjectFinder<UInputMappingContext> tempIMC(
		TEXT("/Script/EnhancedInput.InputMappingContext'/Game/Player/Input/IMC_Player.IMC_Player'"));
	if (tempIMC.Succeeded())
		IMC_Player = tempIMC.Object;

	GetCapsuleComponent()->SetCollisionProfileName(FName("Player"));

	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	SkillComponent = CreateDefaultSubobject<USkillComponent>(TEXT("SkillComponent"));
}

// Called when the game starts or when spawned
void ACBasePlayer::BeginPlay()
{
	Super::BeginPlay();

	GetCharacterMovement()->MaxWalkSpeed = PlayerMoveSpeed;
	PC = Cast<ACPlayerController>(GetController());

	// TODO Init Data Settings
	// 캐릭터를 선택하면 GameMode에서 해당 캐릭터의 정보를 읽고
	// PlayerState의 BeginPlay에서 초기 데이터를 세팅해주도록 변경
	if (HasAuthority())
	{
		GM = Cast<ASpecialSoulGameMode>(GetWorld()->GetAuthGameMode());
		GS = GM->GetGameState<ACGameState>();
		DataSheetUtility = GM->DataSheetUtility;
	}

	// 오브젝트 풀 매니저 가져오기
	if (HasAuthority())
	{
		for (TActorIterator<ACObjectPoolManager> It(GetWorld(), ACObjectPoolManager::StaticClass()); It; ++It)
		{
			ObjectPoolManager = *It;
		}
	}

	// if (IsLocallyControlled())
	// 	InitUpgradeUI(); // 업그레이드 UI 생성
}

void ACBasePlayer::PrintNetLog()
{
	FString logStr = TEXT("Damage : 0"); // 기본값 설정

	if (PS != nullptr)
	{
		int32 dataCount = 0;
		if (this->IsA(ACYasuo::StaticClass()))
			dataCount = YasuoStat.Damage;
		else if (this->IsA(AJinx::StaticClass()))
			dataCount = JinxAttackData.Damage;

		logStr = FString::Printf(TEXT("Damage : %d"), dataCount);
	}
	else
	{
		logStr = TEXT("Data Count : Error (PlayerState is null)");
	}

	DrawDebugString(GetWorld(), GetActorLocation() + FVector::UpVector * 100.0f, logStr, nullptr, FColor::Red, 0, true,
	                1);
}

// Called every frame
void ACBasePlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	PrintNetLog();
	if (!PS)
	{
		PS = Cast<ACPlayerState>(GetPlayerState());
	}
}

// Called to bind functionality to input
void ACBasePlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (!IsLocallyControlled()) return;

	if (auto pc = GetWorld()->GetFirstPlayerController())
	{
		auto LocalPlayer = pc->GetLocalPlayer();
		if (auto subSystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(LocalPlayer))
		{
			subSystem->AddMappingContext(IMC_Player, 0);
		}
	}
	UEnhancedInputComponent* input = CastChecked<UEnhancedInputComponent>(PlayerInputComponent);
	if (input)
		OnInputBindingDel.Broadcast(input);
}

void ACBasePlayer::UpdatePlayerData(const int32 PlayerLevel)
{
	if (!SelectUpgradeWidget) return;
	// 업그레이드 할 항목 지정 후 화면에 출력
	// 랜덤으로 카드 3개를 선택 (남은 카드가 3장보다 적으면 1~2장까지만 뽑음)
	// 업그레이드 가능 항목이 없으면 진행하지 않음
	TArray<FString> cardList = PS->ChooseUpgradeCardList();
	if (cardList.Num() == 0) return;

	SelectUpgradeWidget->SetCardData(cardList);
	SelectUpgradeWidget->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

	UGameplayStatics::SetGamePaused(GetWorld(), true);

	if (this->IsA(ACYasuo::StaticClass()))
	{
		PC->GetNextLevelYasuoMoveStat();
	}
}

void ACBasePlayer::InitUpgradeUI()
{
	auto pc = Cast<ACPlayerController>(Controller);
	if (!pc) return;

	if (pc->SelectUpgradeUIWidgetFactory)
	{
		if (!pc->SelectUpgradeWidget)
			pc->SelectUpgradeWidget = Cast<UCSelectUpgradeWidget>(
				CreateWidget(GetWorld(), pc->SelectUpgradeUIWidgetFactory));

		SelectUpgradeWidget = pc->SelectUpgradeWidget;
		SelectUpgradeWidget->AddToViewport();
	}
}

void ACBasePlayer::SetSkillUsing(ESkillKey Key, bool bUseSkill)
{
	if (Key == ESkillKey::E)
		SkillComponent->bUseESkill = bUseSkill;
	else if (Key == ESkillKey::R)
		SkillComponent->bUseRSkill = bUseSkill;

	bAttacking = bUseSkill;
}

void ACBasePlayer::MyApplyDamage(float Damage, ABaseEnemy* DamagedActor)
{
	DamagedActor->MyDamage(Damage);
	PS->AddKillScore();
}

void ACBasePlayer::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ACBasePlayer, YasuoStat);
	DOREPLIFETIME(ACBasePlayer, YasuoMoveInfo);
	DOREPLIFETIME(ACBasePlayer, JinxAttackData);
	DOREPLIFETIME(ACBasePlayer, bAttacking);
}

void ACBasePlayer::EndUpgrade()
{
	if (SelectUpgradeWidget)
	{
		SelectUpgradeWidget->ClearCardData();
	}
	UGameplayStatics::SetGamePaused(GetWorld(), false);
}
