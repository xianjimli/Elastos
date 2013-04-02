
#ifndef __CAUDIOMANAGER_H__
#define __CAUDIOMANAGER_H__

#include "_CAudioManager.h"
#include "media/AudioManager.h"

CarClass(CAudioManager), public AudioManager
{
public:
    CARAPI AdjustStreamVolume(
        /* [in] */ Int32 streamType,
        /* [in] */ Int32 direction,
        /* [in] */ Int32 flags);

    CARAPI AdjustVolume(
        /* [in] */ Int32 direction,
        /* [in] */ Int32 flags);

    CARAPI AdjustSuggestedStreamVolume(
        /* [in] */ Int32 direction,
        /* [in] */ Int32 suggestedStreamType,
        /* [in] */ Int32 flags);

    CARAPI GetRingerMode(
        /* [out] */ Int32 * pMode);

    CARAPI GetStreamMaxVolume(
        /* [in] */ Int32 streamType,
        /* [out] */ Int32 * pVol);

    CARAPI GetStreamVolume(
        /* [in] */ Int32 streamType,
        /* [out] */ Int32 * pVol);

    CARAPI SetRingerMode(
        /* [in] */ Int32 ringerMode);

    CARAPI SetStreamVolume(
        /* [in] */ Int32 streamType,
        /* [in] */ Int32 index,
        /* [in] */ Int32 flags);

    CARAPI SetStreamSolo(
        /* [in] */ Int32 streamType,
        /* [in] */ Boolean state);

    CARAPI SetStreamMute(
        /* [in] */ Int32 streamType,
        /* [in] */ Boolean state);

    CARAPI ShouldVibrate(
        /* [in] */ Int32 vibrateType,
        /* [out] */ Boolean * pVibrate);

    CARAPI GetVibrateSetting(
        /* [in] */ Int32 vibrateType,
        /* [out] */ Int32 * pSetting);

    CARAPI SetVibrateSetting(
        /* [in] */ Int32 vibrateType,
        /* [in] */ Int32 vibrateSetting);

    CARAPI SetSpeakerphoneOn(
        /* [in] */ Boolean on);

    CARAPI IsSpeakerphoneOn(
        /* [out] */ Boolean * pOn);

    CARAPI IsBluetoothScoAvailableOffCall(
        /* [out] */ Boolean * pOffCall);

    CARAPI StartBluetoothSco();

    CARAPI StopBluetoothSco();

    CARAPI SetBluetoothScoOn(
        /* [in] */ Boolean on);

    CARAPI IsBluetoothScoOn(
        /* [out] */ Boolean * pOn);

    CARAPI SetBluetoothA2dpOn(
        /* [in] */ Boolean on);

    CARAPI IsBluetoothA2dpOn(
        /* [out] */ Boolean * pOn);

    CARAPI SetWiredHeadsetOn(
        /* [in] */ Boolean on);

    CARAPI IsWiredHeadsetOn(
        /* [out] */ Boolean * pOn);

    CARAPI SetMicrophoneMute(
        /* [in] */ Boolean on);

    CARAPI IsMicrophoneMute(
        /* [out] */ Boolean * pMute);

    CARAPI SetMode(
        /* [in] */ Int32 mode);

    CARAPI GetMode(
        /* [out] */ Int32 * pMode);

    CARAPI SetRouting(
        /* [in] */ Int32 mode,
        /* [in] */ Int32 routes,
        /* [in] */ Int32 mask);

    CARAPI GetRouting(
        /* [in] */ Int32 mode,
        /* [out] */ Int32 * pRoute);

    CARAPI IsMusicActive(
        /* [out] */ Boolean * pActive);

    CARAPI SetParameter(
        /* [in] */ const String& key,
        /* [in] */ const String& value);

    CARAPI SetParameters(
        /* [in] */ const String& keyValuePairs);

    CARAPI GetParameters(
        /* [in] */ const String& keys,
        /* [out] */ String * pParameters);

    CARAPI PlaySoundEffect(
        /* [in] */ Int32 effectType);

    CARAPI PlaySoundEffectEx(
        /* [in] */ Int32 effectType,
        /* [in] */ Float volume);

    CARAPI LoadSoundEffects();

    CARAPI UnloadSoundEffects();

    CARAPI RegisterAudioFocusListener(
        /* [in] */ IOnAudioFocusChangeListener * pL);

    CARAPI UnregisterAudioFocusListener(
        /* [in] */ IOnAudioFocusChangeListener * pL);

    CARAPI RequestAudioFocus(
        /* [in] */ IOnAudioFocusChangeListener * pL,
        /* [in] */ Int32 streamType,
        /* [in] */ Int32 durationHint,
        /* [out] */ Int32* result);

    CARAPI AbandonAudioFocus(
        /* [in] */ IOnAudioFocusChangeListener * pL,
        /* [out] */ Int32 * pFocus);

    CARAPI RegisterMediaButtonEventReceiver(
        /* [in] */ IComponentName * pEventReceiver);

    CARAPI UnregisterMediaButtonEventReceiver(
        /* [in] */ IComponentName * pEventReceiver);

    CARAPI ReloadAudioSettings();

    CARAPI constructor(
        /* [in] */ IContext * pContext);

private:
    // TODO: Add your private member variables here.
};

#endif // __CAUDIOMANAGER_H__
