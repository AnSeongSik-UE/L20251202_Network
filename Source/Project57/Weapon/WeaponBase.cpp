// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponBase.h"
#include "Components/SkeletalMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "Kismet/KismetSystemLibrary.h"
#include "BaseDamageType.h"
#include "Engine/DamageEvents.h"
#include "GameFramework/Character.h"
#include "TimerManager.h"
#include "ProjectileBase.h"
#include "Kismet/KismetMathLibrary.h"

#include "../Project57.h"
#include "../Network/NetworkUtil.h"


// Sets default values
AWeaponBase::AWeaponBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
	RootComponent = Mesh;

	SetReplicates(true);
	SetReplicateMovement(true);

}

// Called when the game starts or when spawned
void AWeaponBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AWeaponBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AWeaponBase::Reload()
{
	CurrentBulletCount = MaxBulletCount;
	UE_LOG(LogTemp, Warning, TEXT("Reload %d"), CurrentBulletCount);
}

void AWeaponBase::Fire()
{
	float CurrentTimeofShoot = GetWorld()->TimeSeconds - TimeofLastShoot;

	if (CurrentTimeofShoot < RefireRate)
	{
		return;
	}

	if (bFullAuto)
	{
		GetWorld()->GetTimerManager().SetTimer(RefireTimer, this, &AWeaponBase::Fire, RefireRate, false);
	}

	ACharacter* Character = Cast<ACharacter>(GetOwner());

	if (!Character)
	{
		return;
	}



	//Calculate
	FVector SpawnLocation;
	FVector TargetLocation;
	//FVector BulletDirection;
	FRotator AimRotation;
	FHitResult HitResult;

	//총구에서 발사
	SpawnLocation = Mesh->GetSocketLocation(TEXT("Muzzle"));
	FVector WeaponForward = Mesh->GetSocketRotation(TEXT("Muzzle")).Vector().GetSafeNormal();
	TargetLocation = SpawnLocation + (WeaponForward * 100000.0f);
	AimRotation = UKismetMathLibrary::FindLookAtRotation(SpawnLocation, TargetLocation + (UKismetMathLibrary::RandomUnitVector() * 0.3f));

	//bool bResult = CalculateShootData(SpawnLocation, TargetLocation, BulletDirection, AimRotation);

	//if (!bResult)
	//{
	//	return;
	//}


	FireProjectile(FTransform(AimRotation, SpawnLocation, FVector::OneVector),
		HitResult);

	//호출은 서버에서, 모든 클라이언트에서 실행
	S2A_SpawnMuzzleFlash(SpawnLocation, AimRotation);

	//Recoil
	Character->AddControllerPitchInput(-0.05f);

	CurrentBulletCount--;
	UE_LOG(LogTemp, Warning, TEXT("Fire %d"), CurrentBulletCount);
	S2A_SpawnSound(SpawnLocation);

	TimeofLastShoot = GetWorld()->TimeSeconds;

}

void AWeaponBase::FireProjectile(FTransform SpawnTransform, FHitResult InHitResult)
{
	AProjectileBase* Projectile = GetWorld()->SpawnActor<AProjectileBase>(ProjectileTemplate, SpawnTransform);
	Projectile->HitResult = InHitResult;
	Projectile->SetOwner(this);
}

bool AWeaponBase::CalculateShootData(FVector& OutSpawnLocation, FVector& OutTargetLocation, FVector& OutBulletDirection, FRotator& OutAimRotation)
{
	ACharacter* Character = Cast<ACharacter>(GetOwner());
	if (!Character)
	{
		return false;
	}

	APlayerController* PC = Cast<APlayerController>(Character->GetController());
	if (!PC)
	{
		return false;
	}

	if (PC)
	{
		int32 SizeX = 0;
		int32 SizeY = 0;
		int32 CenterX = 0;
		int32 CenterY = 0;
		FVector WorldDirection;
		FVector WorldLocation;
		FVector CameraLocation;
		FRotator CameraRotation;

		PC->GetViewportSize(SizeX, SizeY);
		CenterX = SizeX / 2;
		CenterY = SizeY / 2;

		PC->DeprojectScreenPositionToWorld((float)CenterX, (float)CenterY,
			WorldLocation, WorldDirection);

		PC->GetPlayerViewPoint(CameraLocation, CameraRotation);

		FVector Start = CameraLocation;
		FVector End = CameraLocation + WorldDirection * 100000.0f;

		TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
		ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldStatic));
		ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldDynamic));
		ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_PhysicsBody));

		TArray<AActor*> IngnoreActors;
		IngnoreActors.Add(GetOwner());
		FHitResult HitResult;

		bool bResult = UKismetSystemLibrary::LineTraceSingleForObjects(
			GetWorld(),
			Start,
			End,
			ObjectTypes,
			true,
			IngnoreActors,
			EDrawDebugTrace::None,
			HitResult,
			true
		);

		//Calculate
		OutSpawnLocation = Mesh->GetSocketLocation(TEXT("Muzzle"));
		OutTargetLocation = bResult ? HitResult.ImpactPoint : End;
		OutBulletDirection = (OutTargetLocation - OutSpawnLocation).GetSafeNormal();

		OutAimRotation = UKismetMathLibrary::FindLookAtRotation(OutSpawnLocation, OutTargetLocation + (UKismetMathLibrary::RandomUnitVector() * 0.3f));
	}

	return true;
}

void AWeaponBase::S2A_SpawnMuzzleFlash_Implementation(const FVector& SpawnLocation, const FRotator& AimRotation)
{
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), MuzzleFlash,
		SpawnLocation,
		AimRotation
	);
}

void AWeaponBase::S2A_SpawnSound_Implementation(const FVector& SpawnLocation)
{
	NET_LOG(TEXT("FireSound"));
	UGameplayStatics::SpawnSoundAtLocation(GetWorld(), FireSound, SpawnLocation);
}


void AWeaponBase::StopFire()
{
	GetWorld()->GetTimerManager().ClearTimer(RefireTimer);
}

