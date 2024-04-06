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

	OnCharacter = false;

	FireRate = 600; // Default fire rate of the weapon class (individual weapons will have a seperate rates)
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
		Mesh->SetCollisionProfileName(TEXT("BlockAllDynamic"));
		Mesh->PhysicsTransformUpdateMode = EPhysicsTransformUpdateMode::ComponentTransformIsKinematic;
		//Mesh->SetCollisionObjectType(ECollisionChannel::)
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
	
	OnCharacter = true;
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
	Mesh->SetCollisionProfileName(TEXT("BlockAllDynamic"));
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
			FVector Start = Muzzle->GetComponentLocation(); //For debugging purposes (displays the projectiles coming out of the weapon's muzzle)
			//FVector Start = CharacterController->PlayerCameraManager->GetCameraLocation();
			FRotator Rotation = CharacterController->PlayerCameraManager->GetCameraRotation();
			FVector Direction = Rotation.Vector();
			FVector End = Start + (Direction * 50000.f);
			DrawDebugLine(GetWorld(), Start, End, FColor::Green, true); //Debug Line
		} 
	}
}

