// Fill out your copyright notice in the Description page of Project Settings.


#include "Utility/CDataSheetUtility.h"

#include "HttpModule.h"
#include "Interfaces/IHttpResponse.h"

UCDataSheetUtility::UCDataSheetUtility()
{
}

void UCDataSheetUtility::FetchGoogleSheetData(const FString& SheetName, const FString& startCell, const FString& endCell)
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
	Request->OnProcessRequestComplete().BindUObject(this, &UCDataSheetUtility::OnResponseReceived);

	// Request 실행
	Request->ProcessRequest();
	UE_LOG(LogTemp, Warning, TEXT("UPAHttpDownloadManager Fetching Google Sheet Data"));
}

void UCDataSheetUtility::OnResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
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
			const TArray<TSharedPtr<FJsonValue>>* Rows;

			if (JsonObject->TryGetArrayField(TEXT("values"), Rows))
			{

				UE_LOG(LogTemp, Warning, TEXT("UPAHttpDownloadManager getarrayfield"));
            
				uint32 Key = 1;
				for (const TSharedPtr<FJsonValue>& RowValue : *Rows)
				{
					const TArray<TSharedPtr<FJsonValue>>& Row = RowValue->AsArray();
					for (const TSharedPtr<FJsonValue>& Value : Row)
					{
						if (Value.IsValid())
						{
							UE_LOG(LogTemp, Log, TEXT("Json Value: %s"), *Value->AsString());
						}
					}

					//if (Row.Num() == 2)
					//{
						//FString Key = Row[0]->AsString();
						float Value = FCString::Atof(*Row[1]->AsString());

						//DataMap.Add(Key, Value);
						Key++;

						UE_LOG(LogTemp, Log, TEXT("UPAHttpDownloadManager DataMap added: %d - %f"), Key, Value);
					//}
				}

				// 데이터 로그 출력
				for (const TPair<FString, float>& Pair : DataMap)
				{
					UE_LOG(LogTemp, Log, TEXT("%s: %f"), *Pair.Key, Pair.Value);
				}

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
