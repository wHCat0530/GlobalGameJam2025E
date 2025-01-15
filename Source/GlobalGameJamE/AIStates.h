// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"

UENUM(BlueprintType)
enum class EAIState : uint8
{
    Inactive     UMETA(DisplayName = "未激活"),
    Patrol       UMETA(DisplayName = "随机寻路"),
    Alert        UMETA(DisplayName = "警戒"),
    Tracking     UMETA(DisplayName = "追踪"),
    Combat       UMETA(DisplayName = "进入战斗"),
    Fleeing      UMETA(DisplayName = "逃离")
};
