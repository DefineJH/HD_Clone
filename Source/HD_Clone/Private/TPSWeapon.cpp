// Fill out your copyright notice in the Description page of Project Settings.


#include "TPSWeapon.h"
#include "Components/SkeletalMeshComponent.h"

// Sets default values
ATPSWeapon::ATPSWeapon() 
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	weaponMeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(L"Weapon Mesh");
	weaponMeshComp->SetupAttachment(GetRootComponent());
}

// Called when the game starts or when spawned
void ATPSWeapon::BeginPlay()
{
	Super::BeginPlay();
	if (weaponMesh)
		weaponMeshComp->SetSkeletalMesh(weaponMesh);
}

// Called every frame
void ATPSWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

