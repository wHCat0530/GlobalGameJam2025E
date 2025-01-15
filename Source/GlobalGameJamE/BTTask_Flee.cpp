// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_Flee.h"
#include "BaseAIController.h"
#include "BaseAICharacter.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "NavigationSystem.h"
#include "NavigationPath.h"

UBTTask_Flee::UBTTask_Flee()
{
	NodeName = "逃离玩家";
}

EBTNodeResult::Type UBTTask_Flee::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	ABaseAIController* AICon = Cast<ABaseAIController>(OwnerComp.GetAIOwner());
	if (!AICon) return EBTNodeResult::Failed;

	ABaseAICharacter* AIChar = Cast<ABaseAICharacter>(AICon->GetPawn());
	if (!AIChar) return EBTNodeResult::Failed;

	AActor* Target = Cast<AActor>(AICon->GetBlackboardComponentPtr()->GetValueAsObject(TEXT("Target")));
	if (!Target) return EBTNodeResult::Failed;

	FVector Direction = AIChar->GetActorLocation() - Target->GetActorLocation();
	Direction.Normalize();

	FVector FleeLocation = AIChar->GetActorLocation() + Direction * 3000.0f; // 逃离3000码

	UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetCurrent(GetWorld());
	if (!NavSystem) return EBTNodeResult::Failed;

	FNavLocation NavLocation;
	bool bFound = NavSystem->GetRandomPointInNavigableRadius(FleeLocation, 1000.0f, NavLocation);

	if (bFound)
	{
		AICon->MoveToLocation(NavLocation.Location);
		return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::Failed;
}