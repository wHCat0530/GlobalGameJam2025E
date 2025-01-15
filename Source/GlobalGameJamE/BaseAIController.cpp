// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BaseAICharacter.h"
#include "Perception/AISense_Sight.h"
#include "Perception/AISense_Hearing.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"
#include "NavigationSystem.h"
#include "NavigationPath.h"

const FName ABaseAIController::TargetKey(TEXT("Target"));
const FName ABaseAIController::AIStateKey(TEXT("AIState"));
const FName ABaseAIController::AlertValueKey(TEXT("AlertValue"));
const FName ABaseAIController::HealthKey(TEXT("Health"));

ABaseAIController::ABaseAIController()
{
    // 初始化行为树组件
    BehaviorComp = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("BehaviorComp"));

    // 初始化黑板组件
    BlackboardComp = CreateDefaultSubobject<UBlackboardComponent>(TEXT("BlackboardComp"));

    // 初始化感知组件
    AIPerceptionComp = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerceptionComp"));

    // 配置视觉感知
    SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("SightConfig"));
    SightConfig->SightRadius = 5000.0f;
    SightConfig->LoseSightRadius = 5500.0f;
    SightConfig->PeripheralVisionAngleDegrees = 90.0f;
    SightConfig->SetMaxAge(5.0f);
    SightConfig->AutoSuccessRangeFromLastSeenLocation = 1200.0f;
    SightConfig->DetectionByAffiliation.bDetectEnemies = true;
    SightConfig->DetectionByAffiliation.bDetectFriendlies = true;
    SightConfig->DetectionByAffiliation.bDetectNeutrals = true;
    AIPerceptionComp->ConfigureSense(*SightConfig);
    AIPerceptionComp->SetDominantSense(SightConfig->GetSenseImplementation());

    // 配置听觉感知
    HearingConfig = CreateDefaultSubobject<UAISenseConfig_Hearing>(TEXT("HearingConfig"));
    HearingConfig->HearingRange = 3000.0f;
    HearingConfig->DetectionByAffiliation.bDetectEnemies = true;
    HearingConfig->DetectionByAffiliation.bDetectFriendlies = true;
    HearingConfig->DetectionByAffiliation.bDetectNeutrals = true;
    AIPerceptionComp->ConfigureSense(*HearingConfig);

    // 绑定感知更新事件
    AIPerceptionComp->OnPerceptionUpdated.AddDynamic(this, &ABaseAIController::OnPerceptionUpdated);

    // 初始化参数
    InactiveDistance = 5000.0f;
    MinActiveDistance = 1000.0f;
    MaxActiveDistance = 5000.0f;
    AlertThreshold = 100.0f;
    AlertValue = 0.0f;

    CurrentState = EAIState::Inactive;
}

void ABaseAIController::OnPossess(APawn* InPawn)
{
    Super::OnPossess(InPawn);

    // 获取黑板和行为树
    ABaseAICharacter* AICharacter = Cast<ABaseAICharacter>(InPawn);
    if (AICharacter && AICharacter->BehaviorTree)
    {
        BlackboardComp->InitializeBlackboard(*AICharacter->BehaviorTree->BlackboardAsset);
        BehaviorComp->StartTree(*AICharacter->BehaviorTree);
    }

    InitializeBlackboard();
}

void ABaseAIController::OnUnPossess()
{
    Super::OnUnPossess();
}

void ABaseAIController::InitializeBlackboard()
{
    if (BlackboardComp)
    {
        BlackboardComp->SetValueAsEnum(AIStateKey, static_cast<uint8>(CurrentState));
        BlackboardComp->SetValueAsFloat(AlertValueKey, AlertValue);
        BlackboardComp->SetValueAsFloat(HealthKey, 100.0f); // 初始健康值为100
    }
}

void ABaseAIController::OnPerceptionUpdated(const TArray<AActor*>& UpdatedActors)
{
    // 处理感知到的目标
    for (auto& Actor : UpdatedActors)
    {
        if (Actor->IsA(ACharacter::StaticClass()))
        {
            // 假设玩家是ACharacter的子类
            SetFocus(Actor);
            BlackboardComp->SetValueAsObject(TargetKey, Actor);
            // 更新状态
            UpdateAIState();
            break;
        }
    }
}

void ABaseAIController::UpdateAIState()
{
    // 获取目标
    AActor* Target = Cast<AActor>(BlackboardComp->GetValueAsObject(TargetKey));
    if (!Target)
    {
        CurrentState = EAIState::Inactive;
        BlackboardComp->SetValueAsEnum(AIStateKey, static_cast<uint8>(CurrentState));
        return;
    }

    // 计算距离
    APawn* AIPawn = GetPawn();
    if (!AIPawn) return;

    float Distance = FVector::Dist(AIPawn->GetActorLocation(), Target->GetActorLocation());

    // 根据距离和警戒值更新状态
    if (Distance > InactiveDistance)
    {
        CurrentState = EAIState::Inactive;
    }
    else if (Distance > MinActiveDistance && Distance <= MaxActiveDistance)
    {
        CurrentState = EAIState::Patrol;
    }
    else if (Distance <= MinActiveDistance)
    {
        // 更新警戒值
        float NewAlertValue = AlertValue;

        if (Distance <= 500.0f)
        {
            NewAlertValue += 50.0f * GetWorld()->GetDeltaSeconds(); // 每秒增加50
        }
        else if (Distance <= 800.0f)
        {
            // 每2秒增加10 => 每秒增加5
            NewAlertValue += 5.0f * GetWorld()->GetDeltaSeconds();
        }
        else if (Distance <= 1000.0f)
        {
            // 每秒增加10
            NewAlertValue += 10.0f * GetWorld()->GetDeltaSeconds();
        }

        AlertValue = FMath::Clamp(NewAlertValue, 0.0f, AlertThreshold + 50.0f); // 防止溢出

        BlackboardComp->SetValueAsFloat(AlertValueKey, AlertValue);

        if (AlertValue >= AlertThreshold)
        {
            CurrentState = EAIState::Tracking;
        }
        else
        {
            CurrentState = EAIState::Alert;
        }
    }

    BlackboardComp->SetValueAsEnum(AIStateKey, static_cast<uint8>(CurrentState));
}

void ABaseAIController::IncreaseAlertValue(float DeltaTime)
{
    // 该函数可用于定时增加警戒值，如果需要使用定时器
    // 目前未使用，可根据需要实现
}