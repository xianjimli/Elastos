#ifndef __CENVIRONMENTALREVERB_H__
#define __CENVIRONMENTALREVERB_H__
#include "_CEnvironmentalReverb.h"
#include "media/audiofx/CAudioEffect.h"
#include "ext/frameworkext.h"
#include "os/Runnable.h"
#include <elastos/AutoPtr.h>
#include <elastos/ElRefBase.h>
using namespace Elastos::Core::Threading;

/**
 * A sound generated within a room travels in many directions. The listener first hears the direct
 * sound from the source itself. Later, he or she hears discrete echoes caused by sound bouncing off
 * nearby walls, the ceiling and the floor. As sound waves arrive after undergoing more and more
 * reflections, individual reflections become indistinguishable and the listener hears continuous
 * reverberation that decays over time.
 * Reverb is vital for modeling a listener's environment. It can be used in music applications
 * to simulate music being played back in various environments, or in games to immerse the
 * listener within the game's environment.
 * The EnvironmentalReverb class allows an application to control each reverb engine property in a
 * global reverb environment and is more suitable for games. For basic control, more suitable for
 * music applications, it is recommended to use the
 * {@link android.media.audiofx.PresetReverb} class.
 * <p>An application creates a EnvironmentalReverb object to instantiate and control a reverb engine
 * in the audio framework.
 * <p>The methods, parameter types and units exposed by the EnvironmentalReverb implementation are
 * directly mapping those defined by the OpenSL ES 1.0.1 Specification
 * (http://www.khronos.org/opensles/) for the SLEnvironmentalReverbItf interface.
 * Please refer to this specification for more details.
 * <p>The EnvironmentalReverb is an output mix auxiliary effect and should be created on
 * Audio session 0. In order for a MediaPlayer or AudioTrack to be fed into this effect,
 * they must be explicitely attached to it and a send level must be specified. Use the effect ID
 * returned by getId() method to designate this particular effect when attaching it to the
 * MediaPlayer or AudioTrack.
 * <p>Creating a reverb on the output mix (audio session 0) requires permission
 * {@link android.Manifest.permission#MODIFY_AUDIO_SETTINGS}
 * <p>See {@link android.media.audiofx.AudioEffect} class for more details on controlling
 * audio effects.
 */

CarClass(CEnvironmentalReverb)
{
public:
    /**
     * The Settings class regroups all bass boost parameters. It is used in
     * conjuntion with getProperties() and setProperties() methods to backup and restore
     * all parameters in a single call.
     */
    class Settings{
    public:
        CARAPI constructor(
            /* [in] */ String* settings);

        CARAPI toString(
            /* [out] */ String* result);

        CARAPI GetParameterInt32(
            /* [in] */ String param,
            /* [out] */  Int32* result);

        CARAPI SetParameterInt32(
            /* [in] */ String param,
            /* [in] */ Int32 result);

        CARAPI GetParameterInt16(
            /* [in] */ String param,
            /* [out] */  Int16* result);

        CARAPI SetParameterInt16(
            /* [in] */ String param,
            /* [in] */ Int16 result);
    public:
        Int16 roomLevel;
        Int16 roomHFLevel;
        Int32 decayTime;
        Int16 decayHFRatio;
        Int16 reflectionsLevel;
        Int32 reflectionsDelay;
        Int16 reverbLevel;
        Int32 reverbDelay;
        Int16 diffusion;
        Int16 density;
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
            /* [in]  */ REIID riid);

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
        CEnvironmentalReverb* mHost;
    };
public:
    CEnvironmentalReverb();

    /**
     * Class constructor.
     * @param priority the priority level requested by the application for controlling the
     * EnvironmentalReverb engine. As the same engine can be shared by several applications, this
     * parameter indicates how much the requesting application needs control of effect parameters.
     * The normal priority is 0, above normal is a positive number, below normal a negative number.
     * @param audioSession  system wide unique audio session identifier. If audioSession
     *  is not 0, the EnvironmentalReverb will be attached to the MediaPlayer or AudioTrack in the
     *  same audio session. Otherwise, the EnvironmentalReverb will apply to the output mix.
     *  As the EnvironmentalReverb is an auxiliary effect it is recommended to instantiate it on
     *  audio session 0 and to attach it to the MediaPLayer auxiliary output.
     *
     * @throws java.lang.IllegalArgumentException
     * @throws java.lang.UnsupportedOperationException
     * @throws java.lang.RuntimeException
     */
    CARAPI constructor(
        /* [in] */ Int32 priority,
        /* [in] */ Int32 audioSession);

    /**
     * Sets the master volume level of the environmental reverb effect.
     * @param room room level in millibels. The valid range is [-9000, 0].
     * @throws IllegalStateException
     * @throws IllegalArgumentException
     * @throws UnsupportedOperationException
     */
    CARAPI SetRoomLevel(
        /* [in] */ Int16 room);
    /**
     * Gets the master volume level of the environmental reverb effect.
     * @return the room level in millibels.
     * @throws IllegalStateException
     * @throws IllegalArgumentException
     * @throws UnsupportedOperationException
     */
    CARAPI GetRoomLevel(
        /* [out] */ Int16* room);

    /**
     * Sets the volume level at 5 kHz relative to the volume level at low frequencies of the
     * overall reverb effect.
     * <p>This controls a low-pass filter that will reduce the level of the high-frequency.
     * @param roomHF high frequency attenuation level in millibels. The valid range is [-9000, 0].
     * @throws IllegalStateException
     * @throws IllegalArgumentException
     * @throws UnsupportedOperationException
     */
    CARAPI SetRoomHFLevel(
        /* [in] */ Int16 roomHF);

    /**
     * Gets the room HF level.
     * @return the room HF level in millibels.
     * @throws IllegalStateException
     * @throws IllegalArgumentException
     * @throws UnsupportedOperationException
     */
    CARAPI GetRoomHFLevel(
        /* [out] */ Int16* roomHF);

    /**
     * Sets the time taken for the level of reverberation to decay by 60 dB.
     * @param decayTime decay time in milliseconds. The valid range is [100, 20000].
     * @throws IllegalStateException
     * @throws IllegalArgumentException
     * @throws UnsupportedOperationException
     */
    CARAPI SetDecayTime(
        /* [in] */ Int32 decayTime);
    /**
     * Gets the decay time.
     * @return the decay time in milliseconds.
     * @throws IllegalStateException
     * @throws IllegalArgumentException
     * @throws UnsupportedOperationException
     */
    CARAPI GetDecayTime(
        /* [out] */ Int32* decayTime);

    /**
     * Sets the ratio of high frequency decay time (at 5 kHz) relative to the decay time at low
     * frequencies.
     * @param decayHFRatio high frequency decay ratio using a permille scale. The valid range is
     * [100, 2000]. A ratio of 1000 indicates that all frequencies decay at the same rate.
     * @throws IllegalStateException
     * @throws IllegalArgumentException
     * @throws UnsupportedOperationException
     */
    CARAPI SetDecayHFRatio(
        /* [in] */ Int16 decayHFRatio);

    /**
     * Gets the ratio of high frequency decay time (at 5 kHz) relative to low frequencies.
     * @return the decay HF ration. See {@link #setDecayHFRatio(short)} for units.
     * @throws IllegalStateException
     * @throws IllegalArgumentException
     * @throws UnsupportedOperationException
     */
    CARAPI GetDecayHFRatio(
        /* [out] */ Int16* decayHFRatio);

    /**
     * Sets the volume level of the early reflections.
     * <p>This level is combined with the overall room level
     * (set using {@link #setRoomLevel(short)}).
     * @param reflectionsLevel reflection level in millibels. The valid range is [-9000, 1000].
     * @throws IllegalStateException
     * @throws IllegalArgumentException
     * @throws UnsupportedOperationException
     */
    CARAPI SetReflectionsLevel(
        /* [in] */ Int16 reflectionsLevel);

    /**
     * Gets the volume level of the early reflections.
     * @return the early reflections level in millibels.
     * @throws IllegalStateException
     * @throws IllegalArgumentException
     * @throws UnsupportedOperationException
     */
    CARAPI GetReflectionsLevel(
        /* [out] */ Int16* reflectionsLevel);

    /**
     * Sets the delay time for the early reflections.
     * <p>This method sets the time between when the direct path is heard and when the first
     * reflection is heard.
     * @param reflectionsDelay reflections delay in milliseconds. The valid range is [0, 300].
     * @throws IllegalStateException
     * @throws IllegalArgumentException
     * @throws UnsupportedOperationException
     */
    CARAPI SetReflectionsDelay(
        /* [in] */ Int32 reflectionsDelay);

    /**
     * Gets the reflections delay.
     * @return the early reflections delay in milliseconds.
     * @throws IllegalStateException
     * @throws IllegalArgumentException
     * @throws UnsupportedOperationException
     */
    CARAPI GetReflectionsDelay(
        /* [out] */ Int32* reflectionsDelay);

    /**
     * Sets the volume level of the late reverberation.
     * <p>This level is combined with the overall room level (set using {@link #setRoomLevel(short)}).
     * @param reverbLevel reverb level in millibels. The valid range is [-9000, 2000].
     * @throws IllegalStateException
     * @throws IllegalArgumentException
     * @throws UnsupportedOperationException
     */
    CARAPI SetReverbLevel(
        /* [in] */ Int16 reverbLevel);

    /**
     * Gets the reverb level.
     * @return the reverb level in millibels.
     * @throws IllegalStateException
     * @throws IllegalArgumentException
     * @throws UnsupportedOperationException
     */
    CARAPI GetReverbLevel(
        /* [out] */ Int16* reverbLevel);

    /**
     * Sets the time between the first reflection and the reverberation.
     * @param reverbDelay reverb delay in milliseconds. The valid range is [0, 100].
     * @throws IllegalStateException
     * @throws IllegalArgumentException
     * @throws UnsupportedOperationException
     */
    CARAPI SetReverbDelay(
        /* [in] */ Int32 reverbDelay);

    /**
     * Gets the reverb delay.
     * @return the reverb delay in milliseconds.
     * @throws IllegalStateException
     * @throws IllegalArgumentException
     * @throws UnsupportedOperationException
     */
    CARAPI GetReverbDelay(
        /* [out] */ Int32* reverbDelay);

    /**
     * Sets the echo density in the late reverberation decay.
     * <p>The scale should approximately map linearly to the perceived change in reverberation.
     * @param diffusion diffusion specified using a permille scale. The diffusion valid range is
     * [0, 1000]. A value of 1000 o/oo indicates a smooth reverberation decay.
     * Values below this level give a more <i>grainy</i> character.
     * @throws IllegalStateException
     * @throws IllegalArgumentException
     * @throws UnsupportedOperationException
     */
    CARAPI SetDiffusion(
        /* [in] */ Int16 diffusion);

    /**
     * Gets diffusion level.
     * @return the diffusion level. See {@link #setDiffusion(short)} for units.
     * @throws IllegalStateException
     * @throws IllegalArgumentException
     * @throws UnsupportedOperationException
     */
    CARAPI GetDiffusion(
        /* [out] */ Int16* diffusion);

    /**
     * Controls the modal density of the late reverberation decay.
     * <p> The scale should approximately map linearly to the perceived change in reverberation.
     * A lower density creates a hollow sound that is useful for simulating small reverberation
     * spaces such as bathrooms.
     * @param density density specified using a permille scale. The valid range is [0, 1000].
     * A value of 1000 o/oo indicates a natural sounding reverberation. Values below this level
     * produce a more colored effect.
     * @throws IllegalStateException
     * @throws IllegalArgumentException
     * @throws UnsupportedOperationException
     */
    CARAPI SetDensity(
        /* [in] */ Int16 density);

    /**
     * Gets the density level.
     * @return the density level. See {@link #setDiffusion(short)} for units.
     * @throws IllegalStateException
     * @throws IllegalArgumentException
     * @throws UnsupportedOperationException
     */
    CARAPI GetDensity(
        /* [out] */ Int16* density);

    /**
     * Registers an OnParameterChangeListener interface.
     * @param listener OnParameterChangeListener interface registered
     */
    CARAPI SetParameterListenerEx(
        /* [in] */ IEnvironmentalReverbOnParameterChangeListener* listener);

    /**
     * Gets the environmental reverb properties. This method is useful when a snapshot of current
     * reverb settings must be saved by the application.
     * @return an EnvironmentalReverb.Settings object containing all current parameters values
     * @throws IllegalStateException
     * @throws IllegalArgumentException
     * @throws UnsupportedOperationException
     */
    CARAPI GetProperties(
        /* [out] */ IEnvironmentalReverbSettings** properties);

    /**
     * Sets the environmental reverb properties. This method is useful when reverb settings have to
     * be applied from a previous backup.
     * @param settings a EnvironmentalReverb.Settings object containing the properties to apply
     * @throws IllegalStateException
     * @throws IllegalArgumentException
     * @throws UnsupportedOperationException
     */
    CARAPI SetProperties(
        /* [in] */ IEnvironmentalReverbSettings* settings);

    // IAudioEffect
    CARAPI ReleaseResources();

    CARAPI GetDescriptor(
        /* [out] */ IAudioEffectDescriptor** descriptor);

    CARAPI SetEnabled(
        /* [in]  */ Boolean enabled,
        /* [out] */ Int32* result);

    CARAPI SetParameter(
        /* [in] */ const ArrayOf<Byte>& param,
        /* [in] */ const ArrayOf<Byte>& value,
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
        /* [in] */ const ArrayOf<Byte>& value,
        /* [out] */ Int32* result);

    CARAPI SetParameterEx4(
        /* [in] */ const ArrayOf<Int32>& param,
        /* [in] */ const ArrayOf<Int32>& value,
        /* [out] */ Int32* result);

    CARAPI SetParameterEx5(
        /* [in] */ const ArrayOf<Int32>& param,
        /* [in] */ const ArrayOf<Int16>& value,
        /* [out] */ Int32* result);

    CARAPI SetParameterEx6(
        /* [in] */ const ArrayOf<Int32>& param,
        /* [in] */ const ArrayOf<Byte>& value,
        /* [out] */ Int32* result);

    CARAPI GetParameter(
        /* [in] */ const ArrayOf<Byte>& param,
        /* [out] */ ArrayOf<Byte>* value,
        /* [out] */  Int32* status);

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
        /* [in] */ const ArrayOf<Int32>& param,
        /* [out] */ ArrayOf<Int32>* value,
        /* [out] */ Int32* status);

    CARAPI GetParameterEx5(
        /* [in] */ const ArrayOf<Int32>& param,
        /* [out] */ ArrayOf<Int16>* value,
        /* [out] */ Int32* status);

    CARAPI GetParameterEx6(
        /* [in] */ const ArrayOf<Int32>& param,
        /* [in] */ const ArrayOf<Byte>& value,
        /* [out] */ Int32* status);

    CARAPI Command(
        /* [in] */ Int32 cmdCode,
        /* [in] */ const ArrayOf<Byte>& command,
        /* [out] */ ArrayOf<Byte>* reply,
        /* [out] */ Int32* result);

    CARAPI GetId(
        /* [out] */ Int32* Id);

    CARAPI GetEnabled(
        /* [out] */ Boolean* getenable);

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

    // used by setProperties()/getProperties
    Int32 EnvironmentalReverb_PARAM_PROPERTIES;

    /**
     * Registered listener for parameter changes
     */
    AutoPtr<IEnvironmentalReverbOnParameterChangeListener> mParamListener;

    /**
     * Listener used internally to to receive raw parameter change event from AudioEffect super
     * class
     */
    AutoPtr<BaseParameterListener> mBaseParamListener;

    /**
     * Lock for access to mParamListener
     */
    Mutex mParamListenerLock;

    // Keep this in sync with sizeof(s_reverb_settings) defined in
    // frameworks/base/include/media/EffectEnvironmentalReverbApi.h
    Int32 EnvironmentalReverb_PROPERTY_SIZE;

    AutoPtr<IAudioEffect> obj;
};

#endif //__CENVIRONMENTALREVERB_H__
