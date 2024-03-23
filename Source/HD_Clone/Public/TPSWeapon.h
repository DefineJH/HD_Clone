// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TPSWeapon.generated.h"

class USkeletalMesh;
class UParticleSystemComponent;
class USoundBase;
class UCameraShakeBase;

UENUM(BlueprintType)
enum class EWeaponType : uint8
{
	WT_Pistol,
	WT_Rifle,
	WT_UnArmed
};
UCLASS()
class HD_CLONE_API ATPSWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATPSWeapon();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	EWeaponType getWeaponType() const { return weaponType; }

	UAnimMontage* getReloadMontage() const
	{
		return reloadMontage == nullptr ? nullptr : reloadMontage;
	}
	UAnimMontage* getFireMontage() const
	{
		return fireMontage == nullptr ? nullptr : fireMontage;
	}
	
	double getFireRate() const { return fireRate; }
	bool canReload() const
	{
		return curMag == 0;
	}
	bool canFire() const { return bCanFire; }
	void Fire();

protected:
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerFire();
	UFUNCTION(NetMulticast, Reliable)
	void MulticastFire();

	void FireInternal();
protected:
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Weapon")
	USkeletalMeshComponent* weaponMeshComp;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Weapon")
	USceneComponent* firePositionComp;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Weapon")
	USkeletalMesh* weaponMesh;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Weapon")
	USoundBase* weaponSound;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Weapon")
	UParticleSystem* weaponHitEffect;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Weapon")
	UParticleSystem* weaponFireEffect;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Weapon")
	UParticleSystemComponent* weaponFireEffectComp;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Weapon")
	EWeaponType weaponType;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Weapon")
	double weaponDamage;
	/*
	* how many 
	*/
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Weapon")
	double fireRate;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Weapon")
	double maxRange;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Weapon")
	int maxMag;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Weapon")
	int curMag;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Weapon")
	int roundPerMag;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Weapon")
	int curRound;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Weapon")
	bool bIsAuto;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Animation")
	UAnimMontage* reloadMontage;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Animation")
	UAnimMontage* fireMontage;
	
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Camera")
	TSubclassOf<UCameraShakeBase> cameraShakeClass;
	double afterFire;
	bool bCanFire;
};
