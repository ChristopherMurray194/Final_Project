// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AIController.h"
#include "AgentController.generated.h"

/**
 * 
 */
UCLASS()
class FINAL_PROJECT_API AAgentController : public AAIController
{
	GENERATED_BODY()

public:
	AAgentController();

	UPROPERTY(transient) // transient property - should not be saved
		class UBlackboardComponent* BlackboardComp;

	/** Custom BehaviourTreeComponent */
	UPROPERTY(transient)
		class UBehaviorTreeComponent* BehvaiourTreeComp;


	virtual void Tick(float DeltaSeconds) override;
	virtual void Possess(class APawn* InPawn) override;

	/** Sets a Blackboard component */
	void SetTarget(class APathNode* target);

	/** This returns the target (that is the PathNode) blackboard component */
	class APathNode* GetTarget() const;

	/* Mutator method for PlayerLocation */
	void SetPlayerLocation(FVector PlayerLocation);
	/* Accessor method for PlayerLocation */
	FVector GetPlayerLocation();

	// Location of the agent which is possessed by this controller
	FVector AgentLocation;
	FVector GetAgentLocation();
	// Get the agent's rotation
	FRotator AgentRotation;
	FRotator GetAgentRotation();

	// Set rotation of the agent possessed by this controller
	void SetActorRotation(FRotator NewRotation);


protected:
	uint8 TargetKeyID;	// Next position in path
	uint8 PlayerPosKeyID; // The location of the player
	
	// Agent that is possessed by this controller
	class AAgent* Agent;
};
