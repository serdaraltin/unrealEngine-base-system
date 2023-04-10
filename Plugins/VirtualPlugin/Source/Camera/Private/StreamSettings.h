// Copyright 2020 ByOwls. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "MyActor.h"


//This is our settings for streaming video
class StreamSettings
{
public:

	//Default Constructor
	StreamSettings(int32 resolution, int32 frameRate, int32 quality, int32 port, FString ipAddress);

	//FPS
	int32 FrameRate;

	//Quality
	int32 Quality;


	//func to get real quality
	int32 GetRealQuality() const
	{
		return FMath::GetMappedRangeValueClamped(FVector2D(5, 100), FVector2D(5, 50), Quality);
	}

	//Phones ip adress
	FString IpAddress;

	//resolution of image
	int32 Resolution;

	//port we connect between phone and pc
	int32 Port;

};
