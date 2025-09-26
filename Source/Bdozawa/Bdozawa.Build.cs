// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Bdozawa : ModuleRules
{
	public Bdozawa(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] {
			"Core",
			"CoreUObject",
			"Engine",
			"InputCore",
			"EnhancedInput",
			"AIModule",
			"StateTreeModule",
			"GameplayStateTreeModule",
			"UMG",
			"Slate"
		});

		PrivateDependencyModuleNames.AddRange(new string[] { });

		PublicIncludePaths.AddRange(new string[] {
			"Bdozawa",
			"Bdozawa/Variant_Platforming",
			"Bdozawa/Variant_Platforming/Animation",
			"Bdozawa/Variant_Combat",
			"Bdozawa/Variant_Combat/AI",
			"Bdozawa/Variant_Combat/Animation",
			"Bdozawa/Variant_Combat/Gameplay",
			"Bdozawa/Variant_Combat/Interfaces",
			"Bdozawa/Variant_Combat/UI",
			"Bdozawa/Variant_SideScrolling",
			"Bdozawa/Variant_SideScrolling/AI",
			"Bdozawa/Variant_SideScrolling/Gameplay",
			"Bdozawa/Variant_SideScrolling/Interfaces",
			"Bdozawa/Variant_SideScrolling/UI"
		});

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
