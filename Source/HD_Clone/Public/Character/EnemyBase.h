// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "EnemyBase.generated.h"


class UBehaviorTree;
class AEnemyAIController;

UCLASS()
class HD_CLONE_API AEnemyBase : public ACharacter
{
	GENERATED_BODY()

public:
	AEnemyBase();
	virtual void PossessedBy(AController* NewController) override;
	void PlayAttackMontage();
protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere,Category= "AI")
	TObjectPtr<UBehaviorTree> BehaviorTree;

	UPROPERTY()
	TObjectPtr<AEnemyAIController> EnemyAIController;

	UFUNCTION(Server,Reliable)
	virtual void ServerAttack();

	UFUNCTION(NetMulticast,Reliable)
	virtual void MultiAttack();

	virtual void EnemyPlayMontage(UAnimMontage* PlayMontage);


private:
	
	/** 맞았을 때 파티클*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
	class UParticleSystem* ImpactParticles;

	/** 맞았을 때 사운드*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
	class USoundBase* ImpactSound;

	/**적 체력 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
	float Health = 100.f;
	
	/**최대 체력 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
	float MaxHealth = 100.f;
	
	/** 적 기본 데미지 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true", MakeEditWidget = "true"))
	float BaseDamage = 20.f;

	/** 순찰 장소 */
	UPROPERTY(EditAnywhere, Category = "Behavior Tree", meta = (AllowPrivateAccess = "true",MakeEditWidget ="true"))
	FVector PatrolPoint;

	/** 순찰 장소 */
	UPROPERTY(EditAnywhere, Category = "Behavior Tree", meta = (AllowPrivateAccess = "true", MakeEditWidget = "true"))
	FVector PatrolPoint2;

	/** 맞을때 애니메이션*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
	UAnimMontage* HitMontage;

	/** 공격 애니메이션*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
	UAnimMontage* AttackMontage;
};
