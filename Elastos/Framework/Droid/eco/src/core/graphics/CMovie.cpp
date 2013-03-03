
#include "ext/frameworkdef.h"
#include "graphics/CMovie.h"
#include "graphics/CMovieHelper.h"
#include "graphics/Canvas.h"
#include "graphics/Paint.h"
#include <elastos/AutoPtr.h>
#include <skia/images/SkMovie.h>

ECode CMovie::constructor(
    /* [in] */ Handle32 nativeMovie)
{
    if (nativeMovie == 0) {
        // throw new RuntimeException("native movie creation failed");
        return E_RUNTIME_EXCEPTION;
    }
    mNativeMovie = nativeMovie;
    return NOERROR;
}

ECode CMovie::Width(
    /* [out] */ Int32* width)
{
    VALIDATE_NOT_NULL(width);

    *width = ((SkMovie*)mNativeMovie)->width();
    return NOERROR;
}

ECode CMovie::Height(
    /* [out] */ Int32* height)
{
    VALIDATE_NOT_NULL(height);

    *height = ((SkMovie*)mNativeMovie)->height();
    return NOERROR;
}

ECode CMovie::IsOpaque(
    /* [out] */ Boolean* isOpaque)
{
    VALIDATE_NOT_NULL(isOpaque);

    *isOpaque = ((SkMovie*)mNativeMovie)->isOpaque();
    return NOERROR;
}

ECode CMovie::Duration(
    /* [out] */ Int32* height)
{
    VALIDATE_NOT_NULL(height);

    *height = ((SkMovie*)mNativeMovie)->duration();
    return NOERROR;
}


ECode CMovie::SetTime(
    /* [in] */ Int32 relativeMilliseconds,
    /* [out] */ Boolean* isSet)
{
    VALIDATE_NOT_NULL(isSet);

    *isSet = ((SkMovie*)mNativeMovie)->setTime(relativeMilliseconds);
    return NOERROR;
}

ECode CMovie::Draw(
    /* [in] */ ICanvas* canvas,
    /* [in] */ Float x,
    /* [in] */ Float y,
    /* [in] */ IPaint* paint)
{
    assert(canvas != NULL);
    // its OK for paint to be null

    SkMovie* m = (SkMovie*)mNativeMovie;
    SkCanvas* c = ((Canvas*)canvas->Probe(EIID_Canvas))->mNativeCanvas;
    SkScalar sx = SkFloatToScalar(x);
    SkScalar sy = SkFloatToScalar(y);
    const SkBitmap& b = m->bitmap();
    const SkPaint* p = paint ? ((Paint*)paint->Probe(EIID_Paint))->mNativePaint : NULL;

    c->drawBitmap(b, sx, sy, p);
    return NOERROR;
}

ECode CMovie::DrawEx(
    /* [in] */ ICanvas* canvas,
    /* [in] */ Float x,
    /* [in] */ Float y)
{
    return Draw(canvas, x, y, NULL);
}

static ECode CreateMovie(
    /* [in] */ SkMovie* moov,
    /* [out] */ IMovie** movie)
{
    if (NULL == moov) {
        *movie = NULL;
        return NOERROR;
    }

    return CMovie::New((Handle32)moov, movie);
}

ECode CMovie::DecodeStream(
    /* [in] */ IInputStream* is,
    /* [out] */ IMovie** movie)
{
    VALIDATE_NOT_NULL(movie);

    if (is == NULL) return E_NULL_POINTER_EXCEPTION;

    // what is the lifetime of the array? Can the skstream hold onto it?
    //Byte byteArray[16 * 1024];
    // SkStream* strm = CreateJavaInputStreamAdaptor(env, istream, byteArray);
    // if (NULL == strm) {
    //     return 0;
    // }

    // SkMovie* moov = SkMovie::DecodeStream(strm);
    // strm->unref();
    // return create_jmovie(env, moov);
    assert(0);
    return E_NOT_IMPLEMENTED;
}

ECode CMovie::DecodeByteArray(
    /* [in] */ const ArrayOf<Byte>& data,
    /* [in] */ Int32 offset,
    /* [in] */ Int32 length,
    /* [out] */ IMovie** movie)
{
    VALIDATE_NOT_NULL(movie);

    Int32 totalLength = data.GetLength();
    if ((offset | length) < 0 || offset + length > totalLength) {
        // doThrow(env, "java/lang/ArrayIndexOutOfBoundsException");
        return E_ARRAY_INDEX_OUT_OF_BOUNDS_EXCEPTION;
    }

    SkMovie* moov = SkMovie::DecodeMemory(data.GetPayload() + offset, length);
    return CreateMovie(moov, movie);
}

ECode CMovie::DecodeFile(
    /* [in] */ const String& pathName,
    /* [out] */ IMovie** movie)
{
    VALIDATE_NOT_NULL(movie);

    AutoPtr<IInputStream> is;
    // try {
    CFileInputStream::New(pathName, (IFileInputStream**)&is);
    // }
    // catch (java.io.FileNotFoundException e) {
    //     return null;
    // }
    return DecodeTempStream(is, movie);
}

ECode CMovie::DecodeTempStream(
    /* [in] */ IInputStream* is,
    /* [out] */IMovie** movie)
{
    // try {
    ECode ec = DecodeStream(is, movie);
    is->Close();
    // }
    // catch (java.io.IOException e) {
        // /*  do nothing.
            // If the exception happened on open, moov will be null.
            // If it happened on close, moov is still valid.
        // */
    // }
    return ec;
}
