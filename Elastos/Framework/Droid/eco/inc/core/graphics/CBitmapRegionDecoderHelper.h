
#ifndef __CBITMAPREGIONDECODERHELPER_H__
#define __CBITMAPREGIONDECODERHELPER_H__

#include "CBitmapRegionDecoder.h"
#include "_CBitmapRegionDecoderHelper.h"
#include <skia/core/SkStream.h>

CarClass(CBitmapRegionDecoderHelper)
{
public:
    CARAPI NewInstance(
        /* [in] */ const ArrayOf<Byte> & data,
        /* [in] */ Int32 offset,
        /* [in] */ Int32 length,
        /* [in] */ Boolean isShareable,
        /* [out] */ IBitmapRegionDecoder ** ppIBrd);

    CARAPI NewInstanceEx(
        /* [in] */ IFileDescriptor * pFd,
        /* [in] */ Boolean isShareable,
        /* [out] */ IBitmapRegionDecoder ** ppIBrd);

    CARAPI NewInstanceEx2(
        /* [in] */ IInputStream * pIs,
        /* [in] */ Boolean isShareable,
        /* [out] */ IBitmapRegionDecoder ** ppIBrd);

    CARAPI NewInstanceEx3(
        /* [in] */ const String& pathName,
        /* [in] */ Boolean isShareable,
        /* [out] */ IBitmapRegionDecoder ** ppIBrd);

private:
    static IBitmapRegionDecoder* NativeNewInstance(
        /* [in] */ const ArrayOf<Byte> & data,
        /* [in] */ Int32 offset,
        /* [in] */ Int32 length,
        /* [in] */ Boolean isShareable);

    static IBitmapRegionDecoder* NativeNewInstance(
        /* [in] */ IFileDescriptor* fd,
        /* [in] */ Boolean isShareable);

    static IBitmapRegionDecoder* NativeNewInstance(
        /* [in] */ IInputStream* is,
        /* [in] */ //ArrayOf<Byte> & storage,
        /* [in] */ Byte* storage,
        /* [in] */ Boolean isShareable);

    static IBitmapRegionDecoder* NativeNewInstance(
        /* [in] */ Int32 asset,
        /* [in] */ Boolean isShareable);

    static IBitmapRegionDecoder* DoBuildTileIndex(
        /* [in] */ SkStream* stream);

    static SkMemoryStream* BuildSkMemoryStream(
        /* [in] */ SkStream *stream);
};

#endif // __CBITMAPREGIONDECODERHELPER_H__
