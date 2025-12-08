// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "GameFramework/Character.h"
#include "GenericTeamAgentInterface.h"
#include "BaseCharacter.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCalculatedHP, const float, InHPPercent);

class UInputAction;
class UAIPerceptionStimuliSourceComponent;

UENUM(BlueprintType)
enum class EWeaponState : uint8
{
	Unarmed = 0 UMETA(DisplayName = "Unarmed"),
	Pistol = 10 UMETA(DisplayName = "Pistol"),
	Rifle = 20 UMETA(DisplayName = "Rifle"),
	GrenadeLauncer = 30 UMETA(DisplayName = "GrenadeLauncer")
};


UCLASS()
class PROJECT57_API ABaseCharacter : public ACharacter, public IGenericTeamAgentInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABaseCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	UPROPERTY(Category = Character, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class USpringArmComponent> SpringArm;

	UPROPERTY(Category = Character, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UCameraComponent> Camera;

	UPROPERTY(Category = Character, EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UChildActorComponent> Weapon;



public:
	FORCEINLINE class USpringArmComponent* GetSpringArm() const 
	{
		return SpringArm;
	}

	FORCEINLINE class UCameraComponent* GetCameraComponent() const
	{
		return Camera;
	}

	UPROPERTY(Category = Character, EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UAIPerceptionStimuliSourceComponent> StimuliSource;


	UFUNCTION(BlueprintCallable)
	void Move(float Forward, float Right);

	UFUNCTION(BlueprintCallable)
	void Look(float Pitch, float Yaw);

	UFUNCTION(BlueprintCallable)
	void Reload();

	UFUNCTION(Server, Reliable)
	void C2S_Reload();
	bool C2S_Reload_Validate();
	void C2S_Reload_Implementation();

	UFUNCTION(NetMulticast, Unreliable)
	void S2A_Reload();
	void S2A_Reload_Implementation();

	UFUNCTION(BlueprintCallable)
	void DoFire();

	UFUNCTION(BlueprintCallable)
	void StartFire();

	UFUNCTION(BlueprintCallable)
	void StopFire();

	UFUNCTION(Server, Reliable)
	void C2S_StartFire();
	void C2S_StartFire_Implementation();

	UFUNCTION(Server, Reliable)
	void C2S_StopFire();
	void C2S_StopFire_Implementation();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Character, Replicated)
	uint8 bSprint : 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Character, Replicated)
	uint8 bLeftLean : 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Character, Replicated)
	uint8 bRightLean : 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Character, Replicated)
	uint8 bAiming : 1;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Character, Replicated)
	EWeaponState WeaponState = EWeaponState::Unarmed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Character)
	TObjectPtr<UAnimMontage> HitMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Character)
	TObjectPtr<UAnimMontage> DeathMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Character)
	TObjectPtr<UInputAction> IA_Reload;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Character)
	TObjectPtr<UInputAction> IA_Fire;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Character)
	TObjectPtr<UInputAction> IA_IronSight;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Character)
	TObjectPtr<UInputAction> IA_Sprint;

	UFUNCTION(BlueprintCallable)
	void ReloadWeapon();

	UFUNCTION()
	void OnRep_CurrentHP();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Character, ReplicatedUsing = "OnRep_CurrentHP")
	float CurrentHP = 100;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Character, Replicated)
	float MaxHP = 100;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Character, Replicated)
	uint8 bIsFire : 1 = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Character, Replicated)
	uint8 bIsIronSight : 1 = false;



	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	UFUNCTION(BlueprintCallable)
	virtual void SpawnHitEffect(FHitResult Hit);

	UFUNCTION(NetMulticast, Unreliable)
	virtual void S2A_SpawnHitEffect(FHitResult Hit);
	virtual void S2A_SpawnHitEffect_Implementation(FHitResult Hit);


	UFUNCTION(BlueprintCallable)
	void DoDeadEnd(const FHitResult& InHitResult);

	UFUNCTION(BlueprintCallable)
	void DoDead(const FHitResult& InHitResult);

	UFUNCTION(NetMulticast, Unreliable)
	void S2A_DoDead(const FHitResult& InHitResult, FName InSectionName);
	void S2A_DoDead_Implementation(const FHitResult& InHitResult, FName InSectionName);

	UFUNCTION(BlueprintCallable)
	void DoHitReact();

	UFUNCTION(NetMulticast, Unreliable)
	void S2A_DoHitReact(FName InSectionName);
	void S2A_DoHitReact_Implementation(FName InSectionName);


	UFUNCTION()
	void ProcessBeginOverlap(AActor* OverlappedActor, AActor* OtherActor);

	void EatItem(class APickupItemBase* PickedUpItem);

	void UseItem(class APickupItemBase* PickedUpItem);

	void EquipItem(class APickupItemBase* PickedUpItem);

	void StartIronSight();

	void StopIronSight();

	UFUNCTION(Server, Reliable)
	void C2S_StartIronSight();
	void C2S_StartIronSight_Implementation();

	UFUNCTION(Server, Reliable)
	void C2S_StopIronSight();
	void C2S_StopIronSight_Implementation();

	void StartSprint();

	void StopSprint();

	UFUNCTION(Server, Reliable)
	void C2S_StartSprint();
	void C2S_StartSprint_Implementation();

	UFUNCTION(Server, Reliable)
	void C2S_StopSprint();
	void C2S_StopSprint_Implementation();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Character)
	TObjectPtr<UParticleSystem> BloodEffect;

	UPROPERTY(BlueprintAssignable)
	FOnCalculatedHP OnCalculateHP;


//----------------------------------------------------------------------//
// IGenericTeamAgentInterface
//----------------------------------------------------------------------//
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Character)
	FGenericTeamId TeamID;

	/** Assigns Team Agent to given TeamID */
	virtual void SetGenericTeamId(const FGenericTeamId& InTeamID) override;

	/** Retrieve team identifier in form of FGenericTeamId */
	virtual FGenericTeamId GetGenericTeamId() const override;



	
	void DrawFrustum();

	FRotator GetAimOffset() const;

	FRotator AimRotation;
};
