// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/CGameInstance.h"

#include "NetworkReplayStreaming.h"
#include "OnlineSessionSettings.h"
#include "SpecialSoul.h"
#include "Kismet/GameplayStatics.h"
#include "Online/OnlineSessionNames.h"

void UCGameInstance::Init()
{
	Super::Init();

	if (auto subsys = IOnlineSubsystem::Get())
	{
		 // 서브시스템으로부터 세션 인터페이스 가져오기
		sessionInterface = subsys->GetSessionInterface();

		 // 콜백함수 바인딩
		sessionInterface->OnCreateSessionCompleteDelegates.AddUObject(this, &UCGameInstance::OnCreateSessionComplete);
		sessionInterface->OnFindSessionsCompleteDelegates.AddUObject(this, &UCGameInstance::OnFindSessionsComplete);
		sessionInterface->OnJoinSessionCompleteDelegates.AddUObject(this, &UCGameInstance::OnJoinSessionComplete);
		sessionInterface->OnDestroySessionCompleteDelegates.AddUObject(this, &UCGameInstance::OnExitRoomComplete);
	}
}

void UCGameInstance::CreateMySession(FString roomName, int32 playerCount)
{
	FOnlineSessionSettings sessionSettings;

	// 1. 데디 서버 X
	sessionSettings.bIsDedicated = false;

	// 2. 랜(로컬) 매칭 or Steam 매칭
	FName subsysName = IOnlineSubsystem::Get()->GetSubsystemName();
	sessionSettings.bIsLANMatch = (subsysName == "NULL");

	// 3. 매칭이 온라인을 통해 노출될지 여부 
	sessionSettings.bShouldAdvertise = true;

	// 4. 온라인 상태(Presence) 정보 활용 여부
	sessionSettings.bUsesPresence = true;
	sessionSettings.bUseLobbiesIfAvailable = true;

	// 5. 게임 진행 중에 참여 허가 여부
	sessionSettings.bAllowJoinViaPresence = true;
	sessionSettings.bAllowJoinInProgress = true;

	// 6. 세션에 참여할 수 있는 Public Connection 최대 수
	sessionSettings.NumPublicConnections = playerCount;
	
	// 7. 커스텀 룸네임 설정
	sessionSettings.Set(FName("ROOM_NAME"), StringBase64Encode(roomName), EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);

	// 8. 호스트네임 설정
	sessionSettings.Set(FName("HOST_NAME"), StringBase64Encode(MySessionName), EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);

	// NetID
	FUniqueNetIdPtr netId = GetWorld()->GetFirstLocalPlayerFromController()->GetUniqueNetIdForPlatformUser().GetUniqueNetId();
	
	LOG_S(Log, TEXT("Create Session Start : %s"), *MySessionName);
	sessionInterface->CreateSession(*netId, FName(MySessionName), sessionSettings);

	CurrentRoomName = roomName;
}

void UCGameInstance::OnCreateSessionComplete(FName sessionName, bool bWasSuccessful)
{
	LOG_S(Log, TEXT("SessionName : %s, bWasSuccessful : %d"), *sessionName.ToString(), bWasSuccessful);

	if( bWasSuccessful == true )
	{
		GetWorld()->ServerTravel(TEXT("/Game/Level/BattleMap?listen"));
	}
}

void UCGameInstance::FindOtherSession()
{
	// 방찾기 버튼 클릭시 true 브로드캐스트
	// 바인딩 ex) 방찾기 버튼 비활성화 등 
	onSearchState.Broadcast(true); 
	sessionSearch = MakeShareable(new FOnlineSessionSearch());

	// 1. 세션 검색 조건 설정
	sessionSearch->QuerySettings.Set(SEARCH_PRESENCE, true, EOnlineComparisonOp::Equals);

	// 2. Lan 여부
	sessionSearch->bIsLanQuery = IOnlineSubsystem::Get()->GetSubsystemName() == FName("NULL");

	// 3. 최대 검색 세션 수
	sessionSearch->MaxSearchResults = 10;

	// 4. 세션검색
	sessionInterface->FindSessions(0, sessionSearch.ToSharedRef());
}

void UCGameInstance::OnFindSessionsComplete(bool bWasSuccessful)
{
	// 찾기 실패시
	if( !bWasSuccessful )
	{
		onSearchState.Broadcast(false);
		LOG_S(Log, TEXT("Session search failed..."));
		return;
	}

	// 세션 검색결과 배열
	auto results = sessionSearch->SearchResults;
	LOG_S(Log, TEXT("Search Result Count : %d"), results.Num());

	for( int i = 0; i < results.Num(); i++ )
	{
		auto sr = results[i];
		if( sr.IsValid() == false )
		{
			continue;
		}

		FString roomName;
		FString hostName;

		// 세션정보 구조체선언
		FSessionInfo sessionInfo;
		sessionInfo.index = i;

		sr.Session.SessionSettings.Get(FName("ROOM_NAME"), roomName);
		sr.Session.SessionSettings.Get(FName("HOST_NAME"), hostName);
		sessionInfo.roomName = StringBase64Decode(roomName);
		sessionInfo.hostName = StringBase64Decode(hostName);

		// 입장가능한 플레이어수
		int32 maxPlayerCount = sr.Session.SessionSettings.NumPublicConnections;
		
		// 현재 입장한 플레이어 수 ( 최대 - 현재 입장 가능한 수 )
		// NumOpenPublicConnections 스팀에서만 정상적으로 값이 들어온다
		int32 currentPlayerCount = maxPlayerCount - sr.Session.NumOpenPublicConnections;

		sessionInfo.playerCount = FString::Printf(TEXT("(%d/%d)"), currentPlayerCount, maxPlayerCount);

		// 핑 정보( 스팀에서는 9999로 나온다 )
		sessionInfo.pingSpeed = sr.PingInMs;

		LOG_S(Log, TEXT("%s"), *sessionInfo.ToString());

		// 델리게이트로 위젯에 알려주기
		onSearchCompleted.Broadcast(sessionInfo);
	}

	onSearchState.Broadcast(false);
}

void UCGameInstance::JoinSelectedSession(int32 index)
{
	auto results = sessionSearch->SearchResults;
	
	results[index].Session.SessionSettings.bUseLobbiesIfAvailable = true;
	results[index].Session.SessionSettings.bUsesPresence = true;

	sessionInterface->JoinSession(0, FName(MySessionName), results[index]);
}

void UCGameInstance::OnJoinSessionComplete(FName sessionName, EOnJoinSessionCompleteResult::Type result)
{
	if (result == EOnJoinSessionCompleteResult::Success)
	{
		// RoomName 지정
		FOnlineSessionSettings* sessionSettings = sessionInterface->GetSessionSettings(sessionName);
		if (sessionSettings)
		{
			FString roomName;
			if (sessionSettings->Get(FName("ROOM_NAME"), roomName))
			{
				CurrentRoomName = StringBase64Decode(roomName);
			}
		}
		
		auto pc = GetWorld()->GetFirstPlayerController();
		FString url;
		sessionInterface->GetResolvedConnectString(sessionName, url);

		LOG_S(Log, TEXT("Joined URL : %s"), *url);

		if (url.IsEmpty()) return;
		pc->ClientTravel(url, ETravelType::TRAVEL_Absolute);
	}
	else
	{
		LOG_S(Log, TEXT("Failed to join the game : %d "), result);
	}
}

void UCGameInstance::ExitRoom()
{
	SRPC_ExitRoom(); // ExitRoom 1)서버에 ExitRoom 요청
}

void UCGameInstance::SRPC_ExitRoom_Implementation()
{
	MRPC_ExitRoom(); // ExitRoom 2) 모든 클라에 요청
}

void UCGameInstance::MRPC_ExitRoom_Implementation()
{
	// 클라 영역 (리슨서버/클라)
	// ExitRoom 3 끝 ) 세션에서 퇴장
	// OnDestroySessionCompleteDelegates 브로드캐스트함
	sessionInterface->DestroySession(FName(*MySessionName));

	//세션 삭제 후 수동 이동
	ENetMode NetMode = GetWorld()->GetNetMode();
	if (NetMode == NM_Client)
	{
		APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
		if (PC && PC->IsLocalController())
		{
			PC->ClientTravel(FString(TEXT("/Game/Level/BattleMap.BattleMap")), ETravelType::TRAVEL_Absolute);
		}
	}
	else if (NetMode == NM_ListenServer)
	{
		GetWorld()->ServerTravel(FString(TEXT("/Game/Level/BattleMap.BattleMap?listen"), ETravelType::TRAVEL_Absolute));
	}
}

void UCGameInstance::OnExitRoomComplete(FName sessionName, bool bWasSuccessful)
{
	// auto pc = GetWorld()->GetFirstPlayerController();
	// FString url = TEXT("/Game/Level/StandbyLevel.StandbyLevel");
	// pc->ClientTravel(url, TRAVEL_Absolute);

	// ENetMode NetMode = GetWorld()->GetNetMode();
	// if (NetMode == NM_Client)
	// {
	// 	APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	// 	if (PC && PC->IsLocalController())
	// 	{
	// 		PC->ClientTravel(FString(TEXT("/Game/Level/BattleMap.BattleMap")), ETravelType::TRAVEL_Absolute);
	// 	}
	// }
}

FString UCGameInstance::StringBase64Encode(const FString& str)
{
	// Set 할 때 : FString -> UTF8 (std::string) -> TArray<uint8> -> base64 로 Encode
	std::string utf8String = TCHAR_TO_UTF8(*str);
	TArray<uint8> arrayData = TArray<uint8>((uint8*)(utf8String.c_str()), utf8String.length());
	return FBase64::Encode(arrayData);
}

FString UCGameInstance::StringBase64Decode(const FString& str)
{
	// Get 할 때 : base64 로 Decode -> TArray<uint8> -> TCHAR
	TArray<uint8> arrayData;
	FBase64::Decode(str, arrayData);
	std::string utf8String((char*)(arrayData.GetData()), arrayData.Num());
	return UTF8_TO_TCHAR(utf8String.c_str());
}
