// GainX, All Rights Reserved

#include "GainXGameplayTags.h"
#include "Engine/EngineTypes.h"
#include "GameplayTagsManager.h"

namespace GainXGameplayTags
{
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(InputTag_Move, "InputTag.Move", "Move input.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(InputTag_Look_Mouse, "InputTag.Look.Mouse", "Look (mouse) input.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(InputTag_Look_Stick, "InputTag.Look.Stick", "Look (stick) input.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(InputTag_Jump, "InputTag.Jump", "Jump input.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(InputTag_Weapon_Fire, "InputTag.Weapon.Fire", "Weapon fire input.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(InputTag_Weapon_Switch_Next, "InputTag.Weapon.Switch.Next", "Next weapon input.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(InputTag_Weapon_Switch_Previous, "InputTag.Weapon.Switch.Previous", "Previous switch input.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(InputTag_Weapon_Reload, "InputTag.Weapon.Reload", "Weapon reload input.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(InputTag_Weapon_ADS, "InputTag.Weapon.ADS", "Weapon ads input.");

	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Ability_ADS, "Ability.ADS", "ability ADS.");

	UE_DEFINE_GAMEPLAY_TAG_COMMENT(GameplayCue_Weapon_Rifle_Fire, "GameplayCue.Weapon.Rifle.Fire", "GameplayCue Weapon Rifle Fire");
};