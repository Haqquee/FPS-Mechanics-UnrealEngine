// Fill out your copyright notice in the Description page of Project Settings.

#include "Weapon.h"
#include "ShooterMechanics\PlayerCharacter.h"
#include "Camera/CameraComponent.h"

// Sets default values
AWeapon::AWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;

	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(Root);

	Muzzle = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MuzzleMesh"));
	Muzzle->SetupAttachment(Mesh);
}

// Called when the game starts or when spawned
void AWeapon::BeginPlay()
{
	Super::BeginPlay();

	//Enable Physics
	Mesh->SetSimulatePhysics(true);
	Mesh->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	
}

// Called every frame
void AWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AWeapon::AttachWeapon(APlayerCharacter* TargetCharacter)
{
	Character = TargetCharacter;
	FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, true);
	AttachToComponent(Character->GetFPSMesh(), AttachmentRules, FName(TEXT("GripPoint")));

	//Disable Physics if Weapon is Attached to Character
	Mesh->SetSimulatePhysics(false);
	Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

}

void AWeapon::DestroyWeapon()
{
	this->Destroy();
}

void AWeapon::DetachWeapon()
{ 
	Mesh->SetSimulatePhysics(true);
	Mesh->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
}

void AWeapon::Fire(APlayerCharacter* TargetCharacter)
{
	UE_LOG(LogTemp, Warning, TEXT("Firing"));
	if (TargetCharacter != nullptr) 
	{
		Character = TargetCharacter;

		// Firing Animation
		UAnimInstance* AnimInstance = Character->GetFPSMesh()->GetAnimInstance();
		if (AnimInstance != nullptr)
		{
			AnimInstance->Montage_Play(FireAnimation, 1.f);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Cannot get Weapon Fire Animation Instance."));
		}

		// Hitscan
		APlayerController* CharacterController = Cast<APlayerController>(Character->GetController());
		if ((CharacterController != nullptr) && (Muzzle != nullptr))
		{
			FHitResult OutHit;
			FVector Start = Muzzle->GetComponentLocation();
			FRotator Rotation = CharacterController->PlayerCameraManager->GetCameraRotation();
			FVector Direction = Rotation.Vector();
			FVector End = Start + (Direction * 1000.f);

			DrawDebugLine(GetWorld(), Start, End, FColor::Green, true);
		} 
		else
		{ 
			UE_LOG(LogTemp, Warning, TEXT("Cant get camera component"));
		}


	}

	UE_LOG(LogTemp, Warning, TEXT("Cannot get Target Character."));
}

