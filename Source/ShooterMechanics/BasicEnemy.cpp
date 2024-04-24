#include "BasicEnemy.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AISenseConfig_Hearing.h"
#include "Perception/AIPerceptionTypes.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "ShooterMechanics\Characters/PlayerCharacter.h"
#include "Engine/DamageEvents.h"
#include "Components/CapsuleComponent.h"



// Sets default values
ABasicEnemy::ABasicEnemy()
{
	//CharacterMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));

	GetCapsuleComponent()->SetOwnerNoSee(true);
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ABasicEnemy::BeginPlay()
{
	Super::BeginPlay();
	
	// Set Physics and Collision
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("BlockAllDynamic"));
	GetMesh()->SetCollisionProfileName(TEXT("CharacterMesh"));

}

// Called every frame
void ABasicEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ABasicEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

float ABasicEnemy::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	Health -= DamageAmount;
	if (Health <= 0)
	{
		GetCapsuleComponent()->Deactivate();
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		GetMesh()->SetCollisionProfileName(TEXT("Ragdoll"));
		GetMesh()->SetSimulatePhysics(true);
	}
	return 0.0f;
}


