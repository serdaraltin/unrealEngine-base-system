// Copyright 2020 ByOwls. All Rights Reserved.

#include "CameraPlatform.h"
#include "Runtime/Engine/Classes/Kismet/KismetSystemLibrary.h"
#include "CameraPlatform.h"

jmethodID FCameraPlatform::CheckInternetConnectionMethod;
jmethodID FCameraPlatform::CheckGooglePlayServicesMethod;
jmethodID FCameraPlatform::GetPersistentUniqueDeviceIdMethod;
jmethodID FCameraPlatform::GetDeviceIdMethod;
jmethodID FCameraPlatform::GetLanIpMethod;

FCameraPlatform::FCameraPlatform()
{
	if (JNIEnv* Env = FAndroidApplication::GetJavaEnv())
	{
		CheckInternetConnectionMethod = FJavaWrapper::FindMethod(Env, FJavaWrapper::GameActivityClassID, "AndroidThunkJava_CheckInternetConnection", "()Z", false);
		CheckGooglePlayServicesMethod = FJavaWrapper::FindMethod(Env, FJavaWrapper::GameActivityClassID, "AndroidThunkJava_CheckGooglePlayServices", "()Z", false);
		GetPersistentUniqueDeviceIdMethod = FJavaWrapper::FindMethod(Env, FJavaWrapper::GameActivityClassID, "AndroidThunkJava_GetPersistentUniqueDeviceId", "()Ljava/lang/String;", false);
		GetDeviceIdMethod = FJavaWrapper::FindMethod(Env, FJavaWrapper::GameActivityClassID, "AndroidThunkJava_GetDeviceId", "()Ljava/lang/String;", false);
		GetLanIpMethod = FJavaWrapper::FindMethod(Env, FJavaWrapper::GameActivityClassID, "AndroidThunkJava_GetLanIp", "()Ljava/lang/String;", false);
	}
}

FCameraPlatform::~FCameraPlatform()
{
}

bool FCameraPlatform::CheckInternetConnection()
{
	bool bResult = false;
	if (JNIEnv* Env = FAndroidApplication::GetJavaEnv())
	{
		bResult = FJavaWrapper::CallBooleanMethod(Env, FJavaWrapper::GameActivityThis, FCameraPlatform::CheckInternetConnectionMethod);
	}
	return bResult;
}

bool FCameraPlatform::CheckGooglePlayServices()
{
	bool bResult = false;
	if (JNIEnv* Env = FAndroidApplication::GetJavaEnv())
	{
		bResult = FJavaWrapper::CallBooleanMethod(Env, FJavaWrapper::GameActivityThis, FCameraPlatform::CheckGooglePlayServicesMethod);
	}
	return bResult;
}

FString FCameraPlatform::GetPersistentUniqueDeviceId()
{
	FString ResultDeviceId = FString("");
	if (JNIEnv* Env = FAndroidApplication::GetJavaEnv())
	{
		jstring ResultDeviceIdString = (jstring)FJavaWrapper::CallObjectMethod(Env, FJavaWrapper::GameActivityThis, FCameraPlatform::GetPersistentUniqueDeviceIdMethod);
		const char *nativeDeviceIdString = Env->GetStringUTFChars(ResultDeviceIdString, 0);
		ResultDeviceId = FString(nativeDeviceIdString);
		Env->ReleaseStringUTFChars(ResultDeviceIdString, nativeDeviceIdString);
		Env->DeleteLocalRef(ResultDeviceIdString);
	}
	return ResultDeviceId;
}

FString FCameraPlatform::GetDeviceId()
{
	FString ResultDeviceId = FString("");
	if (JNIEnv* Env = FAndroidApplication::GetJavaEnv())
	{
		jstring ResultDeviceIdString = (jstring)FJavaWrapper::CallObjectMethod(Env, FJavaWrapper::GameActivityThis, FCameraPlatform::GetDeviceIdMethod);
		const char *nativeDeviceIdString = Env->GetStringUTFChars(ResultDeviceIdString, 0);
		ResultDeviceId = FString(nativeDeviceIdString);
		Env->ReleaseStringUTFChars(ResultDeviceIdString, nativeDeviceIdString);
		Env->DeleteLocalRef(ResultDeviceIdString);
	}
	return ResultDeviceId;
}

FString FCameraPlatform::GetLanIp()
{
	FString ResultDeviceId = FString("");
	if (JNIEnv* Env = FAndroidApplication::GetJavaEnv())
	{
		jstring ResultDeviceIdString = (jstring)FJavaWrapper::CallObjectMethod(Env, FJavaWrapper::GameActivityThis, FCameraPlatform::GetLanIpMethod);
		const char *nativeDeviceIdString = Env->GetStringUTFChars(ResultDeviceIdString, 0);
		ResultDeviceId = FString(nativeDeviceIdString);
		Env->ReleaseStringUTFChars(ResultDeviceIdString, nativeDeviceIdString);
		Env->DeleteLocalRef(ResultDeviceIdString);
	}
	return ResultDeviceId;
}