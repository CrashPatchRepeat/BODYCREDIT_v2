// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class DebugPlugIn : ModuleRules
{
	public DebugPlugIn(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
		
		PrivateIncludePaths.Add(ModuleDirectory);
		
		PublicDependencyModuleNames.Add("Core");
		
		PrivateDependencyModuleNames.Add("CoreUObject");
		PrivateDependencyModuleNames.Add("Engine");
		PrivateDependencyModuleNames.Add("Slate");
		PrivateDependencyModuleNames.Add("SlateCore");
		PrivateDependencyModuleNames.Add("BODYCREDIT_v2");
		PrivateDependencyModuleNames.Add("GameplayDebugger");
	}
}
