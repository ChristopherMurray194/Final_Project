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
	BlackboardComp = CreateDefaultSubobject<UBlackboardComponent>(TEXT("BlackboardComponent"));	
	BehvaiourTreeComp = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("BehaviourTreeComponent"));
	
}

void AAgentController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	// Get the Agent's location
	AgentLocation = Agent->GetActorLocation();
	// Get the Agent's rotation
	AgentRotation = Agent->GetActorRotation();
}

void AAgentController::Possess(class APawn* InPawn)
{
	Super::Possess(InPawn);
	Agent = Cast<AAgent>(InPawn);

	// Check there is an Agent, AgentBT and PathNode for the agent.
	if (Agent && Agent->AgentBehaviourTree && Agent->GetPathNode())
	{
		// Get the blackboard asset linked to the Agent's behaviour tree
		BlackboardComp->InitializeBlackboard(*(Agent->AgentBehaviourTree->BlackboardAsset));

		if (BlackboardComp)
		{
			// String name MUST corresond EXACTLY to name of key in Blackboard editor
			TargetKeyID = BlackboardComp->GetKeyID("Target");	// Convert string to FName
			// Sets the target point value in the blackboard
			SetTarget(Agent->GetPathNode());

			PlayerPosKeyID = BlackboardComp->GetKeyID("PlayerLocation");
			// Sets the location of the PlayerLocation value in the blackboard
			SetPlayerLocation(Agent->GetPlayerLocation());
		}

		BehvaiourTreeComp->StartTree(*(Agent->AgentBehaviourTree));
	}/* // Must be commented out when Launching. GetActorLabel function only avaialable in development builds
	else // Handle errors
	{
		if (GEngine && !Agent->AgentBehaviourTree)
		{
			GEngine->AddOnScreenDebugMessage(-1,			// Key: Passing -1 means create new message. Not passing -1, means update message each tick - UE4 API
											60.f,			// Display time (seconds)
											FColor::Red,	// Color
											FString::Printf(TEXT("There is no Behaviour Tree assigned to %s!"), *Agent->GetActorLabel())	// Message to display
											);
		}
		if (GEngine && !Agent->PathNode)
		{
			GEngine->AddOnScreenDebugMessage(-1,
											60.f, 
											FColor::Red, 
											FString::Printf(TEXT("There is no PathNode assigned to %s!"), *Agent->GetActorLabel())
											);
		}
	}*/
}

void AAgentController::SetTarget(class APathNode* target)
{
	if (BlackboardComp)
	{
		BlackboardComp->SetValueAsObject(TargetKeyID, target);
	}
}

class APathNode* AAgentController::GetTarget() const
{
	if (BlackboardComp)
	{
		return Cast<APathNode>(BlackboardComp->GetValueAsObject(TargetKeyID));
	}
	return NULL;
}

void AAgentController::SetPlayerLocation(FVector PlayerLocation)
{
	if (BlackboardComp)
	{
		BlackboardComp->SetValueAsVector(PlayerPosKeyID, PlayerLocation);
	}
}

FVector AAgentController::GetPlayerLocation()
{
	if (BlackboardComp)
	{
		return BlackboardComp->GetValueAsVector(PlayerPosKeyID);
	}
	return FVector(0.0f, 0.0f, 0.0f);
}

void AAgentController::SetActorRotation(FRotator NewRotation){ Agent->SetActorRotation(NewRotation); }
FVector AAgentController::GetAgentLocation(){ return AgentLocation; }
FRotator AAgentController::GetAgentRotation(){ return AgentRotation; }