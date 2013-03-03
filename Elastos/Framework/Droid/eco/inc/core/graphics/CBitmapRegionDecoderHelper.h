
#ifndef __CBITMAPREGIONDECODERHELPER_H__
#define __CBITMAPREGIONDECODERHELPER_H__

#include "CBitmapRegionDecoder.h"
#include "_CBitmapRegionDecoderHelper.h"
#include <skia/core/SkStream.h>

CarClass(CBitmapRegionDecoderHelper)
{
public:
    CARAPI NewInstance(
        /* [in] */ const ArrayOf<Byte>& data,
        /* [in] */ Int32 offset,
        /* [in] */ Int32 length,
        /* [in] */ Boolean isShareable,
        /* [out] */ IBitmapRegionDecoder** decoder);

    CARAPI NewInstanceEx(
        /* [in] */ IFileDescriptor* fd,
        /* [in] */ Boolean isShareable,
        /* [out] */ IBitmapRegionDecoder** decoder);

    CARAPI NewInstanceEx2(
        /* [in] */ IInputStream* is,
        /* [in] */ Boolean isShareable,
        /* [out] */ IBitmapRegionDecoder** decoder);

    CARAPI NewInstanceEx3(
        /* [in] */ const String& pathName,
        /* [in] */ Boolean isShareable,
        /* [out] */ IBitmapRegionDecoder** decoder);
};

#endif // __CBITMAPREGIONDECODERHELPER_H__
