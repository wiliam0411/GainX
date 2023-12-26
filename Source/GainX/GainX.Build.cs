// GainX, All Rights Reserved

using UnrealBuildTool;

public class GainX : ModuleRules
{
	public GainX(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] 
		{ 
			"Core", 
			"CoreUObject", 
			"Engine", 
			"InputCore", 
			"Niagara",
			"PhysicsCore",
			"GameplayTasks",
			"NavigationSystem"
		});

		PrivateDependencyModuleNames.AddRange(new string[] {  });

		PublicIncludePaths.AddRange(new string[] 
		{ 
			"GainX/Public/Player", 
			"GainX/Public/Components" , 
			"GainX/Public/Dev",
            "GainX/Public/Weapon",
            "GainX/Public/UI",
            "GainX/Public/Animations",
            "GainX/Public/Pickups",
            "GainX/Public/Weapon/Components",
            "GainX/Public/AI",
            "GainX/Public/AI/Tasks",
            "GainX/Public/AI/Services",
            "GainX/Public/AI/Decorators",
            "GainX/Public/AI/EQS",
            "GainX/Public/Menu",
            "GainX/Public/Menu/UI",
            "GainX/Public/Sound"
        });

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });
		
		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
