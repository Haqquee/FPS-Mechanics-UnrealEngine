#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PlayerCharacter.generated.h"


UCLASS()
class SHOOTERMECHANICS_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

	// Components
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* PlayerCamera;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Mesh, meta = (AllowPrivateAccess = "true"))
	class USkeletalMeshComponent* FPSMesh;

public:

	// Movement inputs
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputMappingContext* DefaultMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* JumpAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* MoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* LookAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* SprintAction;

	// Weapon inputs
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* FireAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* ADSAction;

	// Utility inputs
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* InteractAction;

	// Movement Variables
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float DefaultSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float SprintSpeed;

	// Camera Variables
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float DefaultFOV;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float AimFOV;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float InteractionLength;


	// Weapons
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	TSubclassOf<class AWeapon> Weapon1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	TSubclassOf<class AWeapon> Weapon2;

	AWeapon* CurrentWeapon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon)
	bool bHasWeapon;

	UFUNCTION(BlueprintCallable, Category = Weapon)
	void SetHasWeapon(bool NewHasWeapon);

	UFUNCTION(BlueprintCallable, Category = Weapon)
	bool GetHasWeapon();

	/** Boolean for Animation Blueprint to switch to another animation set */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon)
	bool bHasRifle;

	UFUNCTION(BlueprintCallable, Category = Weapon)
	void SetHasRifle(bool NewHasRifle);

	UFUNCTION(BlueprintCallable, Category = Weapon)
	bool GetHasRifle();
	

public:
	// Sets default values for this character's properties
	APlayerCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// Functions for player actions
	void Look(const struct FInputActionValue& Value);

	void Move(const struct FInputActionValue& Value);

	UFUNCTION()
	void Fire();

	void StartSprint();

	void StopSprint();

	void StartADS();

	void StopADS();

	// Weapon Spawning (currently only for debugging purposes)

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* SpawnRifle;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* SpawnHandgun;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* DropWeapon;

	void SpawnEquipRifle();

	void SpawnEquipHandgun();

	void DropCurrentWeapon();

	void Pickup();

	FHitResult PerformLineTrace(float Distance);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool isLookingAtItem;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UAIPerceptionStimuliSourceComponent* StimulusSource;

	void SetupStimulusSource(); // For enemy perception

	// Getter/setter functions
	USkeletalMeshComponent* GetFPSMesh() const;

	UCameraComponent* GetFPSCameraComponent() const;
};
