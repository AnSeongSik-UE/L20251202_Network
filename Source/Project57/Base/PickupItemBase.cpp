// Fill out your copyright notice in the Description page of Project Settings.


#include "PickupItemBase.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"


// Sets default values
APickupItemBase::APickupItemBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	Sphere = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
	RootComponent = Sphere;
	Sphere->SetSphereRadius(50.f);
	Sphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);


	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(RootComponent);

	SetReplicates(true);
	SetReplicateMovement(true);
}

// Called when the game starts or when spawned
void APickupItemBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APickupItemBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

