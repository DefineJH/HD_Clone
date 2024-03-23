// Fill out your copyright notice in the Description page of Project Settings.


#include "TPSWeapon.h"
#include "Components/SkeletalMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"
#include <TPSCharacter.h>
#include <TPSPlayerController.h>
// Sets default values
ATPSWeapon::ATPSWeapon() 
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	weaponMeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(L"Weapon Mesh");
	weaponMeshComp->SetupAttachment(GetRootComponent());

	weaponFireEffectComp = CreateDefaultSubobject<UParticleSystemComponent>(L"Fire Effect");
	weaponFireEffectComp->SetupAttachment(weaponMeshComp);
	
	firePositionComp = CreateDefaultSubobject<USceneComponent>(L"Fire Position");
	firePositionComp->SetupAttachment(weaponMeshComp);

	bReplicates = true;
	SetReplicates(true);
}

// Called when the game starts or when spawned
void ATPSWeapon::BeginPlay()
{
	Super::BeginPlay();
	if (weaponMesh)
		weaponMeshComp->SetSkeletalMesh(weaponMesh);
	if (weaponFireEffect)
		weaponFireEffectComp->SetTemplate(weaponFireEffect);
}

// Called every frame
void ATPSWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	afterFire += DeltaTime;
	if (afterFire >= fireRate)
		bCanFire = true;
}

void ATPSWeapon::Fire()
{
	if (HasAuthority())
		MulticastFire();
	else
		ServerFire();
}

void ATPSWeapon::FireInternal()
{
	afterFire = 0;
	bCanFire = false;
	
	// sound play
	if (weaponSound)
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), weaponSound, GetActorLocation());
	// fire effect activate
	if(!weaponFireEffectComp->Template.IsNull())
		weaponFireEffectComp->Activate(true);
	// camera shake
	if (cameraShakeClass)
	{
		ATPSCharacter* parentChar = Cast<ATPSCharacter>(GetOwner());
		if (!parentChar)
			return;
		ATPSPlayerController* playerCont = Cast<ATPSPlayerController>(parentChar->GetController());
		if (!playerCont)
			return;
		playerCont->PlayerCameraManager->StartCameraShake(cameraShakeClass);
	}
	// hit process
	FHitResult hitResult;
	FVector startPos = firePositionComp->GetComponentLocation();
	FVector endPos = startPos + firePositionComp->GetForwardVector() * maxRange;
	DrawDebugLine(GetWorld(), startPos, endPos, FColor::Red, false, 3.0);
	bool isHit = GetWorld()->LineTraceSingleByChannel(hitResult, startPos, endPos ,ECollisionChannel::ECC_Visibility);
	if (isHit)
	{
		if (hitResult.bBlockingHit)
		{
			DrawDebugSphere(GetWorld(), hitResult.Location, 10.0, 10, FColor::Red, false ,3.0f);
		}
	}
}

void ATPSWeapon::ServerFire_Implementation()
{
	MulticastFire();
}

bool ATPSWeapon::ServerFire_Validate()
{
	return true;
}


void ATPSWeapon::MulticastFire_Implementation()
{
	FireInternal();
}

