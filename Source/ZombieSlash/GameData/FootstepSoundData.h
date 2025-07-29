#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "FootstepSoundData.generated.h"

USTRUCT(BlueprintType)
struct FFootstepSoundData : public FTableRowBase
{
    GENERATED_BODY()

public:
    // 지형 타입
    UPROPERTY(EditAnywhere, Category = "Footstep")
    TEnumAsByte<EPhysicalSurface> SurfaceType = EPhysicalSurface::SurfaceType_Default;

    // 해당 지형의 발자국 사운드 배열
    UPROPERTY(EditAnywhere, Category = "Footstep")
    TArray<TObjectPtr<USoundBase>> FootstepSounds;
};