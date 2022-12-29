/**
* Copyright (C) 2017-2022 eelDev AB
*/

using System.IO;
using UnrealBuildTool;

public class AwsDynamoDB : ModuleRules
{
    public AwsDynamoDB(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
        PrivatePCHHeaderFile = "Private/AwsDynamoDBPrivatePCH.h";

        bEnableExceptions = true;
        PublicDependencyModuleNames.AddRange(new string[] { "Engine", "Core", "CoreUObject", "InputCore", "Projects", "AwsDynamoDBLibrary" });
    }
}
