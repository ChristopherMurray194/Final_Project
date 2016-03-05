// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "BehaviorTree/BTTaskNode.h"
#include "PatrolTask.generated.h"

/**
 * 
 */
UCLASS()
class FINAL_PROJECT_API UPatrolTask : public UBTTaskNode
{
	GENERATED_BODY()

	UPatrolTask();
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory);
};
