// Fill out your copyright notice in the Description page of Project Settings.


#include "BasePlayerController.h"

#include "PlayerController/BaseCharacter.h"


ABasePlayerController::ABasePlayerController()
{
	bShowMouseCursor = true;
	bEnableClickEvents = true;
	bEnableMouseOverEvents = true;

	bAutoManageActiveCameraTarget = false;
}

void ABasePlayerController::BeginPlay()
{
	Super::BeginPlay();

	BasePlayerCharacter = Cast<ABaseCharacter>(GetCharacter());
	if (BasePlayerCharacter == nullptr)
	{
		return;
	}
}

void ABasePlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
}

void ABasePlayerController::MoveForward(float Value)
{
}

void ABasePlayerController::MoveRight(float Value)
{
}

void ABasePlayerController::OnJumpPressed()
{
}

void ABasePlayerController::OnJumpReleased()
{
}

void ABasePlayerController::OnBasicAttack()
{
}

void ABasePlayerController::OnInteract()
{
}

void ABasePlayerController::OnDash()
{
}

void ABasePlayerController::OnToggleRun()
{
}

void ABasePlayerController::OnCameraZoom(float Value)
{
}

void ABasePlayerController::OnCameraRotate(float Value)
{
}

void ABasePlayerController::OnCameraPanX(float Value)
{
}

void ABasePlayerController::OnCameraPanY(float Value)
{
}
