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
		class UBlackboardComponent* m_BlackboardComp;

	/** Custom BehaviourTreeComponent */
	UPROPERTY(transient)
		class UBehaviorTreeComponent* m_BehvaiourTreeComp;

	virtual void Possess(class APawn* InPawn) override;	// Override function

	/** Sets a Blackboard component */
	void SetTarget(class APathNode* target);

	/** This returns the target (that is the PathNode) blackboard component */
	class APathNode* GetTarget() const;

	/* Mutator method for PlayerLocation */
	void SetPlayerLocation(FVector PlayerLocation);

	/* Accessor method for PlayerLocation */
	FVector GetPlayerLocation();

protected:
	FName m_TargetKeyID;	// Next position in path
	FName m_PlayerPosKeyID; // The location of the player
};
