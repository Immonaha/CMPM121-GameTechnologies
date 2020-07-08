// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Runtime/Engine/Classes/Components/BoxComponent.h"
#include "SwarmSpawnVolume.generated.h"

UCLASS()
class ASG1CPP_API ASwarmSpawnVolume : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASwarmSpawnVolume();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

////
public:
	FORCEINLINE class UBoxComponent* GetSpawnLocation() const { return WhereToSpawn; }

	UFUNCTION(BlueprintPure, Category = "Swarm")
	FVector GetRandomPointInVolume();

protected:
	UPROPERTY(EditAnywhere, Category = "Swarm")
	TSubclassOf<class ASwarmActor> WhatToSpawn;

	FTimerHandle SpawnTimer;

	//Min Max spawn delay
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Swarm")
	float SpawnDelayRangeLow;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Swarm")
	float SpawnDelayRangeHigh;
	

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Swarm", meta = (AllowPrivateAccess = "true"));
	class UBoxComponent* WhereToSpawn;

	//handle spawning
	void SpawnSwarmActor();
	//spawn delay
	float SpawnDelay;
};
