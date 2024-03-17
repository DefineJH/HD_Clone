// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TPSWeapon.generated.h"

class USkeletalMesh;

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

protected:
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Weapon")
	USkeletalMeshComponent* weaponMeshComp;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Weapon")
	USkeletalMesh* weaponMesh;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Weapon")
	USoundBase* weaponSound;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Weapon")
	UParticleSystem* weaponHitEffect;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Weapon")
	UParticleSystem* weaponFireEffect;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Weapon")
	EWeaponType weaponType;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Weapon")
	double weaponDamage;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Weapon")
	double fireRate;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Weapon")
	double maxRange;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Weapon")
	int maxMag;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Weapon")
	int curMag;
};
