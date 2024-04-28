// GainX, All Rights Reserved

using UnrealBuildTool;
using UnrealBuildTool.Rules;

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
            "NavigationSystem",
            "GameplayTags",
            "GameplayTasks",
            "GameplayAbilities",
        });

        PrivateDependencyModuleNames.AddRange(new string[] 
        { 
            "Slate", 
            "SlateCore" 
        });

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
            "GainX/Public/Sound",
            "GainX/Public/Input",
            "GainX/Public/System",
            "GainX/Public/Animation",
            "GainX/Public/AbilitySystem",
            "GainX/Public/AbilitySystem/Attributes",
            "GainX/Public/Inventory",
            "GainX/Public/Equipment"
        });
    }
}
