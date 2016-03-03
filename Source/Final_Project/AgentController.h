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

	// transient property - should not be saved
	UPROPERTY(transient)
		UBlackboardComponent* m_Blackboard;
	
	UPROPERTY(transient)
		UBehaviorTreeComponent* m_BehvaiourTree;
	
	virtual void Possess(class APawn* InPawn);

	// Patrol the patrol path
	UFUNCTION(BlueprintCallable, Category = Behaviour)
		void Patrol();

public:
	AAgentController();

protected:
	uint8 m_TargetKeyID;
	uint8 m_TargetLocationID;
};
