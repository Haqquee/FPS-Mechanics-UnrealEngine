#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BasicEnemy.generated.h"

UCLASS()
class SHOOTERMECHANICS_API ABasicEnemy : public ACharacter
{
	GENERATED_BODY()

public:

	// AI
	UPROPERTY(VisibleAnywhere, Category = AI)
	class UAIPerceptionComponent* AIPerception;

	class UAISenseConfig_Sight* AISight;

	class UAISenseConfig_Hearing* AIHearing;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Mesh, meta = (AllowPrivateAccess = "true"))
	class USkeletalMeshComponent* CharacterMesh;


public:
	// Sets default values for this character's properties
	ABasicEnemy();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION()
	void OnPerception(AActor* TargetActor, FAIStimulus const Stimulus);

};
