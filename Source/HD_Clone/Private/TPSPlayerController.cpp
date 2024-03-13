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
		UE_LOG(LogTemp, Log, L"InputBinded");
		Input->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ATPSPlayerController::Move);
		Input->BindAction(LookAction, ETriggerEvent::Triggered, this, &ATPSPlayerController::Look);
	}
	else
	{
		UE_LOG(LogTemp, Log, L"Input is Null");
	}
}


void ATPSPlayerController::OnPossess(APawn* aPawn)
{
	Super::OnPossess(aPawn);
	ATPSCharacter* playerChar = Cast<ATPSCharacter>(GetPawn());
	UE_LOG(LogTemp, Log, L"Possess Started");

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


	// Only Change Character Roation when W pressed
	if (MovementVector.Y > 0)
	{
		
	}
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

	springArmRot.Pitch += (LookVector.X * -1.f);
	springArmRot.Pitch = FMath::ClampAngle(springArmRot.Pitch, -50.f, 38.f);
	springArmRot.Yaw += LookVector.Y;
	springArmComp->SetWorldRotation(springArmRot);
}

