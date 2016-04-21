// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "PatrolTask.generated.h"

/**
*
*/
UCLASS(Config = Game)
class FINAL_PROJECT_API UPatrolTask : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

	UPatrolTask();

public:
	// The distance from the target the agent may stop
	UPROPERTY(Config, Category = Node, EditAnywhere, meta = (ClampMin = "0.0"))
		float AcceptableRadius;

	UPROPERTY(Category = Node, EditAnywhere)
		TSubclassOf<UNavigationQueryFilter> FilterClass;

	UPROPERTY(Category = Node, EditAnywhere)
		uint32 bAllowStrafe : 1;

	UPROPERTY(Category = Node, EditAnywhere, AdvancedDisplay)
		uint32 bAllowPartialPath : 1;

	UPROPERTY(Category = Node, EditAnywhere)
		uint32 bStopOnOverlap : 1;

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual EBTNodeResult::Type AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	virtual uint16 GetInstanceMemorySize() const override;

	virtual void OnMessage(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, FName Message, int32 RequestID, bool bSuccess) override;
	
	virtual void DescribeRuntimeValues(const UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTDescriptionVerbosity::Type Verbosity, TArray<FString>& Values) const override;
	virtual FString GetStaticDescription() const override;

#if WITH_EDITOR
	virtual FName GetNodeIconName() const override;
#endif // WITH_EDITOR

protected:
	EBTNodeResult::Type PerformMoveTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory);
};
