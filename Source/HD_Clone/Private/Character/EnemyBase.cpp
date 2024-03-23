// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/EnemyBase.h"

#include "AI/EnemyAIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/KismetSystemLibrary.h"

AEnemyBase::AEnemyBase()
{
	PrimaryActorTick.bCanEverTick = false;

}

void AEnemyBase::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	if(!HasAuthority()) return;;
	EnemyAIController = Cast<AEnemyAIController>(NewController);

	// EnemyAIController->GetBlackboardComponent()->InitializeBlackboard(*BehaviorTree->BlackboardAsset);
	EnemyAIController->RunBehaviorTree(BehaviorTree);

}

void AEnemyBase::BeginPlay()
{
	Super::BeginPlay();
	
}

void AEnemyBase::EnemyPlayMontage(UAnimMontage* PlayMontage)
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if(AnimInstance && AttackMontage)
	{
		AnimInstance->Montage_Play(PlayMontage);
	}
}

void AEnemyBase::PlayAttackMontage()
{
	EnemyPlayMontage(AttackMontage);
}

void AEnemyBase::ServerAttack_Implementation()
{
	MultiAttack();
}

void AEnemyBase::MultiAttack_Implementation()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if(AnimInstance && AttackMontage)
	{
		AnimInstance->Montage_Play(AttackMontage);
		// float Rate = AnimInstance->Montage_GetPlayRate(AttackMontage);
		// UKismetSystemLibrary::Delay(this,Rate,FLatentActionInfo());
	}
}

