using UnrealBuildTool;

public class BattleRampsUE : ModuleRules
{
    public BattleRampsUE(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
        PublicDependencyModuleNames.AddRange(new string[] {
            "Core", "CoreUObject", "Engine", "InputCore", "PhysicsCore"
        });
    }
}
