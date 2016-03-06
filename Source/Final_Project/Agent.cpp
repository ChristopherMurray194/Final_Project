// Fill out your copyright notice in the Description page of Project Settings.

#include "Final_Project.h"
#include "Agent.h"

// Sets default values
AAgent::AAgent()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GetCharacterMovement()->MaxWalkSpeed = 300.0f;			// Set the maximum walk speed.
	GetCharacterMovement()->MaxWalkSpeedCrouched = 160.0f;	// Set the maximum walk speed whilst crouched.
}

// Called when the game starts or when spawned
void AAgent::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AAgent::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

// Called to bind functionality to input
void AAgent::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
	Super::SetupPlayerInputComponent(InputComponent);
	
}
