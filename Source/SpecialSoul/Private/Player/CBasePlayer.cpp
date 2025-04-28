// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/CBasePlayer.h"

#include "EngineUtils.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SceneCaptureComponent2D.h"
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
#include "Kismet/KismetRenderingLibrary.h"
#include "Net/UnrealNetwork.h"
#include "Player/CYasuo.h"
#include "Player/Jinx.h"
#include "Player/Components/SkillComponent.h"
#include "UI/GameWidget.h"
#include "UI/OverheadStatusWidget.h"
#include "UI/HUD/GameHUD.h"

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

	MiniMapCam = CreateDefaultSubobject<USceneCaptureComponent2D>(TEXT("MiniMapCam"));
	MiniMapCam->SetupAttachment(RootComponent);
	MiniMapCam->SetUsingAbsoluteRotation(true);
	MiniMapCam->SetRelativeLocationAndRotation(FVector(0, 0, 1000), FRotator(-90, 0, 0));

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

	OverheadUIComp = CreateDefaultSubobject<UWidgetComponent>(TEXT("OverheadUIComp"));
	static ConstructorHelpers::FClassFinder<UUserWidget> WidgetClass(TEXT("/Game/UI/WBP_OverheadStatusWidget.WBP_OverheadStatusWidget_C"));
	if (WidgetClass.Succeeded())
	{
		OverheadUIComp->SetWidgetClass(WidgetClass.Class);
	}
	OverheadUIComp->SetupAttachment(GetCameraBoom());
	OverheadUIComp->SetWidgetSpace(EWidgetSpace::World);
	OverheadUIComp->SetDrawAtDesiredSize(true);
	OverheadUIComp->SetIsReplicated(true);
	OverheadUIComp->SetRelativeLocation(FVector(800.f, 0.f, 90.f));
	OverheadUIComp->SetRelativeRotation(FRotator(0,180,0));
	OverheadUIComp->SetRelativeScale3D(FVector(0.7f));
	OverheadUIComp->SetCastShadow(false);
	//OverheadUIComp->SetRenderInMainPass(true);
	//OverheadUIComp->SetTranslucentSortPriority(10);
	OverheadUIComp->SetDepthPriorityGroup(SDPG_Foreground); 
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
	
	if (auto overheadUI = Cast<UOverheadStatusWidget>(OverheadUIComp->GetWidget()))
	{
		overheadUI->SetHP(HP, MaxHP);
		overheadUI->SetEnergy(0, 100);
		overheadUI->SetLevel(1);
	}

	// if (IsLocallyControlled())
	// {
	// 	if (AGameHUD* hud = Cast<AGameHUD>(PC->GetHUD()))
	// 	{
	// 		hud->SetHP(HP, MaxHP);
	// 		hud->SetEnergy(0, 100);
	// 		hud->SetLevel(1);
	// 		hud->SetEXP(0, 1);
	// 	}
	// }
}

void ACBasePlayer::SetLocalInit(class ACPlayerController* InPC)
{
	MoveComp->SetController(InPC);
	InitUpgradeUI(); // 업그레이드 UI 생성
	SkillComponent->OnCooltimeUpdated.AddDynamic(this, &ACBasePlayer::OnCooltimeChanged);
	
	CRPC_SetMinimap(InPC);
	
	//if (IsLocallyControlled())
	//{
	if (AGameHUD* hud = Cast<AGameHUD>(PC->GetHUD()))
	{
		hud->SetHP(HP, MaxHP);
		hud->SetEnergy(0, 100);
		hud->SetLevel(1);
		hud->SetEXP(0, 1);
	}
	if (auto overheadUI = Cast<UOverheadStatusWidget>(OverheadUIComp->GetWidget()))
	{
		overheadUI->SetHP(HP, MaxHP);
	}
	//}
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
	if (!PC)
	{
		PC = Cast<ACPlayerController>(GetController());
	}

	if (PC && PC->IsLocalController() && !bIsLocalInit)
	{
		SetLocalInit(PC);
		bIsLocalInit = true;
		
	}

	if (IsLocallyControlled())
		MiniMapCam->SetRelativeRotation(FRotator(-90, 0, 0));
	
	// if (OverheadUIComp)
	// {
	// 	APlayerCameraManager* cm = UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0);
	// 	if (cm)
	// 	{
	// 		FVector CamLocation = cm->GetCameraLocation();
	// 		FVector ToCamera = CamLocation - OverheadUIComp->GetComponentLocation();
	// 		ToCamera.Z = 180;
	// 		FRotator LookAtRotation = FRotationMatrix::MakeFromX(ToCamera).Rotator();
	//
	// 		OverheadUIComp->SetWorldRotation(LookAtRotation);
	// 	}
	// }
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
	// 이 함수를 호출하는 위치가 서버임
	// 서버에서 업그레이드할 카드를 정해서 넘겨줌
	TArray<FString> cardList = PS->ChooseUpgradeCardList();
	if (cardList.Num() == 0) return;
	//LOG_S(Warning, TEXT("Player Name : %s"), *GetName());
	// for (auto& card : cardList)
	// {
	// 	LOG_S(Warning, TEXT("card : %s"), *card);
	// }
	TArray<FCardStruct> CardData;
	SetCardData(cardList, CardData);
	LOG_S(Warning, TEXT("cardList Count : %d"), cardList.Num());
	LOG_S(Warning, TEXT("CardData Count : %d"), CardData.Num());
	MRPC_ShowUpgradeUI(cardList, CardData);

	if (this->IsA(ACYasuo::StaticClass()))
	{
		PC->GetNextLevelYasuoMoveStat();
	}

	if (auto overheadUI = Cast<UOverheadStatusWidget>(OverheadUIComp->GetWidget()))
	{
		overheadUI->SetLevel(PlayerLevel);
	}
	if (IsLocallyControlled())
	{
		if (AGameHUD* hud = Cast<AGameHUD>(PC->GetHUD()))
		{
			hud->SetLevel(PlayerLevel);
			hud->SetEXP(0, 100);

			// TODO : hud->SetStatData();
		}
	}
}

void ACBasePlayer::SetCardData(const TArray<FString>& CardList, TArray<FCardStruct>& CardData)
{
	for (auto& card : CardList)
	{
		FCardStruct tempCardData;
		GetCurGrade(tempCardData.WeaponGrade, tempCardData.DamageGrade, tempCardData.AbilityHasteGrade,
		            tempCardData.ProjectilesGrade, tempCardData.CritChanceGrade);
		if (card == "Weapon")
		{
			if (this->IsA(ACYasuo::StaticClass()))
			{
				tempCardData.bIsYasuo = true;
				GetCurDamageNextDamage(tempCardData.WeaponGrade, tempCardData.curDamage, tempCardData.nextDamage, true);
				GetCurProjectileNextProjectile(tempCardData.WeaponGrade, tempCardData.curProjectile,
				                               tempCardData.nextProjectile, true);
				// 다음 업글 단계가 Final인지 확인
				tempCardData.bIsNextWeaponFinal = IsNextWeaponFinal(true);
				if (tempCardData.bIsNextWeaponFinal)
				{
					tempCardData.strLevel = FString::Printf(TEXT("진화"));
					tempCardData.strTitle = FString::Printf(TEXT("떠도는 폭풍"));
					tempCardData.strDesc = FString::Printf(TEXT(
						"야스오가 기력을 연마해 수는\n적지만 훨씬 큰 소용돌이를\n방출합니다. 소용돌이는 포물선을\n그리며 날아가며 목표 지점에\n피해를 입히는 폭풍을 남깁니다."));
				}
				else
				{
					tempCardData.strLevel = FString::Printf(TEXT("%d레벨"), tempCardData.WeaponGrade + 1);
					tempCardData.strTitle = FString::Printf(TEXT("강철 폭풍"));
					tempCardData.strDesc = FString::Printf(TEXT("피해량"));
					tempCardData.strStat = FString::Printf(
						TEXT("%d > %d"), tempCardData.curDamage, tempCardData.nextDamage);
					tempCardData.strDesc2 = FString::Printf(TEXT("투사체"));
					tempCardData.strStat2 = FString::Printf(
						TEXT("%d > %d"), tempCardData.curProjectile, tempCardData.nextProjectile);
				}
			}
			else
			{
				tempCardData.bIsYasuo = false;
				GetCurDamageNextDamage(tempCardData.WeaponGrade, tempCardData.curDamage, tempCardData.nextDamage,
				                       false);
				GetCurProjectileNextProjectile(tempCardData.WeaponGrade, tempCardData.curProjectile,
				                               tempCardData.nextProjectile, false);
				tempCardData.bIsNextWeaponFinal = IsNextWeaponFinal(false);
				if (tempCardData.bIsNextWeaponFinal)
				{
					tempCardData.strLevel = FString::Printf(TEXT("진화"));
					tempCardData.strTitle = FString::Printf(TEXT("전투 고양이 총알\n세례"));
					tempCardData.strDesc = FString::Printf(
						TEXT("총알이 대상을 관통합니다. 처음\n적중한 이후에는 피해량이\n감소합니다. 재사용 대기시간이\n크게 감소합니다."));
				}
				else
				{
					tempCardData.strLevel = FString::Printf(TEXT("%d레벨"), tempCardData.WeaponGrade + 1);
					tempCardData.strTitle = FString::Printf(TEXT("야옹 야옹"));
					tempCardData.strDesc = FString::Printf(TEXT("피해량"));
					tempCardData.strStat = FString::Printf(
						TEXT("%d > %d"), tempCardData.curDamage, tempCardData.nextDamage);
					tempCardData.strDesc2 = FString::Printf(TEXT("투사체"));
					tempCardData.strStat2 = FString::Printf(
						TEXT("%d > %d"), tempCardData.curProjectile, tempCardData.nextProjectile);
				}
			}
		}
		else
		{
			tempCardData.strUpgradeStat = GetUpgradeData(card, tempCardData.strDesc, tempCardData.strTitle);
			int32 nextLevel = 0;
			if (card == "Damage")
			{
				nextLevel = tempCardData.DamageGrade + 1;
			}
			else if (card == "AbilityHaste")
			{
				nextLevel = tempCardData.AbilityHasteGrade + 1;
			}
			else if (card == "Projectiles")
			{
				nextLevel = tempCardData.ProjectilesGrade + 1;
			}
			else if (card == "CritChance")
			{
				nextLevel = tempCardData.CritChanceGrade + 1;
			}

			tempCardData.strLevel = FString::Printf(TEXT("%d레벨"), nextLevel);
		}
		CardData.Add(tempCardData);
	}
}

void ACBasePlayer::MRPC_ShowUpgradeUI_Implementation(const TArray<FString>& cardList,
                                                     const TArray<FCardStruct>& CardData)
{
	if (IsLocallyControlled())
		CRPC_ShowUpgradeUI(cardList, CardData);
}

void ACBasePlayer::CRPC_ShowUpgradeUI_Implementation(const TArray<FString>& cardList,
                                                     const TArray<FCardStruct>& CardData)
{
	if (!SelectUpgradeWidget) return;
	// LOG_S(Warning, TEXT("Player Name : %s"), *GetName());
	// for (auto& card : cardList)
	// {
	// 	LOG_S(Warning, TEXT("card : %s"), *card);
	// }
	// for (auto cardData : CardData)
	// 	cardData.Print_Log();
	// LOG_S(Warning, TEXT("============="));

	// 업그레이드 할 항목 지정 후 화면에 출력
	// 랜덤으로 카드 3개를 선택 (남은 카드가 3장보다 적으면 1~2장까지만 뽑음)
	// 업그레이드 가능 항목이 없으면 진행하지 않음
	SelectUpgradeWidget->SetCardData(cardList, CardData);
	SelectUpgradeWidget->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	UGameplayStatics::SetGamePaused(GetWorld(), true);
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

void ACBasePlayer::ResetLeftCooltime(ESkillKey skillKey)
{
	SkillComponent->ResetLeftCooltime(skillKey);
}

void ACBasePlayer::CRPC_SetSkillChargingUI_Implementation(ESkillKey skillKey, bool bIsCharging,
                                                          ACPlayerController* InPC)
{
	if (AGameHUD* hud = Cast<AGameHUD>(InPC->GetHUD()))
	{
		if (hud)
			hud->SetSkillSlotIsCharging(skillKey, bIsCharging);
	}
}

void ACBasePlayer::OnCooltimeChanged(ESkillKey skillKey, FSkillCooltime cooltimeInfo)
{
	if (IsLocallyControlled())
	{
		if (AGameHUD* hud = Cast<AGameHUD>(PC->GetHUD()))
		{
			if (hud)
				hud->UpdateSkillCooltime(skillKey, cooltimeInfo);
		}
	}
}


void ACBasePlayer::MyApplyDamage(float Damage, ABaseEnemy* DamagedActor)
{
	DamagedActor->MyDamage(Damage);
	PS->AddKillScore();
	// MRPC_AddKillScore();
}

void ACBasePlayer::MRPC_AddKillScore_Implementation()
{
	CRPC_AddKillScore();
}

void ACBasePlayer::CRPC_AddKillScore_Implementation()
{
	if (IsLocallyControlled())
		PS->AddKillScore();
}

void ACBasePlayer::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ACBasePlayer, YasuoStat);
	DOREPLIFETIME(ACBasePlayer, YasuoMoveInfo);
	DOREPLIFETIME(ACBasePlayer, JinxAttackData);
	DOREPLIFETIME(ACBasePlayer, bAttacking);
	DOREPLIFETIME(ACBasePlayer, hp);
}

void ACBasePlayer::EndUpgrade()
{
	MRPC_EndUpgrade();
	++GS->UpgradeSelectPlayerCount;
	// if (HasAuthority() && IsLocallyControlled())
	GS->OnRep_UpgradeSelectPlayerCount();
	// UGameplayStatics::SetGamePaused(GetWorld(), false);
}

void ACBasePlayer::MRPC_EndUpgrade_Implementation()
{
	if (IsLocallyControlled())
	{
		CRPC_EndUpgrade();
		// TODO : 업그레이드 UI (ChampionStatusWidget) 반영
		if (AGameHUD* hud = Cast<AGameHUD>(PC->GetHUD()))
		{
			hud->SetHP(HP, MaxHP);
		}
	}
}

void ACBasePlayer::CRPC_EndUpgrade_Implementation()
{
	if (SelectUpgradeWidget)
	{
		SelectUpgradeWidget->ClearCardData();
	}
}

void ACBasePlayer::SRPC_UnPause_Implementation()
{
	MRPC_UnPause();
}

void ACBasePlayer::MRPC_UnPause_Implementation()
{
	if (IsLocallyControlled())
		CRPC_UnPause();
}

void ACBasePlayer::CRPC_UnPause_Implementation()
{
	UGameplayStatics::SetGamePaused(GetWorld(), false);
}

void ACBasePlayer::CRPC_SetMinimap_Implementation(ACPlayerController* InPC)
{
	if (MiniMapCam)
	{
		UTextureRenderTarget2D* RenderTarget = NewObject<UTextureRenderTarget2D>(this);
		if (RenderTarget)
		{
			RenderTarget->InitAutoFormat(1920, 1080); // 해상도 설정
			RenderTarget->RenderTargetFormat = ETextureRenderTargetFormat::RTF_RGBA8; // 포맷 설정
			RenderTarget->ClearColor = FLinearColor::Black; // 배경색 설정
			RenderTarget->UpdateResourceImmediate();
		}

		// 2. SceneCaptureComponent 설정
		if (MiniMapCam)
		{
			MiniMapCam->TextureTarget = RenderTarget; // RenderTarget 연결
			MiniMapCam->CaptureSource = ESceneCaptureSource::SCS_FinalColorLDR; // 캡처 소스 설정
			// MiniMapCam->ProjectionType = ECameraProjectionMode::Orthographic; // 직교 투영 설정 (필요 시)
			// MiniMapCam->OrthoWidth = 1920.0f; // 직교 뷰 크기 설정
			MiniMapCam->bCaptureEveryFrame = true; // 매 프레임 캡처
		}

		if (AGameHUD* hud = Cast<AGameHUD>(InPC->GetHUD()))
		{
			if (hud->GameWidget)
				hud->SetMiniMapTexture(RenderTarget);
		}
	}
}


void ACBasePlayer::DamageProcess(float damage)
{
	if (HasAuthority())
	{
		HP -= damage; // SetHP setter 호출
	}
	else
	{
		LOG_S(Warning, TEXT("DamageProcess"));
	}
}

void ACBasePlayer::OnRep_HP()
{
	if (HP <= 0.f) // 사망 처리
	{
		hp = 0.f;
		bIsDead = true;
	}
	
	if (IsLocallyControlled())
	{
		if (AGameHUD* hud = Cast<AGameHUD>(PC->GetHUD()))
		{
			hud->SetHP(HP, MaxHP);
		}
	}
	
	if (auto overheadUI = Cast<UOverheadStatusWidget>(OverheadUIComp->GetWidget()))
	{
		overheadUI->SetHP(HP, MaxHP);
	}
}

float ACBasePlayer::GetHP()
{
	return hp;
}

void ACBasePlayer::SetHP(float value)
{
	hp = value;
	OnRep_HP();
}
