
#ifndef __AUDIOEFFECT_H__
#define __AUDIOEFFECT_H__

#include "ext/frameworkext.h"
#include <elastos/AutoPtr.h>
#include <elastos/ElRefBase.h>
#include <elastos/HashMap.h>
#include <elastos/Mutex.h>

using namespace Elastos::Core::Threading;

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

class AudioEffect
{
public:
    /**
     * The effect descriptor contains information on a particular effect implemented in the
     * audio framework:<br>
     * <ul>
     *  <li>type: UUID corresponding to the OpenSL ES interface implemented by this effect</li>
     *  <li>uuid: UUID for this particular implementation</li>
     *  <li>connectMode: {@link #EFFECT_INSERT} or {@link #EFFECT_AUXILIARY}</li>
     *  <li>name: human readable effect name</li>
     *  <li>implementor: human readable effect implementor name</li>
     * </ul>
     * The method {@link #queryEffects()} returns an array of Descriptors to facilitate effects
     * enumeration.
     */
    class Descriptor
        : public ElRefBase
        , public IAudioEffectDescriptor
    {
    public:
        Descriptor();

        Descriptor(
            /* [in] */ const String& type1,
            /* [in] */ const String& uuid1,
            /* [in] */ const String& connectMode1,
            /* [in] */ const String& name1,
            /* [in] */ const String& implementor1);

        CARAPI GetType(
            /* [out] */ IUUID** type);

        CARAPI SetType(
            /* [in] */ IUUID* type);

        CARAPI GetUuid(
            /* [out] */ IUUID** uuid);

        CARAPI SetUuid(
            /* [in] */ IUUID* uuid);

        CARAPI GetConnectMode(
            /* [out] */ String* connectMode);

        CARAPI SetConnectMode(
            /* [in] */ String connectMode);

        CARAPI GetName(
            /* [out] */ String* name);

        CARAPI SetName(
            /* [in] */ String name);

        CARAPI GetImplementor(
            /* [out] */ String* implementor);

        CARAPI SetImplementor(
            /* [in] */ String implementor);
    public:
        /**
         *  Indicates the generic type of the effect (Equalizer, Bass boost ...). The UUID
         *  corresponds to the OpenSL ES Interface ID for this type of effect.
         */
        AutoPtr<IUUID> mType;
        /**
         *  Indicates the particular implementation of the effect in that type. Several effects
         *  can have the same type but this uuid is unique to a given implementation.
         */
        AutoPtr<IUUID> mUuid;
        /**
         *  Indicates if the effect is of insert category {@link #EFFECT_INSERT} or auxiliary
         *  category {@link #EFFECT_AUXILIARY}. Insert effects (Typically an Equalizer) are applied
         *  to the entire audio source and usually not shared by several sources. Auxiliary effects
         *  (typically a reverberator) are applied to part of the signal (wet) and the effect output
         *  is added to the original signal (dry).
         */
        String mConnectMode;
        /**
         * Human readable effect name
         */
        String mName;
        /**
         * Human readable effect implementor name
         */
        String mImplementor;
    };

private:
    /**
     * Helper class to handle the forwarding of native events to the appropriate
     * listeners
     */
    class NativeEventHandler
        : public ElRefBase
        , public IApartment
    {
    public:
        NativeEventHandler(
            /* [in] */ IAudioEffect* ae,
            /* [in] */ IApartment* looper);

        CARAPI_(PInterface) Probe(
            /* [in]  */ REIID riid);

        CARAPI_(UInt32) AddRef();

        CARAPI_(UInt32) Release();

        CARAPI GetInterfaceID(
            /* [in] */ IInterface *pObject,
            /* [out] */ InterfaceID *pIID);

        CARAPI HandleMessage(
            /* [in] */ Int32 what,
            /* [in] */ Int32 arg1,
            /* [in] */ Int32 arg2,
            /* [in] */ IInterface* obj);

        CARAPI Start(
            /* [in] */ ApartmentAttr attr);

        CARAPI Finish();

        CARAPI PostCppCallback(
            /* [in] */ Handle32 target,
            /* [in] */ Handle32 func,
            /* [in] */ IParcel* params,
            /* [in] */ Int32 id);

        CARAPI PostCppCallbackAtTime(
            /* [in] */ Handle32 target,
            /* [in] */ Handle32 func,
            /* [in] */ IParcel* params,
            /* [in] */ Int32 id,
            /* [in] */ Millisecond64 uptimeMillis);

        CARAPI PostCppCallbackDelayed(
            /* [in] */ Handle32 target,
            /* [in] */ Handle32 func,
            /* [in] */ IParcel* params,
            /* [in] */ Int32 id,
            /* [in] */ Millisecond64 delayMillis);

        CARAPI PostCppCallbackAtFrontOfQueue(
            /* [in] */ Handle32 target,
            /* [in] */ Handle32 func,
            /* [in] */ IParcel* params,
            /* [in] */ Int32 id);

        CARAPI RemoveCppCallbacks(
            /* [in] */ Handle32 target,
            /* [in] */ Handle32 func);

        CARAPI RemoveCppCallbacksEx(
            /* [in] */ Handle32 target,
            /* [in] */ Handle32 func,
            /* [in] */ Int32 id);

        CARAPI HasCppCallbacks(
            /* [in] */ Handle32 target,
            /* [in] */ Handle32 func,
            /* [out] */ Boolean* result);

        CARAPI HasCppCallbacksEx(
            /* [in] */ Handle32 target,
            /* [in] */ Handle32 func,
            /* [in] */ Int32 id,
            /* [out] */ Boolean* result);

        CARAPI SendMessage(
            /* [in] */ Int32 message,
            /* [in] */ IParcel* params);
    private:
        IAudioEffect* mAudioEffect;
        AudioEffect* mHost;
        AutoPtr<IApartment> mApartment;
    };

public:
    AudioEffect();

    virtual ~AudioEffect();

    /**
     * Releases the native AudioEffect resources. It is a good practice to
     * release the effect engine when not in use as control can be returned to
     * other applications or the native resources released.
     */
    CARAPI ReleaseResources();

    /**
     * Get the effect descriptor.
     *
     * @see android.media.audiofx.AudioEffect.Descriptor
     * @throws IllegalStateException
     */
    CARAPI GetDescriptor(
        /* [out] */ IAudioEffectDescriptor** descriptor);

    // --------------------------------------------------------------------------
    // Effects Enumeration
    // --------------------

    /**
    * Query all effects available on the platform. Returns an array of
    * {@link android.media.audiofx.AudioEffect.Descriptor} objects
    *
    * @throws IllegalStateException
    */

    /* static */ CARAPI QueryEffects(
        /* [out, callee] */ ArrayOf<IAudioEffectDescriptor*>** descriptor);

    /**
     * Enable or disable the effect.
     * Creating an audio effect does not automatically apply this effect on the audio source. It
     * creates the resources necessary to process this effect but the audio signal is still bypassed
     * through the effect engine. Calling this method will make that the effect is actually applied
     * or not to the audio content being played in the corresponding audio session.
     *
     * @param enabled the requested enable state
     * @return {@link #SUCCESS} in case of success, {@link #ERROR_INVALID_OPERATION}
     *         or {@link #ERROR_DEAD_OBJECT} in case of failure.
     * @throws IllegalStateException
     */
    CARAPI SetEnabled(
        /* [in]  */ Boolean enabled,
        /* [out] */ Int32* result);

    /**
     * Set effect parameter. The setParameter method is provided in several
     * forms addressing most common parameter formats. This form is the most
     * generic one where the parameter and its value are both specified as an
     * array of bytes. The parameter and value type and length are therefore
     * totally free. For standard effect defined by OpenSL ES, the parameter
     * format and values must match the definitions in the corresponding OpenSL
     * ES interface.
     *
     * @param param the identifier of the parameter to set
     * @param value the new value for the specified parameter
     * @return {@link #SUCCESS} in case of success, {@link #ERROR_BAD_VALUE},
     *         {@link #ERROR_NO_MEMORY}, {@link #ERROR_INVALID_OPERATION} or
     *         {@link #ERROR_DEAD_OBJECT} in case of failure
     * @throws IllegalStateException
     * @hide
     */
    CARAPI SetParameter(
        /* [in] */ ArrayOf<Byte>* param,
        /* [in] */ ArrayOf<Byte>* value,
        /* [out] */ Int32* result);

    /**
     * Set effect parameter. The parameter and its value are integers.
     *
     * @see #setParameter(byte[], byte[])
     * @hide
     */
    CARAPI SetParameterEx(
        /* [in] */ Int32 param,
        /* [in] */ Int32 value,
        /* [out] */ Int32* result);

    /**
     * Set effect parameter. The parameter is an integer and the value is a
     * short integer.
     *
     * @see #setParameter(byte[], byte[])
     * @hide
     */
    CARAPI SetParameterEx2(
        /* [in] */ Int32 param,
        /* [in] */ Int16 value,
        /* [out] */ Int32* result);

    /**
     * Set effect parameter. The parameter is an integer and the value is an
     * array of bytes.
     *
     * @see #setParameter(byte[], byte[])
     * @hide
     */
    CARAPI SetParameterEx3(
        /* [in] */ Int32 param,
        /* [in] */ ArrayOf<Byte>* value,
        /* [out] */ Int32* result);

    /**
     * Set effect parameter. The parameter is an array of 1 or 2 integers and
     * the value is also an array of 1 or 2 integers
     *
     * @see #setParameter(byte[], byte[])
     * @hide
     */
    CARAPI SetParameterEx4(
        /* [in] */ ArrayOf<Int32>* param,
        /* [in] */ ArrayOf<Int32>* value,
        /* [out] */ Int32* result);

    /**
     * Set effect parameter. The parameter is an array of 1 or 2 integers and
     * the value is an array of 1 or 2 short integers
     *
     * @see #setParameter(byte[], byte[])
     * @hide
     */
    CARAPI SetParameterEx5(
        /* [in] */ ArrayOf<Int32>* param,
        /* [in] */ ArrayOf<Int16>* value,
        /* [out] */ Int32* result);

    /**
     * Set effect parameter. The parameter is an array of 1 or 2 integers and
     * the value is an array of bytes
     *
     * @see #setParameter(byte[], byte[])
     * @hide
     */
    CARAPI SetParameterEx6(
        /* [in] */ ArrayOf<Int32>* param,
        /* [in] */ ArrayOf<Byte>* value,
        /* [out] */ Int32* result);

    /**
     * Get effect parameter. The getParameter method is provided in several
     * forms addressing most common parameter formats. This form is the most
     * generic one where the parameter and its value are both specified as an
     * array of bytes. The parameter and value type and length are therefore
     * totally free.
     *
     * @param param the identifier of the parameter to set
     * @param value the new value for the specified parameter
     * @return {@link #SUCCESS} in case of success, {@link #ERROR_BAD_VALUE},
     *         {@link #ERROR_NO_MEMORY}, {@link #ERROR_INVALID_OPERATION} or
     *         {@link #ERROR_DEAD_OBJECT} in case of failure When called, value.length
     *         indicates the maximum size of the returned parameters value. When
     *         returning, value.length is updated with the actual size of the
     *         returned value.
     * @throws IllegalStateException
     * @hide
     */
    CARAPI GetParameter(
        /* [in] */ ArrayOf<Byte>* param,
        /* [out] */ ArrayOf<Byte>* value,
        /* [out] */ Int32* status);

    /**
     * Get effect parameter. The parameter is an integer and the value is an
     * array of bytes.
     *
     * @see #getParameter(byte[], byte[])
     * @hide
     */
    CARAPI GetParameterEx(
        /* [in] */ Int32 param,
        /* [out] */ ArrayOf<Byte>* value,
        /* [out] */ Int32* status);

    /**
     * Get effect parameter. The parameter is an integer and the value is an
     * array of 1 or 2 integers
     *
     * @see #getParameter(byte[], byte[])
     * @hide
     */
    CARAPI GetParameterEx2(
        /* [in] */ Int32 param,
        /* [out] */ ArrayOf<Int32>* value,
        /* [out] */ Int32* status);

    /**
     * Get effect parameter. The parameter is an integer and the value is an
     * array of 1 or 2 short integers
     *
     * @see #getParameter(byte[], byte[])
     * @hide
     */
    CARAPI GetParameterEx3(
        /* [in] */ Int32 param,
        /* [out] */ ArrayOf<Int16>* value,
        /* [out] */ Int32* status);

    /**
     * Get effect parameter. The parameter is an array of 1 or 2 integers and
     * the value is also an array of 1 or 2 integers
     *
     * @see #getParameter(byte[], byte[])
     * @hide
     */
    CARAPI GetParameterEx4(
        /* [in] */ ArrayOf<Int32>* param,
        /* [out] */ ArrayOf<Int32>* value,
        /* [out] */ Int32* status);

    /**
     * Get effect parameter. The parameter is an array of 1 or 2 integers and
     * the value is an array of 1 or 2 short integers
     *
     * @see #getParameter(byte[], byte[])
     * @hide
     */
    CARAPI GetParameterEx5(
        /* [in] */ ArrayOf<Int32>* param,
        /* [out] */ ArrayOf<Int16>* value,
        /* [out] */ Int32* status);

    /**
     * Get effect parameter. The parameter is an array of 1 or 2 integers and
     * the value is an array of bytes
     *
     * @see #getParameter(byte[], byte[])
     * @hide
     */
    CARAPI GetParameterEx6(
        /* [in] */ ArrayOf<Int32>* param,
        /* [in] */ ArrayOf<Byte>* value,
        /* [out] */ Int32* status);

    /**
     * Send a command to the effect engine. This method is intended to send
     * proprietary commands to a particular effect implementation.
     *
     * @hide
     */
    CARAPI Command(
        /* [in] */ Int32 cmdCode,
        /* [in] */ ArrayOf<Byte>* command,
        /* [out] */ ArrayOf<Byte>* reply,
        /* [out] */ Int32* result);

    /**
     * Returns effect unique identifier. This system wide unique identifier can
     * be used to attach this effect to a MediaPlayer or an AudioTrack when the
     * effect is an auxiliary effect (Reverb)
     *
     * @return the effect identifier.
     * @throws IllegalStateException
     */
    CARAPI GetId(
        /* [out] */ Int32* Id);

    /**
     * Returns effect enabled state
     *
     * @return true if the effect is enabled, false otherwise.
     * @throws IllegalStateException
     */
    CARAPI GetEnabled(
        /* [out] */ Boolean* enabled);

    /**
     * Checks if this AudioEffect object is controlling the effect engine.
     *
     * @return true if this instance has control of effect engine, false
     *         otherwise.
     * @throws IllegalStateException
     */
    CARAPI HasControl(
        /* [out] */ Boolean* control);

    // --------------------------------------------------------------------------
    // Initialization / configuration
    // --------------------
    /**
     * Sets the listener AudioEffect notifies when the effect engine is enabled
     * or disabled.
     *
     * @param listener
     */
    CARAPI SetEnableStatusListener(
        /* [in] */ IAudioEffectOnEnableStatusChangeListener* listener);

     /**
     * Sets the listener AudioEffect notifies when the effect engine control is
     * taken or returned.
     *
     * @param listener
     */
    CARAPI SetControlStatusListener(
        /* [in] */ IAudioEffectOnControlStatusChangeListener* listener);

    /**
    * Sets the listener AudioEffect notifies when a parameter is changed.
    *
    * @param listener
    * @hide
    */
    CARAPI SetParameterListener(
        /* [in] */ IAudioEffectOnParameterChangeListener* listener);

    // ---------------------------------------------------------
    // Utility methods
    // ------------------

    /**
    * @hide
    */
    CARAPI CheckState(
        /* [in] */ CString methodName);

    /**
     * @hide
     */
    CARAPI CheckStatus(
        /* [in] */ Int32 status);

    /**
     * @hide
     */
    CARAPI ByteArrayToInt32(
        /* [in] */ ArrayOf<Byte>* valueBuf,
        /* [out] */ Int32* result);

    /**
     * @hide
     */
    CARAPI ByteArrayToInt32Ex(
        /* [in] */ ArrayOf<Byte>* valueBuf,
        /* [in] */ Int32 offset,
        /* [out] */ Int32* result);

    /**
     * @hide
     */
    CARAPI Int32ToByteArray(
        /* [in] */ Int32 value,
        /* [out, callee] */ ArrayOf<Byte>** result);

    /**
     * @hide
     */
    CARAPI ByteArrayToInt16(
        /* [in] */ ArrayOf<Byte>* valueBuf,
        /* [out] */ Int16* result);

    /**
     * @hide
     */
    CARAPI ByteArrayToInt16Ex(
        /* [in] */ ArrayOf<Byte>* valueBuf,
        /* [in] */ Int32 offset,
        /* [out] */ Int16* result);

    /**
     * @hide
     */
    CARAPI Int16ToByteArray(
        /* [in] */ Int16 value,
        /* [out, callee] */ ArrayOf<Byte>** result);

    /**
     * @hide
     */
    CARAPI ConcatArrays(
        /* [in] */ ArrayOf<Byte>* array1,
        /* [in] */ ArrayOf<Byte>* array2,
        /* [out, callee] */ ArrayOf<Byte>** result);

protected:
    /**
     * Class constructor.
     *
     * @param type type of effect engine created. See {@link #EFFECT_TYPE_ENV_REVERB},
     *            {@link #EFFECT_TYPE_EQUALIZER} ... Types corresponding to
     *            built-in effects are defined by AudioEffect class. Other types
     *            can be specified provided they correspond an existing OpenSL
     *            ES interface ID and the corresponsing effect is available on
     *            the platform. If an unspecified effect type is requested, the
     *            constructor with throw the IllegalArgumentException. This
     *            parameter can be set to {@link #EFFECT_TYPE_NULL} in which
     *            case only the uuid will be used to select the effect.
     * @param uuid unique identifier of a particular effect implementation.
     *            Must be specified if the caller wants to use a particular
     *            implementation of an effect type. This parameter can be set to
     *            {@link #EFFECT_TYPE_NULL} in which case only the type will
     *            be used to select the effect.
     * @param priority the priority level requested by the application for
     *            controlling the effect engine. As the same effect engine can
     *            be shared by several applications, this parameter indicates
     *            how much the requesting application needs control of effect
     *            parameters. The normal priority is 0, above normal is a
     *            positive number, below normal a negative number.
     * @param audioSession system wide unique audio session identifier. If audioSession
     *            is not 0, the effect will be attached to the MediaPlayer or
     *            AudioTrack in the same audio session. Otherwise, the effect
     *            will apply to the output mix.
     *
     * @throws java.lang.IllegalArgumentException
     * @throws java.lang.UnsupportedOperationException
     * @throws java.lang.RuntimeException
     * @hide
     */
    CARAPI Init(
        /* [in] */ IUUID* type,
        /* [in] */ IUUID* uuid,
        /* [in] */ Int32 priority,
        /* [in] */ Int32 audioSession);

private:
    // Convenience method for the creation of the native event handler
    // It is called only when a non-null event listener is set.
    // precondition:
    // mNativeEventHandler is null
    CARAPI_(void) CreateNativeEventHandler();

    // ---------------------------------------------------------
    // Java methods called from the native side
    // --------------------
    static CARAPI_(void) PostEventFromNative(
        /* [in] */ AudioEffect* effect_ref,
        /* [in] */ Int32 what,
        /* [in] */ Int32 arg1,
        /* [in] */ Int32 arg2,
        /* [in] */ IInterface* obj);

    // ---------------------------------------------------------
    // Native methods called from the Java side
    // --------------------

    CARAPI_(void) NativeInit();

    CARAPI_(Int32) NativeSetup(
        /* [in] */ IObject* audioeffect_this,
        /* [in] */ String type,
        /* [in] */ String uuid,
        /* [in] */ Int32 priority,
        /* [in] */ Int32 audioSession,
        /* [in] */ ArrayOf<Int32>* id,
        /* [in] */ ArrayOf<IObject>* desc);

    CARAPI_(void) NativeFinalize();

    CARAPI_(void) NativeRelease();

    CARAPI_(Int32) NativeSetEnabled(
        /* [in] */ Boolean enabled);

    CARAPI_(Boolean) NativeGetEnabled();

    CARAPI_(Boolean) NativeHasControl();

    CARAPI_(Int32) NativeSetParameter(
        /* [in] */ Int32 psize,
        /* [in] */ ArrayOf<Byte>* param,
        /* [in] */ Int32 vsize,
        /* [in] */ ArrayOf<Byte>* value);

    CARAPI_(Int32) NativeGetParameter(
        /* [in] */ Int32 psize,
        /* [in] */ ArrayOf<Byte>* param,
        /* [in] */ ArrayOf<Int32>* vsize,
        /* [in] */ ArrayOf<Byte>* value);

    CARAPI_(Int32) NativeCommand(
        /* [in] */ Int32 cmdCode,
        /* [in] */ Int32 cmdSize,
        /* [in] */ ArrayOf<Byte>* cmdData,
        /* [in] */ ArrayOf<Int32>* repSize,
        /* [in] */ ArrayOf<Byte>* repData);

    CARAPI_(ArrayOf<IAudioEffectDescriptor*>*) NativeQueryEffects();

public:
    /**
     * Lock to protect listeners updates against event notifications
     * @hide
     */

    Mutex mListenerLock;

    /**
     * Handler for events coming from the native code
     * @hide
     */
     AutoPtr<IApartment> mNativeEventHandler;

private:
    static const CString TAG;

    // --------------------------------------------------------------------------
    // Member variables
    // --------------------
    /**
     * Indicates the state of the AudioEffect instance
     */
    Int32 mState;
    /**
     * Lock to synchronize access to mState
     */
    Mutex mStateLock;
    /**
     * System wide unique effect ID
     */
    Int32 mId;

    // accessed by native methods
    Int32 mNativeAudioEffect;
    Int32 mJniData;

    /**
     * Effect descriptor
     */
    AutoPtr<IAudioEffectDescriptor> mDescriptor;

    /**
     * Listener for effect engine state change notifications.
     *
     * @see #setEnableStatusListener(OnEnableStatusChangeListener)
     */
    AutoPtr<IAudioEffectOnEnableStatusChangeListener> mEnableStatusChangeListener;
    /**
     * Listener for effect engine control ownership change notifications.
     *
     * @see #setControlStatusListener(OnControlStatusChangeListener)
     */
    AutoPtr<IAudioEffectOnControlStatusChangeListener> mControlChangeStatusListener;
    /**
     * Listener for effect engine control ownership change notifications.
     *
     * @see #setParameterListener(OnParameterChangeListener)
     */
    AutoPtr<IAudioEffectOnParameterChangeListener> mParameterChangeListener;

};

#endif //__AUDIOEFFECT_H__
