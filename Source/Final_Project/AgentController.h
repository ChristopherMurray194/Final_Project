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
		UBlackboardComponent* m_BlackboardComp;

	/** Custom BehaviourTreeComponent */
	UPROPERTY(transient)
		UBehaviorTreeComponent* m_BehvaiourTreeComp;

	virtual void Possess(class APawn* InPawn);	// Override function

	/** Sets a Blackboard component */
	void setTarget(class AActor* target);

	/** This returns the Blackboard component */
	class AActor* getTarget() const;

protected:
	uint8 m_TargetKeyID;
};
