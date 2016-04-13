// Fill out your copyright notice in the Description page of Project Settings.

#include "Final_Project.h"
#include "Agent.h"
#include "AgentController.h"
#include "Final_ProjectCharacter.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Rifle.h"
#include "Delegate.h"
#include "Engine.h"

// Sets default values
AAgent::AAgent()
{
	// Set the AIController class to use the AgentController class on instantiation
	AIControllerClass = AAgentController::StaticClass();
	Tags.Add(TEXT("Enemy"));

	SensingComp = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("Agent_Perception"));
	SightComp = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("Sight_Configuration"));
	SensingComp->SetDominantSense(SightComp->GetSenseImplementation());
	SensingComp->OnPerceptionUpdated.AddDynamic(this, &AAgent::SensePawn);

	float SightRadiusWeight = 200.0f;
	SightComp->SightRadius = SightRadiusWeight;
	// How far until a sensed pawn is out of range (can no longer be seen).
	SightComp->LoseSightRadius = SightRadiusWeight + 15;
	// FOV angle
	SightComp->PeripheralVisionAngleDegrees = 64.0f;
	SightComp->DetectionByAffiliation.bDetectEnemies = true;
	SightComp->DetectionByAffiliation.bDetectNeutrals = true;
	SightComp->DetectionByAffiliation.bDetectFriendlies = true;
	SensingComp->ConfigureSense(*SightComp);
}

void AAgent::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	
	// Set the Agent colour
	GetMesh()->CreateAndSetMaterialInstanceDynamic(0)->SetVectorParameterValue(TEXT("BodyColor"), EnemyColor);
	
}

void AAgent::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	// If the player has been spotted AND the TimeTillPawnLost has elapsed since then
	UWorld* World = GetWorld();
	if (World)
	{
		if (bPlayerSeen && !bCanSearch && (World->GetTimeSeconds() - LastSeenTime) > TimeTillPawnLost)
		{
			AAgentController* Controller = Cast<AAgentController>(GetController());
			if (Controller != NULL)
			{
				// Reset the player has seen blackboard key so that the Agent can begin searching.
				bPlayerSeen = false;
				Controller->SetPlayerFound(bPlayerSeen);
				// Clear the focus on the player
				Controller->ClearFocus(EAIFocusPriority::Gameplay);
				bCanSearch = true;
				Controller->SetCanSearch(bCanSearch);
				BeginSearch();
			}
		}

		if (!bPlayerSeen && bCanSearch && (World->GetTimeSeconds() - SearchStartTime) > SearchTime)
		{
			AAgentController* Controller = Cast<AAgentController>(GetController());
			if (Controller != NULL)
			{
				// Stop searching
				bCanSearch = false;
				Controller->SetCanSearch(bCanSearch);
			}
		}
	}
}

void AAgent::SensePawn(TArray<AActor*> OtherPawn)
{
	UWorld* World = GetWorld();
	if (World)
	{
		LastSeenTime = World->GetTimeSeconds();
	}
	// For each sensed Actor
	for (AActor* SensedActor : OtherPawn)
	{
		// Make sure it is the Player we are sensing and not other agents
		if ( (SensedActor != NULL) && (SensedActor->ActorHasTag("Player")) )
		{
			// Store the Player's location
			PlayerLocation = SensedActor->GetActorLocation();
			
			// Pass the location to the controller
			AAgentController* Controller = Cast<AAgentController>(GetController());
			if (Controller != NULL)
			{
				// Focus on the player's location
				Controller->SetFocus(SensedActor, EAIFocusPriority::Gameplay);
				// Set the player location as this focal point
				Controller->SetPlayerLocation(Controller->GetFocalPointOnActor(SensedActor));
				// Agent can see the Player
				bPlayerSeen = true;
				Controller->SetPlayerFound(bPlayerSeen);
			}
		}
	}
}

void AAgent::BeginSearch()
{
	if (bCanSearch)
	{
		UWorld* World = GetWorld();
		if (World)
		{
			// Get the time the search is started
			SearchStartTime = World->GetTimeSeconds();
		}
		AAgentController* Controller = Cast<AAgentController>(GetController());
		// Get a random location
		FVector RandVec = FMath::VRand();
		// Set the RandomLocation BlackboardKey
		Controller->SetRandomLocation(RandVec);
	}
}

class APathNode* AAgent::GetPathNode() const { return PathNode; }
FVector AAgent::GetPlayerLocation() const { return PlayerLocation; }
bool AAgent::GetPlayerSeen() const { return bPlayerSeen; }
bool AAgent::GetCanSearch() const { return bCanSearch; }