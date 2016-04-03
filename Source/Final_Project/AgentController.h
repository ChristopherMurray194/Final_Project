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

	virtual void Possess(class APawn* InPawn);	// Override function

	/** Sets a Blackboard component */
	void setTarget(const FName& targetKeyID, class APathNode* target);

	/** This returns the target (that is the PathNode) blackboard component */
	class APathNode* getTarget() const;

protected:
	FName m_TargetKeyID;	// First position in path
};
