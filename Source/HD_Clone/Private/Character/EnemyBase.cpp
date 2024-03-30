// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/EnemyBase.h"

#include "AI/EnemyAIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetSystemLibrary.h"

AEnemyBase::AEnemyBase()
{
	PrimaryActorTick.bCanEverTick = false;

	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll= false;
	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bUseControllerDesiredRotation = true;

}

void AEnemyBase::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	if(!HasAuthority()) return;;
	EnemyAIController = Cast<AEnemyAIController>(NewController);

	// EnemyAIController->GetBlackboardComponent()->InitializeBlackboard(*BehaviorTree->BlackboardAsset);
	if (BehaviorTree)
	{
		EnemyAIController->RunBehaviorTree(BehaviorTree);
	}
}

void AEnemyBase::BeginPlay()
{
	Super::BeginPlay();
	
}

float AEnemyBase::EnemyPlayMontage(UAnimMontage* PlayMontage)
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if(AnimInstance && AttackMontage)
	{
		return AnimInstance->Montage_Play(PlayMontage);
	}
	return 0;
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
	MontageRate = EnemyPlayMontage(AttackMontage);
}

