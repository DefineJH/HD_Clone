// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "TPSPlayerController.generated.h"

class ATPSCharacter;
class UInputMappingContext;
class UInputAction;
struct FInputActionInstance;
/**
 * 
 */
UCLASS()
class HD_CLONE_API ATPSPlayerController : public APlayerController
{
	GENERATED_BODY()
protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;
public:
	virtual void PostInitializeComponents() override;
	virtual void OnPossess(APawn* aPawn) override;
	virtual void OnUnPossess() override;

	UFUNCTION(BlueprintCallable)
	bool GetShouldTurnRight() const { return bShouldTurnRight; }
	UFUNCTION(BlueprintCallable)
	bool GetShouldTurnLeft() const { return bShouldTurnLeft; }
protected:

	UPROPERTY(BlueprintReadOnly)
	ATPSCharacter* PossessedChar = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputMappingContext* DefaultMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputAction* MoveAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputAction* LookAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputAction* AimAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputAction* CrouchAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputAction* ReloadAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputAction* FireAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputAction* SprintAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input)
	float walkSpeed = 300.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input)
	float runSpeed = 600.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera)
	float armLength_Aim = 100.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera)
	float armLength_NotAim = 200.0f;

	void Move(const FInputActionInstance& Instance);
	void Look(const FInputActionInstance& Instance);
	void StartAim(const FInputActionInstance& Instance);
	void EndAim(const FInputActionInstance& Instance);
	void ToggleCrouch(const FInputActionInstance& Instance);
	void Reload(const FInputActionInstance& Instance);
	void FireStart(const FInputActionInstance& Instance);
	void FireEnd(const FInputActionInstance& Instance);
	void StartSprint(const FInputActionInstance& Instance);
	void EndSprint(const FInputActionInstance& Instance);


private:
	bool bIsAiming = false;
	bool bShouldTurnRight = false;
	bool bShouldTurnLeft = false;
};
