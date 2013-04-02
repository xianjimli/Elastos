
#ifndef __CTONEGENERATOR_H__
#define __CTONEGENERATOR_H__

#include "_CToneGenerator.h"

#include "media/ToneGenerator.h"

CarClass(CToneGenerator), public ToneGenerator
{
public:
    CARAPI StartTone(
        /* [in] */ Int32 toneType,
        /* [out] */ Boolean * pRes);

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

    CARAPI constructor(
        /* [in] */ Int32 streamType,
        /* [in] */ Int32 volume);

private:
    // TODO: Add your private member variables here.
};

#endif // __CTONEGENERATOR_H__
