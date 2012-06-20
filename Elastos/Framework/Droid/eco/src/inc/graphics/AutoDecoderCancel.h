#ifndef __HH_AUTODECODERCANCEL_H
#define __HH_AUTODECODERCANCEL_H

//	#include <jni.h>
#include <ext/frameworkext.h>
#include <elastos/AutoPtr.h>
#include <skia/images/SkImageDecoder.h>

using namespace Elastos;

class AutoDecoderCancel {
public:
    AutoDecoderCancel(
        /* [in] */ IBitmapFactoryOptions* options,
        /* [in] */ SkImageDecoder* decoder);

    ~AutoDecoderCancel();

    bool RequestCancel(
        /* [in] */ IBitmapFactoryOptions* options);

private:
    AutoDecoderCancel*              mNext;
    AutoDecoderCancel*              mPrev;
    AutoPtr<IBitmapFactoryOptions>  mOptions;
    SkImageDecoder*                 mDecoder;

#ifdef SK_DEBUG
    static void Validate();
#else
    static void Validate() {}
#endif
};

#endif // __HH_AUTODECODERCANCEL_H

