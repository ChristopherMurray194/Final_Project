// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Character.h"
#include "Agent.generated.h"

UCLASS()
class FINAL_PROJECT_API AAgent : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AAgent();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;

	/* Behaviour Tree to be used */
	UPROPERTY(EditAnywhere, Category = BehaviourTree)
		class UBehaviorTree* AgentBehaviourTree;

	// If not set to value Editor will crash
	UPROPERTY(EditAnywhere, Category = Path)
		class APathNode* PathNode;
};
