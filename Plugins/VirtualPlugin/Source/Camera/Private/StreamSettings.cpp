// Copyright 2020 ByOwls. All Rights Reserved.


#include "StreamSettings.h"

StreamSettings::StreamSettings(int32 resolution, int32 frameRate, int32 quality, int32 port, FString ipAddress)
{
	Resolution = resolution;
	FrameRate = frameRate;
	Quality = quality;
	Port = port;
	IpAddress = ipAddress;
}

