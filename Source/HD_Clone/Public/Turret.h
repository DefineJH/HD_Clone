#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Turret.generated.h"

class USkeletalMeshComponent;
class USphereComponent;
class UArrowComponent;
class ATurretProjectile;
class USoundWave;

UCLASS()
class HD_CLONE_API ATurret : public AActor
{
	GENERATED_BODY()
public:
	ATurret();

protected:
	// Sets default values for this actor's properties


	UPROPERTY(VisibleAnywhere)
	USceneComponent* Root;

	UPROPERTY(EditAnywhere)
	USkeletalMeshComponent* TurretMesh;

	UPROPERTY(EditDefaultsOnly)
	UArrowComponent* ArrowComponent = nullptr;

	UFUNCTION(BlueprintCallable)
	void FindTarget();

	UPROPERTY(EditInstanceOnly, Category = "Turret", meta = (ClampMin = "1.0", ClampMax = "2000.0", UIMin = "1.0", UIMax = "2000.0"))
	float SenseRange = 1000.f;

	UPROPERTY(EditAnywhere, Category = "Turret")
	float InterpolationSpeed = 10.f;

	UPROPERTY(EditAnywhere, Category = "Turret")
	USoundBase* RotationSoundCue = nullptr;

	UPROPERTY(EditInstanceOnly, Category = "Fire")
	float FireRate = 1.f;

	UPROPERTY(EditInstanceOnly, Category = "Fire")
	TSubclassOf<ATurretProjectile> ProjectileActor = nullptr;

	UPROPERTY(EditAnywhere, Category = "Fire")
	USoundWave* FireSoundEffect = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
	float FullHealth;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
	float HealthPercentage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
	float PreviousHealth;

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

public:
	bool bIsInDelayTime = false;
	bool bIsRotating = false;
	float RandValue = 0.f;

	UPROPERTY()
	TArray<AActor*> ActorsToIgnore;

	UPROPERTY()
	TArray<AActor*> ActorIgnoreSphereOverlap;

	UPROPERTY()
	TArray<TEnumAsByte<EObjectTypeQuery>> TraceObjectTypes;

	UPROPERTY(BlueprintReadOnly)
	AActor* BestTarget = nullptr;

	UPROPERTY()
	UAudioComponent* RotationSound = nullptr;

	UPROPERTY()
	UAudioComponent* FireSound = nullptr;

	UFUNCTION(BlueprintCallable, Category = "Health")
	float TakeDamage(float DamageTaken, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;


	FTimerHandle TimerHandle;
	FTimerHandle FireTimerHandle;

	void DrawDetectionRange();
	void RotateToTarget();
	void PlayRotateSound();
	void PlayFireSound();

	UFUNCTION()
	void IdleRotate(const float DeltaSecond);

	UFUNCTION()
	void FireProjectile();

	float RotateValue = 0;

	FHitResult SightHitResult;
private:

};