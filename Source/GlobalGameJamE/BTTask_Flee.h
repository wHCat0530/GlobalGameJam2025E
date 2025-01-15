// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_Flee.generated.h"

/**
 * 
 */
UCLASS()
class GLOBALGAMEJAME_API UBTTask_Flee : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTTask_Flee();

protected:
    virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
