#pragma once

#include "CoreMinimal.h"

// 충돌 프로필 매크로
#define CPROFILE_CHARCAPSULE TEXT("CharacterCapsule")
#define CPROFILE_ZSTRIGGER TEXT("ZSTrigger")

// 엔진 내부에서 인식하는(ini 파일에서 지정된 것) ZSAction 트레이스 채널 enum 매크로
#define CCHANNEL_ZSACTION ECC_GameTraceChannel1