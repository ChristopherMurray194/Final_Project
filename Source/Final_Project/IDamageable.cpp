// Fill out your copyright notice in the Description page of Project Settings.

#include "Final_Project.h"
#include "IDamageable.h"

UIDamageable::UIDamageable(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{

}

float IIDamageable::DealDamage(float Damage)
{
	return Damage;
}