// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BaseAICharacter.generated.h"

UCLASS()
class GLOBALGAMEJAME_API ABaseAICharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ABaseAICharacter();

protected:
	virtual void BeginPlay() override;

public:	
	/** 健康值 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	float Health;

	/** 行为树 */
	UPROPERTY(EditDefaultsOnly, Category = "AI")
	class UBehaviorTree* BehaviorTree;

	/** 获取行为树 */
	UBehaviorTree* GetBehaviorTree() const { return BehaviorTree; }

	/** 攻击方法 */
	UFUNCTION(BlueprintCallable, Category = "AI")
	void Attack(AActor* Target);

	/** 处理被攻击时的逻辑 */
	UFUNCTION(BlueprintCallable, Category = "AI")
	void ReceiveDamage(float DamageAmount);

	/** 逃离目标的函数 */
	UFUNCTION(BlueprintCallable, Category = "AI")
	void FleeFromTarget(AActor* Target);
};
