// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "BasePlayerController.generated.h"

/**
 * 
 */
UCLASS()
class GLOBALGAMEJAM2025_API ABasePlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	ABasePlayerController();

private:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;

	void MoveForward(float Value);
	void MoveRight(float Value);

	void OnJumpPressed();
	void OnJumpReleased();
	void OnBasicAttack();
	void OnInteract();
	void OnDash();
	void OnToggleRun();

	void OnCameraZoom(float Value);
	void OnCameraRotate(float Value);
	void OnCameraPanX(float Value);
	void OnCameraPanY(float Value);

private:
	UPROPERTY()
	class ABaseCharacter* BasePlayerCharacter;
};
