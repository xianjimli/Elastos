#ifndef __CAUDIOEFFECT_H__
#define __CAUDIOEFFECT_H__

#include "_CAudioEffect.h"
#include "ext/frameworkext.h"
#include "os/Runnable.h"
#include <elastos/List.h>
#include <elastos/AutoPtr.h>
#include <media/audiofx/AudioEffect.h>

/**
 * AudioEffect is the base class for controlling audio effects provided by the android audio
 * framework.
 * <p>Applications should not use the AudioEffect class directly but one of its derived classes to
 * control specific effects:
 * <ul>
 *   <li> {@link android.media.audiofx.Equalizer}</li>
 *   <li> {@link android.media.audiofx.Virtualizer}</li>
 *   <li> {@link android.media.audiofx.BassBoost}</li>
 *   <li> {@link android.media.audiofx.PresetReverb}</li>
 *   <li> {@link android.media.audiofx.EnvironmentalReverb}</li>
 * </ul>
 * <p>If the audio effect is to be applied to a specific AudioTrack or MediaPlayer instance,
 * the application must specify the audio session ID of that instance when creating the AudioEffect.
 * (see {@link android.media.MediaPlayer#getAudioSessionId()} for details on audio sessions).
 * To apply an effect to the global audio output mix, session 0 must be specified when creating the
 * AudioEffect.
 * <p>Creating an effect on the output mix (audio session 0) requires permission
 * {@link android.Manifest.permission#MODIFY_AUDIO_SETTINGS}
 * <p>Creating an AudioEffect object will create the corresponding effect engine in the audio
 * framework if no instance of the same effect type exists in the specified audio session.
 * If one exists, this instance will be used.
 * <p>The application creating the AudioEffect object (or a derived class) will either receive
 * control of the effect engine or not depending on the priority parameter. If priority is higher
 * than the priority used by the current effect engine owner, the control will be transfered to the
 * new object. Otherwise control will remain with the previous object. In this case, the new
 * application will be notified of changes in effect engine state or control ownership by the
 * appropiate listener.
 */

CarClass(CAudioEffect), public AudioEffect
{
public:
    CAudioEffect();

    CARAPI constructor(
        /* [in] */ IUUID* type,
        /* [in] */ IUUID* uuid,
        /* [in] */ Int32 priority,
        /* [in] */ Int32 audioSession);

    CARAPI ReleaseResources();

    CARAPI GetDescriptor(
        /* [out] */ IAudioEffectDescriptor** descriptor);

    CARAPI QueryEffects(
        /* [out, callee] */ ArrayOf<IAudioEffectDescriptor*>** descriptor);

    CARAPI SetEnabled(
        /* [in]  */ Boolean enabled,
        /* [out] */ Int32* result);

    CARAPI SetParameter(
        /* [in] */ ArrayOf<Byte>* param,
        /* [in] */ ArrayOf<Byte>* value,
        /* [out] */ Int32* result);

    CARAPI SetParameterEx(
        /* [in] */ Int32 param,
        /* [in] */ Int32 value,
        /* [out] */ Int32* result);

    CARAPI SetParameterEx2(
        /* [in] */ Int32 param,
        /* [in] */ Int16 value,
        /* [out] */ Int32* result);

    CARAPI SetParameterEx3(
        /* [in] */ Int32 param,
        /* [in] */ ArrayOf<Byte>* value,
        /* [out] */ Int32* result);

    CARAPI SetParameterEx4(
        /* [in] */ ArrayOf<Int32>* param,
        /* [in] */ ArrayOf<Int32>* value,
        /* [out] */ Int32* result);

    CARAPI SetParameterEx5(
        /* [in] */ ArrayOf<Int32>* param,
        /* [in] */ ArrayOf<Int16>* value,
        /* [out] */ Int32* result);

    CARAPI SetParameterEx6(
        /* [in] */ ArrayOf<Int32>* param,
        /* [in] */ ArrayOf<Byte>* value,
        /* [out] */ Int32* result);

    CARAPI GetParameter(
        /* [in] */ ArrayOf<Byte>* param,
        /* [out] */ ArrayOf<Byte>* value,
        /* [out] */ Int32* status);

    CARAPI GetParameterEx(
        /* [in] */ Int32 param,
        /* [out] */ ArrayOf<Byte>* value,
        /* [out] */ Int32* status);

    CARAPI GetParameterEx2(
        /* [in] */ Int32 param,
        /* [out] */ ArrayOf<Int32>* value,
        /* [out] */ Int32* status);

    CARAPI GetParameterEx3(
        /* [in] */ Int32 param,
        /* [out] */ ArrayOf<Int16>* value,
        /* [out] */ Int32* status);

    CARAPI GetParameterEx4(
        /* [in] */ ArrayOf<Int32>* param,
        /* [out] */ ArrayOf<Int32>* value,
        /* [out] */ Int32* status);

    CARAPI GetParameterEx5(
        /* [in] */ ArrayOf<Int32>* param,
        /* [out] */ ArrayOf<Int16>* value,
        /* [out] */ Int32* status);

    CARAPI GetParameterEx6(
        /* [in] */ ArrayOf<Int32>* param,
        /* [in] */ ArrayOf<Byte>* value,
        /* [out] */ Int32* status);

    CARAPI Command(
        /* [in] */ Int32 cmdCode,
        /* [in] */ ArrayOf<Byte>* command,
        /* [out] */ ArrayOf<Byte>* reply,
        /* [out] */ Int32* result);

    CARAPI GetId(
        /* [out] */ Int32* Id);

    CARAPI GetEnabled(
        /* [out] */ Boolean* enabled);

    CARAPI HasControl(
        /* [out] */ Boolean* control);

    CARAPI SetEnableStatusListener(
        /* [in] */ IAudioEffectOnEnableStatusChangeListener* listener);

    CARAPI SetControlStatusListener(
        /* [in] */ IAudioEffectOnControlStatusChangeListener* listener);

    CARAPI SetParameterListener(
        /* [in] */ IAudioEffectOnParameterChangeListener* listener);

    CARAPI CheckState(
        /* [in] */ CString methodName);

    CARAPI CheckStatus(
        /* [in] */ Int32 status);

    CARAPI ByteArrayToInt32(
        /* [in] */ const ArrayOf<Byte>& valueBuf,
        /* [out] */ Int32* result);

    CARAPI ByteArrayToInt32Ex(
        /* [in] */ const ArrayOf<Byte>& valueBuf,
        /* [in] */ Int32 offset,
        /* [out] */ Int32* result);

    CARAPI Int32ToByteArray(
        /* [in] */ Int32 value,
        /* [out, callee] */ ArrayOf<Byte>** result);

    CARAPI ByteArrayToInt16(
        /* [in] */ const ArrayOf<Byte>& valueBuf,
        /* [out] */ Int16* result);

    CARAPI ByteArrayToInt16Ex(
        /* [in] */ const ArrayOf<Byte>& valueBuf,
        /* [in] */ Int32 offset,
        /* [out] */ Int16* result);

    CARAPI Int16ToByteArray(
        /* [in] */ Int16 value,
        /* [out, callee] */ ArrayOf<Byte>** result);

    CARAPI ConcatArrays(
        /* [in] */ const ArrayOf<Byte>& array1,
        /* [in] */ const ArrayOf<Byte>& array2,
        /* [out, callee] */ ArrayOf<Byte>** result);
};

#endif //__CAUDIOEFFECT_H__
