// Fill out your copyright notice in the Description page of Project Settings.


#include "ThatCoolSideScroller/Public/Components/HealthComponent.h"
#include "Engine/Engine.h"

// Sets default values for this component's properties
UHealthComponent::UHealthComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	SetDefaultValues();
}


void UHealthComponent::TakeDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
	if (Damage <= 0) {
		return;
	}

	health = FMath::Clamp(health - Damage, minHealth, defaultHealth);

}

// Called when the game starts
void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	AActor* owner = GetOwner();
	if (owner) {
		owner->OnTakeAnyDamage.AddDynamic(this, &UHealthComponent::TakeDamage);
	}
	
}


void UHealthComponent::SetDefaultValues()
{
	defaultHealth = 1.0f;
	health = defaultHealth;
	minHealth = 0.0f;
}

// Called every frame
void UHealthComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

