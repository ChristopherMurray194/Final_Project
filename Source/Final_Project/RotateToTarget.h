// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "RotateToTarget.generated.h"

/**
*
*/
UCLASS(Config = Game)
class FINAL_PROJECT_API URotateToTarget : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

public:
	URotateToTarget();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	//virtual EBTNodeResult::Type AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

};
