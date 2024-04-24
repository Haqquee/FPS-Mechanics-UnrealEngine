#include "Weapon.h"
#include "ShooterMechanics\Characters/PlayerCharacter.h"
#include "ShooterMechanics\BasicEnemy.h"
#include "Camera/CameraComponent.h"
#include "Engine/DamageEvents.h"

// Sets default values
AWeapon::AWeapon()
{
 	// Set this actor to call Tick() every frame. 
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;

	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(Root);

	Muzzle = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MuzzleMesh"));
	Muzzle->SetupAttachment(Mesh);

	OnCharacter = false;

	FireRate = 600; // Default fire rate of the weapon class (individual weapons will have a seperate rates)
	Damage = 40;

	AimSocket = Mesh->GetSocketTransform(FName(TEXT("AimSocket")), ERelativeTransformSpace::RTS_World);
}

// Called when the game starts or when spawned
void AWeapon::BeginPlay()
{
	Super::BeginPlay();

	// Enable Physics when game is started
	//Mesh->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	if (OnCharacter)
	{
		Mesh->SetSimulatePhysics(false);
		Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
	else
	{
		Mesh->SetSimulatePhysics(true);
		Mesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		Mesh->SetCollisionProfileName(TEXT("Weapon"));
		
		
		Mesh->PhysicsTransformUpdateMode = EPhysicsTransformUpdateMode::ComponentTransformIsKinematic;
		
	}
	
	FireRate = 60 / FireRate;
	
}

// Called every frame
void AWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Attach weapon to the player's hand (equip weapon)
void AWeapon::AttachWeapon(APlayerCharacter* TargetCharacter)
{
	Character = TargetCharacter;
	FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, true);
	AttachToComponent(Character->GetFPSMesh(), AttachmentRules, FName(TEXT("GripPoint")));
	Character->CurrentWeapon = this;

	//Disable Physics if weapon is attached to character
	Mesh->SetSimulatePhysics(false);
	//Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Mesh->SetCollisionProfileName(TEXT("NoCollision"));
	
	SetAttachmentTransform();
	OnCharacter = true;
}

void AWeapon::SetAttachmentTransform()
{
	// Base implementation, no default behaviour
	// For testing purposes, using the values for Rifle
	FVector AttachmentLocation = FVector(0.f, 7.5f, 2.5f);
	FRotator AttachmentRotation = FRotator(15.f, -5.f, 0.f);

	Mesh->SetRelativeLocation(AttachmentLocation);
	Mesh->SetRelativeRotation(AttachmentRotation);
}

void AWeapon::OnPickup()
{
	UE_LOG(LogTemp, Warning, TEXT("Picked up weapon."));
}

void AWeapon::DestroyWeapon()
{
	this->Destroy();
}

void AWeapon::DetachWeapon()
{ 
	// Enable weapon physics when weapon is not equipped
	Mesh->SetSimulatePhysics(true);
	Mesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	Mesh->SetCollisionProfileName(TEXT("Weapon"));
}

void AWeapon::OnFire(APlayerCharacter* TargetCharacter)
{
	if (TargetCharacter != nullptr)
	{
		Character = TargetCharacter;

		// Firing Animation
		UAnimInstance* AnimInstance = Character->GetFPSMesh()->GetAnimInstance();
		if (AnimInstance != nullptr)
		{
			AnimInstance->Montage_Play(FireAnimation, 1.f);
		}

		// Raycast (traces a line from the weapon's muzzle to the point at which the player is aiming)
		APlayerController* CharacterController = Cast<APlayerController>(Character->GetController());
		if ((CharacterController != nullptr) && (Muzzle != nullptr))
		{
			FHitResult HitResult;
			FVector DebugLineStart = Muzzle->GetComponentLocation(); //For debugging purposes (displays the projectiles coming out of the weapon's muzzle)
			FVector Start = CharacterController->PlayerCameraManager->GetCameraLocation();
			FRotator Rotation = CharacterController->PlayerCameraManager->GetCameraRotation();
			FVector Direction = Rotation.Vector();
			FVector End = Start + (Direction * 50000.f);
			DrawDebugLine(GetWorld(), DebugLineStart, End, FColor::Green, false, 0.5f); //Debug Line

			FCollisionQueryParams CollisionParameters;
			CollisionParameters.AddIgnoredActor(this);
			if (GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECollisionChannel::ECC_Visibility, CollisionParameters))
			{
				ABasicEnemy* HitActor = Cast<ABasicEnemy>(HitResult.GetActor());
				if (HitActor != nullptr)
				{
					FDamageEvent DamageEvent;
					HitActor->TakeDamage(this->Damage, DamageEvent, nullptr, this);
				}
			}
		} 
	}
}

void AWeapon::StartADS(APlayerCharacter* TargetCharacter)
{
	UCameraComponent* CharacterCamera = TargetCharacter->GetFPSCameraComponent();
	USkeletalMeshComponent* CharacterMesh = TargetCharacter->GetFPSMesh();
	if ((CharacterMesh != nullptr) && (CharacterCamera != nullptr))
	{
		UE_LOG(LogTemp, Warning, TEXT("Mesh valid")); // Debug log
		
		FAttachmentTransformRules AttachmentRules(
			EAttachmentRule::KeepRelative,
			EAttachmentRule::KeepRelative,
			EAttachmentRule::KeepRelative,
			false
		);

		Mesh->AttachToComponent(CharacterCamera , AttachmentRules);
		Mesh->SetRelativeLocation(FVector(0.f, 0.f, -19.f)); 
		Mesh->SetRelativeRotation(FRotator(0.f, -90.f, 0.f)); 

	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Mesh not valid")); // Debug log
	}
	
}

void AWeapon::StopADS(APlayerCharacter* TargetCharacter)
{
	Mesh->DetachFromComponent(FDetachmentTransformRules::KeepRelativeTransform);
	Mesh->AttachToComponent(Root, FAttachmentTransformRules::SnapToTargetIncludingScale);
	AttachWeapon(TargetCharacter);
	
}

