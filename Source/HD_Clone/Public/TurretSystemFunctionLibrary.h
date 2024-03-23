// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "TurretSystemFunctionLibrary.generated.h"

/**
 * 
 */
UCLASS()
class HD_CLONE_API UTurretSystemFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable, Category = "Turret")
	static bool HasLineOfSight(const UObject* WorldContextObject, FHitResult& Hit, const FVector& From,
		const FVector& To, const TArray<AActor*>& ActorsToIgnore);
};
