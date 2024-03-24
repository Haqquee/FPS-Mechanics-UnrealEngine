#include "Weapon.h"
#include "ShooterMechanics\Characters/PlayerCharacter.h"
#include "Camera/CameraComponent.h"

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

	FireRate = 600; // Default fire rate of the weapon class (individual weapons will have a seperate rates)
}

// Called when the game starts or when spawned
void AWeapon::BeginPlay()
{
	Super::BeginPlay();

	// Enable Physics when game is started
	Mesh->SetSimulatePhysics(true);
	Mesh->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);

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

	//Disable Physics if weapon is attached to character
	Mesh->SetSimulatePhysics(false);
	Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

}

void AWeapon::DestroyWeapon()
{
	this->Destroy();
}

void AWeapon::DetachWeapon()
{ 
	// Enable weapon physics when weapon is not equipped
	Mesh->SetSimulatePhysics(true);
	Mesh->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
}

void AWeapon::Fire(APlayerCharacter* TargetCharacter)
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
			FHitResult OutHit;
			FVector Start = Muzzle->GetComponentLocation();
			FRotator Rotation = CharacterController->PlayerCameraManager->GetCameraRotation();
			FVector Direction = Rotation.Vector();
			FVector End = Start + (Direction * 1000.f);
			DrawDebugLine(GetWorld(), Start, End, FColor::Green, true); //Debug Line
		} 
	}
}

