// Fill out your copyright notice in the Description page of Project Settings.

#include "Final_Project.h"
#include "AgentController.h"
#include "Agent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"

AAgentController::AAgentController()
{
	m_Blackboard = CreateDefaultSubobject<UBlackboardComponent>(TEXT("BlackboardComponent"));
	m_BehvaiourTree = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("BehaviourTreeComponent"));
	
}

void AAgentController::Possess(class APawn* InPawn)
{
	Super::Possess(InPawn);

	AAgent* agent = Cast<AAgent>(InPawn);
}

void AAgentController::Patrol()
{

}

