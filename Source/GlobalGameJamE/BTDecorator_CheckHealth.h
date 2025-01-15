// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "BTDecorator_CheckHealth.generated.h"

/**
 * 
 */
UCLASS()
class GLOBALGAMEJAME_API UBTDecorator_CheckHealth : public UBTDecorator
{
	GENERATED_BODY()

public:
	UBTDecorator_CheckHealth();

protected:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;

public:
	/** 健康阈值 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	float HealthThreshold;
};
