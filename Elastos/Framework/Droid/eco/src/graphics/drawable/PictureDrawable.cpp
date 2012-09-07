
#include "graphics/drawable/PictureDrawable.h"
#include "graphics/ElPixelFormat.h"
#include "graphics/CRect.h"


PictureDrawable::PictureDrawable()
{}

/**
 * Construct a new drawable referencing the specified picture. The picture
 * may be null.
 *
 * @param picture The picture to associate with the drawable. May be null.
 */
PictureDrawable::PictureDrawable(
    /* [in] */ IPicture* picture)
    : mPicture(picture)
{}

/**
 * Return the picture associated with the drawable. May be null.
 *
 * @return the picture associated with the drawable, or null.
 */
AutoPtr<IPicture> PictureDrawable::GetPicture()
{
    return mPicture;
}

/**
 * Associate a picture with this drawable. The picture may be null.
 *
 * @param picture The picture to associate with the drawable. May be null.
 */
ECode PictureDrawable::SetPicture(
    /* [in] */ IPicture* picture)
{
    mPicture = picture;
    return NOERROR;
}

//@Override
ECode PictureDrawable::Draw(
    /* [in] */ ICanvas* canvas)
{
    if (mPicture != NULL) {
        AutoPtr<IRect> bounds = GetBounds();
        Int32 count;
        canvas->Save(&count);
        Boolean isNotEmpty;
        canvas->ClipRectEx3(bounds, &isNotEmpty);
        canvas->Translate(
                ((CRect*)bounds.Get())->mLeft, ((CRect*)bounds.Get())->mTop);
        canvas->DrawPicture(mPicture);
        canvas->Restore();
    }
    return NOERROR;
}

//@Override
Int32 PictureDrawable::GetIntrinsicWidth()
{
    Int32 width = -1;
    if (mPicture != NULL) mPicture->GetWidth(&width);
    return width;
}

//@Override
Int32 PictureDrawable::GetIntrinsicHeight()
{
    Int32 height = -1;
    if (mPicture != NULL) mPicture->GetHeight(&height);
    return height;
}

//@Override
Int32 PictureDrawable::GetOpacity()
{
    // not sure, so be safe
    return ElPixelFormat::TRANSLUCENT;
}

//@Override
ECode PictureDrawable::SetFilterBitmap(
    /* [in] */ Boolean filter)
{
    return NOERROR;
}

//@Override
ECode PictureDrawable::SetDither(
    /* [in] */ Boolean dither)
{
    return NOERROR;
}

//@Override
ECode PictureDrawable::SetColorFilter(
    /* [in] */ IColorFilter* colorFilter)
{
    return NOERROR;
}

//@Override
ECode PictureDrawable::SetAlpha(
    /* [in */ Int32 alpha)
{
    return NOERROR;
}

ECode PictureDrawable::Init(
    /* [in] */ IPicture* picture)
{
    mPicture = picture;
    return NOERROR;
}
