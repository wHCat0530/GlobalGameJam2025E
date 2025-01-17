// Fill out your copyright notice in the Description page of Project Settings.


#include "Snowball.h"
#include "Components/SphereComponent.h"

// Sets default values
ASnowball::ASnowball()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	// 初始化球体组件
	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	SphereComponent->InitSphereRadius(15.0f);
	RootComponent = SphereComponent;

	// 初始化网格组件
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	MeshComponent->SetupAttachment(RootComponent);

	// 启用物理模拟
	SphereComponent->SetSimulatePhysics(true);
	SphereComponent->SetNotifyRigidBodyCollision(true);
	SphereComponent->SetEnableGravity(true); // 启用重力
	SphereComponent->SetMassOverrideInKg(NAME_None, 1.0f); // 设置质量为1kg，确保物理效果
	// 设置碰撞属性
	SphereComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics); // 启用碰撞查询和物理响应
	SphereComponent->SetCollisionObjectType(ECC_GameTraceChannel1); // 使用自定义碰撞通道，为 Projectile
	SphereComponent->SetCollisionResponseToAllChannels(ECR_Ignore); // 默认忽略所有通道
	SphereComponent->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block); // 阻挡静态世界对象（如地面、墙壁）
	SphereComponent->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore); // 忽略Pawn（玩家角色），避免与自己发生碰撞
	// 绑定碰撞事件
	SphereComponent->OnComponentHit.AddDynamic(this, &ASnowball::OnHit);

	// 初始化变量
	RollingDistance = 10.0f; // 可以根据需要调整
	CurrentRollingDistance = 0.0f;
	bHasLanded = false;
	RollingVelocity = FVector::ZeroVector;

	// 初始化轨迹相关变量
	bIsFirstFrame = true;

	// 初始化初始缩放比例为1.0
	InitialMeshScale = FVector(1.0f, 1.0f, 1.0f);
}

// Called when the game starts or when spawned
void ASnowball::BeginPlay()
{
	Super::BeginPlay();
	if (MeshComponent)
	{
		// 保存网格体的初始缩放比例
		InitialMeshScale = MeshComponent->GetRelativeScale3D();
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("MeshComponent is null in BeginPlay."));
	}
}

// Called every frame
void ASnowball::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	// 记录当前帧的位置
	FVector CurrentPosition = GetActorLocation();
	if (!bHasLanded)
	{
		if (!bIsFirstFrame)
		{
			// 绘制从上一帧到当前帧的线段
			DrawDebugLine(
				GetWorld(),
				LastPosition,
				CurrentPosition,
				FColor::Green,
				false,
				1.0f,
				0,
				2.0f
			);
		}
		else
		{
			bIsFirstFrame = false;
		}

		// 更新 LastPosition
		LastPosition = CurrentPosition;
	}
	else
	{
		// 控制滚动
		if (CurrentRollingDistance < RollingDistance)
		{
			// 以一定速度滚动
			FVector NewLocation = GetActorLocation() + RollingVelocity * DeltaTime;
			SetActorLocation(NewLocation);
			CurrentRollingDistance += RollingVelocity.Size() * DeltaTime;

			// 计算缩小比例
			float ShrinkRatio = CurrentRollingDistance / RollingDistance;
			ShrinkRatio = FMath::Clamp(ShrinkRatio, 0.0f, 1.0f); // 确保比例在0到1之间

			FVector NewScale = FMath::Lerp(FVector(1.0f), FVector(0.0f), ShrinkRatio);
			NewScale = NewScale.ComponentMax(FVector(0.0f)); // 防止负值
			SetActorScale3D(NewScale);
			// 仅缩小 MeshComponent
			if (MeshComponent)
			{
				// 基于初始缩放比例进行缩放
				FVector FinalScale = InitialMeshScale * NewScale;
				MeshComponent->SetRelativeScale3D(FinalScale);
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("MeshComponent is null."));
			}
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Rolling completed. Destroying Snowball."));
			// 滚动完成后销毁雪球
			Destroy();
		}
	}
}

void ASnowball::SetLaunchDirection(const FVector& Direction)
{
	// 只保存水平分量，忽略Z轴
	LaunchDirection = FVector(Direction.X, Direction.Y, 0).GetSafeNormal();
}

void ASnowball::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	FVector NormalImpulse, const FHitResult& Hit)
{
	if (!bHasLanded)
	{
		bHasLanded = true;

		// 停止物理模拟
		SphereComponent->SetSimulatePhysics(false);

		// 使用发射时的水平方向进行滚动
		RollingVelocity = LaunchDirection * 500.0f;

		// 记录最后的位置
		LastPosition = GetActorLocation();
		// 在落地点绘制红色调试球体
		DrawDebugSphere(
			GetWorld(),
			Hit.Location,
			10.0f, // 半径，可以根据需要调整
			12,    // 分段数
			FColor::Red,
			false,
			1.0f,  // 持续时间
			0,
			2.0f   // 线宽
		);
		UE_LOG(LogTemp, Warning, TEXT("OnHit triggered at location: %s"), *Hit.Location.ToString());
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Landing point marked with red sphere."));
		}
	}
}
