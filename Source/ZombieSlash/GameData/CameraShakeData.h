#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "Enums/CameraShakeType.h"
#include "CameraShakeData.generated.h"

USTRUCT(BlueprintType)
struct FCameraShakeData : public FTableRowBase
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera Shake")
    TSubclassOf<UCameraShakeBase> ShakeClass;
};