// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HttpModule.h"
#include "Interfaces/IHttpRequest.h"
#include "Interfaces/IHttpResponse.h"
#include "UObject/NoExportTypes.h"
#include "CDataSheetUtility.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDataFetched);

UCLASS()
class SPECIALSOUL_API UCDataSheetUtility : public UObject
{
	GENERATED_BODY()

public:
	UCDataSheetUtility();

	template<typename T, typename Y>
	void FetchGoogleSheetData(const FString& SheetName, const FString& startCell, const FString& endCell, TMap<Y, T>& OutDataMap);

	FOnDataFetched OnDataFetched;

protected:
	template<typename T, typename Y>
	void OnResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful, TMap<Y, T>& OutDataMap);

	template<typename T, typename Y>
	void PaserSheetRowsToStructArray(const TArray<TSharedPtr<FJsonValue>>*& Rows, TMap<Y, T>& OutDataMap);

private:
	const FString GoogleSheetID = TEXT("1jDDDp-0KEwhWuXAKkKeoljX5N8G2za3HqO0F8c1oQRY");
	const FString ApiKey = TEXT("AIzaSyB6rO5H4OxqWTOGvHVr04Xtt1YL-da3EdQ");
};

template<typename T, typename Y>
void UCDataSheetUtility::FetchGoogleSheetData(const FString& SheetName, const FString& startCell, const FString& endCell, TMap<Y, T>& OutDataMap)
{
	// HTTP Module 가져오기
	FHttpModule* Http = &FHttpModule::Get();

	// HTTP Request 생성
	TSharedRef<IHttpRequest> Request = Http->CreateRequest();

	// Google Sheets API URL 설정	
	FString GoogleSheetRange = FString::Printf(TEXT("%s!%s%s%s"), *SheetName, *startCell, *FString("%3A"), *endCell);	// SheetName : Yasuo, Cells : B2:H7 (%3A 가 : 와 같음)	
	FString Url = FString::Printf(TEXT("https://sheets.googleapis.com/v4/spreadsheets/%s/values/%s?key=%s"), *GoogleSheetID, *GoogleSheetRange, *ApiKey);

	Request->SetURL(Url);
	Request->SetVerb("GET");
	Request->SetHeader(TEXT("Content-Type"), TEXT("application/json"));

	// Request 완료 시 콜백 설정
	//Request->OnProcessRequestComplete().BindUObject(this, &UCDataSheetUtility::OnResponseReceived<T>);
	 Request->OnProcessRequestComplete().BindLambda(
	 		[this, &OutDataMap](FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
	 				{
	 					this->OnResponseReceived<T>(Request, Response, bWasSuccessful, OutDataMap);
	 				}
				);

	// Request 실행
	Request->ProcessRequest();
	UE_LOG(LogTemp, Warning, TEXT("UPAHttpDownloadManager Fetching Google Sheet Data"));
}

template<typename T, typename Y>
void UCDataSheetUtility::OnResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful, TMap<Y, T>& OutDataMap)
{
	UE_LOG(LogTemp, Warning, TEXT("UPAHttpDownloadManager OnResponseReceived start"));
	if (bWasSuccessful && Response.IsValid())
	{
		UE_LOG(LogTemp, Warning, TEXT("UPAHttpDownloadManager OnResponseReceived valid"));
		FString ResponseString = Response->GetContentAsString();
		

		// JSON 파싱
		TSharedPtr<FJsonObject> JsonObject;
		TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(ResponseString);

		if (FJsonSerializer::Deserialize(Reader, JsonObject) && JsonObject.IsValid())
		{
			UE_LOG(LogTemp, Warning, TEXT("UPAHttpDownloadManager OnResponseReceived deserialized"));
			const TArray<TSharedPtr<FJsonValue>>* Rows; // Json배열 형태 [ ["ID", "ProjectileCount"], ["int32", "int32"], ["1", "2"], ... ] 구조

			if (JsonObject->TryGetArrayField(TEXT("values"), Rows))
			{

				UE_LOG(LogTemp, Warning, TEXT("UPAHttpDownloadManager getarrayfield"));

				PaserSheetRowsToStructArray(Rows, OutDataMap); // 행 데이터들로 구조체를 만들어 저장한다
            
				// uint32 Key = 1;
				// for (const TSharedPtr<FJsonValue>& RowValue : *Rows)
				// {
				// 	//RowValue.
				// 	const TArray<TSharedPtr<FJsonValue>>& Row = RowValue->AsArray();
				// 	for (const TSharedPtr<FJsonValue>& Value : Row)
				// 	{
				// 		
				// 		if (Value.IsValid())
				// 		{
				// 			UE_LOG(LogTemp, Log, TEXT("Json Value: %s"), *Value->AsString());
				// 		}
				// 	}
				//
				// 	float Value = FCString::Atof(*Row[1]->AsString());
				//
				// 	//DataMap.Add(Key, Value);
				// 	Key++;
				//
				// 	UE_LOG(LogTemp, Log, TEXT("UPAHttpDownloadManager DataMap added: %d - %f"), Key, Value);
				// 	
				// }

				// 데이터 로그 출력
				// for (const TPair<FString, float>& Pair : DataMap)
				// {
				// 	UE_LOG(LogTemp, Log, TEXT("%s: %f"), *Pair.Key, Pair.Value);
				// }

				OnDataFetched.Broadcast();
			}
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Failed to deserialize JSON response"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to fetch Google Sheet data"));
	}
}


template <typename T, typename Y>
void UCDataSheetUtility::PaserSheetRowsToStructArray(const TArray<TSharedPtr<FJsonValue>>*& Rows, TMap<Y, T>& OutDataMap)
{
	if (!Rows || Rows->Num() < 3)
	{
		UE_LOG(LogTemp, Error, TEXT("Invalid Rows data."));
		return;
	}
	// 1행은 구조체 변수명
	// 변수명 확인하기
	// for (TSharedPtr<FJsonValue> Value : (*Rows)[0]->AsArray())
	// {
	// 	if (Value.IsValid())
	// 	{
	// 		UE_LOG(LogTemp, Log, TEXT("Json Value: %s"), *Value->AsString());
	// 	}
	// }

	const TArray<TSharedPtr<FJsonValue>>& ColumnNames = (*Rows)[0]->AsArray();	// 0행은 칼럼명 (구조체의 변수명)
	const TArray<TSharedPtr<FJsonValue>>& Types = (*Rows)[1]->AsArray();		// 1행은 데이터타입

	// 2행부터 데이터
	const UStruct* StructDef = T::StaticStruct();
	for (int32 RowIndex = 2; RowIndex < Rows->Num(); ++RowIndex)
	{
		const TArray<TSharedPtr<FJsonValue>>& DataRow = (*Rows)[RowIndex]->AsArray(); // 행 데이터
		T StructInstance;

		 // RowIndex번 행의 모든 열에 대해
		for (int32 ColIndex = 0; ColIndex < DataRow.Num(); ++ColIndex)
		{
			if ( !ColumnNames.IsValidIndex(ColIndex) || !Types.IsValidIndex(ColIndex))
			{
				continue; //  0, 1행에 없는 칼럼이면 continue;
			}

			FName ColumnName = *(ColumnNames[ColIndex]->AsString());
			FString Type = Types[ColIndex]->AsString();
			FString ValueStr = DataRow[ColIndex]->AsString();

			// if (Type == "bool")
			// {
			// 	FString vStr = ValueStr.ToUpper();
			// 	ValueStr = (vStr == "Y" || vStr == "true") ? TEXT("true") : TEXT("false");
			// }

			 // struct 에서 프로퍼티를 가져온다
			FProperty* Property = StructDef->FindPropertyByName(ColumnName);
			if (!Property)
			{
				UE_LOG(LogTemp, Warning, TEXT("Property %s not found in struct."), *ColumnName.ToString());
				continue;
			}
			
			void* StructMemory = &StructInstance;
			void* ValuePtr = Property->ContainerPtrToValuePtr<void>(StructMemory);
			if (ValuePtr)
			{
				if (!Property->ImportText_Direct(*ValueStr, ValuePtr, 0, 0))
				{
					UE_LOG(LogTemp, Warning, TEXT("Failed to import %s"), *ValueStr);
				}
			}
		}

		Y ID;
		if constexpr (std::is_same<Y, int32>::value)
			ID = FCString::Atoi(*DataRow[0]->AsString());
		else if constexpr (std::is_same<Y, FString>::value)
			ID = *DataRow[0]->AsString();
		
		OutDataMap.Add(ID, StructInstance);

	}
}