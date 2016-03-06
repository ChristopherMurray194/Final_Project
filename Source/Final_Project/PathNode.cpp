// Fill out your copyright notice in the Description page of Project Settings.

#include "Final_Project.h"
#include "PathNode.h"

// Sets default values
APathNode::APathNode()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void APathNode::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APathNode::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

// Accessor for the next node in the path
class AActor* APathNode::GetNextNode() const 
{ 
	return NextNode; 
}

