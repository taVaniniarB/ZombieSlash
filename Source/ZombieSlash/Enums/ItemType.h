#pragma once

#include "CoreMinimal.h"
#include "ItemType.generated.h"

UENUM(BlueprintType)
enum class EItemType : uint8
{
	Weapon = 0,
	Ammo,
	Usable,
	Quest,
	None,
};