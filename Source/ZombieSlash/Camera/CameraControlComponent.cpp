// Fill out your copyright notice in the Description page of Project Settings.


#include "Camera/CameraControlComponent.h"
#include "GameData/CameraShakeData.h"
#include "Subsystem/GameplayEventSubsystem.h"
#include "Engine/Datatable.h"

UCameraControlComponent::UCameraControlComponent()
{
    static ConstructorHelpers::FObjectFinder<UDataTable> CameraShakeTalbleRef(TEXT("/Script/Engine.DataTable'/Game/GameData/CameraShakeDataTable.CameraShakeDataTable'"));
    if (nullptr != CameraShakeTalbleRef.Object)
    {
        CameraShakeDataTable = CameraShakeTalbleRef.Object;
    }
}

void UCameraControlComponent::BeginPlay()
{
    Super::BeginPlay();

    UGameplayEventSubsystem::Get(this)->OnCameraShakeRequested.AddDynamic(this, &UCameraControlComponent::PlayCameraShake);
}

void UCameraControlComponent::PlayCameraShake(ECameraShakeType ShakeType, float Scale)
{
    if (!CameraShakeDataTable)
    {
        UE_LOG(LogTemp, Warning, TEXT("CameraShakeDataTable is not set"));
        return;
    }

    // ECameraShakeType의 DisplayName을 FName으로 변환하고, 이것을 Key로 데이터테이블 조회
    const FName RowName = *UEnum::GetDisplayValueAsText(ShakeType).ToString();
    FCameraShakeData* FoundRow = CameraShakeDataTable->FindRow<FCameraShakeData>(RowName, TEXT("Find Camera Shake Class"));
    if (!FoundRow)
    {
        UE_LOG(LogTemp, Warning, TEXT("FoundRow Fail! RowName: %s"), *RowName.ToString());
        return;
    }

    TSubclassOf<UCameraShakeBase> ShakeClass = FoundRow->ShakeClass;

    if (!ShakeClass) return;

    APawn* OwnerPawn = Cast<APawn>(GetOwner());
    if (OwnerPawn)
    {
        if (APlayerController* PC = Cast<APlayerController>(OwnerPawn->GetController()))
        {
            PC->ClientStartCameraShake(ShakeClass, Scale);
        }
    }    
}