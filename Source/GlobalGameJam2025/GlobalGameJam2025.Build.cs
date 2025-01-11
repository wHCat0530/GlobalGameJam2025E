// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class GlobalGameJam2025 : ModuleRules
{
	public GlobalGameJam2025(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "NavigationSystem", "AIModule", "Niagara", "EnhancedInput" });
    }
}
