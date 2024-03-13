// Fill out your copyright notice in the Description page of Project Settings.


#include "TPSCharacter.h"
#include "TPSWeapon.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include <InputMappingContext.h>
#include <InputAction.h>
#include <InputActionValue.h>

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
// Sets default values
ATPSCharacter::ATPSCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	springArmComp = CreateDefaultSubobject<USpringArmComponent>(L"Spring Arm Component");
	springArmComp->SetupAttachment(GetRootComponent());

	cameraComp = CreateDefaultSubobject<UCameraComponent>(L"Camera Component");
	cameraComp->SetupAttachment(springArmComp);
	cameraComp->bUsePawnControlRotation = false;
}

// Called when the game starts or when spawned
void ATPSCharacter::BeginPlay()
{
	Super::BeginPlay();
	// 무기 세팅
	if (mainWeaponClass)
	{
		mainWeapon = SpawnWeapon(mainWeaponClass);
		checkf(mainWeapon != nullptr, L"Main Weapon Didn't Spawned");
		curWeapon = mainWeapon;
	}
	if (subWeaponClass)
	{
		subWeapon = SpawnWeapon(subWeaponClass);
		checkf(subWeapon != nullptr, L"Sub Weapon Didn't Spawned");
		if (subWeapon)
			subWeapon->SetActorHiddenInGame(true);
	}
}

// Called every frame
void ATPSCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ATPSCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

EWeaponType ATPSCharacter::GetCurWeaponType() const
{
	checkf(curWeapon != nullptr, L"Cur Weapon is Nullptr");
	return curWeapon->getWeaponType();
}

ATPSWeapon* ATPSCharacter::SpawnWeapon(TSubclassOf<ATPSWeapon> weaponClass)
{
	FActorSpawnParameters spawnParams;
	spawnParams.Owner = this;
	spawnParams.Instigator = GetInstigator();

	ATPSWeapon* spawnWeapon = GetWorld()->SpawnActor<ATPSWeapon>(mainWeaponClass, spawnParams);
	if (spawnWeapon)
	{
		FName weaponSocketName;

		EWeaponType type = spawnWeapon->getWeaponType();
		switch (type)
		{
		case EWeaponType::WT_Pistol:
			weaponSocketName = L"Pistol_R";
			break;
		case EWeaponType::WT_Rifle:
			weaponSocketName = L"Rifle_R";
			break;
		default:
			weaponSocketName = L"";
		}

		if (weaponSocketName != L"")
		{
			spawnWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, weaponSocketName);
			return spawnWeapon;
		}
	}
	return nullptr;
}

