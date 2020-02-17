// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "StaminaComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class THATCOOLSIDESCROLLER_API UStaminaComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UStaminaComponent();

	UFUNCTION(BlueprintCallable, Category = "Stamina")
		bool StaminaJumpCost(bool isJumping);

	UFUNCTION(BlueprintCallable, Category = "Stamina")
		void StartStaminaRegenTimer();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stamina")
		float staminaRegenInterval;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stamina")
		float staminaRegenAmount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stamina")
		float defaultStamina;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stamina")
		float jumpStaminaCost;

	UPROPERTY(BlueprintReadOnly, Category = "Stamina")
		float stamina;

	UPROPERTY(BlueprintReadOnly, Category = "Stamina")
		float jumpInterval;
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	float minStamina;

	

	FTimerHandle timer;

	void RegenStamina();

public:	
		
};
