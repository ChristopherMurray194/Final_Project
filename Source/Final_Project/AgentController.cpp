// Fill out your copyright notice in the Description page of Project Settings.

#include "Final_Project.h"
#include "AgentController.h"
#include "Agent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"

AAgentController::AAgentController()
{
	m_BlackboardComp = CreateDefaultSubobject<UBlackboardComponent>(TEXT("BlackboardComponent"));	
	m_BehvaiourTreeComp = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("BehaviourTreeComponent"));

}

void AAgentController::Possess(class APawn* InPawn)
{
	Super::Possess(InPawn);
	AAgent* agent = Cast<AAgent>(InPawn);

	// Start behaviour
	if (agent && agent->m_AgentBehaviour)
	{
		m_BlackboardComp->InitializeBlackboard(*(agent->m_AgentBehaviour->BlackboardAsset));

		// String name MUST corresond EXACTLY to name of key in Blackboard editor
		m_TargetKeyID = m_BlackboardComp->GetKeyID("Target");
		// Sets the target point value in the blackboard
		setTarget(agent->targetPoint);

		m_BehvaiourTreeComp->StartTree(*(agent->m_AgentBehaviour));
	}
}

void AAgentController::setTarget(class AActor* target)
{
	if (m_BlackboardComp)
	{
		m_BlackboardComp->SetValueAsObject(m_TargetKeyID, target);
	}
}

class AActor* AAgentController::getTarget() const
{
	if (m_BlackboardComp)
	{
		return Cast<AActor>(m_BlackboardComp->GetValueAsObject(m_TargetKeyID));
	}
	return NULL;
}
