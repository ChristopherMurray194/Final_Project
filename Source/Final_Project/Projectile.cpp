// Fill out your copyright notice in the Description page of Project Settings.

#include "Final_Project.h"
#include "Projectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "IDamageable.h"
#include "BaseCharacter.h"

// Sets default values
AProjectile::AProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("Collision_Component"));
	CollisionComp->SetSphereRadius(5.0f);
	CollisionComp->AttachTo(RootComponent);
	CollisionComp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	CollisionComp->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	
	SphereVisual = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Sphere_Mesh"));
	SphereVisual->AttachTo(CollisionComp);
	static ConstructorHelpers::FObjectFinder<UStaticMesh> SphereVisualAsset(TEXT("/Game/StarterContent/Shapes/Shape_Sphere.Shape_Sphere"));
	if (SphereVisualAsset.Succeeded())
	{
		SphereVisual->SetStaticMesh(SphereVisualAsset.Object);
		SphereVisual->SetRelativeLocation(FVector(-7.0f, 0.0f, 0.0f));
		SphereVisual->SetWorldScale3D(FVector(0.2f, 0.04f, 0.02f));
		SphereVisual->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		
		static ConstructorHelpers::FObjectFinder<UMaterial> MaterialAsset(TEXT("/Game/Materials/BulletMat"));
		if (MaterialAsset.Succeeded())
		{
			SphereVisual->SetMaterial(0, MaterialAsset.Object);
		}
		
		UProjectileMovementComponent* ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Projectile_Movement"));
		ProjectileMovement->InitialSpeed = 1200.0f;
		ProjectileMovement->ProjectileGravityScale = 0.0f;
	}
}

// Called when the game starts or when spawned
void AProjectile::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AProjectile::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

void AProjectile::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);

	// Check the actor implements the IDamageable interface
	if (OtherActor->GetClass()->ImplementsInterface(UIDamageable::StaticClass()))
	{
		// If the actor is an enemy agent
		if (OtherActor->ActorHasTag(TEXT("Enemy")))
		{
			/*
			 * OtherActor is of BaseCharacter class type so create cast
			 * So that we can access the class' functions 
			 */
			ABaseCharacter* BaseCharacter = Cast<ABaseCharacter>(OtherActor);
			// Deal damage to the base character
			BaseCharacter->DealDamage_Implementation(20);
		}
		// Destroy the projectile when we collide
		Destroy();
	}
	else // If we hit anything else that is overlapable
	{
		// Destroy the projectile
		Destroy();
	}
}