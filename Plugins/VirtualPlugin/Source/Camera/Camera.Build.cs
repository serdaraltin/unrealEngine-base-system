// Copyright 2020 ByOwls. All Rights Reserved.

using UnrealBuildTool;
using System.IO;

public class Camera : ModuleRules
{
	public Camera(ReadOnlyTargetRules Target) : base(Target)
	{
        PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
        PrecompileForTargets = PrecompileTargetsType.Editor;

        PublicDefinitions.Add("WITH_MOBILEUTILS=1");

        //PrivateIncludePaths.Add("Camera/Private");

        PublicIncludePaths.AddRange(
            new string[] {
				// ... add public include paths required here ...
			}
            );

        PrivateIncludePaths.AddRange(
            new string[] {
                "Camera/Private"
            }
            );

        PublicDependencyModuleNames.AddRange(
            new string[]
            {
                "Core",
                "UMG",
                "Engine",
                "Json",
                "JsonUtilities"
               
				// ... add other public dependencies that you statically link with here ...
			}
            );
        PrivateDependencyModuleNames.AddRange(
                new string[]
                {
                "Projects",
                "CoreUObject",
                "Sockets",
                "Networking",
                "CoreUObject",
                "Engine",
                "OnlineSubsystem",
                "CinematicCamera",
                "RenderCore",
                "InputCore",
                "RHI",
                "ImageWrapper",
                "SlateRHIRenderer",
                "LevelSequence"
                }
            );

        if (Target.Type == TargetRules.TargetType.Editor)
        {
            PrivateDependencyModuleNames.AddRange(
            new string[]
            {
                 "Projects",
                "InputCore",
                "Slate",
                "SlateCore",
                "Settings",
                "MovieScene",
                "MovieSceneCaptureDialog",
                "MovieSceneCapture"
				// ... add private dependencies that you statically link with here ...	
			}

            );
        }

        DynamicallyLoadedModuleNames.AddRange(
            new string[]
            {
				// ... add any modules that your module loads dynamically here ...
			}
            );

        if (Target.Platform == UnrealTargetPlatform.Android)
        {
            PrivateIncludePaths.Add("Camera/Private/Android");

            PrivateDependencyModuleNames.AddRange(new string[] { "Launch" });
            string PluginPath = Utils.MakePathRelativeTo(ModuleDirectory, Target.RelativeEnginePath);
            AdditionalPropertiesForReceipt.Add("AndroidPlugin", Path.Combine(PluginPath, "Camera_UPL_Android.xml"));
        }
    }
}
