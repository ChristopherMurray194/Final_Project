// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "BaseCharacter.h"
#include "Agent.generated.h"

UCLASS()
class FINAL_PROJECT_API AAgent : public ABaseCharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AAgent();

	// The behaviour tree to be used
	UPROPERTY(EditAnywhere, Category = "Behaviour Tree")
		class UBehaviorTree* AgentBehaviourTree;

	// If not set to value Editor may crash
	UPROPERTY(EditAnywhere, Category = Path)
		class APathNode* PathNode;
};
