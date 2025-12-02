// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectileBase.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Components/DecalComponent.h"
#include "BaseDamageType.h"

// Sets default values
AProjectileBase::AProjectileBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Box = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
	RootComponent = Box;
	Box->SetBoxExtent(FVector(10, 10, 5));
	//Simulation Generated Hit Event
	Box->GetBodyInstance()->bNotifyRigidBodyCollision = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(Box);

	Movement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Movement"));
	Movement->MaxSpeed = 8000.0f;
	Movement->InitialSpeed = 8000.0f;
	
}

// Called when the game starts or when spawned
void AProjectileBase::BeginPlay()
{
	Super::BeginPlay();

	//OnActorBeginOverlap.AddDynamic(this, &AProjectileBase::ProcessBeginOverlap);
	Box->OnComponentHit.AddDynamic(this, &AProjectileBase::ProcessComponentHit);

	SetLifeSpan(5.0f);
	
}

// Called every frame
void AProjectileBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AProjectileBase::ProcessBeginOverlap(AActor* OverlapedActor, AActor* OtherActor)
{
	UDecalComponent* MadeDecal = UGameplayStatics::SpawnDecalAtLocation(GetWorld(),
		Decal,
		FVector(5, 5, 5),
		HitResult.ImpactPoint,
		HitResult.ImpactNormal.Rotation(),
		5.f
	);

	MadeDecal->SetFadeScreenSize(0.005f);

	//RPG 
//UGameplayStatics::ApplyDamage(HitResult.GetActor(),
//	50,
//	PC,
//	this,
//	UBaseDamageType::StaticClass()
//);



	////¹üÀ§ °ø°Ý, ÆøÅº
	//UGameplayStatics::ApplyRadialDamage(HitResult.GetActor(),
	//	10,
	//	HitResult.ImpactPoint,
	//	300.0f,
	//	UBaseDamageType::StaticClass(),
	//	IngnoreActors,
	//	this,
	//	PC,
	//	true
	//);
}

void AProjectileBase::ProcessComponentHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	SpawnHitEffect(Hit);

	APawn* Pawn = Cast<APawn>(GetOwner()->GetOwner());

	if (Pawn)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s %s"), *OtherActor->GetName(), *OtherComp->GetName());

		//ÃÑ½î´Â µ¥¹ÌÁö
		UGameplayStatics::ApplyPointDamage(Hit.GetActor(),
			Damage,
			-Hit.ImpactNormal,
			Hit,
			Pawn->GetController(),
			this,
			UBaseDamageType::StaticClass()
		);
	}
}

void AProjectileBase::SpawnHitEffect(FHitResult Hit)
{
	if (Decal)
	{
		UDecalComponent* MadeDecal = UGameplayStatics::SpawnDecalAtLocation(GetWorld(),
			Decal,
			FVector(5, 5, 5),
			Hit.ImpactPoint,
			Hit.ImpactNormal.Rotation(),
			5.f
		);

		if (MadeDecal)
		{
			MadeDecal->SetFadeScreenSize(0.005f);
		}
	}
}
