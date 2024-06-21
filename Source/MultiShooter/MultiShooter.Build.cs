// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class MultiShooter : ModuleRules
{
	public MultiShooter(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
		bEnableExceptions = true;
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "Niagara", "UMG", "EnhancedInput", "Boost", "MultiplayerSessions", "OnlineSubsystem", "OnlineSubsystemSteam" });
		PublicIncludePaths.AddRange(new string[] { "MultiShooter" });

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });
		
		// Uncomment if you are using online features
		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
