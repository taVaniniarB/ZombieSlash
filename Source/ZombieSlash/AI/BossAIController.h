// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AI/ZSAIController.h"
#include "BossAIController.generated.h"

UENUM(BlueprintType)
enum class EDetectState : uint8
{
    Unaware UMETA(DisplayName = "Unaware"),
    Suspicious UMETA(DisplayName = "Suspicious"),
    Detect UMETA(DisplayName = "Detect")
};

/**
 * 
 */
UCLASS()
class ZOMBIESLASH_API ABossAIController : public AZSAIController
{
	GENERATED_BODY()
	
public:
	ABossAIController();

public:
    UObject* GetTarget();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Detection")
	int32 RaycastCount = 20;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Detection")
    TEnumAsByte<ECollisionChannel> CollisionChannel = ECC_Visibility;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Detection")
    float DetectionUpdateInterval = 0.1f;

    // 블랙보드 키 이름들
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Blackboard Keys")
    FName PlayerLocationKey = TEXT("PlayerLocation");

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Blackboard Keys")
    FName CanSeePlayerKey = TEXT("CanSeePlayer");

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Blackboard Keys")
    FName LastKnownLocationKey = TEXT("LastKnownLocation");

private:
    bool IsPlayerInDetectionCone(AActor* Player);
    TArray<FVector> GetDetectionConePoints();
    void UpdatePlayerDetection();
    void DrawDebugVisualization();
    bool LineTraceToTarget(const FVector& Start, const FVector& End, AActor* TargetActor);
    FVector GetDetectionOrigin();

    FTimerHandle DetectionTimer;

    UPROPERTY()
    AActor* CachedPlayer;

    UPROPERTY(VisibleInstanceOnly)
    EDetectState DetectState;

private:
    float GetDetectRange();
    float GetDetectAngle();
};
