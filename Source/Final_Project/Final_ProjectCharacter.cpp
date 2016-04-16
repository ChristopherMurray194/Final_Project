// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#include "Final_Project.h"
#include "Final_ProjectCharacter.h"
#include "Animation/AnimInstance.h"
#include "Engine.h"
#include "Blueprint/UserWidget.h"
#include "Perception/AISense_Sight.h"
#include "Agent.h"
#include "Rifle.h"

//////////////////////////////////////////////////////////////////////////
// AFinal_ProjectCharacter

AFinal_ProjectCharacter::AFinal_ProjectCharacter()
{
	Tags.Add(TEXT("Player")); // This is the player, used to ensure player cannot damage self

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;
	
	// Change sprint speed for player
	SprintSpeed = 700.0f;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	// Rotate when the controller rotates
	bUseControllerRotationYaw = true;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->AttachTo(RootComponent);
	CameraBoom->TargetArmLength = DefaultCameraDist; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller
	CameraBoom->SetRelativeLocation(FVector(0.0f, 60.0f, 50.0f));

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->AttachTo(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	AIControllerClass = APlayerController::StaticClass();
	// Auto possess player0
	AutoPossessPlayer = EAutoReceiveInput::Player0;
}

void AFinal_ProjectCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	
	// Set the Agent colour
	GetNewMesh()->CreateAndSetMaterialInstanceDynamic(0)->SetVectorParameterValue(TEXT("BodyColor"), PlayerColor);
}

void AFinal_ProjectCharacter::BeginPlay()
{
	Super::BeginPlay();

	UWorld* World = GetWorld();
	if (World)
	{
		if (HUD != NULL)
		{
			// Create the HUD widget
			UUserWidget* Widget = CreateWidget<UUserWidget>(World, HUD);
			// Display the HUD
			Widget->AddToViewport();
		}
	}

	// Player can be seen by AI
	UAIPerceptionSystem::RegisterPerceptionStimuliSource(this, UAISense_Sight::StaticClass(), this);
}

void AFinal_ProjectCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	// Set the reticle location
	ReticleLocation = FVector2D(ReticleX, ReticleY);
}

//////////////////////////////////////////////////////////////////////////
// Input

void AFinal_ProjectCharacter::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
	// Set up gameplay key bindings
	check(InputComponent);
	// Handle space bar input
	InputComponent->BindAction("Jump", IE_Pressed, this, &AFinal_ProjectCharacter::Jump);
	InputComponent->BindAction("Jump", IE_Released, this, &AFinal_ProjectCharacter::StopJumping);
	
	// Hnadle C key input
	InputComponent->BindAction("Crouch", IE_Pressed, this, &AFinal_ProjectCharacter::Crouch);
	InputComponent->BindAction("Crouch", IE_Released, this, &AFinal_ProjectCharacter::UnCrouch);

	// Use same input as Crouch (C key), but only fire when double pressed
	InputComponent->BindAction("Prone", IE_DoubleClick, this, &AFinal_ProjectCharacter::GoProne);

	// Handle WASD key input
	InputComponent->BindAxis("MoveForward", this, &AFinal_ProjectCharacter::MoveForward);
	InputComponent->BindAxis("MoveRight", this, &AFinal_ProjectCharacter::MoveRight);

	// Handle left shift key input
	InputComponent->BindAction("Sprint", IE_Pressed, this, &ABaseCharacter::Sprint);
	InputComponent->BindAction("Sprint", IE_Released, this, &ABaseCharacter::StopSprinting);

	// Handle right mouse button input
	InputComponent->BindAction("Aim", IE_Pressed, this, &ABaseCharacter::AimDownSight);
	InputComponent->BindAction("Aim", IE_Released, this, &ABaseCharacter::StopAiming);

	// Handle left mouse button input
	InputComponent->BindAction("Fire", IE_Pressed, this, &ABaseCharacter::Fire);
	InputComponent->BindAction("Fire", IE_Released, this, &ABaseCharacter::StopFiring);

	// Handle R key input
	InputComponent->BindAction("Reload", IE_Pressed, this, &ABaseCharacter::BeginReload);
	InputComponent->BindAction("Reload", IE_Released, this, &ABaseCharacter::EndReload);

	// Handle double press of Y key input
	InputComponent->BindAction("UnlockYaw", IE_DoubleClick, this, &AFinal_ProjectCharacter::ToggleControllerYaw);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	InputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	InputComponent->BindAxis("TurnRate", this, &AFinal_ProjectCharacter::TurnAtRate);
	InputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	InputComponent->BindAxis("LookUpRate", this, &AFinal_ProjectCharacter::LookUpAtRate);

	// handle touch devices
	InputComponent->BindTouch(IE_Pressed, this, &AFinal_ProjectCharacter::TouchStarted);
	InputComponent->BindTouch(IE_Released, this, &AFinal_ProjectCharacter::TouchStopped);
}


void AFinal_ProjectCharacter::TouchStarted(ETouchIndex::Type FingerIndex, FVector Location)
{
	// jump, but only on the first touch
	if (FingerIndex == ETouchIndex::Touch1)
	{
		Jump();
	}
}

void AFinal_ProjectCharacter::TouchStopped(ETouchIndex::Type FingerIndex, FVector Location)
{
	if (FingerIndex == ETouchIndex::Touch1)
	{
		StopJumping();
	}
}

void AFinal_ProjectCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void AFinal_ProjectCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void AFinal_ProjectCharacter::MoveForward(float Value)
{
	if ((Controller != NULL) && (Value != 0.0f))
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void AFinal_ProjectCharacter::MoveRight(float Value)
{
	if ((Controller != NULL) && (Value != 0.0f))
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);
	}
}

void AFinal_ProjectCharacter::Crouch()
{
	/* 
	Check to ensure isProne not true otherwise, pressing C whilst
	in prone can stop reverting back to standing whilst in prone
	*/
	// First key press
	if (!bStopCrouching && !isProne)
		// Crouch
		isCrouching = true;

	// Second key press
	if (bStopCrouching && !isProne)
		// UnCrouch
		isCrouching = false;
}

void AFinal_ProjectCharacter::UnCrouch()
{
	// If we are crouching (first key press)
	if (isCrouching)
		// On the second key press we want to be able to stop crouching
		bStopCrouching = true;

	// If we are not crouching (second key press)
	if (!isCrouching)
		// On the next key press we want to be able to crouch
		bStopCrouching = false;
}

void AFinal_ProjectCharacter::GoProne()
{
	// If we are already standing
	if (!bCanStand && !isCrouching)
	{
		// Go prone
		isProne = true;
		// We want to be able to stand up
		bCanStand = true;
	}
	else if(bCanStand && !isCrouching)// If can stand up
	{
		// Exit prone
		isProne = false;
		// Cannot stand because we are already standing
		bCanStand = false;
	}
}

void AFinal_ProjectCharacter::ToggleControllerYaw()
{
	// If using controller yaw
	if (!bUnlockYaw)
	{
		// Do not use controller yaw
		bUseControllerRotationYaw = false;
		bUnlockYaw = true;
	}
	else if (bUnlockYaw) // If not using controller yaw
	{
		// Give rotation control back to the controller.
		bUseControllerRotationYaw = true;
		bUnlockYaw = false;
	}
}

void AFinal_ProjectCharacter::AimDownSight()
{
	Super::AimDownSight();
	// Zoom the camera in
	CameraBoom->TargetArmLength = 100.0f;
}

void AFinal_ProjectCharacter::StopAiming()
{
	Super::StopAiming();
	// Zoom the camera back out
	CameraBoom->TargetArmLength = DefaultCameraDist;
}