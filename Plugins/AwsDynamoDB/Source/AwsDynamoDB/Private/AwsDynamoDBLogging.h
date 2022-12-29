/**
* Copyright (C) 2017-2022 eelDev AB
*/

#define LogDebug(format, ...) UE_LOG(LogAwsDynamoDB, Log, TEXT("[%s] " format), *FString(__FUNCTION__), ##__VA_ARGS__)
#define LogVerbose(format, ...) UE_LOG(LogAwsDynamoDB, Verbose, TEXT("[%s] " format), *FString(__FUNCTION__), ##__VA_ARGS__)
#define LogVeryVerbose(format, ...) UE_LOG(LogAwsDynamoDB, VeryVerbose, TEXT("[%s] " format), *FString(__FUNCTION__), ##__VA_ARGS__)
#define LogError(format, ...) UE_LOG(LogAwsDynamoDB, Error, TEXT("[%s] " format), *FString(__FUNCTION__), ##__VA_ARGS__)
#define LogWarning(format, ...) UE_LOG(LogAwsDynamoDB, Warning, TEXT("[%s] " format), *FString(__FUNCTION__), ##__VA_ARGS__)