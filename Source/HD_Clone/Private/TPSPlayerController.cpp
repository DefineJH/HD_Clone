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

void ATPSPlayerController::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}

void ATPSPlayerController::BeginPlay()
{
	Super::BeginPlay();
}

void ATPSPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	UEnhancedInputComponent* Input = Cast<UEnhancedInputComponent>(InputComponent);

	if (Input != nullptr)
	{
		Input->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ATPSPlayerController::Move);
		Input->BindAction(LookAction, ETriggerEvent::Triggered, this, &ATPSPlayerController::Look);
	}

}


void ATPSPlayerController::OnPossess(APawn* aPawn)
{
	Super::OnPossess(aPawn);
	ATPSCharacter* playerChar = Cast<ATPSCharacter>(GetPawn());

	if (playerChar == nullptr)
		return;

	PossessedChar = playerChar;
	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(this->GetLocalPlayer());
	if (Subsystem != nullptr)
	{
		Subsystem->AddMappingContext(DefaultMappingContext, 0);
	}
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

	// Rotate Character Mesh according to moveDirection
	FRotator curMeshRot = PossessedChar->GetMesh()->GetComponentRotation();
	FRotator targetRot = (camForward * MovementVector.Y + camRight * MovementVector.X).Rotation();
	targetRot.Add(0, -90.0f, 0);
	FRotator newRot = FMath::RInterpTo(curMeshRot, targetRot, GetWorld()->GetDeltaSeconds(), 5.0f);
	PossessedChar->GetMesh()->SetWorldRotation(newRot);

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

