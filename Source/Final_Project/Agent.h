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
	
	virtual void PostInitializeComponents() override;
	virtual void Tick(float DeltaSeconds) override;

	// The behaviour tree to be used
	UPROPERTY(EditAnywhere, Category = "Behaviour Tree")
		class UBehaviorTree* AgentBehaviourTree;

	// If not set to value Editor may crash
	UPROPERTY(EditAnywhere, Category = Path)
	class APathNode* PathNode;
	/* PathNode accessor */

	class APathNode* GetPathNode() const;

	// The component that defines the sight of the agent
	UPROPERTY(EditAnywhere)
		class UAIPerceptionComponent* SensingComp;
	
	// Sight sensing configuration
	class UAISenseConfig_Sight* SightComp;

	// Can the agent see the player
	UPROPERTY(BlueprintReadOnly)
		bool bPlayerSeen = false;
	bool GetPlayerSeen() const;

	// Delegate function
	UFUNCTION()
		void SensePawn(TArray<AActor*> OtherPawn);

	// Can the Agent begin searching?
	UPROPERTY(BlueprintReadOnly)
		bool bCanSearch = false;
	bool GetCanSearch() const;

private:
	// Colour of the enemy
	FLinearColor EnemyColor = FLinearColor( (139.0f / 255.0f), (26.0f / 255.0f), (26.0f / 255.0f), 1.0f);

	float LastSeenTime = 0.0f;
	/* Agent has lost sight of player if the player's location
	is not updated after the specified time*/
	float TimeTillPawnLost = 5.0f;

	// Time the agent can search for
	float SearchTime = 10.0f;
	float SearchStartTime = 0.0f;
};
