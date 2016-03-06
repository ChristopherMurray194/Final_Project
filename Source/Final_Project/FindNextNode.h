// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "BehaviorTree/BTTaskNode.h"
#include "FindNextNode.generated.h"

/**
 * 
 */
UCLASS()
class FINAL_PROJECT_API UFindNextNode : public UBTTaskNode
{
	GENERATED_BODY()

	UFindNextNode();
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory);

protected:
	uint8 m_TargetKeyID;	// Current path node KeyID in blackboard
};
