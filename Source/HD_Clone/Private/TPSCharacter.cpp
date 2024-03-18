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

#include "Net/UnrealNetwork.h"

#include "GameFramework/PawnMovementComponent.h"

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


	GetMovementComponent()->GetNavAgentPropertiesRef().bCanCrouch = true;
	bReplicates = true;
	SetReplicateMovement(true);

}

// Called when the game starts or when spawned
void ATPSCharacter::BeginPlay()
{
	Super::BeginPlay();
	SetupWeapon();
}

// Called every frame
void ATPSCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	GetControlRotation_Rep();
	GetTurn_Rep();
	
}

void ATPSCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION(ATPSCharacter, controlRot, COND_SkipOwner);
	DOREPLIFETIME_CONDITION(ATPSCharacter, controlTurn, COND_SkipOwner);
}

// Called to bind functionality to input
void ATPSCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

#pragma region Weapon Functions

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

EWeaponType ATPSCharacter::getCurrentWeaponType() const
{
	if (curWeapon == nullptr)
		return EWeaponType::WT_UnArmed;
	else
		return curWeapon->getWeaponType();
}

void ATPSCharacter::SetupWeapon()
{
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


#pragma region Reload Functions

void ATPSCharacter::Reload()
{
	// Host -> 멀티캐스트
	if (HasAuthority())
	{
		MulticastPlayReloadAnimation();
	}
	// Client -> 서버 함수 호출
	else
	{
		ServerReload();
	}
}

void ATPSCharacter::FireWeapon()
{
	if (curWeapon == nullptr)
		return;

	curWeapon->Fire();
}

void ATPSCharacter::ServerReload_Implementation()
{
	MulticastPlayReloadAnimation();
}

bool ATPSCharacter::ServerReload_Validate()
{
	return true;
}

void ATPSCharacter::MulticastPlayReloadAnimation_Implementation()
{
	PlayReloadAnimation();
}

void ATPSCharacter::PlayReloadAnimation()
{
	if (!curWeapon)
		return;

	UAnimMontage* reloadMontage = curWeapon->getReloadMontage();
	if (reloadMontage)
	{
		PlayAnimMontage(reloadMontage);
	}
}

#pragma endregion

#pragma region Fire Functions

void ATPSCharacter::FireStart()
{
	// Host -> 멀티캐스트
	if (HasAuthority())
	{
		MulticastStartFireAnimation();
	}
	// Client -> 서버 함수 호출
	else
	{
		ServerStartFire();
	}


}

void ATPSCharacter::FireEnd()
{
	// Host -> 멀티캐스트
	if (HasAuthority())
	{
		MulticastStopFireAnimation();
	}
	// Client -> 서버 함수 호출
	else
	{
		ServerStopFire();
	}
}
void ATPSCharacter::ServerStartFire_Implementation()
{
	MulticastStartFireAnimation();
}
bool ATPSCharacter::ServerStartFire_Validate()
{
	return true;
}
void ATPSCharacter::MulticastStartFireAnimation_Implementation()
{
	StartPlayFireAnimation();
}

void ATPSCharacter::ServerStopFire_Implementation()
{
	MulticastStopFireAnimation();
}
bool ATPSCharacter::ServerStopFire_Validate()
{
	return true;
}
void ATPSCharacter::MulticastStopFireAnimation_Implementation()
{
	StopPlayFireAnimation();
}
void ATPSCharacter::StartPlayFireAnimation()
{
	if (curWeapon)
	{
		UAnimMontage* fireMontage = curWeapon->getFireMontage();

		if (fireMontage)
		{
			PlayAnimMontage(fireMontage);
		}
	}
}

void ATPSCharacter::StopPlayFireAnimation()
{
	if (!curWeapon)
		return;
	StopAnimMontage(curWeapon->getFireMontage());
}
#pragma endregion

#pragma endregion

#pragma region Control Replication Functions

void ATPSCharacter::GetControlRotation_Rep()
{
	if (HasAuthority() || IsLocallyControlled())
	{
		controlRot = GetController()->GetControlRotation();
	}
}

void ATPSCharacter::GetTurn_Rep()
{
	if (HasAuthority() || IsLocallyControlled())
	{
		FRotator rot = GetActorForwardVector().Rotation() - cameraComp->GetForwardVector().Rotation();
		rot.Normalize();
		controlTurn = rot.Yaw;
	}
}


#pragma endregion



