// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseCharacter.h"
#include "GameframeWork/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/ChildActorComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "EnhancedInputComponent.h"
#include "../Weapon/WeaponBase.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "../Weapon/BaseDamageType.h"
#include "Engine/DamageEvents.h"
#include "PickupItemBase.h"
#include "Components/DecalComponent.h"
#include "Perception/AIPerceptionStimuliSourceComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Net/UnrealNetwork.h"
#include "../InGame/InGameWidget.h"
#include "../InGame/InGameGM.h"

#include "../Network/NetworkUtil.h"
#include "../Project57.h"


// Sets default values
ABaseCharacter::ABaseCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(GetCapsuleComponent());

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);

	GetMesh()->SetRelativeLocation(FVector(0, 0, -GetCapsuleComponent()->GetUnscaledCapsuleHalfHeight()));

	GetMesh()->SetRelativeRotation(FRotator(0, -90.0f, 0));

	Weapon = CreateDefaultSubobject<UChildActorComponent>(TEXT("Weapon"));
	Weapon->SetupAttachment(GetMesh());

	StimuliSource = CreateDefaultSubobject<UAIPerceptionStimuliSourceComponent>(TEXT("StimuliSource"));

	SetGenericTeamId(1);

	bReplicates = true;
	SetReplicateMovement(true);
	bNetLoadOnClient = true;
	bNetUseOwnerRelevancy = true;
}

// Called when the game starts or when spawned
void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();

	OnActorBeginOverlap.AddDynamic(this, &ABaseCharacter::ProcessBeginOverlap);
}

// Called every frame
void ABaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	AimRotation = FRotator::ZeroRotator;

	//DrawFrustum();
}

// Called to bind functionality to input
void ABaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* UIC = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	if (UIC)
	{
		UIC->BindAction(IA_Reload, ETriggerEvent::Completed, this,
			&ABaseCharacter::Reload);

		UIC->BindAction(IA_Fire, ETriggerEvent::Started, this,
			&ABaseCharacter::StartFire);
		UIC->BindAction(IA_Fire, ETriggerEvent::Completed, this,
			&ABaseCharacter::StopFire);

		UIC->BindAction(IA_IronSight, ETriggerEvent::Started, this,
			&ABaseCharacter::StartIronSight);
		UIC->BindAction(IA_IronSight, ETriggerEvent::Completed, this,
			&ABaseCharacter::StopIronSight);

		UIC->BindAction(IA_Sprint, ETriggerEvent::Started, this,
			&ABaseCharacter::StartSprint);
		UIC->BindAction(IA_Sprint, ETriggerEvent::Completed, this,
			&ABaseCharacter::StopSprint);
	}

}

void ABaseCharacter::Move(float Forward, float Right)
{
	const FRotator CameraRotation =  GetController()->GetControlRotation();
	const FRotator YawRotation = FRotator(0, CameraRotation.Yaw, 0);
	const FRotator YawRollRotation = FRotator(0, CameraRotation.Yaw, CameraRotation.Roll);


	const FVector ForwardVector = UKismetMathLibrary::GetForwardVector(YawRotation);
	AddMovementInput(ForwardVector, Forward);

	const FVector RightVector = UKismetMathLibrary::GetRightVector(YawRollRotation);
	AddMovementInput(RightVector, Right);


//	AddMovementInput(FVector(Forward, Right, 0));
}

void ABaseCharacter::Look(float Pitch, float Yaw)
{
	AddControllerPitchInput(Pitch);
	AddControllerYawInput(Yaw);
}

void ABaseCharacter::Reload()
{
	C2S_Reload();
}

bool ABaseCharacter::C2S_Reload_Validate()
{
	return true;
}

void ABaseCharacter::C2S_Reload_Implementation()
{
	if (!HasAuthority())
	{
		return;
	}
	S2A_Reload();
}

void ABaseCharacter::S2A_Reload_Implementation()
{
	AWeaponBase* ChildWeapon = Cast<AWeaponBase>(Weapon->GetChildActor());
	if (ChildWeapon)
	{
		PlayAnimMontage(ChildWeapon->ReloadMontage);
	}
}

void ABaseCharacter::DoFire()
{
	AWeaponBase* ChildWeapon = Cast<AWeaponBase>(Weapon->GetChildActor());
	if (ChildWeapon)
	{
		ChildWeapon->Fire();
	}
}

void ABaseCharacter::StartFire()
{
	bIsFire = true;
	C2S_StartFire();
}

void ABaseCharacter::StopFire()
{
	bIsFire = false;
	C2S_StopFire();
}

void ABaseCharacter::C2S_StartFire_Implementation()
{
	bIsFire = true;
	DoFire();
}

void ABaseCharacter::C2S_StopFire_Implementation()
{
	bIsFire = false;
	AWeaponBase* ChildWeapon = Cast<AWeaponBase>(Weapon->GetChildActor());
	if (ChildWeapon)
	{
		ChildWeapon->StopFire();
	}
}

void ABaseCharacter::ReloadWeapon()
{
	AWeaponBase* ChildWeapon = Cast<AWeaponBase>(Weapon->GetChildActor());
	if (ChildWeapon)
	{
		ChildWeapon->Reload();
	}
}

void ABaseCharacter::OnRep_CurrentHP()
{
	OnCalculateHP.Broadcast(CurrentHP / MaxHP);
}

float ABaseCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	FHitResult HitResult;

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

		HitResult = Event->HitInfo;
		SpawnHitEffect(HitResult);
	}
	else if (DamageEvent.IsOfType(FRadialDamageEvent::ClassID))
	{
		FRadialDamageEvent* Event = (FRadialDamageEvent*)(&DamageEvent);
		if (Event)
		{
			CurrentHP -= DamageAmount;

			UE_LOG(LogTemp, Warning, TEXT("Radial Damage %f %s"), DamageAmount, *Event->DamageTypeClass->GetName());
		}
	}
	else //(DamageEvent.IsOfType(FDamageEvent::ClassID))
	{
		CurrentHP -= DamageAmount;
		UE_LOG(LogTemp, Warning, TEXT("Damage %f"), DamageAmount);
	}

	DoHitReact();

	OnRep_CurrentHP();

	if (CurrentHP <= 0)
	{
		AInGameGM* GM = Cast<AInGameGM>(UGameplayStatics::GetGameMode(GetWorld()));
		if (GM)
		{
			GM->CheckSurvivorCount();
		}
		DoDead(HitResult);
	}

	return DamageAmount;
}

void ABaseCharacter::DoDeadEnd(const FHitResult& InHitResult)
{
	//GetController()->SetActorEnableCollision(false);
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
	GetMesh()->SetSimulatePhysics(true);
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	GetMesh()->AddImpulse(-InHitResult.ImpactNormal * 10000.f, InHitResult.BoneName);

}

void ABaseCharacter::DoDead(const FHitResult& InHitResult)
{
	FName SectionName = FName(FString::Printf(TEXT("%d"), FMath::RandRange(1, 6)));
	S2A_DoDead(InHitResult, SectionName);
}

void ABaseCharacter::S2A_DoDead_Implementation(const FHitResult& InHitResult, FName InSectionName)
{
	SetPhysicsReplicationMode(EPhysicsReplicationMode::Resimulation);
	GetMesh()->SetIsReplicated(true);
	DoDeadEnd(InHitResult);
	//PlayAnimMontage(DeathMontage, 1.0f, InSectionName);
}

void ABaseCharacter::DoHitReact()
{
	FName SectionName = FName(FString::Printf(TEXT("%d"), FMath::RandRange(1, 8)));
	S2A_DoHitReact(SectionName);
}

void ABaseCharacter::S2A_DoHitReact_Implementation(FName InSectionName)
{
	PlayAnimMontage(HitMontage, 1.0f, InSectionName);
}

void ABaseCharacter::ProcessBeginOverlap(AActor* OverlappedActor, AActor* OtherActor)
{
	//서버와 클라 둘다 실행시키는걸 막기
	APlayerController* PC = Cast<APlayerController>(GetController());
	if (!PC || !PC->HasAuthority())
	{
		return;
	}
	//if (PC)
	//{
	//	if (PC->GetNetMode() == ENetMode::NM_Client)
	//	{
	//		return;
	//	}
	//}

	APickupItemBase* PickedUpItem = Cast<APickupItemBase>(OtherActor);

	if (PickedUpItem)
	{
		switch (PickedUpItem->ItemType)
		{
		case EItemType::Use: //Game Ability System
			UseItem(PickedUpItem);
			break;
		case EItemType::Eat:
			EatItem(PickedUpItem);
			break;
		case EItemType::Equip:
			EquipItem(PickedUpItem);
			break;
		}

		PickedUpItem->SetOwner(this);
		if (!PickedUpItem->bIsInfinity)
		{
			PickedUpItem->Destroy();
		}
	}
}

void ABaseCharacter::EatItem(APickupItemBase* PickedUpItem)
{
}

void ABaseCharacter::UseItem(APickupItemBase* PickedUpItem)
{
}

void ABaseCharacter::EquipItem(APickupItemBase* PickedUpItem)
{
	Weapon->SetChildActorClass(PickedUpItem->ItemTemplate);
	AWeaponBase* ChildWeapon = Cast<AWeaponBase>(Weapon->GetChildActor());

	if (ChildWeapon)
	{
		if (ChildWeapon->Name.Compare(TEXT("Pistol")) == 0)
		{
			ChildWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, ChildWeapon->SocketName);
			WeaponState = EWeaponState::Pistol;
			ChildWeapon->SetOwner(this);
		}
		else if(ChildWeapon->Name.Compare(TEXT("Rifle")) == 0)
		{
			ChildWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, ChildWeapon->SocketName);
			WeaponState = EWeaponState::Rifle;
			ChildWeapon->SetOwner(this);
		}
		else if (ChildWeapon->Name.Compare(TEXT("GrenadeLauncer")) == 0)
		{
			ChildWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, ChildWeapon->SocketName);
			WeaponState = EWeaponState::Rifle;
			//WeaponState = EWeaponState::GrenadeLauncer;
			ChildWeapon->SetOwner(this);
		}
	}
}

void ABaseCharacter::StartIronSight()
{
	bIsIronSight = true;
	bAiming = true;
	C2S_StartIronSight_Implementation();
}

void ABaseCharacter::StopIronSight()
{
	bIsIronSight = false;
	bAiming = false;
	C2S_StopIronSight_Implementation();
}

void ABaseCharacter::C2S_StartIronSight_Implementation()
{
	bIsIronSight = true;
	bAiming = true;
}

void ABaseCharacter::C2S_StopIronSight_Implementation()
{
	bIsIronSight = false;
	bAiming = false;
}

void ABaseCharacter::StartSprint()
{
	//자연스럽게 보이기 위해 일단 속도를 높임
	GetCharacterMovement()->MaxWalkSpeed = 600.0f;
	C2S_StartSprint();
}

void ABaseCharacter::StopSprint()
{
	GetCharacterMovement()->MaxWalkSpeed = 300.0f;
	C2S_StopSprint();
}

void ABaseCharacter::C2S_StartSprint_Implementation()
{
	GetCharacterMovement()->MaxWalkSpeed = 600.0f;
}

void ABaseCharacter::C2S_StopSprint_Implementation()
{
	GetCharacterMovement()->MaxWalkSpeed = 300.0f;
}

void ABaseCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ABaseCharacter, bSprint);
	DOREPLIFETIME(ABaseCharacter, bLeftLean);
	DOREPLIFETIME(ABaseCharacter, bRightLean);
	DOREPLIFETIME(ABaseCharacter, CurrentHP);
	DOREPLIFETIME(ABaseCharacter, MaxHP);
	DOREPLIFETIME(ABaseCharacter, bIsFire);
	DOREPLIFETIME(ABaseCharacter, bIsIronSight);
	DOREPLIFETIME(ABaseCharacter, WeaponState);
}

//----------------------------------------------------------------------//
// IGenericTeamAgentInterface
//----------------------------------------------------------------------//
void ABaseCharacter::SetGenericTeamId(const FGenericTeamId& InTeamID)
{
	TeamID = InTeamID;
}

FGenericTeamId ABaseCharacter::GetGenericTeamId() const
{
	return TeamID;
}

void ABaseCharacter::SpawnHitEffect(FHitResult Hit)
{
	S2A_SpawnHitEffect(Hit);
}

void ABaseCharacter::S2A_SpawnHitEffect_Implementation(FHitResult Hit)
{
	if (BloodEffect)
	{
		//UE_LOG(LogTemp, Warning, TEXT("BloodEffect"));
		UGameplayStatics::SpawnEmitterAtLocation(
			GetWorld(),
			BloodEffect,
			Hit.ImpactPoint,
			Hit.ImpactNormal.Rotation()
		);
	}
}

void ABaseCharacter::DrawFrustum()
{
	FMinimalViewInfo CameraView;
	// 카메라의 현재 뷰 정보(위치, 회전, FOV, AspectRatio 등)를 가져옵니다.
	Camera->GetCameraView(0.0f, CameraView);

	// 1. 필요한 투영 매개변수 정의

	// FOV를 라디안으로 변환 (절반 각도)
	const float HalfFOVRadians = FMath::DegreesToRadians(CameraView.FOV / 2.0f);
	const float AspectRatio = CameraView.AspectRatio;

	// Near/Far Clip Distance 정의 (원근 절두체에 필수)
	// 실제 카메라 컴포넌트 설정을 반영하여 값을 정해야 합니다.
	const float NearClipDistance = 10.0f;  // 카메라 앞 10cm (Near Plane)
	const float FarClipDistance = 1000.0f; // 10미터 (Far Plane)

	// 2. Near/Far Plane의 Half Height/Width 계산 (삼각법 사용)

	// H = D * tan(HalfFOV)
	const float HalfHeightNear = NearClipDistance * FMath::Tan(HalfFOVRadians);
	// W = H * AspectRatio
	const float HalfWidthNear = HalfHeightNear * AspectRatio;

	const float HalfHeightFar = FarClipDistance * FMath::Tan(HalfFOVRadians);
	const float HalfWidthFar = HalfHeightFar * AspectRatio;

	// 3. Frustum의 8개 꼭짓점을 로컬 공간에서 계산
	// 카메라 로컬 좌표계: +X = Forward, +Y = Right, +Z = Up
	FVector Corners[8]; // 0~3: Near Plane, 4~7: Far Plane

	// Near Plane (Z=Up/Down, Y=Right/Left)
	Corners[0] = FVector(NearClipDistance, -HalfWidthNear, HalfHeightNear);  // Near Top-Left
	Corners[1] = FVector(NearClipDistance, HalfWidthNear, HalfHeightNear);   // Near Top-Right
	Corners[2] = FVector(NearClipDistance, HalfWidthNear, -HalfHeightNear);  // Near Bottom-Right
	Corners[3] = FVector(NearClipDistance, -HalfWidthNear, -HalfHeightNear); // Near Bottom-Left

	// Far Plane
	Corners[4] = FVector(FarClipDistance, -HalfWidthFar, HalfHeightFar);   // Far Top-Left
	Corners[5] = FVector(FarClipDistance, HalfWidthFar, HalfHeightFar);    // Far Top-Right
	Corners[6] = FVector(FarClipDistance, HalfWidthFar, -HalfHeightFar);   // Far Bottom-Right
	Corners[7] = FVector(FarClipDistance, -HalfWidthFar, -HalfHeightFar);  // Far Bottom-Left

	// 4. 로컬 좌표를 월드 좌표로 변환
	FTransform CameraTransform(CameraView.Rotation, CameraView.Location, FVector(1.0f));
	FVector WorldCorners[8];
	for (int32 i = 0; i < 8; ++i)
	{
		WorldCorners[i] = CameraTransform.TransformPosition(Corners[i]);
	}

	// 5. DebugDrawLine을 사용하여 절두체의 12개 모서리 그리기
	const float LineDuration = 0.0f; // 한 프레임만 표시
	const float LineThickness = 3.0f;
	const FLinearColor LineColor = FLinearColor::Green;

	// Near Plane (앞면) 그리기 (4개 선)
	UKismetSystemLibrary::DrawDebugLine(GetWorld(), WorldCorners[0], WorldCorners[1], LineColor, LineDuration, LineThickness);
	UKismetSystemLibrary::DrawDebugLine(GetWorld(), WorldCorners[1], WorldCorners[2], LineColor, LineDuration, LineThickness);
	UKismetSystemLibrary::DrawDebugLine(GetWorld(), WorldCorners[2], WorldCorners[3], LineColor, LineDuration, LineThickness);
	UKismetSystemLibrary::DrawDebugLine(GetWorld(), WorldCorners[3], WorldCorners[0], LineColor, LineDuration, LineThickness);

	// Far Plane (뒷면) 그리기 (4개 선)
	UKismetSystemLibrary::DrawDebugLine(GetWorld(), WorldCorners[4], WorldCorners[5], LineColor, LineDuration, LineThickness);
	UKismetSystemLibrary::DrawDebugLine(GetWorld(), WorldCorners[5], WorldCorners[6], LineColor, LineDuration, LineThickness);
	UKismetSystemLibrary::DrawDebugLine(GetWorld(), WorldCorners[6], WorldCorners[7], LineColor, LineDuration, LineThickness);
	UKismetSystemLibrary::DrawDebugLine(GetWorld(), WorldCorners[7], WorldCorners[4], LineColor, LineDuration, LineThickness);

	// 두 평면 연결 (4개 선)
	UKismetSystemLibrary::DrawDebugLine(GetWorld(), WorldCorners[0], WorldCorners[4], LineColor, LineDuration, LineThickness);
	UKismetSystemLibrary::DrawDebugLine(GetWorld(), WorldCorners[1], WorldCorners[5], LineColor, LineDuration, LineThickness);
	UKismetSystemLibrary::DrawDebugLine(GetWorld(), WorldCorners[2], WorldCorners[6], LineColor, LineDuration, LineThickness);
	UKismetSystemLibrary::DrawDebugLine(GetWorld(), WorldCorners[3], WorldCorners[7], LineColor, LineDuration, LineThickness);
}

FRotator ABaseCharacter::GetAimOffset() const
{
	const FVector AimDirWS = GetBaseAimRotation().Vector();
	const FVector AimDirLS = ActorToWorld().InverseTransformVectorNoScale(AimDirWS);
	const FRotator AimRotLS = AimDirLS.Rotation();
	return AimRotLS;
}
