// GainX, All Rights Reserved

using UnrealBuildTool;
using System.Collections.Generic;

public class GainXEditorTarget : TargetRules
{
	public GainXEditorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		DefaultBuildSettings = BuildSettingsVersion.V4;

		ExtraModuleNames.AddRange( new string[] { "GainX" } );
	}
}
