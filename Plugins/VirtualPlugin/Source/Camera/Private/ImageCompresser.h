// Copyright 2020 ByOwls. All Rights Reserved.
#pragma once

#include "CoreMinimal.h"


class ImageCompresser : public FNonAbandonableTask
{

private:

	int32 Quality;
	int32 Width;
	int32 Height;
	uint8* Data;
	int32 DataSize;
	class VideoStreaming* Streaming;
	int32 ControllId;

public:
	FORCEINLINE TStatId GetStatId() const
	{
		RETURN_QUICK_DECLARE_CYCLE_STAT(FActionImageCompress, STATGROUP_ThreadPoolAsyncTasks);
	}
	//default Constructor
	ImageCompresser(class VideoStreaming* InStreaming, int32 InControllId, uint8* InData, int32 InDataSize, int32 InWidth, int32 InHeight, int32 InQuality);
	~ImageCompresser();

	void DoWork();

};
