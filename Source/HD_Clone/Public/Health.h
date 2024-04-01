// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Health.generated.h"

UENUM(BlueprintType)
enum class EDamageType : uint8
{
	E_Normal UMETA(DisplayName = "Normal"),
	E_Explosive UMETA(DisplayName = "Explosive"),
};

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UHealth : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class HD_CLONE_API IHealth
{
	GENERATED_BODY()

public:
	virtual int32 GetMaxHealth() const = 0;
	virtual int32 GetHealth() const = 0;
	virtual void SetHealth(int32 newHealth) = 0;
	virtual void SetDamage(AActor* causer, EDamageType dmgType, int32 damage) = 0;
};
