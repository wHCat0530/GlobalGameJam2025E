// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ProjectileSnowBallCharacter.generated.h"

UCLASS(Blueprintable)
class AProjectileSnowBallCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AProjectileSnowBallCharacter();

	// Called every frame.
	virtual void Tick(float DeltaSeconds) override;

	/** Returns TopDownCameraComponent subobject **/
	FORCEINLINE class UCameraComponent* GetTopDownCameraComponent() const { return TopDownCameraComponent; }
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void LaunchSnowball();
	// ѩ�����ͼ��
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Snowball")
	TSubclassOf<class ASnowball> SnowballClass;

	// �������ʼƫ��
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Snowball")
	FVector LaunchOffset;

	// ������ٶ�
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Snowball")
	FVector LaunchVelocity;


private:
	/** Top down camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* TopDownCameraComponent;

	/** Camera boom positioning the camera above the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;
};

