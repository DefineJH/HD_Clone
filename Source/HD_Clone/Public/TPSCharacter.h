// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "TPSCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class ATPSWeapon;

UCLASS()
class HD_CLONE_API ATPSCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ATPSCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	// Reload Initiate Function
	void Reload();
	
	// Call Fire for curWeapon
	void FireWeapon();

	// FireStart Initiate Function
	void FireStart();
	// FireEnd Initate Function
	void FireEnd();
protected:
	ATPSWeapon* SpawnWeapon(TSubclassOf<ATPSWeapon> weaponClass);
	UFUNCTION( BlueprintCallable )
	EWeaponType getCurrentWeaponType() const;

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerReload();
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerStartFire();
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerStopFire();
	
	
	UFUNCTION(NetMulticast, Reliable)
	void MulticastPlayReloadAnimation();
	UFUNCTION(NetMulticast, Reliable)
	void MulticastStartFireAnimation();
	UFUNCTION(NetMulticast, Reliable)
	void MulticastStopFireAnimation();

	void StartPlayFireAnimation();
	void StopPlayFireAnimation();
	void PlayReloadAnimation();
protected:

	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	void GetControlRotation_Rep();

	void GetTurn_Rep();

private:
	void SetupWeapon();
protected:
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite, Category = "Camera")
	USpringArmComponent* springArmComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Camera")
	UCameraComponent* cameraComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	TSubclassOf<ATPSWeapon> mainWeaponClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	TSubclassOf<ATPSWeapon> subWeaponClass;
	
	UPROPERTY(BlueprintReadWrite, Category = "Weapon")
	ATPSWeapon* mainWeapon;
	UPROPERTY(BlueprintReadWrite, Category = "Weapon")
	ATPSWeapon* subWeapon;
	UPROPERTY(BlueprintReadWrite, Category = "Weapon")
	ATPSWeapon* curWeapon;



	UPROPERTY( BlueprintReadOnly, Replicated)
	FRotator controlRot;
	UPROPERTY(BlueprintReadOnly, Replicated)
	float controlTurn;
};
