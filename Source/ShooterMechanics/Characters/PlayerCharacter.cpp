#include "PlayerCharacter.h"
#include "Camera/CameraComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Perception/AIPerceptionStimuliSourceComponent.h"
#include "Perception/AISense_Sight.h"
#include "ShooterMechanics\Weapons/Weapon.h"

// Sets default values
APlayerCharacter::APlayerCharacter()
{
 	// Set this character to call Tick() every frame. 
	PrimaryActorTick.bCanEverTick = true;

	// Capsule
	GetCapsuleComponent()->InitCapsuleSize(50.0f, 105.0f);

	// Camera
	PlayerCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("PlayerCamera"));
	PlayerCamera->SetupAttachment(GetCapsuleComponent());
	PlayerCamera->bUsePawnControlRotation = true;
	PlayerCamera->SetRelativeLocation(FVector(10.0f, 0.0f, 85.0f));
	DefaultFOV = 120.f;
	AimFOV = 75.f;
	InteractionLength = 300.f;
	PlayerCamera->SetFieldOfView(DefaultFOV);

	// First Person Mesh
	FPSMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FirstPersonMesh"));
	FPSMesh->SetupAttachment(PlayerCamera);
	FPSMesh->SetOnlyOwnerSee(true);
	FPSMesh->bCastDynamicShadow = false;
	FPSMesh->CastShadow = false;
	FPSMesh->SetRelativeLocation(FVector(-10.f, 0.f, -150.f));

	// Movement
	DefaultSpeed = 600.f;
	SprintSpeed = 1000.f;
	GetCharacterMovement()->MaxWalkSpeed = DefaultSpeed;

	// Weapon
	bHasWeapon = false;
	bHasRifle = false;
	CurrentWeapon = nullptr;


	// Stimulus (For enemy detection)
	StimulusSource = CreateDefaultSubobject<UAIPerceptionStimuliSourceComponent>(TEXT("Stimulus"));
	if (StimulusSource != nullptr)
	{
		StimulusSource->RegisterForSense(TSubclassOf<UAISense_Sight>());
		StimulusSource->RegisterWithPerceptionSystem();
	}

	// Other Variables
	

	// Get the transformation vector of the player's grip point (to attach a weapon to)
	const FTransform GripSocket = GetFPSMesh()->GetSocketTransform(FName(TEXT("GripPoint")), ERelativeTransformSpace::RTS_World);
	
	
}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	UE_LOG(LogTemp, Warning, TEXT("Start"));

	// Set the input mapping context to the defualt context when the game starts
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

	FHitResult HitResult = PerformLineTrace(InteractionLength);

	AWeapon* Weapon = Cast<AWeapon>(HitResult.GetActor());
	if (Weapon)
	{
		isLookingAtItem = true;
	}
	else
	{
		isLookingAtItem = false;
	}
}

// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	//Super::SetupPlayerInputComponent(PlayerInputComponent);
	UEnhancedInputComponent* PlayerInput = Cast<UEnhancedInputComponent>(PlayerInputComponent);

	// Movement controls
	PlayerInput->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
	PlayerInput->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);
	PlayerInput->BindAction(LookAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Look);
	PlayerInput->BindAction(MoveAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Move);
	PlayerInput->BindAction(SprintAction, ETriggerEvent::Started, this, &APlayerCharacter::StartSprint);
	PlayerInput->BindAction(SprintAction, ETriggerEvent::Completed, this, &APlayerCharacter::StopSprint);

	// Weapon controls
	PlayerInput->BindAction(SpawnRifle, ETriggerEvent::Triggered, this, &APlayerCharacter::SpawnEquipRifle);
	PlayerInput->BindAction(SpawnHandgun, ETriggerEvent::Triggered, this, &APlayerCharacter::SpawnEquipHandgun);
	PlayerInput->BindAction(DropWeapon, ETriggerEvent::Triggered, this, &APlayerCharacter::DropCurrentWeapon);
	PlayerInput->BindAction(FireAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Fire);
	PlayerInput->BindAction(ADSAction, ETriggerEvent::Started, this, &APlayerCharacter::StartADS);
	PlayerInput->BindAction(ADSAction, ETriggerEvent::Completed, this, &APlayerCharacter::StopADS);

	// Interaction controls
	PlayerInput->BindAction(InteractAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Pickup);

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
	if (CurrentWeapon != nullptr)
	{
		CurrentWeapon->OnFire(this);
	}
}

void APlayerCharacter::StartSprint()
{
	GetCharacterMovement()->MaxWalkSpeed = SprintSpeed;
}

void APlayerCharacter::StopSprint()
{
	GetCharacterMovement()->MaxWalkSpeed = DefaultSpeed;
}

void APlayerCharacter::StartADS()
{
	PlayerCamera->SetFieldOfView(AimFOV);
	if (CurrentWeapon != nullptr)
	{
		CurrentWeapon->StartADS(this);
	}
}

void APlayerCharacter::StopADS()
{
	PlayerCamera->SetFieldOfView(DefaultFOV);
	if (CurrentWeapon != nullptr)
	{
		CurrentWeapon->StopADS(this);	
	}
	
}


void APlayerCharacter::DropCurrentWeapon()
{
	if (CurrentWeapon != nullptr)
	{
		CurrentWeapon->DetachWeapon();
		CurrentWeapon = nullptr;
		
	}
}

void APlayerCharacter::Pickup()
{
	if (PlayerCamera != nullptr)
	{
		// Weapon pickup
		FHitResult ItemHitResult = PerformLineTrace(InteractionLength);
		//UE_LOG(LogTemp, Warning, TEXT("Hit actor class: %s"), *ItemHitResult.GetActor()->GetClass()->GetName()); // Debug line
		AWeapon* Weapon = Cast<AWeapon>(ItemHitResult.GetActor());
		if (Weapon != nullptr)
		{
			Weapon->OnPickup();
			if (CurrentWeapon == nullptr)
			{
				Weapon->AttachWeapon(this);
			}
			else
			{
				this->DropCurrentWeapon();
				Weapon->AttachWeapon(this);
			}
		}

	}
}

FHitResult APlayerCharacter::PerformLineTrace(float Distance)
{
	FHitResult HitResult;
	FVector Start = PlayerCamera->GetComponentLocation();
	FVector Direction = PlayerCamera->GetForwardVector();
	FVector End = Start + (Direction * Distance);
	if (GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECollisionChannel::ECC_Visibility))
	{
		return HitResult;
	}
	return FHitResult();
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

void APlayerCharacter::SetupStimulusSource()
{
}

// Getter/Setter functions

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

UCameraComponent* APlayerCharacter::GetFPSCameraComponent() const
{
	return PlayerCamera;
}

