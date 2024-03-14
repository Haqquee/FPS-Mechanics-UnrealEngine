// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"
#include "Camera/CameraComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "ShooterMechanics\Weapon.h"




// Sets default values
APlayerCharacter::APlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Capsule
	GetCapsuleComponent()->InitCapsuleSize(50.0f, 105.0f);

	// Camera
	PlayerCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("PlayerCamera"));
	PlayerCamera->SetupAttachment(GetCapsuleComponent());
	PlayerCamera->bUsePawnControlRotation = true;
	PlayerCamera->SetRelativeLocation(FVector(10.0f, 0.0f, 85.0f));

	// First Person Mesh
	FPSMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FirstPersonMesh"));
	FPSMesh->SetupAttachment(PlayerCamera);
	FPSMesh->SetOnlyOwnerSee(true);
	FPSMesh->bCastDynamicShadow = false;
	FPSMesh->CastShadow = false;

	// Movement
	DefaultSpeed = 600.f;
	SprintSpeed = 1000.f;
	GetCharacterMovement()->MaxWalkSpeed = DefaultSpeed;

	// Weapon
	bHasWeapon = false;
	bHasRifle = true;
	CurrentWeapon = nullptr;

	const FTransform GripSocket = GetFPSMesh()->GetSocketTransform(FName(TEXT("GripPoint")), ERelativeTransformSpace::RTS_World);
	//
	
	
}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	UE_LOG(LogTemp, Warning, TEXT("Start"));

	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}

	
}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	//Super::SetupPlayerInputComponent(PlayerInputComponent);
	UEnhancedInputComponent* PlayerInput = Cast<UEnhancedInputComponent>(PlayerInputComponent);

	PlayerInput->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
	PlayerInput->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);
	PlayerInput->BindAction(LookAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Look);
	PlayerInput->BindAction(MoveAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Move);
	PlayerInput->BindAction(FireAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Fire);
	PlayerInput->BindAction(SprintAction, ETriggerEvent::Started, this, &APlayerCharacter::StartSprint);
	PlayerInput->BindAction(SprintAction, ETriggerEvent::Completed, this, &APlayerCharacter::StopSprint);

	//Testing Weapon Functionalities
	PlayerInput->BindAction(SpawnRifle, ETriggerEvent::Triggered, this, &APlayerCharacter::SpawnEquipRifle);
	PlayerInput->BindAction(SpawnHandgun, ETriggerEvent::Triggered, this, &APlayerCharacter::SpawnEquipHandgun);

}

void APlayerCharacter::Move(const struct FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr) 
	{
		AddMovementInput(GetActorForwardVector(), MovementVector.Y);
		AddMovementInput(GetActorRightVector(), MovementVector.X);
	}
}


void APlayerCharacter::Look(const struct FInputActionValue& Value)
{
	FVector2D LookVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		AddControllerYawInput(LookVector.X);
		AddControllerPitchInput(LookVector.Y);
	}
}

void APlayerCharacter::Fire()
{

}

void APlayerCharacter::StartSprint()
{
	GetCharacterMovement()->MaxWalkSpeed = SprintSpeed;
}

void APlayerCharacter::StopSprint()
{
	GetCharacterMovement()->MaxWalkSpeed = DefaultSpeed;
}


void APlayerCharacter::SetHasWeapon(bool NewHasWeapon)
{
	bHasWeapon = NewHasWeapon;
}

bool APlayerCharacter::GetHasWeapon()
{
	return bHasWeapon;
}

void APlayerCharacter::SetHasRifle(bool bNewHasRifle)
{
	bHasRifle = bNewHasRifle;
}

bool APlayerCharacter::GetHasRifle()
{
	return bHasRifle;
}


USkeletalMeshComponent* APlayerCharacter::GetFPSMesh() const
{
	return FPSMesh;
}

void APlayerCharacter::SpawnEquipRifle()
{
	if (CurrentWeapon != nullptr)
	{
		CurrentWeapon->Destroy();
	}

	if ((Weapon1 != nullptr))
	{
		AWeapon* NewRifle = GetWorld()->SpawnActor<AWeapon>(Weapon1);
		NewRifle->AttachWeapon(this);
		this->SetHasWeapon(true);
		CurrentWeapon = NewRifle;
	}
}

void APlayerCharacter::SpawnEquipHandgun()
{
	if (CurrentWeapon != nullptr)
	{
		CurrentWeapon->Destroy();
	}

	if ((Weapon2 != nullptr))
	{
		AWeapon* NewHandgun = GetWorld()->SpawnActor<AWeapon>(Weapon2);
		NewHandgun->AttachWeapon(this);
		this->SetHasWeapon(true);
		CurrentWeapon = NewHandgun;
	}
}
