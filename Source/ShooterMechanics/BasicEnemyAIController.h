#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "BasicEnemyAIController.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTERMECHANICS_API ABasicEnemyAIController : public AAIController
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, Category = AI)
	class UAIPerceptionComponent* AIPerception;

	class UAISenseConfig_Sight* AISight;

	class UAISenseConfig_Hearing* AIHearing;

	UPROPERTY(EditAnywhere, Category = AI)
	class UBehaviorTree* BehaviorTree;

	UPROPERTY(EditAnywhere, Category = AI)
	class UBehaviorTreeComponent* BehaviorTreeComponent;

	UFUNCTION()
	void OnPerception(AActor* TargetActor, FAIStimulus const Stimulus);

public:
	ABasicEnemyAIController();

protected:
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
};
