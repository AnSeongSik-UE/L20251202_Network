// Fill out your copyright notice in the Description page of Project Settings.


#include "Zombie.h"
#include "Engine/DamageEvents.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Zombie_AIC.h"


// Sets default values
AZombie::AZombie()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

// Called when the game starts or when spawned
void AZombie::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AZombie::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//APawn* TargetPawn = UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetPawn();

	//FVector Direction = TargetPawn->GetActorLocation() - GetActorLocation();
	//AddMovementInput(Direction, 1);

	//FRotator Rotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), TargetPawn->GetActorLocation());
	//SetActorRotation(Rotation);

}

void AZombie::SetState(EZombieState NewState)
{
	CurrentState = NewState;
}

float AZombie::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	if (CurrentHP <= 0)
	{
		return DamageAmount;
	}

	if (DamageEvent.IsOfType(FPointDamageEvent::ClassID))
	{
		FPointDamageEvent* Event = (FPointDamageEvent*)(&DamageEvent);
		if (Event)
		{
			CurrentHP -= DamageAmount;
		}

		SpawnHitEffect(Event->HitInfo);

		UE_LOG(LogTemp, Warning, TEXT("Point Damage %f %s"), DamageAmount, *Event->DamageTypeClass->GetName());
	}
	else if (DamageEvent.IsOfType(FRadialDamageEvent::ClassID))
	{
		FRadialDamageEvent* Event = (FRadialDamageEvent*)(&DamageEvent);
		if (Event)
		{
			CurrentHP -= DamageAmount;
		}
	}
	else //(DamageEvent.IsOfType(FDamageEvent::ClassID))
	{
		CurrentHP -= DamageAmount;
		UE_LOG(LogTemp, Warning, TEXT("Damage %f"), DamageAmount);
	}



	if (CurrentHP <= 0)
	{
		//죽는다. 애님 몽타주 재생
		//네트워크 할려면 다 RPC로 작업해 됨
		//DoDead();
		SetState(EZombieState::Death);
		AZombie_AIC* AIC = Cast<AZombie_AIC>(GetController());
		if (AIC)
		{
			AIC->SetState(EZombieState::Death);
		}
	}

	return DamageAmount;
}


void AZombie::DoDeadEnd()
{
	GetController()->SetActorEnableCollision(false);
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	GetMesh()->SetSimulatePhysics(true);
}

void AZombie::DoDead()
{
	FName SectionName = FName(FString::Printf(TEXT("%d"), FMath::RandRange(1, 6)));
	//PlayAnimMontage(DeathMontage, 1.0f, SectionName);
}

void AZombie::SpawnHitEffect(FHitResult Hit)
{
	if (BloodEffect)
	{
		UE_LOG(LogTemp, Warning, TEXT("BloodEffect"));
		UGameplayStatics::SpawnEmitterAtLocation(
			GetWorld(),
			BloodEffect,
			Hit.ImpactPoint,
			Hit.ImpactNormal.Rotation()
		);
	}
}

void AZombie::ChangeSpeed(float NewMaxSpeed)
{
	GetCharacterMovement()->MaxWalkSpeed = NewMaxSpeed;
}
