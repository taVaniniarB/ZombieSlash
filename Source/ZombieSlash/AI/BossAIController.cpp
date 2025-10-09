// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BossAIController.h"
#include "Kismet/GameplayStatics.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "ZSAI.h"
#include "Interface/CharacterAIInterface.h"

ABossAIController::ABossAIController()
    : CachedPlayer(nullptr)
{
	PrimaryActorTick.bCanEverTick = true;
    DetectState = EDetectState::Unaware;
}

void ABossAIController::BeginPlay()
{
	Super::BeginPlay();

	CachedPlayer = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	if (CachedPlayer)
	{
		GetBlackboardComponent()->SetValueAsObject(BBKEY_TARGET, CachedPlayer);
	}

	// 감지 타이머 시작
	GetWorldTimerManager().SetTimer(
		DetectionTimer,
		this,
		&ABossAIController::UpdatePlayerDetection,
		DetectionUpdateInterval,
		true
	);
}

void ABossAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	DrawDebugVisualization();
}

bool ABossAIController::IsPlayerInDetectionCone(AActor* Player)
{
    if (!Player || !GetPawn()) return false;

    FVector DetectionOrigin = GetDetectionOrigin();
    FVector PlayerLocation = Player->GetActorLocation();

    // 1. 거리 체크
    float Distance = FVector::Dist(DetectionOrigin, PlayerLocation);
    if (Distance > GetDetectRange()) return false;

    // 2. 각도 체크
    FVector PawnForward = GetPawn()->GetActorForwardVector();
    FVector DirectionToPlayer = (PlayerLocation - DetectionOrigin).GetSafeNormal();

    float DotProduct = FVector::DotProduct(PawnForward, DirectionToPlayer);
    float AngleInRadians = FMath::Acos(FMath::Clamp(DotProduct, -1.0f, 1.0f));
    float AngleInDegrees = FMath::RadiansToDegrees(AngleInRadians);

    if (AngleInDegrees > (GetDetectAngle() * 0.5f)) return false;

    // 3. 라인 오브 사이트 체크
    return LineTraceToTarget(DetectionOrigin, PlayerLocation, Player);
}

TArray<FVector> ABossAIController::GetDetectionConePoints()
{
    TArray<FVector> ConePoints;

    if (!GetPawn()) return ConePoints;

    FVector DetectionOrigin = GetDetectionOrigin();
    FVector PawnForward = GetPawn()->GetActorForwardVector();
    float HalfAngle = GetDetectAngle() * 0.5f;

    for (int32 i = 0; i < RaycastCount; i++)
    {
        float CurAngle = -HalfAngle + (HalfAngle * 2.0f * i / (RaycastCount - 1));
        FVector Direction = PawnForward.RotateAngleAxis(CurAngle, FVector::UpVector);
        FVector EndPoint = DetectionOrigin + Direction * GetDetectRange();

        FHitResult HitResult;
        FCollisionQueryParams QueryParams;
        QueryParams.AddIgnoredActor(GetPawn());
        QueryParams.bTraceComplex = true;

        bool bHit = GetWorld()->LineTraceSingleByChannel(
            HitResult,
            DetectionOrigin,
            EndPoint,
            CollisionChannel,
            QueryParams
        );

        ConePoints.Add(bHit ? HitResult.Location : EndPoint);
    }

    return ConePoints;
}

void ABossAIController::UpdatePlayerDetection()
{
    if (!CachedPlayer || !GetBlackboardComponent()) return;

    bool bCanSeePlayer = IsPlayerInDetectionCone(CachedPlayer);
    FVector PlayerLocation = CachedPlayer->GetActorLocation();

    // 블랙보드 업데이트
    GetBlackboardComponent()->SetValueAsBool(CanSeePlayerKey, bCanSeePlayer);
    GetBlackboardComponent()->SetValueAsVector(PlayerLocationKey, PlayerLocation);

    if (bCanSeePlayer)
    {
        // 마지막으로 본 위치 업데이트
        GetBlackboardComponent()->SetValueAsVector(LastKnownLocationKey, PlayerLocation);

        DetectState = EDetectState::Detect;
        GetBlackboardComponent()->SetValueAsEnum(BBKEY_DETACTSTATE, uint8(DetectState));
    }
}

void ABossAIController::DrawDebugVisualization()
{
    if (!GetPawn() || !GetWorld()) return;

    FVector DetectionOrigin = GetDetectionOrigin();
    TArray<FVector> ConePoints = GetDetectionConePoints();

    if (ConePoints.Num() == 0) return;

    // 실제 감지 영역 (녹색)
    for (int32 i = 0; i < ConePoints.Num() - 1; i++)
    {
        DrawDebugLine(GetWorld(), ConePoints[i], ConePoints[i + 1], FColor::Green, false, -1, 0, 2.0f);
    }

    if (ConePoints.Num() > 0)
    {
        DrawDebugLine(GetWorld(), DetectionOrigin, ConePoints[0], FColor::Green, false, -1, 0, 2.0f);
        DrawDebugLine(GetWorld(), DetectionOrigin, ConePoints.Last(), FColor::Green, false, -1, 0, 2.0f);
    }

    // 최대 감지 범위 (빨간색)
    float HalfAngle = GetDetectAngle() * 0.5f;
    FVector PawnForward = GetPawn()->GetActorForwardVector();
    FVector LeftBoundary = PawnForward.RotateAngleAxis(-HalfAngle, FVector::UpVector) * GetDetectRange();
    FVector RightBoundary = PawnForward.RotateAngleAxis(HalfAngle, FVector::UpVector) * GetDetectRange();

    DrawDebugLine(GetWorld(), DetectionOrigin, DetectionOrigin + LeftBoundary, FColor::Red, false, -1, 0, 1.0f);
    DrawDebugLine(GetWorld(), DetectionOrigin, DetectionOrigin + RightBoundary, FColor::Red, false, -1, 0, 1.0f);


    // 플레이어 감지 상태 표시
    /*if (CachedPlayer && IsPlayerInDetectionCone(CachedPlayer))
    {
        FVector PlayerLocation = CachedPlayer->GetActorLocation();
        DrawDebugLine(GetWorld(), DetectionOrigin, PlayerLocation, FColor::Yellow, false, -1, 0, 3.0f);
        DrawDebugSphere(GetWorld(), PlayerLocation, 50.0f, 8, FColor::Yellow, false, -1, 0, 2.0f);
    }*/
}

bool ABossAIController::LineTraceToTarget(const FVector& Start, const FVector& End, AActor* TargetActor)
{
    if (!GetWorld()) return false;

    FHitResult HitResult;
    FCollisionQueryParams QueryParams;
    QueryParams.AddIgnoredActor(GetPawn());
    QueryParams.bTraceComplex = true;

    bool bHit = GetWorld()->LineTraceSingleByChannel(
        HitResult,
        Start,
        End,
        CollisionChannel,
        QueryParams
    );

    return !bHit || HitResult.GetActor() == TargetActor;
}

FVector ABossAIController::GetDetectionOrigin()
{
    if (APawn* ControlledPawn = GetPawn())
    {
        return ControlledPawn->GetActorLocation();
    }
    return FVector::ZeroVector;
}

UObject* ABossAIController::GetTarget()
{
    return GetBlackboardComponent()->GetValueAsObject(BBKEY_TARGET);
}

float ABossAIController::GetDetectRange()
{
    ICharacterAIInterface* AIPawn = Cast<ICharacterAIInterface>(GetPawn());
    return AIPawn->GetAIDetectRange();
}

float ABossAIController::GetDetectAngle()
{
    ICharacterAIInterface* AIPawn = Cast<ICharacterAIInterface>(GetPawn());
    return AIPawn->GetAIDetectAngle();
}
