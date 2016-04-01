// Fill out your copyright notice in the Description page of Project Settings.

#include "Final_Project.h"
#include "Agent.h"
#include "Engine/Engine.h"

// Sets default values
AAgent::AAgent()
{

}

void AAgent::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	// Set the Agent colour
	GetMesh()->CreateAndSetMaterialInstanceDynamic(0)->SetVectorParameterValue(TEXT("BodyColor"), EnemyColor);
}
