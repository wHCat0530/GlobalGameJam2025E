// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SphereComponent.h"
#include "GameFramework/Actor.h"
#include "Snowball.generated.h"

UCLASS()
class GLOBALGAMEJAME_API ASnowball : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ASnowball();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
private:
	// 上一帧的位置
	FVector LastPosition;
	// 是否是第一帧
	bool bIsFirstFrame;
	// 初始缩放比例
	FVector InitialMeshScale;
public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// 球体组件
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USphereComponent* SphereComponent;

	// 静态网格组件
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* MeshComponent;

	// 滚动距离控制
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rolling")
	float RollingDistance;

	// 当前滚动距离
	float CurrentRollingDistance;

	// 是否已经着地
	bool bHasLanded;

	// 速度
	FVector RollingVelocity;

	// 发射时的水平方向
	FVector LaunchDirection;

	// 设置发射方向
	void SetLaunchDirection(const FVector& Direction);
	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

};
