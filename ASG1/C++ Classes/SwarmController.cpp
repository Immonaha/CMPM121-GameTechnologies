// Fill out your copyright notice in the Description page of Project Settings.


#include "SwarmController.h"
#include "Components/BrushComponent.h"
#include "Runtime\Core\Public\Math\Box.h"
#include "Runtime\Engine\Classes\Kismet\KismetMathLibrary.h"
#include "Runtime/Engine/Classes/Components/InstancedStaticMeshComponent.h"

ASwarmController::ASwarmController(const FObjectInitializer& ObjectInitializer)
	//:super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = true;
	SetActorTickEnabled(true);


	myMeshComponent = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("myMeshComponent"));
	myMeshComponent->SetupAttachment(RootComponent);
	GetBrushComponent()->Mobility = EComponentMobility::Static;

	BrushColor = FColor(255, 255, 255, 255);

	bColored = true;

	playerPawn = GetWorld()->GetFirstPlayerController()->GetPawn();

	//playerActor->GetActorLocation();
}

ASwarmController::~ASwarmController()
{
	mySwarmData.Empty();
}

void ASwarmController::BeginPlay()
{
	Super::BeginPlay();

	myMeshComponent->SetStaticMesh(myMesh);
	mySwarmData.Reserve(myParameters.mySwarmSize);

	myBounds = GetComponentsBoundingBox(false);

	myAABBMin = myBounds.GetCenter() - myBounds.GetExtent();
	myAABBMax = myBounds.GetCenter() + myBounds.GetExtent();

	//create each object at random positions in bounding box
	for (int i = 0; i < myParameters.mySwarmSize; i++)
	{
		int index = mySwarmData.Emplace();
		SwarmData& data = mySwarmData[index];

		data.myPosition = FVector(0.0f, 0.0f, 0.0f);//(FMath::RandPointInBox(GetComponentsBoundingBox(true)) / GetTransform().GetScale3D());
		data.myVelocity = FVector(FMath::VRand());

		FTransform tx;
		tx.SetLocation(data.myPosition);
		tx.SetScale3D(FVector(myMeshScale));
		myMeshComponent->AddInstance(tx);
	}

	myParameters.Update();
}

void ASwarmController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	int i = 0;
	//for every boid in the swarm
	for (SwarmData& data : mySwarmData)
	{
		FVector meanPos(0.f);
		FVector meanVel(0.f);
		FVector avoidance(0.f);

		int numInCohesionRange = 0;
		int numInAlignRange = 0;
		int j = 0;

		for (const SwarmData& innerSwarmNode : mySwarmData)
		{
			if (i != j)
			{
				float dist2 = FVector::DistSquared(data.myPosition, innerSwarmNode.myPosition);

				//Cohesion, Avoidance, and Alignment

				//Cohesion: find the average position of nearby boids
				//Every bird attempts to move towards the average position of other nearby birds.
				if (dist2 < myParameters.myCohesionRange2)
				{
					meanPos += innerSwarmNode.myPosition;
					numInCohesionRange++;
				}

				//Avoidance: create a vector pointing away from nearby boids
				//Birds try to change their position so that it corresponds with the average alignment of other nearby birds.
				if (dist2 < (myParameters.myAvoidRange2))
				{
					avoidance += data.myPosition - innerSwarmNode.myPosition;
				}

				//Alignment: find the mean velocity of nearby boids
				//Each bird attempts to maintain a reasonable amount of distance between itself and any nearby birds, to prevent overcrowding.
				if (dist2 < (myParameters.myAlignRange2))
				{
					meanVel += innerSwarmNode.myVelocity;
				}
			}
			++j;
		}

		//Coherence is a vector pointing from current pos towards the mean position of other boids within range
		if (numInCohesionRange)
			meanPos /= numInCohesionRange;

		FVector coherence = (meanPos - data.myPosition) * myParameters.myCohesionWeight;

		if (numInAlignRange)
			meanVel /= numInAlignRange;

		FVector alignment = meanVel * myParameters.myAlignWeight;

		// Calculate bounds vector, push us back inside the volume if we leave it
		FVector bounds(0.f);

		if (data.myPosition.X < myAABBMin.X) {
			bounds.X = myParameters.myAvoidBoundsWeight;
		}
		else if (data.myPosition.X > myAABBMax.X) {
			bounds.X = -myParameters.myAvoidBoundsWeight;
		}

		if (data.myPosition.Y < myAABBMin.Y) {
			bounds.Y = myParameters.myAvoidBoundsWeight;
		}
		else if (data.myPosition.Y > myAABBMax.Y) {
			bounds.Y = -myParameters.myAvoidBoundsWeight;
		}

		if (data.myPosition.Z < myAABBMin.Z) {
			bounds.Z = myParameters.myAvoidBoundsWeight;
		}
		else if (data.myPosition.Z > myAABBMax.Z) {
			bounds.Z = -myParameters.myAvoidBoundsWeight;
		}

		bounds *= myParameters.myAvoidBoundsWeight;


		// Add up the component vectors, and Lerp towards to the new velocity to keep things smooth.
		data.myVelocity = FMath::Lerp(data.myVelocity, (data.myVelocity + (coherence + avoidance + alignment + bounds)).GetClampedToMaxSize(myParameters.myVelocityMax), DeltaSeconds * 10.f);
		data.myPosition += data.myVelocity * DeltaSeconds;

		// Calculate instance transform. Probably a more efficient way to calculate rotation here?
		FTransform tx;
		myMeshComponent->GetInstanceTransform(i, tx);
		FQuat lookAtRotator = FRotationMatrix::MakeFromX(data.myVelocity).ToQuat();
		tx.SetRotation(lookAtRotator);
		tx.SetLocation(data.myPosition);

		// Apply the new transform
		myMeshComponent->UpdateInstanceTransform(i, tx, false, i == mySwarmData.Num() - 1, true);

		++i;
	}
}