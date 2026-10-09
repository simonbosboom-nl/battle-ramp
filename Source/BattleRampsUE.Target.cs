using UnrealBuildTool;
using System.Collections.Generic;

public class BattleRampsUETarget : TargetRules
{
    public BattleRampsUETarget(TargetInfo Target) : base(Target)
    {
        Type = TargetType.Game;
        DefaultBuildSettings = BuildSettingsVersion.V5;
        IncludeOrderVersion = EngineIncludeOrderVersion.Unreal5_8;
        ExtraModuleNames.Add("BattleRampsUE");
    }
}
