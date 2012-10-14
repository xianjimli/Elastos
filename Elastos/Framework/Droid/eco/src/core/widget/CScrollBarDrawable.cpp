
#include "widget/CScrollBarDrawable.h"

IDRAWABLE_METHODS_IMPL(CScrollBarDrawable, ScrollBarDrawable, ScrollBarDrawable);

PInterface CScrollBarDrawable::Probe(
    /* [in] */ REIID riid)
{
    return _CScrollBarDrawable::Probe(riid);
}

ECode CScrollBarDrawable::SetAlwaysDrawHorizontalTrack(
    /* [in] */ Boolean alwaysDrawTrack)
{
    ScrollBarDrawable::SetAlwaysDrawHorizontalTrack(alwaysDrawTrack);
    return NOERROR;
}

/**
 * Indicate whether the vertical scrollbar track should always be drawn regardless of the
 * extent. Defaults to false.
 *
 * @param alwaysDrawTrack Set to true if the track should always be drawn
 */
ECode CScrollBarDrawable::SetAlwaysDrawVerticalTrack(
    /* [in] */ Boolean alwaysDrawTrack)
{
    ScrollBarDrawable::SetAlwaysDrawVerticalTrack(alwaysDrawTrack);
    return NOERROR;
}

/**
 * Indicates whether the horizontal scrollbar track should always be drawn regardless of the
 * extent.
 */
ECode CScrollBarDrawable::GetAlwaysDrawHorizontalTrack(
    /* [out] */ Boolean* alwaysDrawTrack)
{
    VALIDATE_NOT_NULL(alwaysDrawTrack);

    *alwaysDrawTrack = ScrollBarDrawable::GetAlwaysDrawHorizontalTrack();
    return NOERROR;
}

/**
 * Indicates whether the vertical scrollbar track should always be drawn regardless of the
 * extent.
 */
ECode CScrollBarDrawable::GetAlwaysDrawVerticalTrack(
    /* [out] */ Boolean* alwaysDrawTrack)
{
    VALIDATE_NOT_NULL(alwaysDrawTrack);

    *alwaysDrawTrack = ScrollBarDrawable::GetAlwaysDrawVerticalTrack();
    return NOERROR;
}

ECode CScrollBarDrawable::SetParameters(
    /* [in] */ Int32 range,
    /* [in] */ Int32 offset,
    /* [in] */ Int32 extent,
    /* [in] */ Boolean vertical)
{
    ScrollBarDrawable::SetParameters(range, offset, extent, vertical);
    return NOERROR;
}

ECode CScrollBarDrawable::SetVerticalThumbDrawable(
    /* [in] */ IDrawable* thumb)
{
    ScrollBarDrawable::SetVerticalThumbDrawable(thumb);
    return NOERROR;
}

ECode CScrollBarDrawable::SetVerticalTrackDrawable(
    /* [in] */ IDrawable* track)
{
    ScrollBarDrawable::SetVerticalTrackDrawable(track);
    return NOERROR;
}

ECode CScrollBarDrawable::SetHorizontalThumbDrawable(
    /* [in] */ IDrawable* thumb)
{
    ScrollBarDrawable::SetHorizontalThumbDrawable(thumb);
    return NOERROR;
}

ECode CScrollBarDrawable::SetHorizontalTrackDrawable(
    /* [in] */ IDrawable* track)
{
    ScrollBarDrawable::SetHorizontalTrackDrawable(track);
    return NOERROR;
}

ECode CScrollBarDrawable::GetSize(
    /* [in] */ Boolean vertical,
    /* [out] */ Int32* size)
{
    VALIDATE_NOT_NULL(size);

    *size = ScrollBarDrawable::GetSize(vertical);
    return NOERROR;
}
