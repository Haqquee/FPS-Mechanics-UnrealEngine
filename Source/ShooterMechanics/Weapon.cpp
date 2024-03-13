// Fill out your copyright notice in the Description page of Project Settings.

#include "Weapon.h"
#include "ShooterMechanics\PlayerCharacter.h"

// Sets default values
AWeapon::AWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;

	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(Root);
}

// Called when the game starts or when spawned
void AWeapon::BeginPlay()
{
	Super::BeginPlay();
	
	//Mesh->SetVisibility(false);
	
}

// Called every frame
void AWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AWeapon::AttachWeapon(APlayerCharacter* TargetCharacter)
{
	Character = TargetCharacter;
	// -10 0 0
	// 0 20 345
	// 1 1 1
	FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, true);

	//FTransform SocketTransform = Character->GetFPSMesh()->GetSocketTransform(FName(TEXT("GripPoint")));
	//FVector TranslationOffset(-10.0f, 0.0f, 0.0f);
	//FRotator RotationOffset(0.0f, 20.0f, 345.0f);
	//SocketTransform.AddToTranslation(TranslationOffset);
	//SocketTransform.ConcatenateRotation(RotationOffset.Quaternion());
	AttachToComponent(Character->GetFPSMesh(), AttachmentRules, FName(TEXT("GripPoint")));

}

