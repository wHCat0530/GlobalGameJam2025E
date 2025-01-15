// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseAICharacter.h"
#include "BaseAIController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "NavigationSystem.h"
#include "NavigationPath.h"

ABaseAICharacter::ABaseAICharacter()
{
    PrimaryActorTick.bCanEverTick = true;
    Health = 100.0f;

    // 默认移动速度
    GetCharacterMovement()->MaxWalkSpeed = 300.0f;
}

void ABaseAICharacter::BeginPlay()
{
    Super::BeginPlay();
}

void ABaseAICharacter::Attack(AActor* Target)
{
    if (Target)
    {
        // 实现具体的攻击逻辑，例如播放攻击动画、造成伤害等
        // 这里简单地输出日志和应用伤害
        UE_LOG(LogTemp, Log, TEXT("攻击目标: %s"), *Target->GetName());

        // 示例：应用伤害
        UGameplayStatics::ApplyDamage(Target, 10.0f, GetController(), this, UDamageType::StaticClass());
    }
}

void ABaseAICharacter::ReceiveDamage(float DamageAmount)
{
    Health -= DamageAmount;
    Health = FMath::Clamp(Health, 0.0f, 100.0f);

    // 更新黑板中的健康值
    ABaseAIController* AIController = Cast<ABaseAIController>(GetController());
    if (AIController && AIController->GetBlackboardComponentPtr())
    {
        AIController->GetBlackboardComponentPtr()->SetValueAsFloat(TEXT("Health"), Health);
    }

    if (Health < 15.0f)
    {
        // 触发逃离逻辑，可以通过行为树处理
        // 例如，切换黑板状态
        if (AIController && AIController->GetBlackboardComponentPtr())
        {
            AIController->GetBlackboardComponentPtr()->SetValueAsEnum(TEXT("AIState"), static_cast<uint8>(EAIState::Fleeing));
        }
    }
}

void ABaseAICharacter::FleeFromTarget(AActor* Target)
{
    if (Target)
    {
        // 实现逃离逻辑，例如快速移动到远离目标的位置
        FVector Direction = GetActorLocation() - Target->GetActorLocation();
        Direction.Normalize();
        FVector FleeLocation = GetActorLocation() + Direction * 3000.0f; // 逃离3000码

        // 使用导航系统移动到目标位置
        UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetCurrent(GetWorld());
        if (NavSystem)
        {
            FNavLocation NavLocation;
            if (NavSystem->GetRandomPointInNavigableRadius(FleeLocation, 1000.0f, NavLocation))
            {
                // 获取AIController并调用MoveToLocation
                AAIController* AICon = Cast<AAIController>(GetController());
                if (AICon)
                {
                    AICon->MoveToLocation(NavLocation.Location);
                }
            }
        }
    }
}