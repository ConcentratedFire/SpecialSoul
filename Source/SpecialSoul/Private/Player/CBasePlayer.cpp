// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/CBasePlayer.h"

#include "EngineUtils.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Game/CGameState.h"
#include "Game/SpecialSoulGameMode.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Item/CBaseItem.h"
#include "ObjectPool/CObjectPoolManager.h"
#include "Player/Components/CMovementComponent.h"
#include "Utility/CDataSheetUtility.h"

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

	MoveComp = CreateDefaultSubobject<UCMovementComponent>(TEXT("MoveComp"));

	ConstructorHelpers::FObjectFinder<UInputMappingContext> tempIMC(
		TEXT("/Script/EnhancedInput.InputMappingContext'/Game/Player/Input/IMC_Player.IMC_Player'"));
	if (tempIMC.Succeeded())
		IMC_Player = tempIMC.Object;

	GetCapsuleComponent()->SetCollisionProfileName(FName("Player"));
	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &ACBasePlayer::OnCharacterBeginOverlap);

	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

// Called when the game starts or when spawned
void ACBasePlayer::BeginPlay()
{
	Super::BeginPlay();

	// TODO 서버일때만 하도록 처리
	GM = Cast<ASpecialSoulGameMode>(GetWorld()->GetAuthGameMode());
	if (GM)
	{
		GS = GM->GetGameState<ACGameState>();
		if (GS)
		{
			// TODO Init Data Settings
			// 캐릭터를 선택하면 GameMode에서 해당 캐릭터의 정보를 읽고
			// Player의 BeginPlay에서 초기 데이터를 세팅해주도록 변경
			DataSheetUtility = NewObject<UCDataSheetUtility>(this);

			if (DataSheetUtility)
			{
				DataSheetUtility->OnDataFetched.AddDynamic(GM, &ASpecialSoulGameMode::PrintRegenDataMap);
				DataSheetUtility->OnDataFetched.AddDynamic(GS, &ACGameState::PrintExpDataMap);
				
				DataSheetUtility->FetchGoogleSheetData<FYasuoAttackData>("Yasuo", "A1", "H8", YasuoAttackDataMap);
				DataSheetUtility->FetchGoogleSheetData<FYasuoMoveData>("YasuoMove", "A1", "D5", YasuoMoveDataMap);
				DataSheetUtility->FetchGoogleSheetData<FRegenData>("Regen", "A1", "E23", GM->RegenDataMap);
				DataSheetUtility->FetchGoogleSheetData<FEXPData>("EXP", "A1", "B22", GS->EXPDataMap);

				DataSheetUtility->FetchGoogleSheetData<FJinxAttackData>("Jinx", "A1", "G8", AttackDataMap); // 기본공격 데이터

				
				// 리소스 해제
				// DataSheetUtility->ConditionalBeginDestroy();
				// DataSheetUtility = nullptr;
			}
		}
	}

	// 오브젝트 풀 매니저 가져오기
	for (TActorIterator<ACObjectPoolManager> It(GetWorld(), ACObjectPoolManager::StaticClass()); It; ++It)
	{
		ObjectPoolManager = *It;
	}
	
	GetCharacterMovement()->MaxWalkSpeed = PlayerMoveSpeed;
}

// Called every frame
void ACBasePlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void ACBasePlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

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

void ACBasePlayer::PrintAttackDataMap()
{
}

void ACBasePlayer::OnCharacterBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                           UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
                                           const FHitResult& SweepResult)
{
	if (auto Item = Cast<ACBaseItem>(OtherActor))
	{
		Item->ActiveItem();
	}
}
