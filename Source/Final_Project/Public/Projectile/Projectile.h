// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "Projectile.generated.h"

UCLASS()
class FINAL_PROJECT_API AProjectile : public AActor
{
	GENERATED_BODY()

	class USphereComponent* CollisionComp;
	class UStaticMeshComponent* SphereVisual;
	
public:	
	// Sets default values for this actor's properties
	AProjectile();
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	// Called when the projectile overlaps another Actor
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;

};
