// Fill out your copyright notice in the Description page of Project Settings.


#include "ThatCoolSideScroller/Public/Actors/Enemy.h"
#include "Particles/ParticleSystemComponent.h"
#include "Engine/Classes/Components/StaticMeshComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Engine/Engine.h"
#include "Kismet/GameplayStatics.h"
#include "ThatCoolSideScroller/ThatCoolSideScrollerCharacter.h"
// Sets default values
AEnemy::AEnemy()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	sphereRadius = 100.0f;

	CollisionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("Collision Sphere"));
	CollisionSphere->InitSphereRadius(sphereRadius);
	CollisionSphere->SetCollisionProfileName("Trigger");

	VisualMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	VisualMesh->SetupAttachment(RootComponent);
	static ConstructorHelpers::FObjectFinder<UStaticMesh> CubeVisualAsset(TEXT("/Game/StarterContent/Shapes/Shape_Cube.Shape_Cube"));
	if (CubeVisualAsset.Succeeded())
	{
		VisualMesh->SetStaticMesh(CubeVisualAsset.Object);
		VisualMesh->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
	}

	// Create a particle system that we can activate or deactivate
	OurParticleSystem = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("MovementParticles"));
	OurParticleSystem->SetupAttachment(VisualMesh);
	OurParticleSystem->bAutoActivate = true;
	OurParticleSystem->SetRelativeLocation(FVector(-20.0f, 0.0f, 40.0f));

	static ConstructorHelpers::FObjectFinder<UParticleSystem> ParticleAsset(TEXT("/Game/StarterContent/Particles/P_Fire.P_Fire"));
	if (ParticleAsset.Succeeded())
	{
		OurParticleSystem->SetTemplate(ParticleAsset.Object);
	}

	CollisionSphere->OnComponentBeginOverlap.AddDynamic(this, &AEnemy::OnOverlapBegin);
	CollisionSphere->OnComponentEndOverlap.AddDynamic(this, &AEnemy::OnOverlapEnd);
}

void AEnemy::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor != nullptr && OtherActor != this && OtherComp != nullptr && OtherActor == GetWorld()->GetFirstPlayerController()->GetPawn()) {
		//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("Overlapping with player"));

		AThatCoolSideScrollerCharacter* player = Cast<AThatCoolSideScrollerCharacter>(GetWorld()->GetFirstPlayerController()->GetCharacter());
		player->SetOnFire();
	}
}

void AEnemy::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor != nullptr && OtherActor != this && OtherComp != nullptr && OtherActor == GetWorld()->GetFirstPlayerController()->GetPawn()) {
		//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("STOPPED Overlapping with player"));
		AThatCoolSideScrollerCharacter* player = Cast<AThatCoolSideScrollerCharacter>(GetWorld()->GetFirstPlayerController()->GetCharacter());
		player->SetFireTimer();
	}
}
// Called when the game starts or when spawned
void AEnemy::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	FVector NewLocation = GetActorLocation();
	FRotator NewRotation = GetActorRotation();
	float RunningTime = GetGameTimeSinceCreation();
	float DeltaHeight = (FMath::Sin(RunningTime + DeltaTime) - FMath::Sin(RunningTime));
	NewLocation.Z += DeltaHeight * FloatSpeed;       //Scale our height by a factor of 20
	float DeltaRotation = DeltaTime * RotationSpeed;    //Rotate by 20 degrees per second
	NewRotation.Yaw += DeltaRotation;
	SetActorLocationAndRotation(NewLocation, NewRotation);
}

