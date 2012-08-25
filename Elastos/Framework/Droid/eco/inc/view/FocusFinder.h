
#ifndef __FOCUSFINDER_H__
#define __FOCUSFINDER_H__

#include "ext/frameworkext.h"
#include <pthread.h>
#include <elastos/AutoPtr.h>

/**
 * The algorithm used for finding the next focusable view in a given direction
 * from a view that currently has focus.
 */
class FocusFinder
{
public:
    static CARAPI_(FocusFinder*) GetInstance();

protected:
    static CARAPI_(Int32) MajorAxisDistance(
        /* [in] */ Int32 direction,
        /* [in] */ IRect* source,
        /* [in] */ IRect* dest);

    static CARAPI_(Int32) MajorAxisDistanceRaw(
        /* [in] */ Int32 direction,
        /* [in] */ IRect* source,
        /* [in] */ IRect* dest);

    static CARAPI_(Int32) MajorAxisDistanceToFarEdge(
        /* [in] */ Int32 direction,
        /* [in] */ IRect* source,
        /* [in] */ IRect* dest);

    static CARAPI_(Int32) MajorAxisDistanceToFarEdgeRaw(
        /* [in] */ Int32 direction,
        /* [in] */ IRect* source,
        /* [in] */ IRect* dest);

    static CARAPI_(Int32) MinorAxisDistance(
        /* [in] */ Int32 direction,
        /* [in] */ IRect* source,
        /* [in] */ IRect* dest);

private:
    FocusFinder();

public:
    CARAPI_(AutoPtr<IView>) FindNextFocus(
        /* [in] */ IViewGroup* root,
        /* [in] */ IView* focused,
        /* [in] */ Int32 direction);

    CARAPI_(AutoPtr<IView>) FindNextFocusFromRect(
        /* [in] */ IViewGroup* root,
        /* [in] */ IRect* focusedRect,
        /* [in] */ Int32 direction);

    CARAPI_(AutoPtr<IView>) FindNearestTouchable(
        /* [in] */ IViewGroup* root,
        /* [in] */ Int32 x,
        /* [in] */ Int32 y,
        /* [in] */ Int32 direction,
        /* [in] */ Int32* deltas);

protected:
    CARAPI_(Boolean) IsBetterCandidate(
        /* [in] */ Int32 direction,
        /* [in] */ IRect* source,
        /* [in] */ IRect* rect1,
        /* [in] */ IRect* rect2);

    CARAPI_(Boolean) BeamBeats(
        /* [in] */ Int32 direction,
        /* [in] */ IRect* source,
        /* [in] */ IRect* rect1,
        /* [in] */ IRect* rect2);

    CARAPI_(Int32) GetWeightedDistanceFor(
        /* [in] */ Int32 majorAxisDistance,
        /* [in] */ Int32 minorAxisDistance);

    CARAPI_(Boolean) IsCandidate(
        /* [in] */ IRect* srcRect,
        /* [in] */ IRect* destRect,
        /* [in] */ Int32 direction);

    CARAPI_(Boolean) BeamsOverlap(
        /* [in] */ Int32 direction,
        /* [in] */ IRect* rect1,
        /* [in] */ IRect* rect2);

    CARAPI_(Boolean) IsToDirectionOf(
        /* [in] */ Int32 direction,
        /* [in] */ IRect* src,
        /* [in] */ IRect* dest);

private:
    CARAPI_(AutoPtr<IView>) FindNextFocus(
        /* [in] */ IViewGroup* root,
        /* [in] */ IView* focused,
        /* [in] */ IRect* focusedRect,
        /* [in] */ Int32 direction);

    CARAPI_(Boolean) IsTouchCandidate(
        /* [in] */ Int32 x,
        /* [in] */ Int32 y,
        /* [in] */ IRect* dest,
        /* [in] */ Int32 direction);

protected:
    AutoPtr<IRect> mFocusedRect;
    AutoPtr<IRect> mOtherRect;
    AutoPtr<IRect> mBestCandidateRect;

private:
    static pthread_key_t sKeyFocusFinder;
};
#endif  //__FOCUSFINDER_H__
