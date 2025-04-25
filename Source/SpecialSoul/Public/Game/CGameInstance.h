// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "CGameInstance.generated.h"

USTRUCT(BlueprintType)
struct FSessionInfo
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly)
	FString roomName;
	UPROPERTY(BlueprintReadOnly)
	FString hostName;
	UPROPERTY(BlueprintReadOnly)
	FString playerCount;
	UPROPERTY(BlueprintReadOnly)
	int32 pingSpeed;
	UPROPERTY(BlueprintReadOnly)
	int32 index;

	inline FString ToString()
	{
		return FString::Printf(TEXT("[%d] %s : %s - %s, %dms"), index, *roomName, *hostName, *playerCount, pingSpeed);
	}
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSearchSignature, const FSessionInfo&, sessionInfo);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSearchStateSignature, bool, bIsSearching);

UCLASS()
class SPECIALSOUL_API UCGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	virtual void Init() override;

	IOnlineSessionPtr sessionInterface;
	FString MySessionName{"Guest"};

	// 방 생성
public: 
	void CreateMySession(FString roomName, int32 playerCount);
private:
	UFUNCTION()
	void OnCreateSessionComplete(FName sessionName, bool bWasSuccessful);
	
	// 방 찾기
public: 
	TSharedPtr<FOnlineSessionSearch> sessionSearch;
	void FindOtherSession();
	// 찾은 방 정보를 넘겨주기 위한 델리게이트
	FSearchSignature onSearchCompleted;
	// 방을 찾는 중인지 아닌지에 대한 델리게이트
	FSearchStateSignature onSearchState;
private:
	void OnFindSessionsComplete(bool bWasSuccessful);

	// 방 입장
public: 
	void JoinSelectedSession(int32 index);
private:
	void OnJoinSessionComplete(FName sessionName, EOnJoinSessionCompleteResult::Type result);
	
	// 방 퇴장
public: 
	void ExitRoom();

	UFUNCTION(Server, Reliable)
	void SRPC_ExitRoom(); // 서버에게 방 나간다고 요청

	UFUNCTION(NetMulticast, Reliable)
	void MRPC_ExitRoom(); // 모든 클라에게 특정 클라가 나감 요청
private:
	void OnExitRoomComplete(FName sessionName, bool bWasSuccessful);

	// bool IsInRoom();
	
// private:
// 	const int32 playerCount{4};

	// 다국어 인코딩 -------------------------------------------------
	FString StringBase64Encode(const FString& str);
	FString StringBase64Decode(const FString& str);
};
