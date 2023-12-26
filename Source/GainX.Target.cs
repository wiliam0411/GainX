// GainX, All Rights Reserved

using UnrealBuildTool;
using System.Collections.Generic;

public class GainXTarget : TargetRules
{
	public GainXTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		DefaultBuildSettings = BuildSettingsVersion.V4;

		ExtraModuleNames.AddRange( new string[] { "GainX" } );
	}
}
