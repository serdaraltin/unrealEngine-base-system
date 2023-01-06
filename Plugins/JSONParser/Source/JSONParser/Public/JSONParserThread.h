// Copyright 2017 David Romanski(Socke). All Rights Reserved.
#pragma once

#include "JSONParser.h"
//#include "JSONParserThead.generated.h"

class JSONPARSER_API FJSONParserThread : public FRunnable {

public:

	FJSONParserThread(struct FSJsonThreadJobData jobDataP) : 
		jobData(jobDataP) {
		FString threadName = "FJSONParserThread" + FGuid::NewGuid().ToString();
		thread = FRunnableThread::Create(this, *threadName, 0, EThreadPriority::TPri_Normal);
	}

	virtual uint32 Run() override;

protected:
	struct FSJsonThreadJobData jobData;
	FRunnableThread* thread = nullptr;
};