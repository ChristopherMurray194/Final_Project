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
	if (agent && agent->AgentBehaviourTree && agent->GetPathNode())
	{
		// Get the blackboard asset linked to the Agent's behaviour tree
		m_BlackboardComp->InitializeBlackboard(*(agent->AgentBehaviourTree->BlackboardAsset));

		// String name MUST corresond EXACTLY to name of key in Blackboard editor
		m_TargetKeyID = FName("Target");	// Convert string to FName
		// Sets the target point value in the blackboard
		SetTarget(agent->GetPathNode());

		m_PlayerPosKeyID = FName("PlayerLocation");
		// Sets the location of the PlayerLocation value in the blackboard
		SetPlayerLocation(agent->GetPlayerLocation());

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

void AAgentController::SetTarget(class APathNode* target)
{
	if (m_BlackboardComp)
	{
		m_BlackboardComp->SetValueAsObject(m_TargetKeyID, target);
	}
}

class APathNode* AAgentController::GetTarget() const
{
	if (m_BlackboardComp)
	{
		return Cast<APathNode>(m_BlackboardComp->GetValueAsObject(m_TargetKeyID));
	}
	return NULL;
}

void AAgentController::SetPlayerLocation(FVector PlayerLocation)
{
	if (m_BlackboardComp)
	{
		m_BlackboardComp->SetValueAsVector(m_PlayerPosKeyID, PlayerLocation);
	}
}

FVector AAgentController::GetPlayerLocation()
{
	if (m_BlackboardComp)
	{
		return m_BlackboardComp->GetValueAsVector(m_PlayerPosKeyID);
	}
	return FVector(0.0f, 0.0f, 0.0f);
}