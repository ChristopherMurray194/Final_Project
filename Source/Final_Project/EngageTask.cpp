// Fill out your copyright notice in the Description page of Project Settings.

#include "Final_Project.h"
#include "EngageTask.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h" // Necessary for GET_AI_CONFIG_VAR
#include "BehaviorTree/Blackboard/BlackboardKeyType_Vector.h"
#include "AgentController.h"
#include "Rifle.h"
#include "Agent.h"
#include "Engine.h"

UEngageTask::UEngageTask()
{
	bNotifyTick = true;
	BlackboardKey.AddVectorFilter(this, GET_MEMBER_NAME_CHECKED(UEngageTask, BlackboardKey));
}

EBTNodeResult::Type UEngageTask::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
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
		Agent = Controller->GetAgentOwner();
		// If the agent's weapon has ammo in it
		if (Agent->GetCurrentWeapon()->CalculateAmmo() > 0)
		{
			// Ensure reload is set to false because it should have been reloaded at this stage
			Agent->EndReload();
			// Shoot
			Agent->Fire();
		}
		else
		{
			// Stop firing if firing
			Agent->StopFiring();
			// Reload
			Agent->BeginReload();
		}

		return EBTNodeResult::Succeeded;
	}
	return EBTNodeResult::Failed;
}
