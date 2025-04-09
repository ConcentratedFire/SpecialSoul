// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interfaces/IHttpRequest.h"
#include "UObject/NoExportTypes.h"
#include "CDataSheetUtility.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDataFetched);

UCLASS()
class SPECIALSOUL_API UCDataSheetUtility : public UObject
{
	GENERATED_BODY()

public:
	UCDataSheetUtility();

	
	// UFUNCTION(BlueprintCallable, Category = HTTP)
	template<typename T>
	void FetchGoogleSheetData(const FString& SheetName, const FString& startCell, const FString& endCell, TMap<int32, T>& OutDataMap);

	
	//UPROPERTY()
	//TMap<FString, float> DataMap;

	FOnDataFetched OnDataFetched;

protected:
	template<typename T>
	void OnResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful, TMap<int32, T>& OutDataMap);

	template<typename T>
	void PaserSheetRowsToStructArray(const TArray<TSharedPtr<FJsonValue>>*& Rows, TMap<int32, T>& OutDataMap);

private:
	const FString GoogleSheetID = TEXT("1jDDDp-0KEwhWuXAKkKeoljX5N8G2za3HqO0F8c1oQRY");
	const FString ApiKey = TEXT("AIzaSyB6rO5H4OxqWTOGvHVr04Xtt1YL-da3EdQ");
};

