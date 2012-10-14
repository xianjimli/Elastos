
#ifndef _CBITMAPFACTORY_H__
#define _CBITMAPFACTORY_H__

#include "_CBitmapFactory.h"
#include <skia/core/SkStream.h>
#include <skia/images/SkImageDecoder.h>
#include <utils/Asset.h>

CarClass(CBitmapFactory)
{
public:
    CARAPI CreateBitmap(
        /* [in] */ IBitmap* source,
        /* [out] */ IBitmap** bitmap);

    CARAPI CreateBitmapEx(
        /* [in] */ IBitmap* source,
        /* [in] */ Int32 x,
        /* [in] */ Int32 y,
        /* [in] */ Int32 width,
        /* [in] */ Int32 height,
        /* [out] */ IBitmap** bitmap);

    CARAPI CreateBitmapEx2(
        /* [in] */ IBitmap* source,
        /* [in] */ Int32 x,
        /* [in] */ Int32 y,
        /* [in] */ Int32 width,
        /* [in] */ Int32 height,
        /* [in] */ IMatrix* m,
        /* [in] */ Boolean filter,
        /* [out] */ IBitmap** bitmap);

    CARAPI CreateBitmapEx3(
        /* [in] */ Int32 width,
        /* [in] */ Int32 height,
        /* [in] */ BitmapConfig config,
        /* [out] */ IBitmap** bitmap);

    CARAPI CreateBitmapEx4(
        /* [in] */ const ArrayOf<Int32>& colors,
        /* [in] */ Int32 offset,
        /* [in] */ Int32 stride,
        /* [in] */ Int32 width,
        /* [in] */ Int32 height,
        /* [in] */ BitmapConfig config,
        /* [out] */ IBitmap** bitmap);

    CARAPI CreateBitmapEx5(
        /* [in] */ const ArrayOf<Int32>& colors,
        /* [in] */ Int32 width,
        /* [in] */ Int32 height,
        /* [in] */ BitmapConfig config,
        /* [out] */ IBitmap** bitmap);

    CARAPI DecodeFile(
        /* [in] */ const String& pathName,
        /* [in] */ IBitmapFactoryOptions * pOpts,
        /* [out] */ IBitmap ** ppBitmap);

    CARAPI DecodeFileEx(
        /* [in] */ const String& pathName,
        /* [out] */ IBitmap ** ppBitmap);

    CARAPI DecodeResourceStream(
        /* [in] */ IResources * pRes,
        /* [in] */ ITypedValue * pValue,
        /* [in] */ IInputStream * pIs,
        /* [in] */ IRect * pPad,
        /* [in] */ IBitmapFactoryOptions * pOpts,
        /* [out] */ IBitmap ** ppBitmap);

    CARAPI DecodeResource(
        /* [in] */ IResources* res,
        /* [in] */ Int32 id,
        /* [in] */ IBitmapFactoryOptions* opts,
        /* [out] */ IBitmap** bitmap);

    CARAPI DecodeResourceEx(
        /* [in] */ IResources* res,
        /* [in] */ Int32 id,
        /* [out] */ IBitmap** bitmap);

    CARAPI DecodeByteArray(
        /* [in] */ const ArrayOf<Byte>& data,
        /* [in] */ Int32 offset,
        /* [in] */ Int32 length,
        /* [in] */ IBitmapFactoryOptions* opts,
        /* [out] */ IBitmap** bitmap);

    CARAPI DecodeByteArrayEx(
        /* [in] */ const ArrayOf<Byte>& data,
        /* [in] */ Int32 offset,
        /* [in] */ Int32 length,
        /* [out] */ IBitmap** bitmap);

    CARAPI DecodeStream(
        /* [in] */ IInputStream* is,
        /* [in] */ IRect* outPadding,
        /* [in] */ IBitmapFactoryOptions* opts,
        /* [out] */ IBitmap** bitmap);

    CARAPI DecodeStreamEx(
        /* [in] */ IInputStream* is,
        /* [out] */ IBitmap** bitmap);

    CARAPI DecodeFileDescriptor(
        /* [in] */ IFileDescriptor* fd,
        /* [in] */ IRect* outPadding,
        /* [in] */ IBitmapFactoryOptions* opts,
        /* [out] */ IBitmap** bitmap);

    CARAPI DecodeFileDescriptorEx(
        /* [in] */ IFileDescriptor* fd,
        /* [out] */ IBitmap** bitmap);

    CARAPI SetDefaultConfig(
        /* [in] */ BitmapConfig config);

private:
    CARAPI FinishDecode(
        /* [in] */ IBitmap* bm,
        /* [in] */ IRect* outPadding,
        /* [in] */ IBitmapFactoryOptions* opts,
        /* [out] */ IBitmap ** ppBitmap);

    CARAPI NativeDecodeStream(
        /* [in] */ IInputStream* pIs,
        /* [in] */ ArrayOf<Byte>* tempStorage,
        /* [in] */ IRect* pOutPadding,
        /* [in] */ IBitmapFactoryOptions* pOpts,
        /* [out] */ IBitmap ** ppBitmap);

    static CARAPI DoDecode(
        /* [in] */ SkStream* stream,
        /* [in] */ IRect* padding,
        /* [in] */ IBitmapFactoryOptions* options,
        /* [in] */ Boolean allowPurgeable,
        /* [in] */ Boolean forcePurgeable, // default is false;
        /* [out] */ IBitmap ** ppBitmap);

    static CARAPI_(SkPixelRef*) InstallPixelRef(
        /* [in] */ SkBitmap* bitmap,
        /* [in] */ SkStream* stream,
        /* [in] */ Int32 sampleSize,
        /* [in] */ Boolean ditherImage);

    static CARAPI_(void) GetMimeTypeString(
        /* [in] */ SkImageDecoder::Format format,
        /* [out] */ String* typeStr);

    /*  make a deep copy of the asset, and return it as a stream, or NULL if there
        was an error.
     */
    static SkStream* CopyAssetToStream(
        /* [in] */ android::Asset* asset);

    static CARAPI_(void) NativeSetDefaultConfig(
        /* [in] */ Int32 nativeConfig);

    static CARAPI NativeDecodeFileDescriptor(
        /* [in] */ IFileDescriptor* fd,
        /* [in] */ IRect* padding,
        /* [in] */ IBitmapFactoryOptions* opts,
        /* [out] */ IBitmap** bitmap);

    static CARAPI NativeDecodeAsset(
        /* [in] */ Int32 native_asset,
        /* [in] */ IRect* padding,
        /* [in] */ IBitmapFactoryOptions* opts,
        /* [out] */ IBitmap** bitmap);

    static CARAPI NativeDecodeByteArray(
        /* [in] */ const ArrayOf<Byte>& data,
        /* [in] */ Int32 offset,
        /* [in] */ Int32 length,
        /* [in] */ IBitmapFactoryOptions* opts,
        /* [out] */ IBitmap** bitmap);

    static CARAPI NativeScaleNinePatch(
        /* [in] */ const ArrayOf<Byte>& chunkObject,
        /* [in] */ Float scale,
        /* [in] */ IRect* padding);

private:
    static const char* TAG;
};

#endif //_CBITMAPFACTORY_H__

