#include "BasicEnemy.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AISenseConfig_Hearing.h"
#include "Perception/AIPerceptionTypes.h"
#include "ShooterMechanics\Characters/PlayerCharacter.h"


// Sets default values
ABasicEnemy::ABasicEnemy()
{

	// Setup Perception System
	AIPerception = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AI"));

	// Sight Perception
	AISight = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("Sight"));
	AISight->SightRadius = 50.f;
	AISight->LoseSightRadius = AISight->SightRadius + 10.f;
	AISight->PeripheralVisionAngleDegrees = 45.f;
	AISight->SetMaxAge(5.f);
	AISight->DetectionByAffiliation.bDetectEnemies = true;
	AISight->DetectionByAffiliation.bDetectFriendlies = true;
	AISight->DetectionByAffiliation.bDetectNeutrals = true;	

	AIPerception->OnTargetPerceptionUpdated.AddDynamic(this, &ABasicEnemy::OnPerception);
	AIPerception->ConfigureSense(*AISight);
	




	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ABasicEnemy::BeginPlay()
{
	Super::BeginPlay();
	
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


void ABasicEnemy::OnPerception(AActor* TargetActor, FAIStimulus const Stimulus)
{
	UE_LOG(LogTemp, Warning, TEXT("Trying to detect player"));
	//APlayerCharacter* TargetPlayer = Cast<APlayerCharacter>(TargetActor);
	if (auto* const character = Cast<APlayerCharacter>(TargetActor))
	{
		//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, "Player Detected");
		UE_LOG(LogTemp, Warning, TEXT("Player Detected"));
	}

}

