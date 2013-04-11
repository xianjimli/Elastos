
#ifndef __CTONEGENERATOR_H__
#define __CTONEGENERATOR_H__

#include "_CToneGenerator.h"

/**
 * This class provides methods to play DTMF tones (ITU-T Recommendation Q.23),
 * call supervisory tones (3GPP TS 22.001, CEPT) and proprietary tones (3GPP TS 31.111).
 * Depending on call state and routing options, tones are mixed to the downlink audio
 * or output to the speaker phone or headset.
 * This API is not for generating tones over the uplink audio path.
 */
CarClass(CToneGenerator)
{
public:
    ~CToneGenerator();

    CARAPI constructor(
        /* [in] */ Int32 streamType,
        /* [in] */ Int32 volume);

    CARAPI StartTone(
        /* [in] */ Int32 toneType,
        /* [out] */ Boolean* result);

    CARAPI StartToneEx(
        /* [in] */ Int32 toneType,
        /* [in] */ Int32 durationMs,
        /* [out] */ Boolean* result);

    /**
     * This method stops the tone currently playing playback.
     * @see #ToneGenerator(int, int)
     */
    CARAPI StopTone();

    /**
     * Releases resources associated with this ToneGenerator object. It is good
     * practice to call this method when you're done using the ToneGenerator.
     */
    CARAPI ReleaseResources();

private:
    CARAPI NativeSetup(
        /* [in] */ Int32 streamType,
        /* [in] */ Int32 volume);

    CARAPI_(void) NativeFinalize();

private:
    Int32 mNativeContext; // accessed by native methods
};

#endif // __CTONEGENERATOR_H__
