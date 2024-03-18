// Fill out your copyright notice in the Description page of Project Settings.


#include "TPSPlayerController.h"
#include <InputMappingContext.h>
#include <InputAction.h>
#include <InputActionValue.h>

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

#include "TPSCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"

#include "GameFramework/CharacterMovementComponent.h"

void ATPSPlayerController::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}

void ATPSPlayerController::BeginPlay()
{
	Super::BeginPlay();

	ATPSCharacter* playerChar = Cast<ATPSCharacter>(GetPawn());

	if (playerChar == nullptr)
		return;

	PossessedChar = playerChar;

	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	if (Subsystem != nullptr)
	{
		Subsystem->AddMappingContext(DefaultMappingContext, 0);
	}
}

void ATPSPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	UEnhancedInputComponent* Input = Cast<UEnhancedInputComponent>(InputComponent);

	if (Input != nullptr)
	{
		Input->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ATPSPlayerController::Move);
		Input->BindAction(LookAction, ETriggerEvent::Triggered, this, &ATPSPlayerController::Look);
		Input->BindAction(FocusAction, ETriggerEvent::Started, this, &ATPSPlayerController::FocusStart);
		Input->BindAction(FocusAction, ETriggerEvent::Completed, this, &ATPSPlayerController::FocusEnd);
		Input->BindAction(CrouchAction, ETriggerEvent::Triggered, this, &ATPSPlayerController::ToggleCrouch);
		Input->BindAction(ReloadAction, ETriggerEvent::Triggered, this, &ATPSPlayerController::Reload);
		Input->BindAction(FireAction, ETriggerEvent::Started, this, &ATPSPlayerController::FireStart);
		Input->BindAction(FireAction, ETriggerEvent::Completed, this, &ATPSPlayerController::FireEnd);
		Input->BindAction(SprintAction, ETriggerEvent::Started, this, &ATPSPlayerController::StartSprint);
		Input->BindAction(SprintAction, ETriggerEvent::Completed, this, &ATPSPlayerController::EndSprint);
	}

}


void ATPSPlayerController::OnPossess(APawn* aPawn)
{
	Super::OnPossess(aPawn);
}

void ATPSPlayerController::OnUnPossess()
{
	Super::OnUnPossess();

	PossessedChar = nullptr;
}

void ATPSPlayerController::Move(const FInputActionInstance& Instance)
{
	if (PossessedChar == nullptr)
		return;

	UCameraComponent* playerCameraComp = PossessedChar->GetComponentByClass<UCameraComponent>();
	
	if (playerCameraComp == nullptr)
		return;

	FVector camForward = playerCameraComp->GetForwardVector();
	camForward.Z = 0; camForward.Normalize();

	FVector camRight = playerCameraComp->GetRightVector();
	camRight.Z = 0; camRight.Normalize();

	FVector2D MovementVector = Instance.GetValue().Get<FVector2D>();


	PossessedChar->AddMovementInput(camForward, MovementVector.Y);
	PossessedChar->AddMovementInput(camRight, MovementVector.X);
}

void ATPSPlayerController::Look(const FInputActionInstance& Instance)
{

	if (PossessedChar == nullptr)
		return;

	USpringArmComponent* springArmComp = PossessedChar->GetComponentByClass<USpringArmComponent>();

	if (springArmComp == nullptr)
		return;

	FRotator springArmRot = springArmComp->GetComponentRotation();
	FVector2D LookVector = Instance.GetValue().Get<FVector2D>();
	PossessedChar->AddControllerYawInput(LookVector.Y);
	PossessedChar->AddControllerPitchInput(LookVector.X);
}

void ATPSPlayerController::FocusStart(const FInputActionInstance& Instance)
{
 
}

void ATPSPlayerController::FocusEnd(const FInputActionInstance& Instance)
{
}

void ATPSPlayerController::ToggleCrouch(const FInputActionInstance& Instance)
{
	if (PossessedChar == nullptr)
		return;

	if (PossessedChar->bIsCrouched)
		PossessedChar->UnCrouch(true);
	else
		PossessedChar->Crouch(true);
}

void ATPSPlayerController::Reload(const FInputActionInstance& Instance)
{
	if (PossessedChar == nullptr)
		return;

	PossessedChar->Reload();
}

void ATPSPlayerController::FireStart(const FInputActionInstance& Instance)
{
	if (PossessedChar == nullptr)
		return;

	if (PossessedChar->GetCharacterMovement()->MaxWalkSpeed == runSpeed) {
		EndSprint(Instance); 
	}
	PossessedChar->FireStart();
}

void ATPSPlayerController::FireEnd(const FInputActionInstance& Instance)
{
	if (PossessedChar == nullptr)
		return;

	PossessedChar->FireEnd();
}

void ATPSPlayerController::StartSprint(const FInputActionInstance& Instance)
{
	if (PossessedChar == nullptr)
		return;
	
	FireEnd(Instance);
	PossessedChar->GetCharacterMovement()->MaxWalkSpeed = runSpeed;
}

void ATPSPlayerController::EndSprint(const FInputActionInstance& Instance)
{
	if (PossessedChar == nullptr)
		return;

	PossessedChar->GetCharacterMovement()->MaxWalkSpeed = walkSpeed;
}

