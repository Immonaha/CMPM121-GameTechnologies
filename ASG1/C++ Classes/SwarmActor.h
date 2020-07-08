// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SwarmActor.generated.h"
//include end

UCLASS()
class ASG1CPP_API ASwarmActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASwarmActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

////

public:
	//retunr the mesh
	FORCEINLINE class UStaticMeshComponent* GetMesh() const { return SwarmActorMesh; }
	UFUNCTION(BlueprintPure, Category = "Swarm")
	bool IsActive();

	UFUNCTION(BlueprintCallable, Category = "Swarm")
	void SetActive(bool NewState);

protected:
	bool bIsActive;

private:
	//mesh for swarm actor in level
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Swarm", meta = (AllowPrivateAccess = "true"));
	class UStaticMeshComponent* SwarmActorMesh;

};
