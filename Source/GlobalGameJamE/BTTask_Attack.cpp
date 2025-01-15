// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_Attack.h"
#include "BaseAIController.h"
#include "BaseAICharacter.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Actor.h"

UBTTask_Attack::UBTTask_Attack()
{
	NodeName = "攻击玩家";
}

EBTNodeResult::Type UBTTask_Attack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	ABaseAIController* AICon = Cast<ABaseAIController>(OwnerComp.GetAIOwner());
	if (!AICon) return EBTNodeResult::Failed;

	ABaseAICharacter* AIChar = Cast<ABaseAICharacter>(AICon->GetPawn());
	if (!AIChar) return EBTNodeResult::Failed;

	AActor* Target = Cast<AActor>(AICon->GetBlackboardComponentPtr()->GetValueAsObject(TEXT("Target")));
	if (Target)
	{
		AIChar->Attack(Target);
		return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::Failed;
}