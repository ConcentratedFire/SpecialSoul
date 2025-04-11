// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

DECLARE_LOG_CATEGORY_EXTERN(LogSpecialSoul, Log, All);

#define CALLINFO (FString(__FUNCTION__) + TEXT("(") + FString::FromInt(__LINE__) + TEXT(")"))

// NetMode, 소스 라인번호, 로그 내용 출력
#define LOG_S(Verbosity, fmt, ...) UE_LOG(LogSpecialSoul, Verbosity, TEXT("%s : %s"), *CALLINFO, *FString::Printf(fmt, ##__VA_ARGS__));
// Screen에다가 LOG를 직접 찍는 매크로
#define	LOG_SCREEN(fmt, ...) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT(fmt), ##__VA_ARGS__))
// 위의 함수에 시간을 넣는 매크로.
#define	LOG_SCREEN_T(Time, fmt, ...) GEngine->AddOnScreenDebugMessage(-1, Time, FColor::Red, FString::Printf(TEXT(fmt), ##__VA_ARGS__))
// Screen에다가 LOG를 직접 찍는 매크로, 인덱스, 색 추가
#define	LOG_SCREEN_IDX(idx, color, fmt, ...) GEngine->AddOnScreenDebugMessage(idx, 5.f, color, FString::Printf(TEXT(fmt), ##__VA_ARGS__))

#define ECC_FlowField ECC_GameTraceChannel1