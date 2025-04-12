// Fill out your copyright notice in the Description page of Project Settings.


#include "PathFinding//FlowFieldActor.h"
#include "Kismet/GameplayStatics.h"
#include "Player/CBasePlayer.h"

#define ECC_FF_Obstacle ECC_GameTraceChannel6

AFlowFieldActor::AFlowFieldActor()
{
	PrimaryActorTick.bCanEverTick = true;

}

void AFlowFieldActor::BeginPlay()
{
	Super::BeginPlay();

	StartGenTimer();
	
	FindObstaclePoints();
	InitializeGrid();
}

FVector2D AFlowFieldActor::FindFlowDirection(const FVector& WorldPos) const
{
	FIntPoint CellPos = WorldToCellCoord(WorldPos);
	if (!IsValidCell(CellPos))
		return FVector2D::ZeroVector;

	const FFlowFieldCell& FlowDir = Grid[CellPos];
	
	return FlowDir.FlowDirection;
}

void AFlowFieldActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AFlowFieldActor::GenerateField()
{
	FindPlayerPoints();
	FindObstaclePoints();
	
	GenerateCostField();
	GenerateIntegrationField();
	GenerateFlowField();
	
	if (bShowCostField)
		DrawDebugCostField();
	
	if (bShowIntegrationField)
		DrawDebugIntegrationField();
	
	if (bShowFlowField)
		DrawDebugFlowField();
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

		// [-2m, +2m] LineTrace 
		FVector StartPos = CellToWorldCoord(Point) + FVector(0, 0, 1000);
		FVector EndPos = StartPos + FVector(0 , 0, -1100);

		FHitResult Hit;
		FCollisionQueryParams Params;
		Params.bTraceComplex = true; // Complex Collision와도 체크!
		
		bool bHit = GetWorld()->LineTraceSingleByChannel(Hit,
			StartPos, EndPos, ECC_FF_Obstacle, Params);
		
		if (bHit)
		{
			ObstaclePoints.Add(Point);
		}
	}
}

void AFlowFieldActor::InitializeGrid()
{
	Grid.Empty();

	for (int32 y = -(GridHeight/2); y < (GridHeight/2); y++) // 0 ~ 99 -> // -50 ~ 49
	{
		for (int32 x =-(GridWidth/2); x < (GridWidth/2); x++)
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
	for (auto& pair : Grid)
	{
		 //  Pair.Value == FFlowFieldCell
		 // 장애물 -> 255
		 // 일반   -> 1
		FIntPoint& Point = pair.Key;
		FFlowFieldCell& Cell = pair.Value;
		Cell.MoveCost = ObstaclePoints.Contains(Point) ? 255.f : 1.f;
	}
}

void AFlowFieldActor::GenerateIntegrationField()
{
	 // IntegrationField Max로 초기화
	for (auto& pair : Grid)
	{
		pair.Value.IntegrationCost = FLT_MAX;
	}

	 // 타겟 지점들로부터 Queue BFS를 돌린다.
	TQueue<FIntPoint> Queue;

	 // 타겟지점들을 Queue에 넣는다.
	for (FIntPoint Point : TargetPoints)
	{
		if (!IsValidCell(Point)) continue;
		
		Grid[Point].IntegrationCost = 0.f; // 타겟지점은 0
		Queue.Enqueue(Point);
	}

	while (!Queue.IsEmpty())
	{
		FIntPoint CurrPoint;
		Queue.Dequeue(CurrPoint); // Pop하고 Curr에 반환
		float CurrIntegCost = Grid[CurrPoint].IntegrationCost;

		 // 8방향에 대해 확인 
		for (FIntPoint Dir : Directions)
		{
			FIntPoint NextPoint = CurrPoint + Dir;
			if (!IsValidCell(NextPoint)) continue;

			 // 다음 통합비용 = 현지점의 통합비용 + 다음지점 이동비용
			float DiagonalWeight = (FMath::Abs(Dir.X) == 1 && FMath::Abs(Dir.Y) == 1) ? 1.4f : 1.f;
			float NewIntegCost = CurrIntegCost + Grid[NextPoint].MoveCost * DiagonalWeight;

			 // 이전 값보다 작으면 갱신
			if (NewIntegCost < Grid[NextPoint].IntegrationCost)
			{
				Grid[NextPoint].IntegrationCost = NewIntegCost;
				Queue.Enqueue(NextPoint);
			}
		}
		
	}
}

void AFlowFieldActor::GenerateFlowField()
{
	for (auto& Pair : Grid)
	{
		FIntPoint& CurrPoint = Pair.Key;
		FFlowFieldCell& CurrCell = Pair.Value;

		 // 현재 Cell의 Integratin Cost
		float MinIntegCost = FLT_MAX;
		FVector2D BestDir = FVector2D::ZeroVector;

		 // 8방향 중에 어떤 방향으로의 Integration Cost가 가장 적은지 찾는다

		for (FIntPoint DirOffset : Directions)
		{
			FIntPoint NextPoint = CurrPoint + DirOffset;
			if (!IsValidCell(NextPoint)) continue;

			float IntegrationCost = Grid[NextPoint].IntegrationCost;

			 // 통합비용이 더 작은 쪽으로 향한다
			if (IntegrationCost < MinIntegCost)
			{
				MinIntegCost = IntegrationCost;
				BestDir = FVector2D(DirOffset);
			}
		}

		 // Flow Field 방향 정규화
		CurrCell.FlowDirection = BestDir.GetSafeNormal();
	}
}

void AFlowFieldActor::StartGenTimer()
{
	GetWorld()->GetTimerManager().SetTimer(GenTimer, this, &AFlowFieldActor::GenerateField, GenerationUpdateTime,
		true, GenerationUpdateTime);
}

void AFlowFieldActor::EndGenTimer()
{
	GetWorld()->GetTimerManager().ClearTimer(GenTimer);
}

void AFlowFieldActor::DrawDebugFlowField()
{
	// 그리드의 모든 셀의 Flow Direction을 그린다.
	for (auto& Pair : Grid)
	{
		FIntPoint& CurrPoint = Pair.Key;
		FFlowFieldCell& CurrCell = Pair.Value;
		FVector Pos = CurrCell.WorldPosition + FVector(0,0,10);

		FVector Dir = FVector(CurrCell.FlowDirection, 0) * 40.f; // 40cm로 그리기
		FColor ArrowColor = ObstaclePoints.Contains(CurrPoint) ? FColor::Red :
			TargetPoints.Contains(CurrPoint) ? FColor::Green : FColor::Blue;

		DrawDebugDirectionalArrow(GetWorld(), Pos, Pos+Dir, 20.f, ArrowColor, false, GenerationUpdateTime);
	}
	//UE_LOG(LogTemp, Display, TEXT("AFlowFieldActor::DrawDebugFlowField"));
}

void AFlowFieldActor::DrawDebugCostField()
{
	for (auto& Pair : Grid)
	{
		FIntPoint& CurrPoint = Pair.Key;
		FFlowFieldCell& CurrCell = Pair.Value;
		FVector Pos = CurrCell.WorldPosition + FVector(0,0,20);

		uint32 MoveCost = CurrCell.MoveCost;
		FString MoveCostString = FString::Printf(TEXT("%u"),MoveCost);
		DrawDebugString(GetWorld(), Pos, *MoveCostString, nullptr, FColor::Yellow, GenerationUpdateTime * 0.9f, false, 1.f);
	}
	//UE_LOG(LogTemp, Display, TEXT("AFlowFieldActor::DrawDebugCostField"));
}

void AFlowFieldActor::DrawDebugIntegrationField()
{
	for (auto& Pair : Grid)
	{
		FIntPoint& CurrPoint = Pair.Key;
		FFlowFieldCell& CurrCell = Pair.Value;
		FVector Pos = CurrCell.WorldPosition + FVector(0,0,40);

		uint32 IntegrationCost = CurrCell.IntegrationCost;
		FString IntegrationCostString = FString::Printf(TEXT("%u"),IntegrationCost);
		DrawDebugString(GetWorld(), Pos, *IntegrationCostString, nullptr, FColor::Purple, GenerationUpdateTime * 0.9f, false, 1.f);
	}
	//UE_LOG(LogTemp, Display, TEXT("AFlowFieldActor::DrawDebugIntegrationField"));
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
