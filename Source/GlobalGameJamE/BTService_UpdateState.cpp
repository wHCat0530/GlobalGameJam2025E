// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_UpdateState.h"
#include "BaseAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIStates.h"

UBTService_UpdateState::UBTService_UpdateState()
{
	NodeName = "更新AI状态";
	Interval = 1.0f; // 每秒更新一次
}

void UBTService_UpdateState::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	ABaseAIController* AICon = Cast<ABaseAIController>(OwnerComp.GetAIOwner());
	if (!AICon) return;

	AICon->PublicUpdateAIState(); // 使用公开方法
}