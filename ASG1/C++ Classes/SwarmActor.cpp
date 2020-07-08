// Fill out your copyright notice in the Description page of Project Settings.


#include "SwarmActor.h"

// Sets default values
ASwarmActor::ASwarmActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//
	bIsActive = true;

	//create static mech component
	SwarmActorMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SwarmActorMesh"));
	RootComponent = SwarmActorMesh;

}

// Called when the game starts or when spawned
void ASwarmActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASwarmActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

bool ASwarmActor::IsActive()
{
	return bIsActive;
}

void ASwarmActor::SetActive(bool NewState)
{
	bIsActive = NewState;
}

