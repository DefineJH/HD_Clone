// Fill out your copyright notice in the Description page of Project Settings.


#include "TPSPlayerState.h"
#include "Net/UnrealNetwork.h"



void ATPSPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ATPSPlayerState, bShouldTurnRight);
	DOREPLIFETIME(ATPSPlayerState, bShouldTurnLeft);
}

void ATPSPlayerState::ServerRequestTurn_Implementation(bool turnRight, bool turnLeft)
{
	bShouldTurnRight = turnRight;
	bShouldTurnLeft = turnLeft;
}
bool ATPSPlayerState::ServerRequestTurn_Validate(bool turnRight, bool turnLeft)
{
	if (turnRight && turnLeft)
		return false;
	else return true;
}