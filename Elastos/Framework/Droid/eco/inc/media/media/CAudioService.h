
#ifndef __CAUDIOSERVICE_H__
#define __CAUDIOSERVICE_H__

#include "_CAudioService.h"
#include "media/AudioService.h"

CarClass(CAudioService), public AudioService
{
public:
    CARAPI AdjustVolume(
        /* [in] */ Int32 direction,
        /* [in] */ Int32 flags);

    CARAPI AdjustSuggestedStreamVolume(
        /* [in] */ Int32 direction,
        /* [in] */ Int32 suggestedStreamType,
        /* [in] */ Int32 flags);

    CARAPI AdjustStreamVolume(
        /* [in] */ Int32 streamType,
        /* [in] */ Int32 direction,
        /* [in] */ Int32 flags);

    CARAPI SetStreamVolume(
        /* [in] */ Int32 streamType,
        /* [in] */ Int32 index,
        /* [in] */ Int32 flags);

    CARAPI SetStreamSolo(
        /* [in] */ Int32 streamType,
        /* [in] */ Boolean state,
        /* [in] */ IBinder * pCb);

    CARAPI SetStreamMute(
        /* [in] */ Int32 streamType,
        /* [in] */ Boolean state,
        /* [in] */ IBinder * pCb);

    CARAPI GetStreamVolume(
        /* [in] */ Int32 streamType,
        /* [out] */ Int32 * pVolume);

    CARAPI GetStreamMaxVolume(
        /* [in] */ Int32 streamType,
        /* [out] */ Int32 * pVolume);

    CARAPI SetRingerMode(
        /* [in] */ Int32 ringerMode);

    CARAPI GetRingerMode(
        /* [out] */ Int32 * pMode);

    CARAPI SetVibrateSetting(
        /* [in] */ Int32 vibrateType,
        /* [in] */ Int32 vibrateSetting);

    CARAPI GetVibrateSetting(
        /* [in] */ Int32 vibrateType,
        /* [out] */ Int32 * pVibrateSetting);

    CARAPI ShouldVibrate(
        /* [in] */ Int32 vibrateType,
        /* [out] */ Boolean * pVibrate);

    CARAPI SetMode(
        /* [in] */ Int32 mode,
        /* [in] */ IBinder * pCb);

    CARAPI GetMode(
        /* [out] */ Int32 * pMode);

    CARAPI PlaySoundEffect(
        /* [in] */ Int32 effectType);

    CARAPI PlaySoundEffectVolume(
        /* [in] */ Int32 effectType,
        /* [in] */ Float volume);

    CARAPI LoadSoundEffects(
        /* [out] */ Boolean * pLoad);

    CARAPI UnloadSoundEffects();

    CARAPI ReloadAudioSettings();

    CARAPI SetSpeakerphoneOn(
        /* [in] */ Boolean on);

    CARAPI IsSpeakerphoneOn(
        /* [out] */ Boolean * pSpeakerphoneOn);

    CARAPI SetBluetoothScoOn(
        /* [in] */ Boolean on);

    CARAPI IsBluetoothScoOn(
        /* [out] */ Boolean * pOn);

    CARAPI RequestAudioFocus(
        /* [in] */ Int32 mainStreamType,
        /* [in] */ Int32 durationHint,
        /* [in] */ IBinder * pCb,
        /* [in] */ IAudioFocusDispatcher * pL,
        /* [in] */ const String& clientId,
        /* [out] */ Int32 * pFocus);

    CARAPI AbandonAudioFocus(
        /* [in] */ IAudioFocusDispatcher * pL,
        /* [in] */ const String& clientId,
        /* [out] */ Int32 * pFocus);

    CARAPI UnregisterAudioFocusClient(
        /* [in] */ const String& clientId);

    CARAPI RegisterMediaButtonEventReceiver(
        /* [in] */ IComponentName * pEventReceiver);

    CARAPI UnregisterMediaButtonEventReceiver(
        /* [in] */ IComponentName * pEventReceiver);

    CARAPI StartBluetoothSco(
        /* [in] */ IBinder * pCb);

    CARAPI StopBluetoothSco(
        /* [in] */ IBinder * pCb);

    CARAPI IsStreamAffectedByRingerMode(
        /* [in] */ Int32 streamType,
        /* [out] */ Boolean * pMode);

    CARAPI IsStreamAffectedByMute(
        /* [in] */ Int32 streamType,
        /* [out] */ Boolean * pMute);

    CARAPI SetVolumeIndexSettingName(
        /* [in] */ const String& settingName);

    CARAPI AdjustIndex(
        /* [in] */ Int32 deltaIndex);

    CARAPI SetIndex(
        /* [in] */ Int32 index,
        /* [in] */ Boolean lastAudible);

    CARAPI SetLastAudibleIndex(
        /* [in] */ Int32 index);

    CARAPI AdjustLastAudibleIndex(
        /* [in] */ Int32 deltaIndex);

    CARAPI GetMaxIndex(
        /* [out] */ Int32 * pIndex);

    CARAPI Mute(
        /* [in] */ IBinder * pCb,
        /* [in] */ Boolean state);

    CARAPI constructor(
        /* [in] */ IContext * pContext);

private:
    // TODO: Add your private member variables here.
};

#endif // __CAUDIOSERVICE_H__
