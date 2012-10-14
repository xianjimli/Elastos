
#ifndef __SCROLLBARDRAWABLE_H__
#define __SCROLLBARDRAWABLE_H__

#include "ext/frameworkext.h"
#include "graphics/drawable/Drawable.h"
#include <elastos/AutoPtr.h>

class ScrollBarDrawable : public Drawable
{
public:
    ScrollBarDrawable();

    /**
     * Indicate whether the horizontal scrollbar track should always be drawn regardless of the
     * extent. Defaults to false.
     *
     * @param alwaysDrawTrack Set to true if the track should always be drawn
     */
    CARAPI_(void) SetAlwaysDrawHorizontalTrack(
        /* [in] */ Boolean alwaysDrawTrack);

    /**
     * Indicate whether the vertical scrollbar track should always be drawn regardless of the
     * extent. Defaults to false.
     *
     * @param alwaysDrawTrack Set to true if the track should always be drawn
     */
    CARAPI_(void) SetAlwaysDrawVerticalTrack(
        /* [in] */ Boolean alwaysDrawTrack);

    /**
     * Indicates whether the horizontal scrollbar track should always be drawn regardless of the
     * extent.
     */
    CARAPI_(Boolean) GetAlwaysDrawHorizontalTrack();

    /**
     * Indicates whether the vertical scrollbar track should always be drawn regardless of the
     * extent.
     */
    CARAPI_(Boolean) GetAlwaysDrawVerticalTrack();

    CARAPI_(void) SetParameters(
        /* [in] */ Int32 range,
        /* [in] */ Int32 offset,
        /* [in] */ Int32 extent,
        /* [in] */ Boolean vertical);

    CARAPI Draw(
        /* [in] */ ICanvas* canvas);

    CARAPI_(void) SetVerticalThumbDrawable(
        /* [in] */ IDrawable* thumb);

    CARAPI_(void) SetVerticalTrackDrawable(
        /* [in] */ IDrawable* track);

    CARAPI_(void) SetHorizontalThumbDrawable(
        /* [in] */ IDrawable* thumb);

    CARAPI_(void) SetHorizontalTrackDrawable(
        /* [in] */ IDrawable* track);

    CARAPI_(Int32) GetSize(
        /* [in] */ Boolean vertical);

    CARAPI SetAlpha(
        /* [in] */ Int32 alpha);

    CARAPI SetColorFilter(
        /* [in] */ IColorFilter* cf);

    CARAPI_(Int32) GetOpacity();

protected:
    virtual CARAPI_(void) OnBoundsChange(
        /* [in] */ IRect* bounds);

    CARAPI_(void) DrawTrack(
        /* [in] */ ICanvas* canvas,
        /* [in] */ IRect* bounds,
        /* [in] */ Boolean vertical);

    CARAPI_(void) DrawThumb(
        /* [in] */ ICanvas* canvas,
        /* [in] */ IRect* bounds,
        /* [in] */ Int32 offset,
        /* [in] */ Int32 length,
        /* [in] */ Boolean vertical);

private:
    AutoPtr<IDrawable> mVerticalTrack;
    AutoPtr<IDrawable> mHorizontalTrack;
    AutoPtr<IDrawable> mVerticalThumb;
    AutoPtr<IDrawable> mHorizontalThumb;
    Int32 mRange;
    Int32 mOffset;
    Int32 mExtent;
    Boolean mVertical;
    Boolean mChanged;
    Boolean mRangeChanged;
    AutoPtr<IRect> mTempBounds;
    Boolean mAlwaysDrawHorizontalTrack;
    Boolean mAlwaysDrawVerticalTrack;
};

#endif //__SCROLLBARDRAWABLE_H__
