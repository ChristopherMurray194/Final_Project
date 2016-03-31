// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "Rifle.generated.h"

UCLASS()
class FINAL_PROJECT_API ARifle : public AActor
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere)
		USkeletalMeshComponent* GunMesh;

	UPROPERTY(EditAnywhere)
		UArrowComponent* ArrowComp;

public:	
	// Sets default values for this actor's properties
	ARifle();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

protected:

	bool isTriggerPulled = false;

	// Called when the fire key is pressed
	void PullTrigger();
	// Called when the fire key is released
	void ReleaseTrigger();
	void Fire();
	
private:
	// Timer handle
	FTimerHandle TimerHandle;

	// Rounds per second
	float RPS = 10.0f;
};
