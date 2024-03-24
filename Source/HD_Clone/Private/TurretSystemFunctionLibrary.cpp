// Fill out your copyright notice in the Description page of Project Settings.


#include "TurretSystemFunctionLibrary.h"

bool UTurretSystemFunctionLibrary::HasLineOfSight(const UObject* WorldContextObject, FHitResult& Hit,
	const FVector& From, const FVector& To, const TArray<AActor*>& ActorsToIgnore)
{
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActors(ActorsToIgnore);
	
	return !WorldContextObject->GetWorld()->LineTraceSingleByChannel(Hit,From, To - (To.GetSafeNormal() * 100.f), ECC_Visibility, QueryParams);
}