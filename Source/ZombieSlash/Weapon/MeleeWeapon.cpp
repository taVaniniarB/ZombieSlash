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
    // 타이머가 유효하다 = 타이머가 돌고 있다 = 이 시점의 입력은 다음 콤보로 넘어갈 수 있는 입력으로 취급
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
    AnimInst->Montage_Play(ComboActionMontage, 1.0f); // 재생할 몽타주, 재생 속도
    FOnMontageEnded EndDelegate;
    EndDelegate.BindUObject(this, &AMeleeWeapon::ComboActionEnd);
    AnimInst->Montage_SetEndDelegate(EndDelegate, ComboActionMontage);
    // 공격 콤보 시작 시, 타이머 핸들을 초기화
    ComboTimerHandle.Invalidate();
    SetComboCheckTimer();
}


void AMeleeWeapon::ComboActionEnd(UAnimMontage* TargetMontage, bool IsProperlyEnded)
{
    ensure(CurCombo != 0); // 이 함수가 호출될 땐 현재 콤보가 0이 아님을 보장
    CurCombo = 0;

    ICharacterWeaponInterface* OwnerCharacter = Cast<ICharacterWeaponInterface>(GetOwner());
    if (AController* PC = OwnerCharacter->GetWeaponOwnerController())
    {
        PC->SetIgnoreMoveInput(false);
    }
}

void AMeleeWeapon::SetComboCheckTimer()
{
    // 현재 몇 콤보인가? (콤보액션 데이터에셋의 인덱스 상)
    int32 ComboIndex = CurCombo - 1;
    ensure(ComboActionData->EffectiveFrameCount.IsValidIndex(ComboIndex));
    const float AttackSpeedRate = 1.0f;
    // 다음 콤보 입력으로 인정해줄 시간(초)
    float ComboEffectiveTime = (ComboActionData->EffectiveFrameCount[ComboIndex] / ComboActionData->FrameRate) / AttackSpeedRate;
    // 이 값이 양수라면, 현재 콤보가 마지막 콤보는 아니었다는 뜻
    if (ComboEffectiveTime > 0.f)
    {
        // 해당 시간 후 특정 함수가 실행되도록 'SetTimer'를 이용한다.
        // SetTimer 사용을 위해 타이머 핸들과 함수가 필요하다.
        GetWorld()->GetTimerManager().SetTimer(ComboTimerHandle, this, &AMeleeWeapon::ComboCheck, ComboEffectiveTime, false);
    }
}

// 타이머 종료 시점에서 호출
void AMeleeWeapon::ComboCheck()
{
    ComboTimerHandle.Invalidate(); // 타이머 핸들 초기화
    if (bHasNextComboCommand) // 타이머가 도는 동안 입력이 들어온 경우
    {
        ICharacterWeaponInterface* OwnerCharacter = Cast<ICharacterWeaponInterface>(GetOwner());

        // 콤보를 1 올린 다음, 몽타주 섹션을 넘겨주고, 다시 타이머를 돌린다
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
    // 원래 애니메이션 노티파이에 의해 호출되는 함수인데
    // NPC도 사용해야 하는 거라 무기에선 호출하지 않는 것이 좋으려나


//    FHitResult OutHitResult;
//    // Trace Tag: 콜리전 분석할 때의 식별자. 수행할 작업에 대해 Attack이라는 태그로 조사할 수 있게 함
//    // Trace Complex: 복잡한 형태(오목 concave)는 올라서는 것만 가능한데, 이런 복잡한 충돌체도 감지할 것인지에 대한 옵션
//    // 무시할 액터 (이 경우, 자기 자신)
//    FCollisionQueryParams Params(SCENE_QUERY_STAT(Attack), false, this);
//
//
//    // 데이터에서 공격 정보 가져오기
//    UMeleeWeaponItemData* MeleeData = GetMeleeWeaponData();
//    if (!MeleeData)
//    {
//        return;
//    }
//
//    // 나머지 로직은 동일하지만, OwnerCharacter를 통해 위치 정보 접근
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