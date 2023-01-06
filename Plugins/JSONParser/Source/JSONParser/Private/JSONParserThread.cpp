// Copyright 2017 David Romanski(Socke). All Rights Reserved.

#include "JSONParserThread.h"

uint32 FJSONParserThread::Run(){
	switch (jobData.jobType)
	{
	case EJSONThreadJobType::E_StringToObject:
	{
		bool successful = false;
		struct FSJsonObject jsonObject = UJSONParserBPLibrary::getJSONParserBPLibrary()->JsonStringToObject(jobData.jsonString, jobData.removeAndSavePadding, successful);
		if (jobData.JSONParserAsyncStringToObject != nullptr) {
			struct FSJsonThreadJobData jobDataCopy = jobData;
			AsyncTask(ENamedThreads::GameThread, [jobDataCopy,jsonObject,successful]() {
				jobDataCopy.JSONParserAsyncStringToObject->jobComplete(jsonObject, successful);
			});
		}
	}
		break;
	case EJSONThreadJobType::E_FileToObject:
	{
		FString jsonString = FString();
		struct FSJsonObject jsonObject;
		bool successful = UJSONParserBPLibrary::getJSONParserBPLibrary()->readStringFromFile(jobData.directoryType, jobData.filePath, jsonString);
		if (successful) {
			jsonObject = UJSONParserBPLibrary::getJSONParserBPLibrary()->JsonStringToObject(jsonString, jobData.removeAndSavePadding, successful);
		}
		if (jobData.JSONParserAsyncStringToObject != nullptr) {
			struct FSJsonThreadJobData jobDataCopy = jobData;
			AsyncTask(ENamedThreads::GameThread, [jobDataCopy, jsonObject, successful]() {
				jobDataCopy.JSONParserAsyncStringToObject->jobComplete(jsonObject, successful);
				});
		}
	}
	break;
	case EJSONThreadJobType::E_ObjectToString:
	{
		bool successful = false;
		FString jsonString = UJSONParserBPLibrary::getJSONParserBPLibrary()->JsonObjectToString(successful, jobData.jsonObject, jobData.removeLineBreaks, jobData.addPadding);
		if (jobData.JSONParserAsyncObjectToString != nullptr) {
			struct FSJsonThreadJobData jobDataCopy = jobData;
			AsyncTask(ENamedThreads::GameThread, [jobDataCopy, jsonString, successful]() {
				jobDataCopy.JSONParserAsyncObjectToString->jobComplete(jsonString, successful);
			});
		}
	}
		break;
	case EJSONThreadJobType::E_ObjectToFile:
	{
		bool successful = false;
		FString jsonString = UJSONParserBPLibrary::getJSONParserBPLibrary()->JsonObjectToString(successful, jobData.jsonObject, jobData.removeLineBreaks, jobData.addPadding);
		if (successful) {
			successful = UJSONParserBPLibrary::writeStringToFile(jobData.directoryType, jsonString, jobData.filePath);
		}
		if (jobData.JSONParserAsyncObjectToString != nullptr) {
			struct FSJsonThreadJobData jobDataCopy = jobData;
			AsyncTask(ENamedThreads::GameThread, [jobDataCopy, jsonString, successful]() {
				jobDataCopy.JSONParserAsyncObjectToString->jobComplete(jsonString, successful);
				});
		}
	}
	break;
	}

	thread = nullptr;
	return 0;
}
