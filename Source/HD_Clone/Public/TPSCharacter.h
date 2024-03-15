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

	UFUNCTION(BlueprintCallable)
	EWeaponType GetCurWeaponType() const;

private:
	ATPSWeapon* SpawnWeapon(TSubclassOf<ATPSWeapon> weaponClass);

protected:

	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	void GetControlRotation_Rep();

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


	UPROPERTY( BlueprintReadOnly, Replicated )
	FRotator controlRot;
};
