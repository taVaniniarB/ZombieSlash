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

	// ����ü ���� �� ������ �۾� ���� flaot ����
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

	// ����ü + ����ü = �� ������ �Ӽ����� ���� ���� ���ο� ������ ��ȯ
	// float�� �����̴�, ������� ���� �˻��ϰ� ������ �ʰ� ���� Ȯ���ؼ� �ϰ������� ��� �����ϸ� �� �Ӽ��� �߰����� �� ������ �ʿ� ������
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