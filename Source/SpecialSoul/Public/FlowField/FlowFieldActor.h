// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FlowFieldActor.generated.h"

USTRUCT()
struct FFlowFieldCell
{
	GENERATED_BODY()

	FVector WorldPosition {FVector::ZeroVector}; // 셀의 월드 좌표
	
	float MoveCost {1.f}; // 이동 비용 (Cost Field)
	float IntegrationCost = MAX_FLT; // 타겟까지의 누적 비용 (Integration Field)
	FVector2D FlowDirection {FVector2D::ZeroVector}; // 방향(Flow Field)
};

UCLASS()
class SPECIALSOUL_API AFlowFieldActor : public AActor
{
	GENERATED_BODY()
	
public:	
	AFlowFieldActor();
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;

public:	

	UPROPERTY(EditAnywhere, Category = FlowField)
	int32 GridWidth {100};

	UPROPERTY(EditAnywhere, Category = FlowField)
	int32 GridHeight {100};

	UPROPERTY(EditAnywhere, Category = FlowField)
	float TileSize {100.f}; // 1m

	UPROPERTY(EditAnywhere, Category = FlowField)
	TArray<FIntPoint> TargetPoints; // 타겟(플레이어)의 2D 좌표
	
	UPROPERTY(EditAnywhere, Category = FlowField)
	TArray<FIntPoint> ObstaclePoints; // 못지나가는 장애물들의 2D좌표

	// 에디터에서 호출
	UFUNCTION(CallInEditor, Category = FlowField)
	void GenerateField();

	UPROPERTY(EditAnywhere, Category = FlowField)
	float GenerationUpdateTime {0.2}; // 0.2초마다 갱신

	UPROPERTY(EditAnywhere, Category = FlowFieldDebug)
	bool bShowCostField {false};
	UPROPERTY(EditAnywhere, Category = FlowFieldDebug)
	bool bShowIntegrationField {false};
	UPROPERTY(EditAnywhere, Category = FlowFieldDebug)
	bool bShowFlowField {false};
	
	void StartGenTimer();
	void EndGenTimer();

protected:
	
	void InitializeGrid();
	void GenerateCostField();
	void GenerateIntegrationField();
	void GenerateFlowField();

	void FindPlayerPoints();
	void FindObstaclePoints();

	void DrawDebugCostField();
	void DrawDebugIntegrationField();
	void DrawDebugFlowField();

	bool IsValidCell(FIntPoint CellCoord) const;

	FIntPoint WorldToCellCoord(const FVector& WorldPos) const;
	FVector CellToWorldCoord(FIntPoint CellPos) const;

private:
 	 // TMap인 이유) FIntPoint를 key로 넣으면 그에 맞는 Cell을 얻기위해
	 // unordered_map 해시테이블
	TMap<FIntPoint, FFlowFieldCell> Grid;
	 // 12시부터 시계방향
	const TArray<FIntPoint> Directions = {
		{1,0}, {1,1}, {0, 1}, {-1, 1},
		{-1, 0}, {-1, -1}, {0, -1}, {1, -1}
	};
	
	FTimerHandle GenTimer;
};

