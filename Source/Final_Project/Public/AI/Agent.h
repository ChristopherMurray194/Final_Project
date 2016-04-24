// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "BaseCharacter.h"
#include "Agent.generated.h"

UCLASS()
class FINAL_PROJECT_API AAgent : public ABaseCharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AAgent();
	
	virtual void PostInitializeComponents() override;
	virtual void Tick(float DeltaSeconds) override;
	virtual void BeginPlay() override;

	// The behaviour tree to be used
	UPROPERTY(EditAnywhere, Category = "Behaviour Tree")
		class UBehaviorTree* AgentBehaviourTree;

	// If not set to value Editor may crash
	UPROPERTY(EditAnywhere, Category = Path)
	class APathNode* PathNode;
	/* PathNode accessor */

	class APathNode* GetPathNode() const;

	// The component that defines the sight of the agent
	UPROPERTY(EditAnywhere)
		class UAIPerceptionComponent* SensingComp;
	
	// Sight sensing configuration
	class UAISenseConfig_Sight* SightComp;

	// Can the agent see the player
	UPROPERTY(BlueprintReadOnly)
		bool bPlayerSeen = false;
	bool GetPlayerSeen() const;

	// Delegate function
	UFUNCTION()
		void SensePawn(TArray<AActor*> OtherPawn);

	// Can the Agent begin searching?
	UPROPERTY(BlueprintReadWrite)
		bool bCanSearch = false;
	bool GetCanSearch() const;

	UPROPERTY(BlueprintReadOnly)
		bool bPistolEquipped = false;
	UPROPERTY(BlueprintReadOnly)
		bool bShotgunEquipped = false;
	UPROPERTY(BlueprintReadOnly)
		bool bRifleEquipped = true;

private:
	// Colour of the enemy
	FLinearColor EnemyColor = FLinearColor( (139.0f / 255.0f), (26.0f / 255.0f), (26.0f / 255.0f), 1.0f);

	float LastSeenTime = 0.0f;
	/* Agent has lost sight of player if the player's location
	is not updated after the specified time*/
	float TimeTillPawnLost = 5.0f;

	// Time the agent can search for
	float SearchTime = 10.0f;
	float SearchStartTime = 0.0f;

	/* If we can see the player, check we have a clear LoS */
	void CheckLoS();

	/* List of weapons owned by the Agent */
	TArray<ABaseWeapon*> Weapons;

	//============Weapons==============//
	class ABaseWeapon* SpawnedPistol;
	class ABaseWeapon* SpawnedShotgun;
	// SpawnedRifle is in BaseCharacter header
	//=================================//

	/* Handle weapon selection 
	* @param Dist - Distance of the agent to the player, used to calculate the desirability
	*/
	void SelectWeapon(float Dist);

	/* Equip a new weapon 
	* @param NewWeapon - New weapon to be equipped
	*/
	void EquipWeapon(ABaseWeapon* NewWeapon);
};
