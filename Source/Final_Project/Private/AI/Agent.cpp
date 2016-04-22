// Fill out your copyright notice in the Description page of Project Settings.

#include "Final_Project.h"
#include "Agent.h"
#include "AgentController.h"
#include "Final_ProjectCharacter.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "BaseWeapon.h"
#include "Rifle.h"
#include "Pistol.h"
#include "Shotgun.h"
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
	float SightRadiusWeight = 512.0f;
	SightComp->SightRadius = SightRadiusWeight;
	// How far until a sensed pawn is out of range (can no longer be seen).
	SightComp->LoseSightRadius = 3500.0;
	// FOV angle
	SightComp->PeripheralVisionAngleDegrees = 130.0f;
	SightComp->DetectionByAffiliation.bDetectEnemies = true;
	SightComp->DetectionByAffiliation.bDetectNeutrals = true;
	SightComp->DetectionByAffiliation.bDetectFriendlies = true;
	SensingComp->ConfigureSense(*SightComp);
	SensingComp->SetDominantSense(SightComp->GetSenseImplementation());
	SensingComp->OnPerceptionUpdated.AddDynamic(this, &AAgent::SensePawn);

	// Add the weapons to the Array
	Weapons.Add(SpawnedShotgun);
	Weapons.Add(SpawnedRifle);
	Weapons.Add(SpawnedPistol);
}

void AAgent::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	
	// Set the Agent colour
	GetNewMesh()->CreateAndSetMaterialInstanceDynamic(0)->SetVectorParameterValue(TEXT("BodyColor"), EnemyColor);
	
}

void AAgent::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	
	SensingComp->RequestStimuliListenerUpdate();

	if (bPlayerSeen)  
	{
		// Check our LoS to the player and act accordingly when broken.
		CheckLoS();
		// Inside the bPlayerSeen check because there is no point in the agent
		// changing weapon if they cannot see the weapon
		FVector AgentPos = GetActorLocation();
		FVector PlayerPos = Cast<AAgentController>( GetController() )->GetFocalPoint();
		// Get the vector from the agent to the player
		FVector AgentToPlayer = PlayerPos - AgentPos;
		// Pass the distance between the agent and the player (magnitude of the vector)
		// Continiously make choices on which weapon to select
		SelectWeapon(AgentToPlayer.Size());
	}
}

void AAgent::SensePawn(TArray<AActor*> OtherPawn)
{
	// For each sensed Actor
	for (AActor* SensedActor : OtherPawn)
	{
		// Make sure it is the Player we are sensing and not other agents
		if ( (SensedActor != NULL) && (SensedActor->ActorHasTag("Player")) )
		{
			//===============Line Trace setup================//
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

void AAgent::CheckLoS()
{
	AAgentController* Controller = Cast<AAgentController>(GetController());
	if (Controller != NULL)
	{
		// Get the location of the agent
		FVector AgentLocation = GetActorLocation();
		// Get the direction the agent is facing
		FVector Direction = GetActorForwardVector();
		// Default trace params
		FCollisionQueryParams TraceParams(TEXT("LineOfSight_Trace"), false, this);
		TraceParams.bTraceAsyncScene = true;

		//=====Draw line trace from agent to player=====//
		FHitResult Hit(ForceInit);
		UWorld* World = GetWorld();

		World->LineTraceSingleByChannel(Hit, AgentLocation + Direction, Controller->GetFocalPoint(), ECollisionChannel::ECC_Visibility, TraceParams, FCollisionResponseParams::DefaultResponseParam);
		DrawDebugLine(World, AgentLocation + Direction, Controller->GetFocalPoint(), FColor::Yellow, false, -1, 0, 2.0f);
		//==============================================//

		AActor* HitActor = Hit.GetActor();
		if (HitActor != NULL && HitActor->ActorHasTag("Player"))
		{
		}
		/* Otherwise we can assume the actor intersecting the line trace is blocking the line of sight
		from the agent to the player */
		else if (HitActor != NULL)
		{
			/* The focal point is currently on the player actor. Set the PlayerLocation blackboard key
			to the location of this focal point, so that when the agent moves into the Search behaviour
			it will move to the actual location of the player when the agent lost LoS as opposed to the last
			location it sensed the player at.
			*/
			Controller->SetPlayerLocation(Controller->GetFocalPoint());
			// LoS to player is blocked
			bPlayerSeen = false;
			// Reset the player has seen blackboard key so that the Agent can begin searching.
			Controller->SetPlayerFound(bPlayerSeen);
			// Clear the focus on the player
			Controller->ClearFocus(EAIFocusPriority::Gameplay);
			bCanSearch = true;
			Controller->SetCanSearch(bCanSearch);
			// Stop firing if still firing
			if (isFiring)
				StopFiring();
		}
	}
}

void AAgent::SelectWeapon(float Dist)
{
	// Get the currently equipped weapon
	ABaseWeapon* CurrentWeapon = GetCurrentWeapon();
	// New weapon to change to
	ABaseWeapon* NewWeapon = new ABaseWeapon();
	double CurrentDesirability = CurrentWeapon->CalculateDesirability(Dist);
	// The desirability of the most desirable weapon at the moment
	double TempDesirability = 0.0;
	for (auto &it : Weapons)
	{
		// Calculate the desirability of this weapon
		double OtherDesirability = it->CalculateDesirability(Dist);
		// If the this weapon is more desirable than the currently equipped weapon 
		if (OtherDesirability > CurrentDesirability)
		{
			// Check for the first iteration of the Weapons array
			if (TempDesirability == 0.0)
			{
				// This is currently the most desirable weapon
				TempDesirability = OtherDesirability;
				// Set as new weapon to equip
				NewWeapon = it;
			}
			else if (OtherDesirability > TempDesirability)
				// Set this as the new weapon to equip
				NewWeapon = it;
		}
	}
	// Equip the weapon
	EquipWeapon(NewWeapon);
}

void AAgent::EquipWeapon(ABaseWeapon* NewWeapon)
{
	// Get the currently equipped weapon
	ABaseWeapon* CurrentWeapon = GetCurrentWeapon();
	// Destroy that weapon
	CurrentWeapon->Destroy();

	// Here we would spawn in the NewWeapon object and attach it
	// to the rootcomponent of the character. However I do have a
	// shotgun or a pistol asset (mesh, skeletal, material etc.) so
	// to visualize that they are equipped I shall merely display the 
	// shotgun or pistol animations respectively.
	if (NewWeapon == SpawnedPistol)
	{
		bRifleEquipped = false;
		bShotgunEquipped = false;
		bPistolEquipped = true;
	}
	else if (NewWeapon == SpawnedShotgun)
	{
		bRifleEquipped = false;
		bShotgunEquipped = true;
		bPistolEquipped = false;
	}
	// However because the Rifle does have a mesh the spawn and attach code will need
	// to be added specifically for the instance where the rifle is equipped.
	else
	{
		bRifleEquipped = true;
		bShotgunEquipped = false;
		bPistolEquipped = false;

		// Spawn and euip the rifle
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
		SpawnParams.Instigator = Instigator;
		UWorld* World = GetWorld();
		if (World)
		{
			NewWeapon = World->SpawnActor<ARifle>(SpawnParams);
			if (SpawnedRifle != NULL)
			{
				SpawnedRifle->AttachRootComponentTo(ManMesh, TEXT("GunSocket"), EAttachLocation::SnapToTargetIncludingScale, true);
				CurrentWeapon = SpawnedRifle;
			}
		}
	}
}

class APathNode* AAgent::GetPathNode() const { return PathNode; }
bool AAgent::GetPlayerSeen() const { return bPlayerSeen; }
bool AAgent::GetCanSearch() const { return bCanSearch; }