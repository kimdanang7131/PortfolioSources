// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class UE4_Portfolio : ModuleRules
{
	public UE4_Portfolio(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[]
		{
			"Core",
			"CoreUObject",
			"Engine",
			"InputCore",
			"GameplayTasks",
			"AIModule"
			,"UMG"
			,"Niagara"
			,"GameplayCameras"
		});

		PublicIncludePaths.Add(ModuleDirectory);

	}
}
