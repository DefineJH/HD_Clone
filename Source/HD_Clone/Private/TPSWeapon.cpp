// Fill out your copyright notice in the Description page of Project Settings.


#include "TPSWeapon.h"
#include "Components/SkeletalMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"
// Sets default values
ATPSWeapon::ATPSWeapon() 
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	weaponMeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(L"Weapon Mesh");
	weaponMeshComp->SetupAttachment(GetRootComponent());

	weaponFireEffectComp = CreateDefaultSubobject<UParticleSystemComponent>(L"Fire Effect");
	weaponFireEffectComp->SetupAttachment(weaponMeshComp);

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
	if (weaponSound)
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), weaponSound, GetActorLocation());
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

