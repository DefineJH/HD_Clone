// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_Attack.generated.h"

/**
 * 
 */
UCLASS()
class HD_CLONE_API UBTTask_Attack : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

	UBTTask_Attack();
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	void FinishTask();
private:

	TObjectPtr<UBehaviorTreeComponent> MyOwnerComp;
	
	FTimerDelegate TimerDelegate;
	FTimerHandle AttackTimer;
};
