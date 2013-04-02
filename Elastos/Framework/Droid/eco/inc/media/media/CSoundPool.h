
#ifndef __CSOUNDPOOL_H__
#define __CSOUNDPOOL_H__

#include "_CSoundPool.h"

#include "media/SoundPool.h"

CarClass(CSoundPool), public SoundPool
{
public:
    CARAPI Load(
        /* [in] */ const String& path,
        /* [in] */ Int32 priority,
        /* [out] */ Int32 * pID);

    CARAPI LoadEx(
        /* [in] */ IContext * pContext,
        /* [in] */ Int32 resId,
        /* [in] */ Int32 priority,
        /* [out] */ Int32 * pID);

    CARAPI LoadEx2(
        /* [in] */ IAssetFileDescriptor * pAfd,
        /* [in] */ Int32 priority,
        /* [out] */ Int32 * pID);

    CARAPI LoadEx3(
        /* [in] */ IFileDescriptor * pFd,
        /* [in] */ Int64 offset,
        /* [in] */ Int64 length,
        /* [in] */ Int32 priority,
        /* [out] */ Int32 * pID);

    /**
     * Unload a sound from a sound ID.
     *
     * Unloads the sound specified by the soundID. This is the value
     * returned by the load() function. Returns true if the sound is
     * successfully unloaded, false if the sound was already unloaded.
     *
     * @param soundID a soundID returned by the load() function
     * @return true if just unloaded, false if previously unloaded
     */
    CARAPI Unload(
        /* [in] */ Int32 soundID,
        /* [out] */ Boolean* result);

    /**
     * Play a sound from a sound ID.
     *
     * Play the sound specified by the soundID. This is the value
     * returned by the load() function. Returns a non-zero streamID
     * if successful, zero if it fails. The streamID can be used to
     * further control playback. Note that calling play() may cause
     * another sound to stop playing if the maximum number of active
     * streams is exceeded. A loop value of -1 means loop forever,
     * a value of 0 means don't loop, other values indicate the
     * number of repeats, e.g. a value of 1 plays the audio twice.
     * The playback rate allows the application to vary the playback
     * rate (pitch) of the sound. A value of 1.0 means play back at
     * the original frequency. A value of 2.0 means play back twice
     * as fast, and a value of 0.5 means playback at half speed.
     *
     * @param soundID a soundID returned by the load() function
     * @param leftVolume left volume value (range = 0.0 to 1.0)
     * @param rightVolume right volume value (range = 0.0 to 1.0)
     * @param priority stream priority (0 = lowest priority)
     * @param loop loop mode (0 = no loop, -1 = loop forever)
     * @param rate playback rate (1.0 = normal playback, range 0.5 to 2.0)
     * @return non-zero streamID if successful, zero if failed
     */
    CARAPI Play(
        /* [in] */ Int32 soundID,
        /* [in] */ Float leftVolume,
        /* [in] */ Float rightVolume,
        /* [in] */ Int32 priority,
        /* [in] */ Int32 loop,
        /* [in] */ Float rate,
        /* [out] */ Int32* id);

    /**
     * Pause a playback stream.
     *
     * Pause the stream specified by the streamID. This is the
     * value returned by the play() function. If the stream is
     * playing, it will be paused. If the stream is not playing
     * (e.g. is stopped or was previously paused), calling this
     * function will have no effect.
     *
     * @param streamID a streamID returned by the play() function
     */
    CARAPI Pause(
        /* [in] */ Int32 streamID);

    /**
     * Resume a playback stream.
     *
     * Resume the stream specified by the streamID. This
     * is the value returned by the play() function. If the stream
     * is paused, this will resume playback. If the stream was not
     * previously paused, calling this function will have no effect.
     *
     * @param streamID a streamID returned by the play() function
     */
    CARAPI Resume(
        /* [in] */ Int32 streamID);

    /**
     * Pause all active streams.
     *
     * Pause all streams that are currently playing. This function
     * iterates through all the active streams and pauses any that
     * are playing. It also sets a flag so that any streams that
     * are playing can be resumed by calling autoResume().
     */
    CARAPI AutoPause();

    /**
     * Resume all previously active streams.
     *
     * Automatically resumes all streams that were paused in previous
     * calls to autoPause().
     */
    CARAPI AutoResume();

    /**
     * Stop a playback stream.
     *
     * Stop the stream specified by the streamID. This
     * is the value returned by the play() function. If the stream
     * is playing, it will be stopped. It also releases any native
     * resources associated with this stream. If the stream is not
     * playing, it will have no effect.
     *
     * @param streamID a streamID returned by the play() function
     */
    CARAPI Stop(
        /* [in] */ Int32 streamID);

    /**
     * Set stream volume.
     *
     * Sets the volume on the stream specified by the streamID.
     * This is the value returned by the play() function. The
     * value must be in the range of 0.0 to 1.0. If the stream does
     * not exist, it will have no effect.
     *
     * @param streamID a streamID returned by the play() function
     * @param leftVolume left volume value (range = 0.0 to 1.0)
     * @param rightVolume right volume value (range = 0.0 to 1.0)
     */
    CARAPI SetVolume(
        /* [in] */ Int32 streamID,
        /* [in] */ Float leftVolume,
        /* [in] */ Float rightVolume);

    /**
     * Change stream priority.
     *
     * Change the priority of the stream specified by the streamID.
     * This is the value returned by the play() function. Affects the
     * order in which streams are re-used to play new sounds. If the
     * stream does not exist, it will have no effect.
     *
     * @param streamID a streamID returned by the play() function
     */
    CARAPI SetPriority(
        /* [in] */ Int32 streamID,
        /* [in] */ Int32 priority);

    /**
     * Set loop mode.
     *
     * Change the loop mode. A loop value of -1 means loop forever,
     * a value of 0 means don't loop, other values indicate the
     * number of repeats, e.g. a value of 1 plays the audio twice.
     * If the stream does not exist, it will have no effect.
     *
     * @param streamID a streamID returned by the play() function
     * @param loop loop mode (0 = no loop, -1 = loop forever)
     */
    CARAPI SetLoop(
        /* [in] */ Int32 streamID,
        /* [in] */ Int32 loop);

    /**
     * Change playback rate.
     *
     * The playback rate allows the application to vary the playback
     * rate (pitch) of the sound. A value of 1.0 means playback at
     * the original frequency. A value of 2.0 means playback twice
     * as fast, and a value of 0.5 means playback at half speed.
     * If the stream does not exist, it will have no effect.
     *
     * @param streamID a streamID returned by the play() function
     * @param rate playback rate (1.0 = normal playback, range 0.5 to 2.0)
     */
    CARAPI SetRate(
        /* [in] */ Int32 streamID,
        /* [in] */ Float rate);

    CARAPI SetOnLoadCompleteListener(
        /* [in] */ IOnLoadCompleteListener * pListener);

    /**
     * Release the SoundPool resources.
     *
     * Release all memory and native resources used by the SoundPool
     * object. The SoundPool can no longer be used and the reference
     * should be set to null.
     */
    CARAPI ReleaseResources();

    CARAPI constructor(
        /* [in] */ Int32 maxStreams,
        /* [in] */ Int32 streamType,
        /* [in] */ Int32 srcQuality);

private:
    // TODO: Add your private member variables here.
};

#endif // __CSOUNDPOOL_H__
