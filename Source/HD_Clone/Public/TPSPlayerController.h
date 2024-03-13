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
	UInputAction* FocusAction;

	void Move(const FInputActionInstance& Instance);
	void Look(const FInputActionInstance& Instance);
	void FocusStart(const FInputActionInstance& Instance);
	void FocusEnd(const FInputActionInstance& Instance);
};
