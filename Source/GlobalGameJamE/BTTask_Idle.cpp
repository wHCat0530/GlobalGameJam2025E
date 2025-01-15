// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_Idle.h"
#include "BaseAIController.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_Idle::UBTTask_Idle()
{
	NodeName = "保持未激活状态";
}

EBTNodeResult::Type UBTTask_Idle::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	// AI保持未激活状态，不进行任何操作
	// 这里可以选择让AI停下或者做一些空动作
	// For simplicity, just return Succeeded
	return EBTNodeResult::Succeeded;
}