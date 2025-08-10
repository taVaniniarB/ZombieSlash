#pragma once

#include "CoreMinimal.h"
#include "CameraShakeType.generated.h"

UENUM(BlueprintType)
enum class ECameraShakeType : uint8
{
    None        UMETA(DisplayName = "None"),
    WeaponFire  UMETA(DisplayName = "WeaponFire"),
    WeaponSwing UMETA(DisplayName = "WeaponSwing"),
    EnemyDeath  UMETA(DisplayName = "EnemyDeath"),
    PlayerHit   UMETA(DisplayName = "PlayerHit"),
    EnemyHit    UMETA(DisplayName = "EnemyHit"),
    Explosion   UMETA(DisplayName = "Explosion"),
};