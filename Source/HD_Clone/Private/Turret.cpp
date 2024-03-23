// Fill out your copyright notice in the Description page of Project Settings.


#include "Turret.h"
#include "DrawDebugHelpers.h"
#include "TurretProjectile.h"
#include "TurretSystemFunctionLibrary.h"
#include "Components/ArrowComponent.h"
#include "Components/AudioComponent.h"
#include "Components/SphereComponent.h"
#include "Engine/SkeletalMesh.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Sound/SoundCue.h"
#include "UObject/ConstructorHelpers.h"

// Sets default values
ATurret::ATurret()
{
	bReplicates = true;
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> DefaultMesh(TEXT("/Game/Skills/Turrets/Models/SM_Turret1.SM_Turret1"));

	TurretMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("TurretMesh"));
	TurretMesh->SetSkeletalMesh(DefaultMesh.Object);
	SetRootComponent(RootComponent);

	RotationSound = CreateDefaultSubobject<UAudioComponent>("RotationAudioComponent");
	RotationSound->SetupAttachment(TurretMesh);
	RotationSound->bAlwaysPlay = true;

	FireSound = CreateDefaultSubobject<UAudioComponent>("FireAudioComponent");
	FireSound->SetupAttachment(TurretMesh);
	FireSound->bAlwaysPlay = true;

	ArrowComponent = CreateDefaultSubobject<UArrowComponent>("ArrowComponent");
	ArrowComponent->SetupAttachment(TurretMesh);
	ArrowComponent->SetRelativeLocation({ 0.f,80,70 });
	ArrowComponent->SetRelativeRotation({ 0,0,90 });

	ActorsToIgnore.Add(this);
}

void ATurret::FindTarget()
{

	DrawDetectionRange();

	TArray<AActor*> OverlappingActors;

	const bool IsOverlapped = UKismetSystemLibrary::SphereOverlapActors(GetWorld(), GetActorLocation(),
		SenseRange, TraceObjectTypes, nullptr,
		ActorIgnoreSphereOverlap, OverlappingActors);
	float BestDistance = SenseRange;
	AActor* ClosestTarget = nullptr;

	if (IsOverlapped)
	{
		for (AActor*& HitResult : OverlappingActors)
		{
			//ActorsToIgnore.Add(HitResult);

			if (!ClosestTarget || GetDistanceTo(HitResult) < BestDistance)
			{
				UE_LOG(LogTemp, Log, L"%s overlapped", *HitResult->GetName());
				bool bHasObstacle = UTurretSystemFunctionLibrary::HasLineOfSight(this, SightHitResult, GetActorLocation(), HitResult->GetActorLocation(), ActorsToIgnore);
				if (!bHasObstacle)
				{
					ClosestTarget = HitResult;
					BestDistance = GetDistanceTo(ClosestTarget);
				}
			}
		}
		BestTarget = ClosestTarget;
	}
	else
	{
		BestTarget = nullptr;
	}
}

float ATurret::TakeDamage(float DamageTaken, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	return 0.0f;
}

void ATurret::DrawDetectionRange()
{
	if (GetWorld())
	{
		DrawDebugSphere(GetWorld(), GetActorLocation(), SenseRange, 32, FColor::Red, false, -1.0f, SDPG_World);
	}
}

// Called when the game starts or when spawned
void ATurret::BeginPlay()
{
	Super::BeginPlay();

	FullHealth = 100.0f;
	HealthPercentage = 1.0f;


	TraceObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_Pawn));

	if (RotationSoundCue)
	{
		RotationSound->SetSound(RotationSoundCue);
	}

	if (FireSoundEffect)
	{
		FireSound->SetSound(FireSoundEffect);
	}

	ActorsToIgnore.Add(ProjectileActor.GetDefaultObject());


}

// Called every frame
void ATurret::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	FireProjectile();
	FindTarget();
	if (BestTarget)
	{
		RotateToTarget();

		FireProjectile();
	}
	else
	{
		if (GetWorld()->GetTimerManager().IsTimerActive(FireTimerHandle))
		{
			GetWorld()->GetTimerManager().ClearTimer(FireTimerHandle);
		}
		IdleRotate(DeltaTime);
	}
}
void ATurret::RotateToTarget()
{
	if (BestTarget && TurretMesh)
	{
		const FRotator DesiredRotation = UKismetMathLibrary::FindLookAtRotation(TurretMesh->GetRelativeLocation(), BestTarget->GetActorLocation());
		TurretMesh->SetRelativeRotation({ TurretMesh->GetRelativeRotation().Pitch, DesiredRotation.Yaw, TurretMesh->GetRelativeRotation().Roll });
	}
}

void ATurret::PlayRotateSound()
{
	if (RotationSoundCue)
	{
		RotationSound->Stop();
		RotationSound->Play();
	}
}

void ATurret::PlayFireSound()
{
	if (FireSound)
	{
		FireSound->Stop();
		FireSound->Play();
	}
}

void ATurret::IdleRotate(const float DeltaSecond)
{
	if (!bIsRotating)
	{
		RandValue = FMath::FRandRange(-180.f, 180.f);

		PlayRotateSound();
		bIsRotating = true;
	}

	if (bIsRotating && !bIsInDelayTime)
	{
		RotateValue = FMath::FInterpTo(TurretMesh->GetRelativeRotation().Yaw, RandValue, DeltaSecond, InterpolationSpeed);
		TurretMesh->SetRelativeRotation({ TurretMesh->GetRelativeRotation().Pitch, RotateValue, TurretMesh->GetRelativeRotation().Roll });
	}

	if (FMath::IsNearlyEqual(RandValue, TurretMesh->GetRelativeRotation().Yaw, 1.f) && !bIsInDelayTime)
	{
		bIsInDelayTime = true;

		if (bIsInDelayTime)
		{
			GetWorld()->GetTimerManager().SetTimer(TimerHandle, [&]()
				{
					GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
					bIsInDelayTime = false;
					bIsRotating = false;
				}, 1.f, false, FMath::RandRange(1.1f, 1.6f));
		}
	}
}//gpt

void ATurret::FireProjectile()
{
	ATurretProjectile* TurretProjectile = GetWorld()->SpawnActor<ATurretProjectile>(ProjectileActor.Get(),
		ArrowComponent->GetComponentLocation(), { 0,TurretMesh->GetRelativeRotation().Yaw, 0 });
	PlayFireSound();
}
