// Fill out your copyright notice in the Description page of Project Settings.

#include "Final_Project.h"
#include "Projectile.h"
#include "GameFramework/ProjectileMovementComponent.h"

// Sets default values
AProjectile::AProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("Collision_Component"));
	CollisionComp->SetSphereRadius(10.0f);
	CollisionComp->AttachTo(RootComponent);

	SphereVisual = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Sphere_Mesh"));
	SphereVisual->AttachTo(CollisionComp);
	static ConstructorHelpers::FObjectFinder<UStaticMesh> SphereVisualAsset(TEXT("/Game/StarterContent/Shapes/Shape_Sphere.Shape_Sphere"));
	if (SphereVisualAsset.Succeeded())
	{
		SphereVisual->SetStaticMesh(SphereVisualAsset.Object);
		SphereVisual->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
		SphereVisual->SetWorldScale3D(FVector(0.1f, 0.01f, 0.01f));
		SphereVisual->SetCollisionEnabled(ECollisionEnabled::NoCollision);	// Set non collidable

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

