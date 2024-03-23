// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Tasks/BTTask_Attack.h"

#include "AIController.h"
#include "Character/EnemyBase.h"

EBTNodeResult::Type UBTTask_Attack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AEnemyBase* Enemy = Cast<AEnemyBase>(OwnerComp.GetAIOwner()->GetPawn());
	if(Enemy)
	{
		Enemy->PlayAttackMontage();
	}
	
	return EBTNodeResult::Succeeded;
}
