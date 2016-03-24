// Fill out your copyright notice in the Description page of Project Settings.

#include "Final_Project.h"
#include "PatrolTask.h"
#include "AgentController.h"
#include "PathNode.h"

UPatrolTask::UPatrolTask()
{
	bNotifyTick = true;
	bNotifyTaskFinished = true;
}

EBTNodeResult::Type UPatrolTask::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	UBehaviorTreeComponent* BTComp = &OwnerComp;
	AAgentController* agentController = BTComp ? Cast<AAgentController>(BTComp->GetOwner()) : NULL;
	
	// If there is no AI controller
	if (agentController == NULL)
	{
		// Task execution failed
		return EBTNodeResult::Failed;
	}
	else
	{
		// Get the PathNode location
		APathNode* target = agentController->getTarget();
		// Call the MoveTo function inherited from AIController class
		agentController->MoveTo(target);

		// Task executed successfully
		return EBTNodeResult::Succeeded;
	}
	
	return EBTNodeResult::Failed;
}

