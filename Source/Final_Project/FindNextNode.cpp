// Fill out your copyright notice in the Description page of Project Settings.

#include "Final_Project.h"
#include "FindNextNode.h"
#include "AgentController.h"
#include "PathNode.h"

UFindNextNode::UFindNextNode()
{
	bNotifyTick = true;
	bNotifyTaskFinished = true;
}

EBTNodeResult::Type UFindNextNode::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	UBehaviorTreeComponent* BTComp = &OwnerComp;
	AAgentController* agentController = BTComp ? Cast<AAgentController>(BTComp->GetOwner()) : NULL;

	if (agentController == NULL)
	{
		// Task execution failed
		return EBTNodeResult::Failed;
	}
	else
	{
		// Get the blackboard component key ID, Target node
		m_TargetKeyID = FName("Target");
		// Get the current node in path (in the blackboard)
		APathNode* CurrentNode = Cast<APathNode>(agentController->getTarget());
		// Set the Current node in path as next node in path (in the blackboard)
		agentController->setTarget(m_TargetKeyID, CurrentNode->GetNextNode());
		// Task succeeded
		return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::Failed;
}

