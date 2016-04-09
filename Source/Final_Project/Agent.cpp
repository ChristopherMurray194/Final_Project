// Fill out your copyright notice in the Description page of Project Settings.

#include "Final_Project.h"
#include "Agent.h"
#include "AgentController.h"
#include "Final_ProjectCharacter.h"
#include "Perception/PawnSensingComponent.h"
#include "Rifle.h"
#include "Delegate.h"
#include "Engine.h"

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

void AAgent::OnSeePawn(APawn* OtherPawn)
{
	
	// Ensure the other actor is the player
	if (OtherPawn->ActorHasTag(TEXT("Player")))
	{
		UWorld* World = GetWorld();
		if (World)
		{
			LastSeenTime = World->GetTimeSeconds();
		}
		// Store the Player's location
		PlayerLocation = OtherPawn->GetActorLocation();
		
		// Pass the location to the controller
		AAgentController* Controller = Cast<AAgentController>(GetController());
		Controller->SetPlayerLocation(PlayerLocation);
		// Agent can see the Player
		bPlayerSeen = true;
		Controller->SetPlayerFound(bPlayerSeen);
	}
	else
	{
		bPlayerSeen = false;
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