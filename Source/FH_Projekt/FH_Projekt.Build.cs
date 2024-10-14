// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class FH_Projekt : ModuleRules
{
	public FH_Projekt(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] {   "Core",
            "CoreUObject",
            "Engine",
            "InputCore",
            "CoreUObject",
            "EnhancedInput",
            "Niagara",
            "AIModule",
            "NavigationSystem",
            "GameplayTasks",
            "UMG", });
	}
}
