// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Tasks/BTTask_Attack.h"

#include "AIController.h"
#include "Character/EnemyBase.h"

UBTTask_Attack::UBTTask_Attack() 
{
	TimerDelegate = FTimerDelegate::CreateUObject(this, &UBTTask_Attack::FinishTask);
}

EBTNodeResult::Type UBTTask_Attack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	MyOwnerComp = &OwnerComp;
	AEnemyBase* Enemy = Cast<AEnemyBase>(OwnerComp.GetAIOwner()->GetPawn());
	if(Enemy)
	{
		Enemy->ServerAttack();
		GetWorld()->GetTimerManager().SetTimer(AttackTimer,TimerDelegate,Enemy->GetMontageRate(),false);
	}

	return EBTNodeResult::InProgress;
}

void UBTTask_Attack::FinishTask()
{
	UE_LOG(LogTemp,Error,TEXT("125"));
	FinishLatentTask(*MyOwnerComp,EBTNodeResult::Succeeded);
}
