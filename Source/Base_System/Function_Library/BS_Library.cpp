// Fill out your copyright notice in the Description page of Project Settings.


#include "BS_Library.h"
#include "DrawDebugHelpers.h"
#include "VectorTypes.h"
#include "Math/Color.h"
#include "Util/ColorConstants.h"

// Sets default values
ABS_Library::ABS_Library()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ABS_Library::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ABS_Library::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

FHitResult ABS_Library::SingleLineTraceViaActor(
	AActor* AIgnoreActor, float FDistance, 
	bool bDrawDebugLine, FLinearColor DebugColor,
	bool bPrintHitActorName, FLinearColor NameColor, bool bRandomLineColor)
{
	// Get world from game viewport
	UWorld *UWorld = GEngine->GameViewport->GetWorld();
	// Get actor location from ignore actor parameter
	FVector FStartPoint = AIgnoreActor->GetActorLocation();
	// Calculate End point from distance 
	FVector FEndPoint = (AIgnoreActor->GetActorForwardVector() * FDistance) + FStartPoint;
	// Field to hit result object reference
	FHitResult FHitResult;
	// Collision parameters filed
	FCollisionQueryParams QueryParams;
	// Collision parameter add ignore actor reference for self ignore
	QueryParams.AddIgnoredActor(AIgnoreActor);
	// Create a Single Channel Line Trace with parameters
	UWorld->LineTraceSingleByChannel(FHitResult, FStartPoint, FEndPoint,
		ECollisionChannel::ECC_Camera, QueryParams);

	
	if (bDrawDebugLine)
		DrawDebugLine(UWorld, FStartPoint, FEndPoint,
			// Draw Line Random color (inline if statement declaration)
			(bRandomLineColor) ? 
			UE::Geometry::LinearColors::MakeColor3f<FColor>(random()+1.f,random()+1.f,random()+1.f):
			DebugColor.ToFColor(false),
			
			false, 2.f);

	
	if (bPrintHitActorName && FHitResult.GetActor())
		GEngine->AddOnScreenDebugMessage(
			1, 1.f, NameColor.ToFColor(false),
			FString::Printf(TEXT("Hit Actor : %s"), *FHitResult.GetActor()->GetName()));

	// Return the hit result object reference
	return FHitResult;
}
