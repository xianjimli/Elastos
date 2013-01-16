
#include "media/CAudioService.h"

ECode CAudioService::AdjustVolume(
    /* [in] */ Int32 direction,
    /* [in] */ Int32 flags)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CAudioService::AdjustSuggestedStreamVolume(
    /* [in] */ Int32 direction,
    /* [in] */ Int32 suggestedStreamType,
    /* [in] */ Int32 flags)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CAudioService::AdjustStreamVolume(
    /* [in] */ Int32 streamType,
    /* [in] */ Int32 direction,
    /* [in] */ Int32 flags)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CAudioService::SetStreamVolume(
    /* [in] */ Int32 streamType,
    /* [in] */ Int32 index,
    /* [in] */ Int32 flags)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CAudioService::SetStreamSolo(
    /* [in] */ Int32 streamType,
    /* [in] */ Boolean state,
    /* [in] */ IBinder * pCb)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CAudioService::SetStreamMute(
    /* [in] */ Int32 streamType,
    /* [in] */ Boolean state,
    /* [in] */ IBinder * pCb)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CAudioService::GetStreamVolume(
    /* [in] */ Int32 streamType,
    /* [out] */ Int32 * pVolume)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CAudioService::GetStreamMaxVolume(
    /* [in] */ Int32 streamType,
    /* [out] */ Int32 * pVolume)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CAudioService::SetRingerMode(
    /* [in] */ Int32 ringerMode)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CAudioService::GetRingerMode(
    /* [out] */ Int32 * pMode)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CAudioService::SetVibrateSetting(
    /* [in] */ Int32 vibrateType,
    /* [in] */ Int32 vibrateSetting)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CAudioService::GetVibrateSetting(
    /* [in] */ Int32 vibrateType,
    /* [out] */ Int32 * pVibrateSetting)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CAudioService::ShouldVibrate(
    /* [in] */ Int32 vibrateType,
    /* [out] */ Boolean * pVibrate)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CAudioService::SetMode(
    /* [in] */ Int32 mode,
    /* [in] */ IBinder * pCb)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CAudioService::GetMode(
    /* [out] */ Int32 * pMode)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CAudioService::PlaySoundEffect(
    /* [in] */ Int32 effectType)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CAudioService::PlaySoundEffectVolume(
    /* [in] */ Int32 effectType,
    /* [in] */ Float volume)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CAudioService::LoadSoundEffects(
    /* [out] */ Boolean * pLoad)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CAudioService::UnloadSoundEffects()
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CAudioService::ReloadAudioSettings()
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CAudioService::SetSpeakerphoneOn(
    /* [in] */ Boolean on)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CAudioService::IsSpeakerphoneOn(
    /* [out] */ Boolean * pSpeakerphoneOn)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CAudioService::SetBluetoothScoOn(
    /* [in] */ Boolean on)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CAudioService::IsBluetoothScoOn(
    /* [out] */ Boolean * pOn)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CAudioService::RequestAudioFocus(
    /* [in] */ Int32 mainStreamType,
    /* [in] */ Int32 durationHint,
    /* [in] */ IBinder * pCb,
    /* [in] */ IAudioFocusDispatcher * pL,
    /* [in] */ const String& clientId,
    /* [out] */ Int32 * pFocus)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CAudioService::AbandonAudioFocus(
    /* [in] */ IAudioFocusDispatcher * pL,
    /* [in] */ const String& clientId,
    /* [out] */ Int32 * pFocus)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CAudioService::UnregisterAudioFocusClient(
    /* [in] */ const String& clientId)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CAudioService::RegisterMediaButtonEventReceiver(
    /* [in] */ IComponentName * pEventReceiver)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CAudioService::UnregisterMediaButtonEventReceiver(
    /* [in] */ IComponentName * pEventReceiver)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CAudioService::StartBluetoothSco(
    /* [in] */ IBinder * pCb)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CAudioService::StopBluetoothSco(
    /* [in] */ IBinder * pCb)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CAudioService::IsStreamAffectedByRingerMode(
    /* [in] */ Int32 streamType,
    /* [out] */ Boolean * pMode)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CAudioService::IsStreamAffectedByMute(
    /* [in] */ Int32 streamType,
    /* [out] */ Boolean * pMute)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CAudioService::SetVolumeIndexSettingName(
    /* [in] */ const String& settingName)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CAudioService::AdjustIndex(
    /* [in] */ Int32 deltaIndex)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CAudioService::SetIndex(
    /* [in] */ Int32 index,
    /* [in] */ Boolean lastAudible)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CAudioService::SetLastAudibleIndex(
    /* [in] */ Int32 index)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CAudioService::AdjustLastAudibleIndex(
    /* [in] */ Int32 deltaIndex)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CAudioService::GetMaxIndex(
    /* [out] */ Int32 * pIndex)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CAudioService::Mute(
    /* [in] */ IBinder * pCb,
    /* [in] */ Boolean state)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CAudioService::constructor(
    /* [in] */ IContext * pContext)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

