// GainX, All Rights Reserved

#include "GainXGameInstance.h"
#include "Sound/GainXSoundFunctionLibrary.h"

void UGainXGameInstance::ToggleVolume()
{
	UGainXSoundFunctionLibrary::ToggleSoundClassVolume(MasterSoundClass);
}
