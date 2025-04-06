// Fill out your copyright notice in the Description page of Project Settings.


#include "FlowField/FlowFieldActor.h"

#include "SpecialSoul.h"
#include "Kismet/GameplayStatics.h"
#include "Player/CBasePlayer.h"

AFlowFieldActor::AFlowFieldActor()
{
	PrimaryActorTick.bCanEverTick = true;

}

void AFlowFieldActor::BeginPlay()
{
	Super::BeginPlay();

	// 런타임에 한번 호출되도록
	// (에디터에서는 수동 호출)
#if !WITH_EDITOR
	GenerateField();
#endif
}

void AFlowFieldActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// if (bShowCostField)
	// 	DrawDebugCostField();
	// if (bShowIntegrationField)
	// 	DrawDebugIntegrationField();

	GenerateField();
	if (bShowFlowField)
		DrawDebugFlowField();
}

void AFlowFieldActor::GenerateField()
{
GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Flow field")));
	FindPlayerPoints();
	FindObstaclePoints();
	
	InitializeGrid();
	GenerateCostField();
	GenerateIntegrationField();
	GenerateFlowField();
}

void AFlowFieldActor::FindPlayerPoints()
{
	TargetPoints.Empty();

	TArray<AActor*> Players;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACBasePlayer::StaticClass(), Players);

	for (AActor* Player : Players)
	{
		FIntPoint CellPoint = WorldToCellCoord(Player->GetActorLocation());
		TargetPoints.Add(CellPoint);
	}
	
}

void AFlowFieldActor::FindObstaclePoints()
{
	 // LineTrace로 알아낸다
	ObstaclePoints.Empty();

	for (auto& Pair : Grid)
	{
		FIntPoint Point = Pair.Key;
		FFlowFieldCell Cell = Pair.Value;

		// 2m 위에서 쏜다
		FVector StartPos = CellToWorldCoord(Point) + FVector(0, 0, 200);
		FVector EndPos = StartPos + FVector(0 , 0, -400);

		FHitResult Hit;
		FCollisionQueryParams Params;
		Params.bTraceComplex = true; // Complex Collision와도 체크!

		bool bHit = GetWorld()->LineTraceSingleByChannel(Hit,
			StartPos, EndPos, ECC_FlowField, Params);

		if (bHit)
		{
			ObstaclePoints.Add(Point);
		}
	}
}

void AFlowFieldActor::InitializeGrid()
{
	Grid.Empty();

	for (int32 y = 0; y < GridHeight; y++)
	{
		for (int32 x = 0; x < GridWidth; x++)
		{
			FIntPoint Pos(x, y);
			FVector WorldPos = GetActorLocation() + FVector(x*TileSize, y*TileSize, 0);

			FFlowFieldCell Cell;
			Cell.WorldPosition= WorldPos;
			Grid.Add(Pos, Cell);
		}
	}
}

void AFlowFieldActor::GenerateCostField()
{
	for (TTuple<FIntPoint, FFlowFieldCell>& Pair : Grid)
	{
		 //  Pair.Value == FFlowFieldCell
		 // 장애물 -> 255
		 // 일반   -> 1
		Pair.Value.Cost = ObstaclePoints.Contains(Pair.Key) ? 255.f : 1.f;
	}
}

void AFlowFieldActor::GenerateIntegrationField()
{
	 // IntegrationField 초기화
	for (auto& Pair : Grid)
	{
		Pair.Value.Integration = FLT_MAX;
	}

	 // 타겟 지점들로부터 Queue BFS를 돌린다.
	TQueue<FIntPoint> Queue;

	 // 타겟지점들을 Queue에 넣는다.
	for (FIntPoint Point : TargetPoints)
	{
		if (!IsValidCell(Point)) continue;
		Grid[Point].Integration = 0.f; // 타겟지점은 0
		Queue.Enqueue(Point);
	}

	while (!Queue.IsEmpty())
	{
		FIntPoint CurrPoint;
		Queue.Dequeue(CurrPoint); // Pop하고 Curr에 반환
		float CurrCost = Grid[CurrPoint].Integration;

		 // 8방향에 대해 확인 
		for (FIntPoint Dir : Directions)
		{
			FIntPoint NextPoint = CurrPoint + Dir;
			if (!IsValidCell(NextPoint)) continue;

			float NewIntegCost = CurrCost + Grid[NextPoint].Cost;

			 // 이전 값보다 작으면 갱신
			if (NewIntegCost >= Grid[NextPoint].Integration)
				continue;

			Grid[NextPoint].Integration = NewIntegCost;
			Queue.Enqueue(NextPoint);
		}
	}
}

void AFlowFieldActor::GenerateFlowField()
{
	for (auto& Pair : Grid)
	{
		FIntPoint CurrPoint = Pair.Key;
		FFlowFieldCell CurrCell = Pair.Value;

		 // 현재 Cell의 Integratin Cost
		float MinCost = CurrCell.Integration;
		FVector2D BestDir = FVector2D::ZeroVector;

		 // 8방향 중에 어떤 방향으로의 Integration Cost가 가장 적은지 찾는다

		for (FIntPoint DirOffset : Directions)
		{
			FIntPoint NextPoint = CurrPoint + DirOffset;
			if (!IsValidCell(NextPoint)) continue;

			float IntegrationCost = Grid[NextPoint].Integration;
			if (IntegrationCost < MinCost)
			{
				MinCost = IntegrationCost;
				BestDir = FVector2D(DirOffset);
			}
		}

		 // Flow Field 방향 정규화
		CurrCell.FlowDirection = BestDir.GetSafeNormal();
	}
}

void AFlowFieldActor::DrawDebugFlowField()
{
	// 그리드의 모든 셀의 Flow Direction을 그린다.
	for (auto& Pair : Grid)
	{
		FIntPoint CurrPoint = Pair.Key;
		FFlowFieldCell CurrCell = Pair.Value;
		FVector Pos = CurrCell.WorldPosition + FVector(0,0,10);

		FVector Dir = FVector(CurrCell.FlowDirection, 0) * 40.f; // 40cm로 그리기
		FColor ArrowColor = ObstaclePoints.Contains(CurrPoint) ? FColor::Red :
			TargetPoints.Contains(CurrPoint) ? FColor::Blue : FColor::Green;

		DrawDebugDirectionalArrow(GetWorld(), Pos, Pos+Dir, 20.f, ArrowColor);
		
	}
}

bool AFlowFieldActor::IsValidCell(FIntPoint Coord) const
{
	return Grid.Contains(Coord);
}

FIntPoint AFlowFieldActor::WorldToCellCoord(const FVector& WorldPos) const
{
	 // Grid에서의 WorldPos의 local좌표 
	FVector LocalPos = WorldPos - GetActorLocation();
	return FIntPoint(
		FMath::FloorToInt(LocalPos.X / TileSize),
		FMath::FloorToInt(LocalPos.Y / TileSize)
		);
}


FVector AFlowFieldActor::CellToWorldCoord(FIntPoint CellPos) const
{
	return GetActorLocation() +
		FVector(CellPos.X * TileSize, CellPos.Y * TileSize , 0.f);
}
