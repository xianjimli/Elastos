#ifndef __CAUDIOEFFECT_H__
#define __CAUDIOEFFECT_H__
#include "_CAudioEffect.h"
#include "ext/frameworkext.h"
#include "os/Runnable.h"
#include <elastos/List.h>
#include <elastos/AutoPtr.h>
using namespace Elastos::Core::Threading;

CarClass(CAudioEffect)
{
private:
    class NativeEventHandler : public Runnable
    {
        public:
        private:
    };
public:
    CAudioEffect();
    CARAPI constructor(
        /* [in] */ //IUUID* type,
        /* [in] */ //IUUID* uuid,
        /* [in] */ Int32 proiority,
        /* [in] */ Int32 audioSession);

    CARAPI AudioEffect(
        /* [in] */ //IUUID* type,
        /* [in] */ //IUUID* uuid,
        /* [in] */ Int32 proiority,
        /* [in] */ Int32 audioSession);

    /**
     * Releases the native AudioEffect resources. It is a good practice to
     * release the effect engine when not in use as control can be returned to
     * other applications or the native resources released.
     */
    CARAPI ReleaseIt();

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

    CARAPI QueryEffects(
        /* [out, callee] */ ArrayOf<IAudioEffectDescriptor>** descriptor);
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
        /* [in] */ const ArrayOf<Byte>& param,
        /* [in] */ const ArrayOf<Byte>& value);

    /**
     * Set effect parameter. The parameter and its value are integers.
     *
     * @see #setParameter(byte[], byte[])
     * @hide
     */        
    CARAPI SetParameterEx(
        /* [in] */ Int32 param,
        /* [in] */ Int32 value);

    /**
     * Set effect parameter. The parameter is an integer and the value is a
     * short integer.
     *
     * @see #setParameter(byte[], byte[])
     * @hide
     */
    CARAPI SetParameterEx2(
        /* [in] */ Int32 param,
        /* [in] */ Int16 value);

    /**
     * Set effect parameter. The parameter is an integer and the value is an
     * array of bytes.
     *
     * @see #setParameter(byte[], byte[])
     * @hide
     */
    CARAPI SetParameterEx3(
        /* [in] */ Int32 param,
        /* [in] */ const ArrayOf<Byte>& value);

    /**
     * Set effect parameter. The parameter is an array of 1 or 2 integers and
     * the value is also an array of 1 or 2 integers
     *
     * @see #setParameter(byte[], byte[])
     * @hide
     */
    CARAPI SetParameterEx4(
        /* [in] */ const ArrayOf<Int32>& param,
        /* [in] */ const ArrayOf<Int32>& value);

    /**
     * Set effect parameter. The parameter is an array of 1 or 2 integers and
     * the value is an array of 1 or 2 short integers
     *
     * @see #setParameter(byte[], byte[])
     * @hide
     */
    CARAPI SetParameterEx5(
        /* [in] */ const ArrayOf<Int32>& param,
        /* [in] */ const ArrayOf<Int16>& value);

    /**
     * Set effect parameter. The parameter is an array of 1 or 2 integers and
     * the value is an array of bytes
     *
     * @see #setParameter(byte[], byte[])
     * @hide
     */
    SetParameterEx6(
        /* [in] */ const ArrayOf<Int32>& param,
        /* [in] */ const ArrayOf<Byte>& value);
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
        /* [in] */ const ArrayOf<Byte>& param,
        /* [out] */ ArrayOf<Byte>* value,
        /* [out] */  Int32* status);

    /**
     * Get effect parameter. The parameter is an integer and the value is an
     * array of bytes.
     *
     * @see #getParameter(byte[], byte[])
     * @hide
     */
    CARAPI GetParameterEx(
        /* [in] */ const ArrayOf<Int32>& param,
        /* [in] */ const ArrayOf<Byte>& value,
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
        /* [in] */ const ArrayOf<Int32>& param,
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
        /* [in] */ const ArrayOf<Int32>& param,
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
        /* [in] */ const ArrayOf<Int32>& param,
        /* [in] */ const ArrayOf<Byte>& value);

    /**
     * Send a command to the effect engine. This method is intended to send
     * proprietary commands to a particular effect implementation.
     *
     * @hide
     */
    CARAPI Command(
        /* [in] */ Int32 cmdCode,
        /* [in] */ const ArrayOf<Byte>& command,
        /* [out] */ ArrayOf<Byte>* reply,
        /* [out] */ Int32* status);

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
        /* [out] */ Boolean* getenable);

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
        /* [in] */ const ArrayOf<Byte>& valueBuf,
        /* [out] */ Int32* value);

    /**
     * @hide
     */
    CARAPI ByteArrayToInt32Ex(
        /* [in] */ const ArrayOf<Byte>& valueBuf,
        /* [in] */ Int32 offset,
        /* [out] */ Int32* value);

    /**
     * @hide
     */
    CARAPI Int32ToByteArray(
        /* [in] */ Int32 value);

    /**
     * @hide
     */
    CARAPI ByteArrayToInt16(
        /* [in] */ const ArrayOf<Byte>& valueBuf);

    /**
     * @hide
     */
    CARAPI ByteArrayToInt16Ex(
        /* [in] */ const ArrayOf<Byte>& valueBuf,
        /* [in] */ Int32 offset);

    /**
     * @hide
     */
    CARAPI Int16ToByteArray(
        /* [in] */ Int16 value);

    /**
     * @hide
     */
    CARAPI ConcatArrays(
        /* [in] */ const ArrayOf<Byte>& array1,
        /* [in] */ const ArrayOf<Byte>& array2,
        /* [out, callee] */ ArrayOf<Byte>** result);
private:

    // Convenience method for the creation of the native event handler
    // It is called only when a non-null event listener is set.
    // precondition:
    // mNativeEventHandler is null
    CARAPI_(void) CreateNativeEventHandler();
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
     //NativeEventHandler mNativeEventHandler;

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
#endif //__CAUDIOEFFECT_H__