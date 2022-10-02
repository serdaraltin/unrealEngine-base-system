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

FHitResult ABS_Library::MakeSingleLineTrace(
	AActor* AIgnoreActor, float FDistance, 
	bool bDrawDebugLine, FLinearColor DebugColor,
	bool bPrintHitActorName, FLinearColor NameColor, bool bRandomColor)
{
	float r = random()+1.f, g=random()+1.f, b=random()+1.f;
	UWorld *UWorld = GEngine->GameViewport->GetWorld();
	FVector FStartPoint = AIgnoreActor->GetActorLocation();
	FVector FEndPoint = (AIgnoreActor->GetActorUpVector() * (-1*FDistance)) + FStartPoint;
	FHitResult FHitResult;
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(AIgnoreActor);
	UWorld->LineTraceSingleByChannel(FHitResult, FStartPoint, FEndPoint,
		ECollisionChannel::ECC_Camera, QueryParams);
	if (bDrawDebugLine)
		DrawDebugLine(UWorld, FStartPoint, FEndPoint,
			(bRandomColor) ? DebugColor.ToFColor(false): UE::Geometry::LinearColors::MakeColor3f<FColor>(r,g,b),
			false, 5.f,0,1.5f);
	if (bPrintHitActorName && FHitResult.GetActor())
		GEngine->AddOnScreenDebugMessage(
			1, 1.f, NameColor.ToFColor(false),
			FString::Printf(TEXT("Actor : %s"), *FHitResult.GetActor()->GetName()));

	return FHitResult;
}
