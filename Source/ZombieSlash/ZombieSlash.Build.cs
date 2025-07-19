// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class ZombieSlash : ModuleRules
{
	public ZombieSlash(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
        PublicIncludePaths.AddRange(new string[] { "ZombieSlash" });
        PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput", "UMG" });
	}
}
