// Fill out your copyright notice in the Description page of Project Settings.


#include "BTDecorator_CheckHealth.h"
#include "BaseAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIStates.h"



UBTDecorator_CheckHealth::UBTDecorator_CheckHealth()
{
	HealthThreshold = 15.0f; // 默认15%
}

bool UBTDecorator_CheckHealth::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	ABaseAIController* AICon = Cast<ABaseAIController>(OwnerComp.GetAIOwner());
	if (!AICon) return false;

	float Health = AICon->GetBlackboardComponentPtr()->GetValueAsFloat(TEXT("Health"));
	return Health < HealthThreshold;
}