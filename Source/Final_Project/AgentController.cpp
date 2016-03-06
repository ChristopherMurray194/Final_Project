// Fill out your copyright notice in the Description page of Project Settings.

#include "Final_Project.h"
#include "AgentController.h"
#include "Agent.h"
#include "PathNode.h"
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

	// Start behaviour tree
	if (agent && agent->m_AgentBehaviour)
	{
		m_BlackboardComp->InitializeBlackboard(*(agent->m_AgentBehaviour->BlackboardAsset));

		// String name MUST corresond EXACTLY to name of key in Blackboard editor
		m_TargetKeyID = m_BlackboardComp->GetKeyID("Target");
		// Sets the target point value in the blackboard
		setTarget(m_TargetKeyID, agent->targetPoint);

		m_BehvaiourTreeComp->StartTree(*(agent->m_AgentBehaviour));
	}
}

void AAgentController::setTarget(uint8 targetKeyID, class APathNode* target)
{
	if (m_BlackboardComp)
	{
		m_BlackboardComp->SetValueAsObject(targetKeyID, target);
	}
}

class APathNode* AAgentController::getTarget() const
{
	if (m_BlackboardComp)
	{
		return Cast<APathNode>(m_BlackboardComp->GetValueAsObject(m_TargetKeyID));
	}
	return NULL;
}