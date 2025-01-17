// Copyright Epic Games, Inc. All Rights Reserved.

#include "ProjectileSnowBallCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "Camera/CameraComponent.h"
#include "Components/DecalComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/SpringArmComponent.h"
#include "Materials/Material.h"
#include "Engine/World.h"
#include "Snowball.h"

AProjectileSnowBallCharacter::AProjectileSnowBallCharacter()
{
	// Set size for player capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Don't rotate character to camera direction
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Rotate character to moving direction
	GetCharacterMovement()->RotationRate = FRotator(0.f, 640.f, 0.f);
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;

	// Create a camera boom...
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->SetUsingAbsoluteRotation(true); // Don't want arm to rotate when character does
	CameraBoom->TargetArmLength = 800.f;
	CameraBoom->SetRelativeRotation(FRotator(-60.f, 0.f, 0.f));
	CameraBoom->bDoCollisionTest = false; // Don't want to pull camera in when it collides with level

	// Create a camera...
	TopDownCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("TopDownCamera"));
	TopDownCameraComponent->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	TopDownCameraComponent->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Activate ticking in order to update the cursor every frame.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;

	// 初始化发射偏移（例如在雪人的头顶）
	LaunchOffset = FVector(0.0f, 0.0f, 30.0f);

	// 初始化发射初速度（根据需求调整方向和大小）
	LaunchVelocity = FVector(500.0f, 0.0f, 0.0f); // 示例值

}

void AProjectileSnowBallCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

void AProjectileSnowBallCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAction("LaunchSnowball", IE_Pressed, this, &AProjectileSnowBallCharacter::LaunchSnowball);
}

void AProjectileSnowBallCharacter::LaunchSnowball()
{

	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("LaunchSnowball called!"));
	}

	if (SnowballClass)
	{
		// 计算发射位置
		FVector SpawnLocation = GetActorLocation() + LaunchOffset;
		// 使用角色的前方向作为发射方向
		FVector LaunchDirection = GetActorForwardVector(); // 确保角色朝向正确
		FRotator SpawnRotation = LaunchDirection.Rotation();

		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
		SpawnParams.Instigator = GetInstigator();

		// 生成雪球
		ASnowball* NewSnowball = GetWorld()->SpawnActor<ASnowball>(SnowballClass, SpawnLocation, SpawnRotation, SpawnParams);
		if (NewSnowball)
		{
			if (GEngine)
			{
				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("Snowball spawned!"));
			}
			// 设置雪球的发射方向
			NewSnowball->SetLaunchDirection(LaunchDirection);
			// 设置雪球的初始速度
			FVector FinalLaunchVelocity = LaunchDirection * LaunchVelocity.Size() + FVector(0, 0, LaunchVelocity.Z);
			NewSnowball->SphereComponent->SetPhysicsLinearVelocity(FinalLaunchVelocity);

		}
		else
		{
			if (GEngine)
			{
				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Failed to spawn Snowball!"));
			}
		}
	}

	else
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("SnowballClass is not set!"));
		}
	}
}

