// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_Patrol.h"
#include "BaseAIController.h"
#include "BaseAICharacter.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "NavigationSystem.h"
#include "NavigationPath.h"


UBTTask_Patrol::UBTTask_Patrol()
{
	NodeName = "随机寻路";
}

EBTNodeResult::Type UBTTask_Patrol::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	ABaseAIController* AICon = Cast<ABaseAIController>(OwnerComp.GetAIOwner());
	if (!AICon) return EBTNodeResult::Failed;

	APawn* AIPawn = AICon->GetPawn();
	if (!AIPawn) return EBTNodeResult::Failed;

	UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetCurrent(GetWorld());
	if (!NavSystem) return EBTNodeResult::Failed;

	FVector Origin = AIPawn->GetActorLocation();
	float MaxDistance = AICon->GetMaxActiveDistance(); // 使用公共getter

	FNavLocation RandomLocation;
	bool bFound = NavSystem->GetRandomPointInNavigableRadius(Origin, MaxDistance, RandomLocation);

	if (bFound)
	{
		AICon->MoveToLocation(RandomLocation.Location);
		return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::Failed;
}