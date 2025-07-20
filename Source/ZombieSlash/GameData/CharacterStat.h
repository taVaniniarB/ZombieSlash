#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "CharacterStat.generated.h"

USTRUCT()
struct FCharacterStat : public FTableRowBase
{
	GENERATED_BODY()

public:
	FCharacterStat()
		: MaxHP(0.f), Attack(0.f), AttackRange(0.f), AttackSpeed(0.f), MovementSpeed(0.f) {
	}

	// 구조체 수정 시 유연한 작업 위해 flaot 통일
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat)
	float MaxHP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat)
	float Attack;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat)
	float AttackRange;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat)
	float AttackSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat)
	float MovementSpeed;

	// 구조체 + 구조체 = 두 스탯의 속성별로 각각 더한 새로운 스탯을 반환
	// float의 집합이니, 멤버변수 뭔지 검사하고 더하지 않고 개수 확인해서 일괄적으로 계산 가능하며 새 속성이 추가됐을 때 수정할 필요 없어짐
	FCharacterStat operator +(const FCharacterStat& Other) const
	{
		const float* const ThisPtr = reinterpret_cast<const float* const>(this);
		const float* const OtherPtr = reinterpret_cast<const float* const>(&Other);

		FCharacterStat Result;
		float* ResultPtr = reinterpret_cast<float*>(&Result);
		int32 StatNum = sizeof(FCharacterStat) / sizeof(float);
		for (int32 i = 0; i < StatNum; i++)
		{
			ResultPtr[i] = ThisPtr[i] + OtherPtr[i];
		}
		return Result;
	}
};