// Fill out your copyright notice in the Description page of Project Settings.

#include "Final_Project.h"
#include "Agent.h"
#include "AgentController.h"
#include "Perception/PawnSensingComponent.h"
#include "Rifle.h"
#include "Delegate.h"

// Sets default values
AAgent::AAgent()
{
	// Set the AIController class to use the AgentController class on instantiation
	AIControllerClass = AAgentController::StaticClass();
	Tags.Add(TEXT("Enemy"));

	SensingComp = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("Agent_Sight"));
	SensingComp->SightRadius = 1055.0f;
	SensingComp->SetPeripheralVisionAngle(64.0f);
}

void AAgent::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	
	// Set the Agent colour
	GetMesh()->CreateAndSetMaterialInstanceDynamic(0)->SetVectorParameterValue(TEXT("BodyColor"), EnemyColor);

	if (SensingComp != NULL)
		SensingComp->OnSeePawn.AddDynamic(this, &AAgent::OnSeePawn);
}

void AAgent::OnSeePawn(APawn* OtherPawn)
{
	
}
