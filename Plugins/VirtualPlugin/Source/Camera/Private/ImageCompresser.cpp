// Copyright 2020 ByOwls. All Rights Reserved.
#include "ImageCompresser.h"

#include "VideoStreaming.h"
#include "StreamCommunication.h"
#include "StreamSettings.h"
#include "IImageWrapper.h"
#include "IImageWrapperModule.h"


ImageCompresser::ImageCompresser(class VideoStreaming* InStreaming, int32 InControllId, uint8* InData, int32 InDataSize, int32 InWidth, int32 InHeight, int32 InQuality)
	: Streaming(InStreaming)
{
	Width = InWidth;
	Height = InHeight;
	Quality = InQuality;
	ControllId = InControllId;
	Data = InData;
	DataSize = InDataSize;
}

void ImageCompresser::DoWork()
{
	static const FName ImageWrapperModuleName("ImageWrapper");
	IImageWrapperModule& ImageWrapperModule = FModuleManager::GetModuleChecked<IImageWrapperModule>(ImageWrapperModuleName);
	TSharedPtr<IImageWrapper> Wrapper = ImageWrapperModule.CreateImageWrapper(EImageFormat::JPEG);

	const bool bResult = Wrapper->SetRaw(Data, DataSize, Width, Height, ERGBFormat::RGBA, 8);
	if (bResult)
	{
		 const TArray64<uint8> CompressedData = Wrapper->GetCompressed(Quality);

		if (CompressedData.Num() > 0)
		{
			uint32 DatLen = CompressedData.Num();

			uint8 Orientation = Streaming->GetOrientation(ControllId);
			FBufferArchive Buffer;
			Buffer << Orientation;
			Buffer.Append(CompressedData.GetData(), CompressedData.Num());

			if (Streaming)
			{
				//UE_LOG(LogTemp, Warning, TEXT("Send Compressed Image"));
				Streaming->SendAction(ControllId, 2, Buffer);

			}
		}
	}
}



ImageCompresser::~ImageCompresser()
{
	delete[] Data;
}
