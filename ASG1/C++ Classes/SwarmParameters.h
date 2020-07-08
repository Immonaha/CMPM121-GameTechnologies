// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SwarmParameters.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct ASG1CPP_API FUSwarmParameters
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int mySwarmSize;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float myAvoidRange;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float myAvoidWeight;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float myCohesionWeight;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float myCohesionRange;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float myAlignWeight;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float myAlignRange;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float myAvoidBoundsWeight;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float myVelocityMax;

	float myAvoidRange2, myCohesionRange2, myAlignRange2;

	//easier to set params this way
	FUSwarmParameters() :
		mySwarmSize(500),
		myAvoidRange(0.5f),
		myAvoidWeight(0.5f),
		myCohesionWeight(0.01f),
		myCohesionRange(0.5f),
		myAlignWeight(0.01f),
		myAlignRange(0.5f),
		myAvoidBoundsWeight(0.0f),
		myVelocityMax(50.f),
		myAvoidRange2(0.f),
		myCohesionRange2(0.f),
		myAlignRange2(0.f) {}

	void Update()
	{
		myAvoidRange2 = myAvoidRange * myAvoidRange;
		myCohesionRange2 = myCohesionRange * myCohesionRange;
		myAlignRange2 = myAlignRange * myAlignRange;
	}
	
};