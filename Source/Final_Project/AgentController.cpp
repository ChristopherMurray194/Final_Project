// Fill out your copyright notice in the Description page of Project Settings.

#include "Final_Project.h"
#include "AgentController.h"
#include "Agent.h"
#include "PathNode.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Engine.h"

AAgentController::AAgentController()
{
	m_BlackboardComp = CreateDefaultSubobject<UBlackboardComponent>(TEXT("BlackboardComponent"));	
	m_BehvaiourTreeComp = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("BehaviourTreeComponent"));

}

void AAgentController::Possess(class APawn* InPawn)
{
	Super::Possess(InPawn);
	AAgent* agent = Cast<AAgent>(InPawn);

	// Check there is an Agent, AgentBT and PathNode for the agent.
	if (agent && agent->AgentBehaviourTree && agent->PathNode)
	{
		// Get the blackboard asset linked to the Agent's behaviour tree
		m_BlackboardComp->InitializeBlackboard(*(agent->AgentBehaviourTree->BlackboardAsset));

		// String name MUST corresond EXACTLY to name of key in Blackboard editor
		m_TargetKeyID = FName("Target");	// Convert string to FName
		// Sets the target point value in the blackboard
		setTarget(m_TargetKeyID, agent->PathNode);

		m_BehvaiourTreeComp->StartTree(*(agent->AgentBehaviourTree));
	}/* // Must be commented out when Launching. GetActorLabel function only avaialable in development builds
	else // Handle errors
	{
		if (GEngine && !agent->AgentBehaviourTree)
		{
			GEngine->AddOnScreenDebugMessage(-1,			// Key: Passing -1 means create new message. Not passing -1, means update message each tick - UE4 API
											60.f,			// Display time (seconds)
											FColor::Red,	// Color
											FString::Printf(TEXT("There is no Behaviour Tree assigned to %s!"), *agent->GetActorLabel())	// Message to display
											);
		}
		if (GEngine && !agent->PathNode)
		{
			GEngine->AddOnScreenDebugMessage(-1,
											60.f, 
											FColor::Red, 
											FString::Printf(TEXT("There is no PathNode assigned to %s!"), *agent->GetActorLabel())
											);
		}
	}*/
}

void AAgentController::setTarget(const FName& targetKeyID, class APathNode* target)
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