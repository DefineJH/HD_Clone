// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/EnemyAIController.h"

#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AIPerceptionTypes.h"

AEnemyAIController::AEnemyAIController()
{
	// Blackboard = CreateDefaultSubobject<UBlackboardComponent>("BlackBoardComponent");
	// check(Blackboard);
	//
	// BehaviorTreeComponent =CreateDefaultSubobject<UBehaviorTreeComponent>("BehaviorTreeComponent");
	// check(BehaviorTreeComponent);

	PerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>("AIPerception");
}
