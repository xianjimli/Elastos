
#ifndef __CBASSBOOST_H__
#define __CBASSBOOST_H__

#include "_CBassBoost.h"
#include "media/audiofx/CAudioEffect.h"
#include "ext/frameworkext.h"
#include "os/Runnable.h"
#include <elastos/AutoPtr.h>
#include <elastos/ElRefBase.h>
#include <media/audiofx/AudioEffect.h>
using namespace Elastos::Core::Threading;

/**
 * Bass boost is an audio effect to boost or amplify low frequencies of the sound. It is comparable
 * to a simple equalizer but limited to one band amplification in the low frequency range.
 * <p>An application creates a BassBoost object to instantiate and control a bass boost engine in
 * the audio framework.
 * <p>The methods, parameter types and units exposed by the BassBoost implementation are directly
 * mapping those defined by the OpenSL ES 1.0.1 Specification (http://www.khronos.org/opensles/)
 * for the SLBassBoostItf interface. Please refer to this specification for more details.
 * <p>To attach the BassBoost to a particular AudioTrack or MediaPlayer, specify the audio session
 * ID of this AudioTrack or MediaPlayer when constructing the BassBoost.
 * If the audio session ID 0 is specified, the BassBoost applies to the main audio output mix.
 * <p>Creating a BassBoost on the output mix (audio session 0) requires permission
 * {@link android.Manifest.permission#MODIFY_AUDIO_SETTINGS}
 * <p>See {@link android.media.MediaPlayer#getAudioSessionId()} for details on audio sessions.
 * <p>See {@link android.media.audiofx.AudioEffect} class for more details on
 * controlling audio effects.
 */

CarClass(CBassBoost), public AudioEffect
{
public:
    class Settings{
    public:
        CARAPI constructor(
            /* [in] */ String* settings);

        CARAPI toString(
            /* [out] */ String* result);

        CARAPI GetStrength(
            /* [out] */ Int16* result);

        CARAPI SetStrength(
            /* [in] */ Int16 result);
    public:
        Int16 strength;
    };
private:
    /**
     * Listener used internally to receive unformatted parameter change events from AudioEffect
     * super class.
     */
    class BaseParameterListener
        : public ElRefBase
        , public IAudioEffectOnParameterChangeListener
    {
    public:
        CARAPI_(PInterface) Probe(
            /* [in] */ REIID riid);

        CARAPI_(UInt32) AddRef();

        CARAPI_(UInt32) Release();

        CARAPI GetInterfaceID(
            /* [in] */ IInterface *pObject,
            /* [out] */ InterfaceID *pIID);

        CARAPI OnParameterChange(
            /* [in] */ IAudioEffect* effect,
            /* [in] */ Int32 status,
            /* [in] */ const ArrayOf<Byte>& param,
            /* [in] */ const ArrayOf<Byte>& value);
    private:
        CBassBoost* mHost;
    };

public:
    CBassBoost();

    /**
     * Class constructor.
     * @param priority the priority level requested by the application for controlling the BassBoost
     * engine. As the same engine can be shared by several applications, this parameter indicates
     * how much the requesting application needs control of effect parameters. The normal priority
     * is 0, above normal is a positive number, below normal a negative number.
     * @param audioSession system wide unique audio session identifier. If audioSession
     *  is not 0, the BassBoost will be attached to the MediaPlayer or AudioTrack in the
     *  same audio session. Otherwise, the BassBoost will apply to the output mix.
     *
     * @throws java.lang.IllegalStateException
     * @throws java.lang.IllegalArgumentException
     * @throws java.lang.UnsupportedOperationException
     * @throws java.lang.RuntimeException
     */
    CARAPI constructor(
        /* [in] */ Int32 priority,
        /* [in] */ Int32 audioSession);

    /**
     * Indicates whether setting strength is supported. If this method returns false, only one
     * strength is supported and the setStrength() method always rounds to that value.
     * @return true is strength parameter is supported, false otherwise
     */
    CARAPI GetStrengthSupported(
        /* [out] */ Boolean* isSupported);

    /**
     * Sets the strength of the bass boost effect. If the implementation does not support per mille
     * accuracy for setting the strength, it is allowed to round the given strength to the nearest
     * supported value. You can use the {@link #getRoundedStrength()} method to query the
     * (possibly rounded) value that was actually set.
     * @param strength strength of the effect. The valid range for strength strength is [0, 1000],
     * where 0 per mille designates the mildest effect and 1000 per mille designates the strongest.
     * @throws IllegalStateException
     * @throws IllegalArgumentException
     * @throws UnsupportedOperationException
     */
    CARAPI SetStrength(
        /* [in] */ Int16 strength);

    /**
     * Gets the current strength of the effect.
     * @return the strength of the effect. The valid range for strength is [0, 1000], where 0 per
     * mille designates the mildest effect and 1000 per mille the strongest
     * @throws IllegalStateException
     * @throws IllegalArgumentException
     * @throws UnsupportedOperationException
     */
    CARAPI GetRoundedStrength(
        /* [out] */ Int16* strength);

    /**
     * Registers an OnParameterChangeListener interface.
     * @param listener OnParameterChangeListener interface registered
     */
    CARAPI SetParameterListenerEx(
        /* [in] */ IBassBoostOnParameterChangeListener* listener);

    /**
     * Gets the bass boost properties. This method is useful when a snapshot of current
     * bass boost settings must be saved by the application.
     * @return a BassBoost.Settings object containing all current parameters values
     * @throws IllegalStateException
     * @throws IllegalArgumentException
     * @throws UnsupportedOperationException
     */
    CARAPI GetProperties(
        /* [out] */ IBassBoostSettings** properties);

    /**
     * Sets the bass boost properties. This method is useful when bass boost settings have to
     * be applied from a previous backup.
     * @param settings a BassBoost.Settings object containing the properties to apply
     * @throws IllegalStateException
     * @throws IllegalArgumentException
     * @throws UnsupportedOperationException
     */
    CARAPI SetProperties(
        /* in */ IBassBoostSettings* settings);

    // IAudioEffect
    CARAPI ReleaseResources();

    CARAPI GetDescriptor(
        /* [out] */ IAudioEffectDescriptor** descriptor);

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

private:
    static const CString TAG;

    /**
     * Indicates if strength parameter is supported by the bass boost engine
     */
    Boolean mStrengthSupported;

    /**
     * Registered listener for parameter changes.
     */
    AutoPtr<IBassBoostOnParameterChangeListener> mParamListener;

    /**
     * Listener used internally to to receive raw parameter change event from AudioEffect super class
     */
    AutoPtr<BaseParameterListener> mBaseParamListener;

    /**
     * Lock for access to mParamListener
     */
    Mutex mParamListenerLock;

    AutoPtr<IAudioEffect> obj;
};

#endif //__CBASSBOOST_H__
