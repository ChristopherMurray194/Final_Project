// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine/TargetPoint.h"
#include "PathNode.generated.h"

UCLASS()
class FINAL_PROJECT_API APathNode : public ATargetPoint
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APathNode();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	class APathNode* GetNextNode() const;

	UPROPERTY(EditAnywhere, Category = Node)
		APathNode* NextNode;
};
