
#include "graphics/CMovie.h"
#include <skia/images/SkMovie.h>


SkMovie* gMoive = NULL;

ECode CMovie::DrawEx(
    /* [in] */ ICanvas * pCanvas,
    /* [in] */ Float x,
    /* [in] */ Float y)
{
    Draw(pCanvas, x, y, NULL);
    return NOERROR;
}

ECode CMovie::decodeFile(
    /* [in] */ const String& pathName,
    /* [out] */ IMovie ** ppMovie)
{
    IInputStream* is;
    CFileInputStream::New(pathName, (IFileInputStream**)&is);
    if (is == NULL) {
        return E_RUNTIME_EXCEPTION;
    } else {
        decodeTempStream(is, ppMovie);
    }

    return NOERROR;
}

ECode CMovie::decodeTempStream(
    /* [in] */ IInputStream * pIs,
    /* [out] */ IMovie ** ppMovie)
{

    DecodeStream(pIs, ppMovie);
    pIs->Close();

    return NOERROR;
}

ECode CMovie::constructor(
    /* [in] */ Int32 nativeMovie)
{
    if (nativeMovie == 0) {
        return E_RUNTIME_EXCEPTION;
    }

    mNativeMovie = nativeMovie;
    return NOERROR;
}


ECode CMovie::Width(
    /* [out] */ Int32 * pWidth)
{
    SkASSERT(gMoive);
    *pWidth = gMoive->width();
    return NOERROR;
}

ECode CMovie::Height(
    /* [out] */ Int32 * pHeight)
{
    SkASSERT(gMoive);
    *pHeight = gMoive->height();
    return NOERROR;
}

ECode CMovie::IsOpaque(
    /* [out] */ Boolean * pIsOpaque)
{
    SkASSERT(gMoive);
    *pIsOpaque = gMoive->isOpaque();
    return NOERROR;
}

ECode CMovie::Duration(
    /* [out] */ Int32 * pHeight)
{
    SkASSERT(gMoive);
    *pHeight = gMoive->duration();
    return NOERROR;
}


ECode CMovie::SetTime(
    /* [in] */ Int32 relativeMilliseconds,
    /* [out] */ Boolean * pIsSet)
{
    SkASSERT(gMoive);
    *pIsSet = gMoive->setTime(relativeMilliseconds);
    return NOERROR;
}

ECode CMovie::Draw(
    /* [in] */ ICanvas * pCanvas,
    /* [in] */ Float x,
    /* [in] */ Float y,
    /* [in] */ IPaint * pPaint)
{
    if (pCanvas == NULL) {
        return NULL;
    }
    // its OK for paint to be null

    // SkMovie* m = J2Movie(env, movie);
    // SkCanvas* c = GraphicsJNI::getNativeCanvas(env, canvas);
    // SkScalar sx = SkFloatToScalar(fx);
    // SkScalar sy = SkFloatToScalar(fy);
    // const SkBitmap& b = m->bitmap();
    // const SkPaint* p = jpaint ? GraphicsJNI::getNativePaint(env, jpaint) : NULL;

    // c->drawBitmap(b, sx, sy, p);

    return NOERROR;
}

IMovie* create_jmovie(SkMovie* moov) {
    if (NULL == moov) {
        return NULL;
    }

    // jobject obj = env->AllocObject(gMovie_class);
    // if (obj) {
    //     env->CallVoidMethod(obj, gMovie_constructorMethodID, (jint)moov);
    // }

    return NULL;
}

ECode CMovie::DecodeStream(
    /* [in] */ IInputStream * pIs,
    /* [out] */ IMovie ** ppMovie)
{

    // what is the lifetime of the array? Can the skstream hold onto it?
    //Byte byteArray[16 * 1024];
    // SkStream* strm = CreateJavaInputStreamAdaptor(env, istream, byteArray);
    // if (NULL == strm) {
    //     return 0;
    // }

    // SkMovie* moov = SkMovie::DecodeStream(strm);
    // strm->unref();
    // return create_jmovie(env, moov);

    return NOERROR;
}

ECode CMovie::decodeByteArray(
                            ArrayOf<Byte> data,
                            Int32 offset,
                            Int32 length,
                            IMovie ** ppMovie)
{
    if (data == NULL) {
        return NULL;
    }

    Int32 totalLength = data.GetLength();
    if ((offset | length) < 0 || offset + length > totalLength) {
        return E_ARRAY_INDEX_OUT_OF_BOUNDS_EXCEPTION;
    }

    SkMovie* moov = SkMovie::DecodeMemory(data.GetPayload() + offset, length);
    *ppMovie = create_jmovie(moov);

    return NOERROR;
}
