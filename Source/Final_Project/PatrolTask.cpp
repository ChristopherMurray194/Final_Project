// Fill out your copyright notice in the Description page of Project Settings.

#include "Final_Project.h"
#include "PatrolTask.h"
#include "AgentController.h"
#include "Engine.h"

UPatrolTask::UPatrolTask()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, FString(TEXT("IN CONSTRUCTOR")));
	bNotifyTick = true;
	bNotifyTaskFinished = true;
}

EBTNodeResult::Type UPatrolTask::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, FString(TEXT("ENTERED METHOD")));
	UBehaviorTreeComponent* BTComp = &OwnerComp;
	AAgentController* agentController = BTComp ? Cast<AAgentController>(BTComp->GetOwner()) : NULL;
	
	// If there is no AI controller
	if (agentController == NULL)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString(TEXT("NO CONTROLLER")));
		// Task execution failed
		return EBTNodeResult::Failed;
	}
	else
	{
		// Get the target location
		AActor* target = agentController->getTarget();

		// Move to target location
		agentController->MoveToActor(target, 100.0f, true, true, true);
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, FString(TEXT("WORKING")));

		// Task executed successfully
		return EBTNodeResult::Succeeded;
	}
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, FString(TEXT("OUTRIGHT FAIL")));
	
	return EBTNodeResult::Failed;
}

