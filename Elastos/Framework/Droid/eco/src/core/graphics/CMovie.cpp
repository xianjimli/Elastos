
#include "graphics/CMovie.h"
#include "graphics/CMovieHelper.h"
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
