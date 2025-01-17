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
	// ��һ֡��λ��
	FVector LastPosition;
	// �Ƿ��ǵ�һ֡
	bool bIsFirstFrame;
	// ��ʼ���ű���
	FVector InitialMeshScale;
public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// �������
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USphereComponent* SphereComponent;

	// ��̬�������
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* MeshComponent;

	// �����������
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rolling")
	float RollingDistance;

	// ��ǰ��������
	float CurrentRollingDistance;

	// �Ƿ��Ѿ��ŵ�
	bool bHasLanded;

	// �ٶ�
	FVector RollingVelocity;

	// ����ʱ��ˮƽ����
	FVector LaunchDirection;

	// ���÷��䷽��
	void SetLaunchDirection(const FVector& Direction);
	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

};
