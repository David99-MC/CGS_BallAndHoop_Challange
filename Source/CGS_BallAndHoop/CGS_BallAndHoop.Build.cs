// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class CGS_BallAndHoop : ModuleRules
{
	public CGS_BallAndHoop(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay", "EnhancedInput" });
	}
}
