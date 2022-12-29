/**
* Copyright (C) 2017-2022 eelDev AB
*/

using System.IO;
using UnrealBuildTool;

public class AwsDynamoDBLibrary : ModuleRules
{
    public string SDKBaseDir
    {
        get { return ModuleDirectory + "/" + Name; }
    }

       public virtual string SDKBinariesDir
    {
        get
        {
            if (Target.Platform == UnrealTargetPlatform.IOS)
            {
                return Path.Combine(SDKBaseDir, "Bin", "IOS");
            }
            else if (Target.Platform == UnrealTargetPlatform.Android)
            {
                return Path.Combine(SDKBaseDir, "Bin", "Android");
            }
            else if (Target.Platform == UnrealTargetPlatform.Linux)
            {
                return Path.Combine(SDKBaseDir, "Bin", "Linux");
            }
            else if (Target.Platform == UnrealTargetPlatform.Mac)
            {
                return Path.Combine(SDKBaseDir, "Bin", "Mac");
            }
            else
            {
                return Path.Combine(SDKBaseDir, "Bin", "Win64");
            }
        }
    }
    public virtual string SDKLibsDir
    {
        get
        {
            if (Target.Platform == UnrealTargetPlatform.IOS)
            {
                return Path.Combine(SDKBaseDir, "Lib", "IOS");
            }
            else if (Target.Platform == UnrealTargetPlatform.Android)
            {
                return Path.Combine(SDKBaseDir, "Lib", "Android");
            }
            else if (Target.Platform == UnrealTargetPlatform.Linux)
            {
                return Path.Combine(SDKBaseDir, "Lib", "Linux");
            }
            else if (Target.Platform == UnrealTargetPlatform.Mac)
            {
                return Path.Combine(SDKBaseDir, "Lib", "Mac");
            }
            else
            {
                return Path.Combine(SDKBaseDir, "Lib", "Win64");
            }
        }
    }
	
    public virtual string SDKIncludesDir
    {
        get
        {
            return Path.Combine(SDKBaseDir, "src");
        }
    }
    public AwsDynamoDBLibrary(ReadOnlyTargetRules Target) : base(Target)
    {
        Type = ModuleType.External;

        bool bDisablePlugin = false;

        if (Target.Version.MinorVersion < 25 && Target.Version.MajorVersion != 5)
        {
            if (Target.Platform == UnrealTargetPlatform.Android)
            {
                bDisablePlugin = true;
            }

            if (Target.Platform == UnrealTargetPlatform.IOS)
            {
                bDisablePlugin = true;
            }
        }

        if (bDisablePlugin)
        {
            PublicDefinitions.Add("DISABLE_DYNAMODB=1");
        }
        else
        {
            PublicDefinitions.Add("DISABLE_DYNAMODB=0");
        }

        if (!bDisablePlugin)
        {
            PublicDefinitions.Add("USE_IMPORT_EXPORT");
            PublicIncludePaths.Add(SDKIncludesDir);
            string Version = (Target.Version.MajorVersion == 5 ? "5" : "4") + Target.Version.MinorVersion.ToString();

            if (Target.Platform == UnrealTargetPlatform.Win64)
            {
                PublicAdditionalLibraries.Add(Path.Combine(SDKLibsDir, "aws-c-common.lib"));
                PublicAdditionalLibraries.Add(Path.Combine(SDKLibsDir, "aws-c-event-stream.lib"));
                PublicAdditionalLibraries.Add(Path.Combine(SDKLibsDir, "aws-checksums.lib"));
                PublicAdditionalLibraries.Add(Path.Combine(SDKLibsDir, "aws-cpp-sdk-core.lib"));
                PublicAdditionalLibraries.Add(Path.Combine(SDKLibsDir, "aws-cpp-sdk-dynamodb.lib"));
                PublicAdditionalLibraries.Add(Path.Combine(SDKLibsDir, "aws-cpp-sdk-dynamodbstreams.lib"));

                PublicDelayLoadDLLs.Add("aws-c-common.dll");
                PublicDelayLoadDLLs.Add("aws-c-event-stream.dll");
                PublicDelayLoadDLLs.Add("aws-checksums.dll");
                PublicDelayLoadDLLs.Add("aws-cpp-sdk-core.dll");
                PublicDelayLoadDLLs.Add("aws-cpp-sdk-dynamodb.dll");
                PublicDelayLoadDLLs.Add("aws-cpp-sdk-dynamodbstreams.dll");

                RuntimeDependencies.Add(Path.Combine(SDKBinariesDir, "aws-c-common.dll"));
                RuntimeDependencies.Add(Path.Combine(SDKBinariesDir, "aws-c-event-stream.dll"));
                RuntimeDependencies.Add(Path.Combine(SDKBinariesDir, "aws-checksums.dll"));
                RuntimeDependencies.Add(Path.Combine(SDKBinariesDir, "aws-cpp-sdk-core.dll"));
                RuntimeDependencies.Add(Path.Combine(SDKBinariesDir, "aws-cpp-sdk-dynamodb.dll"));
                RuntimeDependencies.Add(Path.Combine(SDKBinariesDir, "aws-cpp-sdk-dynamodbstreams.dll"));
            }
            else if (Target.Platform == UnrealTargetPlatform.Linux)
            {
                PublicAdditionalLibraries.Add(Path.Combine(SDKLibsDir, "libaws-c-common.so"));
                PublicAdditionalLibraries.Add(Path.Combine(SDKLibsDir, "libaws-c-event-stream.so"));
                PublicAdditionalLibraries.Add(Path.Combine(SDKLibsDir, "libaws-checksums.so"));
                PublicAdditionalLibraries.Add(Path.Combine(SDKLibsDir, "libaws-cpp-sdk-core.so"));
                PublicAdditionalLibraries.Add(Path.Combine(SDKLibsDir, "libaws-cpp-sdk-dynamodb.so"));
                PublicAdditionalLibraries.Add(Path.Combine(SDKLibsDir, "libaws-cpp-sdk-dynamodbstreams.so"));

                RuntimeDependencies.Add(Path.Combine(SDKLibsDir, "libaws-c-common.so"));
                RuntimeDependencies.Add(Path.Combine(SDKLibsDir, "libaws-c-common.so.0unstable"));
                RuntimeDependencies.Add(Path.Combine(SDKLibsDir, "libaws-c-common.so.1.0.0"));
                RuntimeDependencies.Add(Path.Combine(SDKLibsDir, "libaws-c-event-stream.so"));
                RuntimeDependencies.Add(Path.Combine(SDKLibsDir, "libaws-c-event-stream.so.0unstable"));
                RuntimeDependencies.Add(Path.Combine(SDKLibsDir, "libaws-c-event-stream.so.1.0.0"));
                RuntimeDependencies.Add(Path.Combine(SDKLibsDir, "libaws-checksums.so"));
                RuntimeDependencies.Add(Path.Combine(SDKLibsDir, "libaws-cpp-sdk-core.so"));
                RuntimeDependencies.Add(Path.Combine(SDKLibsDir, "libaws-cpp-sdk-dynamodb.so"));
                RuntimeDependencies.Add(Path.Combine(SDKLibsDir, "libaws-cpp-sdk-dynamodbstreams.so"));

                RuntimeDependencies.Add(ModuleDirectory + "/cacert.pem");
            }
            else if (Target.Platform == UnrealTargetPlatform.Mac)
            {
                PublicAdditionalLibraries.Add(Path.Combine(SDKLibsDir, "libaws-c-common.dylib"));
                PublicAdditionalLibraries.Add(Path.Combine(SDKLibsDir, "libaws-c-event-stream.dylib"));
                PublicAdditionalLibraries.Add(Path.Combine(SDKLibsDir, "libaws-checksums.dylib"));
                PublicAdditionalLibraries.Add(Path.Combine(SDKLibsDir, "libaws-cpp-sdk-core.dylib"));
                PublicAdditionalLibraries.Add(Path.Combine(SDKLibsDir, "libaws-cpp-sdk-dynamodb.dylib"));
                PublicAdditionalLibraries.Add(Path.Combine(SDKLibsDir, "libaws-cpp-sdk-dynamodbstreams.dylib"));

                RuntimeDependencies.Add(Path.Combine(SDKLibsDir, "libaws-c-common.dylib"));
                RuntimeDependencies.Add(Path.Combine(SDKLibsDir, "libaws-c-common.0unstable.dylib"));
                RuntimeDependencies.Add(Path.Combine(SDKLibsDir, "libaws-c-event-stream.0unstable.dylib"));
                RuntimeDependencies.Add(Path.Combine(SDKLibsDir, "libaws-c-event-stream.dylib"));
                RuntimeDependencies.Add(Path.Combine(SDKLibsDir, "libaws-checksums.dylib"));
                RuntimeDependencies.Add(Path.Combine(SDKLibsDir, "libaws-cpp-sdk-core.dylib"));
                RuntimeDependencies.Add(Path.Combine(SDKLibsDir, "libaws-cpp-sdk-dynamodb.dylib"));
                RuntimeDependencies.Add(Path.Combine(SDKLibsDir, "libaws-cpp-sdk-dynamodbstreams.dylib"));

                RuntimeDependencies.Add(ModuleDirectory + "/cacert.pem");
            }
            else if (Target.Platform == UnrealTargetPlatform.IOS)
            {
                PublicFrameworks.Add("Foundation");
                PublicFrameworks.Add("Security");

                PrivateDefinitions.Add("FORCE_ANSI_ALLOCATOR=1");

                PublicAdditionalLibraries.Add(Path.Combine(SDKLibsDir, Version, "libcurl.a"));
                PublicAdditionalLibraries.Add(Path.Combine(SDKLibsDir, Version, "libaws-c-common.a"));
                PublicAdditionalLibraries.Add(Path.Combine(SDKLibsDir, Version, "libaws-c-event-stream.a"));
                PublicAdditionalLibraries.Add(Path.Combine(SDKLibsDir, Version, "libaws-checksums.a"));
                PublicAdditionalLibraries.Add(Path.Combine(SDKLibsDir, Version, "libaws-cpp-sdk-core.a"));
                PublicAdditionalLibraries.Add(Path.Combine(SDKLibsDir, Version, "libaws-cpp-sdk-dynamodb.a"));
                PublicAdditionalLibraries.Add(Path.Combine(SDKLibsDir, Version, "libaws-cpp-sdk-dynamodbstreams.a"));

                RuntimeDependencies.Add(ModuleDirectory + "/cacert.pem");
            }
            else if (Target.Platform == UnrealTargetPlatform.Android)
            {
                PublicDefinitions.Add("_GLIBCXX_FULLY_DYNAMIC_STRING");
                
                PublicAdditionalLibraries.Add(Path.Combine(SDKLibsDir, Version, "libaws-c-common.a"));
                PublicAdditionalLibraries.Add(Path.Combine(SDKLibsDir, Version, "libaws-c-event-stream.a"));
                PublicAdditionalLibraries.Add(Path.Combine(SDKLibsDir, Version, "libaws-checksums.a"));
                PublicAdditionalLibraries.Add(Path.Combine(SDKLibsDir, Version, "libaws-cpp-sdk-core.a"));
                PublicAdditionalLibraries.Add(Path.Combine(SDKLibsDir, Version, "libaws-cpp-sdk-dynamodb.a"));
                PublicAdditionalLibraries.Add(Path.Combine(SDKLibsDir, Version, "libaws-cpp-sdk-dynamodbstreams.a"));

                AdditionalPropertiesForReceipt.Add("AndroidPlugin", Path.Combine(ModuleDirectory, "LibAwsDynamoDB_APL.xml"));

                RuntimeDependencies.Add(ModuleDirectory + "/cacert.pem");

                PublicIncludePathModuleNames.AddRange(new string[] { "Launch" });
            }
        }
    }
}
