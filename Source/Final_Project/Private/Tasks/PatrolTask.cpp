// Fill out your copyright notice in the Description page of Project Settings.
// Essentially as Epic Games' BTTask_MoveTo.cpp, used to learn how to create InProgress BT Tasks in UE4

#include "Final_Project.h"
#include "PatrolTask.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h" // Necessary for GET_AI_CONFIG_VAR
#include "BehaviorTree/Blackboard/BlackboardKeyType_Object.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Vector.h"
#include "Tasks/AITask_MoveTo.h"
#include "AIModuleClasses.h" // Needed for FBTMoveToTaskMemory struct
#include "AIController.h"

UPatrolTask::UPatrolTask()
{
	NodeName = "Patrol Task";
	bNotifyTick = true;

	AcceptableRadius = GET_AI_CONFIG_VAR(AcceptanceRadius);
	bStopOnOverlap = GET_AI_CONFIG_VAR(bFinishMoveOnGoalOverlap);
	bAllowStrafe = GET_AI_CONFIG_VAR(bAllowStrafing);
	bAllowPartialPath = GET_AI_CONFIG_VAR(bAcceptPartialPaths);

	// Accept only actors and vectors
	BlackboardKey.AddObjectFilter(this, GET_MEMBER_NAME_CHECKED(UPatrolTask, BlackboardKey), AActor::StaticClass());
	BlackboardKey.AddVectorFilter(this, GET_MEMBER_NAME_CHECKED(UPatrolTask, BlackboardKey));
}

EBTNodeResult::Type UPatrolTask::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	// The task is ongoing until MoveTo completes
	EBTNodeResult::Type NodeResult = EBTNodeResult::InProgress;

	FBTMoveToTaskMemory* Memory = (FBTMoveToTaskMemory*)NodeMemory;
	AAIController* Controller = OwnerComp.GetAIOwner();

	Memory->bWaitingForPath = Controller->ShouldPostponePathUpdates();
	if (!Memory->bWaitingForPath)
	{
		NodeResult = PerformMoveTask(OwnerComp, NodeMemory);
	}

	return NodeResult;
}

EBTNodeResult::Type UPatrolTask::PerformMoveTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	const UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
	FBTMoveToTaskMemory* Memory = (FBTMoveToTaskMemory*)NodeMemory;
	AAIController* Controller = OwnerComp.GetAIOwner();

	EBTNodeResult::Type NodeResult = EBTNodeResult::Failed;

	if (Controller && BlackboardComp)
	{
		if (GET_AI_CONFIG_VAR(bEnableBTAITasks))
		{
			UAITask_MoveTo* AIMoveTask = NewBTAITask<UAITask_MoveTo>(OwnerComp);

			if (AIMoveTask != nullptr)
			{
				bool bSetUp = false;
				if (BlackboardKey.SelectedKeyType == UBlackboardKeyType_Object::StaticClass())
				{
					UObject* KeyValue = BlackboardComp->GetValue<UBlackboardKeyType_Object>(BlackboardKey.GetSelectedKeyID());
					AActor* TargetActor = Cast<AActor>(KeyValue);
					if (TargetActor)
					{
						AIMoveTask->SetUp(Controller, FVector::ZeroVector, TargetActor, AcceptableRadius, true, FAISystem::BoolToAIOption(bStopOnOverlap), FAISystem::BoolToAIOption(bAllowPartialPath));
						NodeResult = EBTNodeResult::InProgress;
					}
				}
				else if (BlackboardKey.SelectedKeyType == UBlackboardKeyType_Vector::StaticClass())
				{
					const FVector TargetLocation = BlackboardComp->GetValue<UBlackboardKeyType_Vector>(BlackboardKey.GetSelectedKeyID());
					AIMoveTask->SetUp(Controller, TargetLocation, nullptr, AcceptableRadius, true, FAISystem::BoolToAIOption(bStopOnOverlap), FAISystem::BoolToAIOption(bAllowPartialPath));
					NodeResult = EBTNodeResult::InProgress;
				}

				if (NodeResult == EBTNodeResult::InProgress)
				{
					/* Causes crash - Cannot figure out why,
					regardless task works without it. */
					//AIMoveTask->ReadyForActivation();
				}
			}
		}
		else
		{
			EPathFollowingRequestResult::Type RequestResult = EPathFollowingRequestResult::Failed;

			FAIMoveRequest MoveReq;
			MoveReq.SetNavigationFilter(FilterClass);
			MoveReq.SetAllowPartialPath(bAllowPartialPath);
			MoveReq.SetAcceptanceRadius(AcceptableRadius);
			MoveReq.SetCanStrafe(bAllowStrafe);
			MoveReq.SetStopOnOverlap(bStopOnOverlap);

			if (BlackboardKey.SelectedKeyType == UBlackboardKeyType_Object::StaticClass())
			{
				UObject* KeyValue = BlackboardComp->GetValue<UBlackboardKeyType_Object>(BlackboardKey.GetSelectedKeyID());
				AActor* TargetActor = Cast<AActor>(KeyValue);
				if (TargetActor)
				{
					MoveReq.SetGoalActor(TargetActor);

					RequestResult = Controller->MoveTo(MoveReq);
				}
			}
			else if (BlackboardKey.SelectedKeyType == UBlackboardKeyType_Vector::StaticClass())
			{
				const FVector TargetLocation = BlackboardComp->GetValue<UBlackboardKeyType_Vector>(BlackboardKey.GetSelectedKeyID());
				MoveReq.SetGoalLocation(TargetLocation);

				RequestResult = Controller->MoveTo(MoveReq);
			}

			// If the Request was successfully received
			if (RequestResult == EPathFollowingRequestResult::RequestSuccessful)
			{
				const FAIRequestID RequestID = Controller->GetCurrentMoveRequestID();

				Memory->MoveRequestID = RequestID;
				WaitForMessage(OwnerComp, UBrainComponent::AIMessage_MoveFinished, RequestID);
				WaitForMessage(OwnerComp, UBrainComponent::AIMessage_RepathFailed);

				NodeResult = EBTNodeResult::InProgress;
			}
			// If the agenti is at the MoveTo target
			else if (RequestResult == EPathFollowingRequestResult::AlreadyAtGoal)
			{
				NodeResult = EBTNodeResult::Succeeded;
			}
		}
	}

	return NodeResult;
}

EBTNodeResult::Type UPatrolTask::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	FBTMoveToTaskMemory* Memory = (FBTMoveToTaskMemory*)NodeMemory;
	if (!Memory->bWaitingForPath)
	{
		AAIController* Controller = OwnerComp.GetAIOwner();

		if (Controller && Controller->GetPathFollowingComponent())
		{
			Controller->GetPathFollowingComponent()->AbortMove(TEXT("BehaviorTree abort"), Memory->MoveRequestID);
		}
	}

	return Super::AbortTask(OwnerComp, NodeMemory);
}

void UPatrolTask::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	FBTMoveToTaskMemory* Memory = (FBTMoveToTaskMemory*)NodeMemory;
	if (Memory->bWaitingForPath && !OwnerComp.IsPaused())
	{
		AAIController* Controller = OwnerComp.GetAIOwner();
		if (Controller && !Controller->ShouldPostponePathUpdates())
		{
			Memory->bWaitingForPath = false;

			const EBTNodeResult::Type NodeResult = PerformMoveTask(OwnerComp, NodeMemory);
			if (NodeResult != EBTNodeResult::InProgress)
			{
				FinishLatentTask(OwnerComp, NodeResult);
			}
		}
	}
}

void UPatrolTask::OnMessage(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, FName Message, int32 SenderID, bool bSuccess)
{
	bSuccess &= (Message != UBrainComponent::AIMessage_RepathFailed);
	Super::OnMessage(OwnerComp, NodeMemory, Message, SenderID, bSuccess);
}

FString UPatrolTask::GetStaticDescription() const
{
	FString KeyDesc("invalid");
	if (BlackboardKey.SelectedKeyType == UBlackboardKeyType_Object::StaticClass() ||
		BlackboardKey.SelectedKeyType == UBlackboardKeyType_Vector::StaticClass())
	{
		KeyDesc = BlackboardKey.SelectedKeyName.ToString();
	}

	return FString::Printf(TEXT("%s: %s"), *Super::GetStaticDescription(), *KeyDesc);
}

void UPatrolTask::DescribeRuntimeValues(const UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTDescriptionVerbosity::Type Verbosity, TArray<FString>& Values) const
{
	Super::DescribeRuntimeValues(OwnerComp, NodeMemory, Verbosity, Values);
	
	const UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
	FBTMoveToTaskMemory* Memory = (FBTMoveToTaskMemory*)NodeMemory;

	if (Memory->MoveRequestID && BlackboardComp)
	{
		FString KeyValue = BlackboardComp->DescribeKeyValue(BlackboardKey.GetSelectedKeyID(), EBlackboardDescription::OnlyValue);
		Values.Add(FString::Printf(TEXT("move target: %s"), *KeyValue));
	}
}

uint16 UPatrolTask::GetInstanceMemorySize() const
{
	return sizeof(FBTMoveToTaskMemory);
}

#if WITH_EDITOR

FName UPatrolTask::GetNodeIconName() const
{
	return FName("BTEditor.Graph.BTNode.Task.MoveTo.Icon");
}

#endif // WITH_EDITOR