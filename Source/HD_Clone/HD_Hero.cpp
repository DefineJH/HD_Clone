// Fill out your copyright notice in the Description page of Project Settings.


#include "HD_Hero.h"

// Sets default values
AHD_Hero::AHD_Hero()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AHD_Hero::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AHD_Hero::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AHD_Hero::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

