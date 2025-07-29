#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "FootstepSoundData.generated.h"

USTRUCT(BlueprintType)
struct FFootstepSoundData : public FTableRowBase
{
    GENERATED_BODY()

public:
    // ���� Ÿ��
    UPROPERTY(EditAnywhere, Category = "Footstep")
    TEnumAsByte<EPhysicalSurface> SurfaceType = EPhysicalSurface::SurfaceType_Default;

    // �ش� ������ ���ڱ� ���� �迭
    UPROPERTY(EditAnywhere, Category = "Footstep")
    TArray<TObjectPtr<USoundBase>> FootstepSounds;
};