// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/MeleeWeapon.h"
#include "Animation/ComboActionData.h"
#include "Physics/ZSCollision.h"
#include "Engine/DamageEvents.h"
#include "Item/MeleeData.h"
#include "Interface/CharacterWeaponInterface.h"

void AMeleeWeapon::BeginPlay()
{
    Super::BeginPlay();

    const UMeleeData* Melee = GetMeleeWeaponData();
    ComboActionMontage = Melee->ComboActionMontage;
    ComboActionData = Melee->ComboActionData;
}

void AMeleeWeapon::StartAttack()
{
    ProcessComboCommand();
}

void AMeleeWeapon::ProcessComboCommand()
{
    if (CurCombo == 0)
    {
        ComboActionBegin();
        return;
    }
    // Ÿ�̸Ӱ� ��ȿ�ϴ� = Ÿ�̸Ӱ� ���� �ִ� = �� ������ �Է��� ���� �޺��� �Ѿ �� �ִ� �Է����� ���
    if (ComboTimerHandle.IsValid())
    {
        bHasNextComboCommand = true;
    }
    else
    {
        bHasNextComboCommand = false;
    }
}
void AMeleeWeapon::ComboActionBegin()
{
    CurCombo = 1;

    ICharacterWeaponInterface* OwnerCharacter = Cast<ICharacterWeaponInterface>(GetOwner());

    if (AController* PC = OwnerCharacter->GetWeaponOwnerController())
    {
        PC->SetIgnoreMoveInput(true);
    }
    UAnimInstance* AnimInst = OwnerCharacter->GetWeaponOwnerAnimInstance();
    AnimInst->Montage_Play(ComboActionMontage, 1.0f); // ����� ��Ÿ��, ��� �ӵ�
    FOnMontageEnded EndDelegate;
    EndDelegate.BindUObject(this, &AMeleeWeapon::ComboActionEnd);
    AnimInst->Montage_SetEndDelegate(EndDelegate, ComboActionMontage);
    // ���� �޺� ���� ��, Ÿ�̸� �ڵ��� �ʱ�ȭ
    ComboTimerHandle.Invalidate();
    SetComboCheckTimer();
}


void AMeleeWeapon::ComboActionEnd(UAnimMontage* TargetMontage, bool IsProperlyEnded)
{
    ensure(CurCombo != 0); // �� �Լ��� ȣ��� �� ���� �޺��� 0�� �ƴ��� ����
    CurCombo = 0;

    ICharacterWeaponInterface* OwnerCharacter = Cast<ICharacterWeaponInterface>(GetOwner());
    if (AController* PC = OwnerCharacter->GetWeaponOwnerController())
    {
        PC->SetIgnoreMoveInput(false);
    }
}

void AMeleeWeapon::SetComboCheckTimer()
{
    // ���� �� �޺��ΰ�? (�޺��׼� �����Ϳ����� �ε��� ��)
    int32 ComboIndex = CurCombo - 1;
    ensure(ComboActionData->EffectiveFrameCount.IsValidIndex(ComboIndex));
    const float AttackSpeedRate = 1.0f;
    // ���� �޺� �Է����� �������� �ð�(��)
    float ComboEffectiveTime = (ComboActionData->EffectiveFrameCount[ComboIndex] / ComboActionData->FrameRate) / AttackSpeedRate;
    // �� ���� ������, ���� �޺��� ������ �޺��� �ƴϾ��ٴ� ��
    if (ComboEffectiveTime > 0.f)
    {
        // �ش� �ð� �� Ư�� �Լ��� ����ǵ��� 'SetTimer'�� �̿��Ѵ�.
        // SetTimer ����� ���� Ÿ�̸� �ڵ�� �Լ��� �ʿ��ϴ�.
        GetWorld()->GetTimerManager().SetTimer(ComboTimerHandle, this, &AMeleeWeapon::ComboCheck, ComboEffectiveTime, false);
    }
}

// Ÿ�̸� ���� �������� ȣ��
void AMeleeWeapon::ComboCheck()
{
    ComboTimerHandle.Invalidate(); // Ÿ�̸� �ڵ� �ʱ�ȭ
    if (bHasNextComboCommand) // Ÿ�̸Ӱ� ���� ���� �Է��� ���� ���
    {
        ICharacterWeaponInterface* OwnerCharacter = Cast<ICharacterWeaponInterface>(GetOwner());

        // �޺��� 1 �ø� ����, ��Ÿ�� ������ �Ѱ��ְ�, �ٽ� Ÿ�̸Ӹ� ������
        UAnimInstance* AnimInst = OwnerCharacter->GetWeaponOwnerAnimInstance();
        CurCombo = FMath::Clamp(CurCombo + 1, 1, ComboActionData->MaxComboCount);
        FName NextSection = *FString::Printf(TEXT("%s%d"), *ComboActionData->MontageSectionNamePrefix, CurCombo);
        AnimInst->Montage_JumpToSection(NextSection, ComboActionMontage);
        SetComboCheckTimer();
        bHasNextComboCommand = false;
    }
}

void AMeleeWeapon::AttackHitCheck()
{
    // ���� �ִϸ��̼� ��Ƽ���̿� ���� ȣ��Ǵ� �Լ��ε�
    // NPC�� ����ؾ� �ϴ� �Ŷ� ���⿡�� ȣ������ �ʴ� ���� ��������


//    FHitResult OutHitResult;
//    // Trace Tag: �ݸ��� �м��� ���� �ĺ���. ������ �۾��� ���� Attack�̶�� �±׷� ������ �� �ְ� ��
//    // Trace Complex: ������ ����(���� concave)�� �ö󼭴� �͸� �����ѵ�, �̷� ������ �浹ü�� ������ �������� ���� �ɼ�
//    // ������ ���� (�� ���, �ڱ� �ڽ�)
//    FCollisionQueryParams Params(SCENE_QUERY_STAT(Attack), false, this);
//
//
//    // �����Ϳ��� ���� ���� ��������
//    UMeleeWeaponItemData* MeleeData = GetMeleeWeaponData();
//    if (!MeleeData)
//    {
//        return;
//    }
//
//    // ������ ������ ����������, OwnerCharacter�� ���� ��ġ ���� ����
//    const FVector Start = OwnerCharacter->GetActorLocation() +
//        OwnerCharacter->GetActorForwardVector() *
//        (OwnerCharacter->GetCapsuleComponent()->GetScaledCapsuleRadius());
//
//    const FVector End = Start + GetActorForwardVector() * AttackRange;
//    bool HitDetected = GetWorld()->SweepSingleByChannel(
//        OutHitResult, Start, End,
//        FQuat::Identity, CCHANNEL_ZSACTION,
//        FCollisionShape::MakeSphere(AttackRadius), Params);
//
//    if (HitDetected)
//    {
//        float Damage = OwningPlayer->GetTotalStat().Attack;
//        
//        FDamageEvent DamageEvent;
//        OutHitResult.GetActor()->TakeDamage(Damage, DamageEvent, OwnerCharacter->GetController(), this);
//    }
//
//#if ENABLE_DRAW_DEBUG
//    FVector CapsuleOrigin = Start + 0.5 * (End - Start);
//    float CapsuleHalfHeight = (AttackRange * 0.5) + AttackRadius;
//    FColor DrawColor = HitDetected ? FColor::Green : FColor::Red;
//    DrawDebugCapsule(GetWorld(), CapsuleOrigin, CapsuleHalfHeight, AttackRadius,
//        FRotationMatrix::MakeFromZ(GetActorForwardVector()).ToQuat(), DrawColor,
//        false, 3.f);
//#endif
}