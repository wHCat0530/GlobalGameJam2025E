// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "AIStates.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AISenseConfig_Hearing.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BaseAIController.generated.h"

/**
 * 
 */
UCLASS()
class GLOBALGAMEJAME_API ABaseAIController : public AAIController
{
	GENERATED_BODY()

public:
    ABaseAIController();

    virtual void OnPossess(APawn* InPawn) override;
    virtual void OnUnPossess() override;

    /** 获取黑板组件的公共方法 */
    UBlackboardComponent* GetBlackboardComponentPtr() const { return BlackboardComp; }

    /** 获取MaxActiveDistance */
    UFUNCTION(BlueprintCallable, Category = "AI")
    float GetMaxActiveDistance() const { return MaxActiveDistance; }

    /** 公开更新AI状态的方法 */
    UFUNCTION(BlueprintCallable, Category = "AI")
    void PublicUpdateAIState() { UpdateAIState(); }

protected:
    /** 黑板组件 */
    UPROPERTY(transient)
    UBlackboardComponent* BlackboardComp;

    /** 行为树组件 */
    UPROPERTY(transient)
    UBehaviorTreeComponent* BehaviorComp;

    /** 感知组件 */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
    UAIPerceptionComponent* AIPerceptionComp;

    /** 视觉感知配置 */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
    UAISenseConfig_Sight* SightConfig;

    /** 听觉感知配置 */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
    UAISenseConfig_Hearing* HearingConfig;

    /** 当前状态 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI State")
    EAIState CurrentState;

    /** 黑板键名称 */
    static const FName TargetKey;
    static const FName AIStateKey;
    static const FName AlertValueKey;
    static const FName HealthKey;

    /** 状态管理函数 */
    void InitializeBlackboard();

    /** 感知事件处理函数 */
    UFUNCTION()
    void OnPerceptionUpdated(const TArray<AActor*>& UpdatedActors);

    /** 状态转换逻辑 */
    void UpdateAIState();

    /** 参数 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI Parameters")
    float InactiveDistance;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI Parameters")
    float MinActiveDistance;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI Parameters")
    float MaxActiveDistance;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI Parameters")
    float AlertThreshold;

    /** 警戒值 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI State")
    float AlertValue;

    /** 定时器句柄 */
    FTimerHandle AlertTimerHandle;

    /** 增加警戒值的函数 */
    void IncreaseAlertValue(float DeltaTime);
};
