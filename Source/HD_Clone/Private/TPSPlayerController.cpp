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
	USpringArmComponent* springArmComp = PossessedChar->GetComponentByClass<USpringArmComponent>();

	if (springArmComp != nullptr)
		springArmComp->TargetArmLength = armLength_NotAim;

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
		Input->BindAction(AimAction, ETriggerEvent::Started, this, &ATPSPlayerController::StartAim);
		Input->BindAction(AimAction, ETriggerEvent::Completed, this, &ATPSPlayerController::EndAim);
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

	//FRotator offset = GetControlRotation() - PossessedChar->GetActorRotation();
	//offset.Normalize();
	//UCharacterMovementComponent* movementComp = Cast<UCharacterMovementComponent>(PossessedChar->GetMovementComponent());
	//if (PossessedChar->GetVelocity().Length() == 0)
	//{
	//	if (offset.Yaw >= 90.f)
	//	{
	//		//Turn Right
	//		movementComp->bUseControllerDesiredRotation = true;
	//		movementComp->bOrientRotationToMovement = false;
	//		bShouldTurnRight = true;
	//	}
	//	else if (offset.Yaw <= -90.f)
	//	{
	//		// Turn Left
	//		movementComp->bUseControllerDesiredRotation = true;
	//		movementComp->bOrientRotationToMovement = false;
	//		bShouldTurnLeft = true;
	//	}
	//	else if(-1.f <= offset.Yaw && offset.Yaw <= 1.f)
	//	{
	//		movementComp->bUseControllerDesiredRotation = false;
	//		movementComp->bOrientRotationToMovement = true;
	//		bShouldTurnRight = true;
	//		bShouldTurnLeft = true;
	//	}
	//}
	//UE_LOG(LogTemp, Log, L"%f", offset.Yaw);
	PossessedChar->AddControllerYawInput(LookVector.Y);
	PossessedChar->AddControllerPitchInput(LookVector.X);
}

void ATPSPlayerController::StartAim(const FInputActionInstance& Instance)
{
	if (PossessedChar == nullptr)
		return;

	USpringArmComponent* springArmComp = PossessedChar->GetComponentByClass<USpringArmComponent>();

	if (springArmComp == nullptr)
		return;
	EndSprint(Instance);
	bIsAiming = true;
	Cast<UCharacterMovementComponent>(PossessedChar->GetMovementComponent())->bUseControllerDesiredRotation = true;
	Cast<UCharacterMovementComponent>(PossessedChar->GetMovementComponent())->bOrientRotationToMovement = false;
	springArmComp->TargetArmLength = armLength_Aim;
}

void ATPSPlayerController::EndAim(const FInputActionInstance& Instance)
{
	if (PossessedChar == nullptr)
		return;

	USpringArmComponent* springArmComp = PossessedChar->GetComponentByClass<USpringArmComponent>();
	
	if (springArmComp == nullptr)
		return;
	Cast<UCharacterMovementComponent>(PossessedChar->GetMovementComponent())->bUseControllerDesiredRotation = false;
	Cast<UCharacterMovementComponent>(PossessedChar->GetMovementComponent())->bOrientRotationToMovement = true;
	bIsAiming = false;
	springArmComp->TargetArmLength = armLength_NotAim;
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
	EndAim(Instance);
	PossessedChar->GetCharacterMovement()->MaxWalkSpeed = runSpeed;
}

void ATPSPlayerController::EndSprint(const FInputActionInstance& Instance)
{
	if (PossessedChar == nullptr)
		return;

	PossessedChar->GetCharacterMovement()->MaxWalkSpeed = walkSpeed;
}

