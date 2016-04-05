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

	virtual void PostInitializeComponents();

	// The behaviour tree to be used
	UPROPERTY(EditAnywhere, Category = "Behaviour Tree")
		class UBehaviorTree* AgentBehaviourTree;

	// If not set to value Editor may crash
	UPROPERTY(EditAnywhere, Category = Path)
		class APathNode* PathNode;

	// The component that defines the sight of the agent
	UPROPERTY(EditAnywhere)
		class UPawnSensingComponent* SensingComp;

	// The OnSeePawn delegate function
	UFUNCTION()
		void OnSeePawn(APawn* OtherPawn);

private:
	// Colour of the enemy
	FLinearColor EnemyColor = FLinearColor( (139.0f / 255.0f), (26.0f / 255.0f), (26.0f / 255.0f), 1.0f);
};
