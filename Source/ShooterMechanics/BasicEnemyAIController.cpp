// Fill out your copyright notice in the Description page of Project Settings.


#include "BasicEnemyAIController.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AISenseConfig_Hearing.h"
#include "Perception/AIPerceptionTypes.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "ShooterMechanics\Characters/PlayerCharacter.h"
#include "ShooterMechanics\BasicEnemy.h"


ABasicEnemyAIController::ABasicEnemyAIController()
{
	BehaviorTreeComponent = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("Behavior Tree Component"));

	Blackboard = CreateDefaultSubobject<UBlackboardComponent>(TEXT("Blackboard Component"));

	// Setup Perception System
	AIPerception = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AI"));

	// Sight Perception
	AISight = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("Sight"));
	AISight->SightRadius = 100.f;
	AISight->LoseSightRadius = AISight->SightRadius + 10.f;
	AISight->PeripheralVisionAngleDegrees = 90.f;
	AISight->SetMaxAge(5.f);
	//AISight->DetectionByAffiliation.bDetectEnemies = true;
	//AISight->DetectionByAffiliation.bDetectFriendlies = true;
	AISight->DetectionByAffiliation.bDetectNeutrals = true;

	AIPerception->ConfigureSense(*AISight);
	AIPerception->OnTargetPerceptionUpdated.AddDynamic(this, &ABasicEnemyAIController::OnPerception);
}

void ABasicEnemyAIController::BeginPlay()
{
	Super::BeginPlay();
}

void ABasicEnemyAIController::Tick(float DeltaTime)
{
}

void ABasicEnemyAIController::OnPerception(AActor* TargetActor, FAIStimulus const Stimulus)
{
	UE_LOG(LogTemp, Warning, TEXT("Trying to detect player"));
	//APlayerCharacter* TargetPlayer = Cast<APlayerCharacter>(TargetActor);
	if (auto* const character = Cast<APlayerCharacter>(TargetActor))
	{
		if (Stimulus.WasSuccessfullySensed())
		{
			UE_LOG(LogTemp, Warning, TEXT("Player Detected"));
		}
		//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, "Player Detected");
		UE_LOG(LogTemp, Warning, TEXT("Player Detected"));
	}
}
