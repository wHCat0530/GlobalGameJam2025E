// Fill out your copyright notice in the Description page of Project Settings.


#include "Snowball.h"
#include "Components/SphereComponent.h"

// Sets default values
ASnowball::ASnowball()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	// ��ʼ���������
	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	SphereComponent->InitSphereRadius(15.0f);
	RootComponent = SphereComponent;

	// ��ʼ���������
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	MeshComponent->SetupAttachment(RootComponent);

	// ��������ģ��
	SphereComponent->SetSimulatePhysics(true);
	SphereComponent->SetNotifyRigidBodyCollision(true);
	SphereComponent->SetEnableGravity(true); // ��������
	SphereComponent->SetMassOverrideInKg(NAME_None, 1.0f); // ��������Ϊ1kg��ȷ������Ч��
	// ������ײ����
	SphereComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics); // ������ײ��ѯ��������Ӧ
	SphereComponent->SetCollisionObjectType(ECC_GameTraceChannel1); // ʹ���Զ�����ײͨ����Ϊ Projectile
	SphereComponent->SetCollisionResponseToAllChannels(ECR_Ignore); // Ĭ�Ϻ�������ͨ��
	SphereComponent->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block); // �赲��̬�����������桢ǽ�ڣ�
	SphereComponent->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore); // ����Pawn����ҽ�ɫ�����������Լ�������ײ
	// ����ײ�¼�
	SphereComponent->OnComponentHit.AddDynamic(this, &ASnowball::OnHit);

	// ��ʼ������
	RollingDistance = 10.0f; // ���Ը�����Ҫ����
	CurrentRollingDistance = 0.0f;
	bHasLanded = false;
	RollingVelocity = FVector::ZeroVector;

	// ��ʼ���켣��ر���
	bIsFirstFrame = true;

	// ��ʼ����ʼ���ű���Ϊ1.0
	InitialMeshScale = FVector(1.0f, 1.0f, 1.0f);
}

// Called when the game starts or when spawned
void ASnowball::BeginPlay()
{
	Super::BeginPlay();
	if (MeshComponent)
	{
		// ����������ĳ�ʼ���ű���
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
	// ��¼��ǰ֡��λ��
	FVector CurrentPosition = GetActorLocation();
	if (!bHasLanded)
	{
		if (!bIsFirstFrame)
		{
			// ���ƴ���һ֡����ǰ֡���߶�
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

		// ���� LastPosition
		LastPosition = CurrentPosition;
	}
	else
	{
		// ���ƹ���
		if (CurrentRollingDistance < RollingDistance)
		{
			// ��һ���ٶȹ���
			FVector NewLocation = GetActorLocation() + RollingVelocity * DeltaTime;
			SetActorLocation(NewLocation);
			CurrentRollingDistance += RollingVelocity.Size() * DeltaTime;

			// ������С����
			float ShrinkRatio = CurrentRollingDistance / RollingDistance;
			ShrinkRatio = FMath::Clamp(ShrinkRatio, 0.0f, 1.0f); // ȷ��������0��1֮��

			FVector NewScale = FMath::Lerp(FVector(1.0f), FVector(0.0f), ShrinkRatio);
			NewScale = NewScale.ComponentMax(FVector(0.0f)); // ��ֹ��ֵ
			SetActorScale3D(NewScale);
			// ����С MeshComponent
			if (MeshComponent)
			{
				// ���ڳ�ʼ���ű�����������
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
			// ������ɺ�����ѩ��
			Destroy();
		}
	}
}

void ASnowball::SetLaunchDirection(const FVector& Direction)
{
	// ֻ����ˮƽ����������Z��
	LaunchDirection = FVector(Direction.X, Direction.Y, 0).GetSafeNormal();
}

void ASnowball::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	FVector NormalImpulse, const FHitResult& Hit)
{
	if (!bHasLanded)
	{
		bHasLanded = true;

		// ֹͣ����ģ��
		SphereComponent->SetSimulatePhysics(false);

		// ʹ�÷���ʱ��ˮƽ������й���
		RollingVelocity = LaunchDirection * 500.0f;

		// ��¼����λ��
		LastPosition = GetActorLocation();
		// ����ص���ƺ�ɫ��������
		DrawDebugSphere(
			GetWorld(),
			Hit.Location,
			10.0f, // �뾶�����Ը�����Ҫ����
			12,    // �ֶ���
			FColor::Red,
			false,
			1.0f,  // ����ʱ��
			0,
			2.0f   // �߿�
		);
		UE_LOG(LogTemp, Warning, TEXT("OnHit triggered at location: %s"), *Hit.Location.ToString());
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Landing point marked with red sphere."));
		}
	}
}
