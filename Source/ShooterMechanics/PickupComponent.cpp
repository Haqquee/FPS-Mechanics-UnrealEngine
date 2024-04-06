// Fill out your copyright notice in the Description page of Project Settings.


#include "PickupComponent.h"

// Sets default values
APickupComponent::APickupComponent()
{

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;
	
	CollisionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("Collider"));
	CollisionSphere->SetSphereRadius(175.f);
	CollisionSphere->AttachToComponent(RootComponent, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
	CollisionSphere->OnComponentBeginOverlap.AddDynamic(this, &APickupComponent::OnOverlap);

 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void APickupComponent::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APickupComponent::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APickupComponent::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	APlayerCharacter* Character = Cast<APlayerCharacter>(OtherActor);

	if (Character != nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Overlapped"));
	}
	
}

