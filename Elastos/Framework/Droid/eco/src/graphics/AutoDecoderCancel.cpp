#include "graphics/AutoDecoderCancel.h"
#include <elastos/Mutex.h>

using namespace Elastos::System::Threading;

static Mutex  gAutoDecoderCancelMutex;
static AutoDecoderCancel* gAutoDecoderCancel;
#ifdef SK_DEBUG
static int gAutoDecoderCancelCount;
#endif

AutoDecoderCancel::AutoDecoderCancel(
    /* [in] */ IBitmapFactoryOptions* options,
    /* [in] */ SkImageDecoder* decoder)
{
    mOptions = options;
    mDecoder = decoder;

    if (NULL != options) {
        Mutex::Autolock lock(gAutoDecoderCancelMutex);

        // Add us as the head of the list
        mPrev = NULL;
        mNext = gAutoDecoderCancel;
        if (gAutoDecoderCancel) {
            gAutoDecoderCancel->mPrev = this;
        }
        gAutoDecoderCancel = this;

        SkDEBUGCODE(gAutoDecoderCancelCount += 1;)
        Validate();
    }
}

AutoDecoderCancel::~AutoDecoderCancel() {
    if (NULL != mOptions) {
        Mutex::Autolock lock(gAutoDecoderCancelMutex);

        // take us out of the dllist
        AutoDecoderCancel* prev = mPrev;
        AutoDecoderCancel* next = mNext;

        if (prev) {
            SkASSERT(prev->mNext == this);
            prev->mNext = next;
        } else {
            SkASSERT(gAutoDecoderCancel == this);
            gAutoDecoderCancel = next;
        }
        if (next) {
            SkASSERT(next->mPrev == this);
            next->mPrev = prev;
        }

        SkDEBUGCODE(gAutoDecoderCancelCount -= 1;)
        Validate();
    }
}

bool AutoDecoderCancel::RequestCancel(
    /* [in] */ IBitmapFactoryOptions* options)
{
    Mutex::Autolock lock(gAutoDecoderCancelMutex);

    Validate();

    AutoDecoderCancel* pair = gAutoDecoderCancel;
    while (pair != NULL) {
        if (pair->mOptions.Get() == options) {
            pair->mDecoder->cancelDecode();
            return true;
        }
        pair = pair->mNext;
    }
    return false;
}

#ifdef SK_DEBUG
// can only call this inside a lock on gAutoDecoderCancelMutex
void AutoDecoderCancel::Validate() {
    const int gCount = gAutoDecoderCancelCount;

    if (gCount == 0) {
        SkASSERT(gAutoDecoderCancel == NULL);
    } else {
        SkASSERT(gCount > 0);

        AutoDecoderCancel* curr = gAutoDecoderCancel;
        SkASSERT(curr);
        SkASSERT(curr->mPrev == NULL);

        int count = 0;
        while (curr) {
            count += 1;
            SkASSERT(count <= gCount);
            if (curr->mPrev) {
                SkASSERT(curr->mPrev->mNext == curr);
            }
            if (curr->mNext) {
                SkASSERT(curr->mNext->mPrev == curr);
            }
            curr = curr->mNext;
        }
        SkASSERT(count == gCount);
    }
}
#endif

