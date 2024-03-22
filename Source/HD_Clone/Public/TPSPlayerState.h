// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "TPSPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class HD_CLONE_API ATPSPlayerState : public APlayerState
{
	GENERATED_BODY()
	
public:
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerRequestTurn(bool turnRight, bool turnLeft);

protected:
	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

protected:
	UPROPERTY(Replicated, BlueprintReadOnly)
	bool bShouldTurnRight = false;
	UPROPERTY(Replicated, BlueprintReadOnly)
	bool bShouldTurnLeft = false;
};
