// Fill out your copyright notice in the Description page of Project Settings.


#include "ThatCoolSideScroller/Public/Components/StaminaComponent.h"
#include "Engine/World.h"
#include "TimerManager.h"

// Sets default values for this component's properties
UStaminaComponent::UStaminaComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	defaultStamina = 1.0f;
	stamina = defaultStamina;
	jumpStaminaCost = 0.2f;
	staminaRegenInterval = 0.01f;
	staminaRegenAmount = 0.001f;
	minStamina = 0.0f;
}


bool UStaminaComponent::StaminaJumpCost(bool isJumping)
{
	if (stamina >= jumpStaminaCost && !isJumping) {
		stamina -= jumpStaminaCost;
		return true;
	}
	else {
		return false;
	}
}

void UStaminaComponent::StartStaminaRegenTimer()
{
	UWorld* world = GetWorld();
	
	if (world != nullptr) {
		world->GetTimerManager().SetTimer(timer, this, &UStaminaComponent::RegenStamina, staminaRegenInterval, true);
	}
	
}

// Called when the game starts
void UStaminaComponent::BeginPlay()
{
	Super::BeginPlay();
	StartStaminaRegenTimer();
}


void UStaminaComponent::RegenStamina()
{
	stamina = FMath::Clamp(stamina + staminaRegenAmount, minStamina, defaultStamina);

	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red,FString::FromInt(stamina));
}


