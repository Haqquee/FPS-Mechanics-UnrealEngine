// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Weapon.generated.h"

UCLASS()
class SHOOTERMECHANICS_API AWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWeapon();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Components
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	class USceneComponent* Root;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	class USkeletalMeshComponent* Mesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	class UStaticMeshComponent* Muzzle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	FTransform PlacementTransform;

	// Sound
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	USoundBase* FireSound;

	// Animation
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	UAnimMontage* FireAnimation;

	// Input
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputMappingContext* FireMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* FireAction;

	APlayerCharacter* Character;
	
	// Actions
	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void AttachWeapon(APlayerCharacter* TargetCharacter);

	void OnPickup();

	void DestroyWeapon();

	void DetachWeapon();

	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void OnFire(APlayerCharacter* TargetCharacter);

	bool OnCharacter;

	// Weapon Specific Properties
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Poperties")
	int ClipSize;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Poperties")
	int MaxAmmo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Poperties")
	float FireRate;


};
