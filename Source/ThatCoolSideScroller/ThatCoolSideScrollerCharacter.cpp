// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "ThatCoolSideScrollerCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Engine/Engine.h"
#include "UObject/ConstructorHelpers.h"
#include "Particles/ParticleSystemComponent.h"
#include "TimerManager.h"

AThatCoolSideScrollerCharacter::AThatCoolSideScrollerCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Create a particle system that we can activate or deactivate
	PlayerParticles = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("MovementParticles"));
	PlayerParticles->SetupAttachment(GetCapsuleComponent());

	static ConstructorHelpers::FObjectFinder<UParticleSystem> ParticleAsset(TEXT("/Game/StarterContent/Particles/P_Fire.P_Fire"));
	if (ParticleAsset.Succeeded())
	{
		PlayerParticles->SetTemplate(ParticleAsset.Object);
	}

	// Don't rotate when the controller rotates.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Create a camera boom attached to the root (capsule)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->SetUsingAbsoluteRotation(true); // Rotation of the character should not affect rotation of boom
	CameraBoom->bDoCollisionTest = false;
	CameraBoom->TargetArmLength = 500.f;
	CameraBoom->SocketOffset = FVector(0.f, 0.f, 75.f);
	CameraBoom->SetRelativeRotation(FRotator(0.f, 180.f, 0.f));

	// Create a camera and attach to boom
	SideViewCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("SideViewCamera"));
	SideViewCameraComponent->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	SideViewCameraComponent->bUsePawnControlRotation = false; // We don't want the controller rotating the camera

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Face in the direction we are moving..
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 720.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->GravityScale = 2.f;
	GetCharacterMovement()->AirControl = 0.80f;
	GetCharacterMovement()->JumpZVelocity = 1000.f;
	GetCharacterMovement()->GroundFriction = 3.f;
	GetCharacterMovement()->MaxWalkSpeed = 600.f;
	GetCharacterMovement()->MaxFlySpeed = 600.f;

	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->SetPlaneConstraintAxisSetting(EPlaneConstraintAxisSetting::X);

	StaminaComponent = CreateDefaultSubobject<UStaminaComponent>(TEXT("StaminaComponent"));
	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("HealthComponent"));

	IsOnFire = false;
	FireDamage = 0.001;
	FireDuration = 1.0f;

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)
}

void AThatCoolSideScrollerCharacter::SetOnFire()
{
	PlayerParticles->ActivateSystem();
	IsOnFire = true;
}

void AThatCoolSideScrollerCharacter::PutOutFire()
{
	PlayerParticles->DeactivateSystem();
	IsOnFire = false;

	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red,TEXT("PUT OUT FIRE"));
}

void AThatCoolSideScrollerCharacter::SetFireTimer()
{
	GetWorld()->GetTimerManager().SetTimer(FireTimer, this, &AThatCoolSideScrollerCharacter::PutOutFire, FireDuration, false);
}

void AThatCoolSideScrollerCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// set up gameplay key bindings
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &AThatCoolSideScrollerCharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);
	PlayerInputComponent->BindAxis("MoveRight", this, &AThatCoolSideScrollerCharacter::MoveRight);

	PlayerInputComponent->BindTouch(IE_Pressed, this, &AThatCoolSideScrollerCharacter::TouchStarted);
	PlayerInputComponent->BindTouch(IE_Released, this, &AThatCoolSideScrollerCharacter::TouchStopped);
}

void AThatCoolSideScrollerCharacter::Jump()
{
	//AThatCoolSideScrollerCharacter::FindComponentByClass<UStaminaComponent>() //that's how to find component
	if (StaminaComponent->StaminaJumpCost(GetCharacterMovement()->IsFalling()) && StaminaComponent != nullptr) {
		ACharacter::Jump();
	}
}

void AThatCoolSideScrollerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (IsOnFire) {
		HealthComponent->ApplyDamage(FireDamage);
	}

}

void AThatCoolSideScrollerCharacter::MoveRight(float Value)
{
	// add movement in that direction
	AddMovementInput(FVector(0.f, -1.f, 0.f), Value);
}

void AThatCoolSideScrollerCharacter::TouchStarted(const ETouchIndex::Type FingerIndex, const FVector Location)
{
	// jump on any touch
	Jump();
}

void AThatCoolSideScrollerCharacter::TouchStopped(const ETouchIndex::Type FingerIndex, const FVector Location)
{
	StopJumping();
}

