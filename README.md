# Unreal Engine 5 | 3인칭 액션 프로젝트

[![Demo Video](https://img.youtube.com/vi/b_bkvLUsCxQ/0.jpg)](https://youtu.be/b_bkvLUsCxQ)

## 프로젝트 개요
- 이 프로젝트는 언리얼 엔진을 활용한 3인칭 액션 게임 프로토타입으로, 무기 전환, 총기 상태 관리, 근접 공격, 아이템 및 상호작용 시스템을 구현했습니다. 
- 플레이어의 입력과 애니메이션을 자연스럽게 연결하며, 확장 가능한 시스템 설계를 목표로 했습니다.

- 장르: TPS
- 개발 기간: 2025.07 ~ 2025.09 (2개월)
- 개발 인원: 1인
- 개발 환경: Unreal Engine 5.4

## 주요 기능
- **무기 전환 시스템**: Enhanced Input System을 사용해 총기와 근접 무기 간 동적 입력 전환.
- **총기 상태 관리**: 상태 패턴을 활용한 총기 상태(Ready, Aim, Reload) 관리 및 입력 제한.
- **근접 공격**: 공격 중 이동 입력 시 애니메이션 끊김 문제를 해결한 부드러운 전환.
- **아이템 시스템**: `FPrimaryAssetId` 기반의 메타데이터 테이블과 비동기 로딩.
- **이펙트 시스템**: 아이템 사용 시 캐릭터에 적용되는 다양한 이펙트 관리.
- **상호작용 시스템**: 인터페이스를 활용한 일반화된 상호작용 로직.

## 기술 스택
- **엔진**: Unreal Engine 5.4
- **언어**: C++
- **주요 시스템**: Enhanced Input System, AssetManager, Subsystem, Data Table
- **디자인 패턴**: 상태, 델리게이트, 싱글턴

## 상세 설명
[노션 링크](https://wooden-wednesday-dc2.notion.site/Unreal-3-270169b4aa3f808fb5aec70fa133ce4b?source=copy_link)

## 주요 구현 포인트
- 입력 최적화: Enhanced Input System으로 무기별 동적 입력 매핑
- 애니메이션: ExitSection을 활용한 근접 공격 중 이동 전환 최적화
- 아이템 관리: 메타데이터 테이블과 비동기 로딩으로 메모리 효율성 확보
- 확장성: 인터페이스와 컴포넌트를 활용한 모듈화된 상호작용 시스템
