// Fill out your copyright notice in the Description page of Project Settings.

#include "Final_Project.h"
#include "RotateToTarget.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h" // Necessary for GET_AI_CONFIG_VAR
#include "BehaviorTree/Blackboard/BlackboardKeyType_Vector.h"
#include "AgentController.h"
#include "Agent.h"
#include "Engine.h"

URotateToTarget::URotateToTarget()
{
	bNotifyTick = true;
	BlackboardKey.AddVectorFilter(this, GET_MEMBER_NAME_CHECKED(URotateToTarget, BlackboardKey));
}

EBTNodeResult::Type URotateToTarget::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	UBehaviorTreeComponent* BTComp = &OwnerComp;
	const UBlackboardComponent* BlackboardComp = BTComp->GetBlackboardComponent();
	AAgentController* Controller = Cast<AAgentController>(BTComp->GetAIOwner());

	if (Controller == NULL)
	{
		return EBTNodeResult::Failed;
	}
	else
	{
		// Get the PlayerLocation value currently held in the blackboard
		FVector KeyValue = BlackboardComp->GetValue<UBlackboardKeyType_Vector>(BlackboardKey.GetSelectedKeyID());
		// Store it locally
		FVector PlayerLocation = KeyValue;
		// Get the Agent's location
		FVector AgentLocation = Controller->GetAgentLocation();
		// Create new vector from Agent's location to Player's location
		FVector AgentToPlayer = PlayerLocation - AgentLocation;
		// Get the Agent's rotation
		FRotator AgentRotation = Controller->GetAgentRotation();

		UWorld* World = GetWorld();
		if (World)
		{
			FRotator NewRotation = FMath::RInterpTo(AgentRotation, AgentToPlayer.Rotation(), World->GetDeltaSeconds(), 10.0f);
			// Set the new controller rotation which drives the Agent.
			Controller->SetControlRotation(NewRotation);
		}

		return EBTNodeResult::Succeeded;
	}
	return EBTNodeResult::Failed;
}
