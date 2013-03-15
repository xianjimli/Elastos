
#include "view/FocusFinder.h"
#include "graphics/CRect.h"
#include "view/View.h"
#include "view/ViewGroup.h"
#include <elastos/Math.h>
#include <Logger.h>

using namespace Elastos::Utility;
using namespace Elastos::Utility::Logging;
using namespace Elastos::Core;

pthread_key_t FocusFinder::sKeyFocusFinder;

/**
 * Get the focus finder for this thread.
 */
FocusFinder* FocusFinder::GetInstance()
{
    FocusFinder* focusFinder = (FocusFinder*)pthread_getspecific(sKeyFocusFinder);
    if (focusFinder == NULL) {
        focusFinder = new FocusFinder;
        assert(pthread_key_create(&sKeyFocusFinder, NULL) == 0);
        assert(pthread_setspecific(sKeyFocusFinder, focusFinder) == 0);

    }
    assert(focusFinder);

    return focusFinder;
}

// enforce thread local access
FocusFinder::FocusFinder()
{
    assert(SUCCEEDED(CRect::New((IRect**)&mFocusedRect)));
    assert(SUCCEEDED(CRect::New((IRect**)&mOtherRect)));
    assert(SUCCEEDED(CRect::New((IRect**)&mBestCandidateRect)));
}

/**
 * Find the next view to take focus in root's descendants, starting from the view
 * that currently is focused.
 * @param root Contains focused
 * @param focused Has focus now.
 * @param direction Direction to look.
 * @return The next focusable view, or NULL if none exists.
 */
AutoPtr<IView> FocusFinder::FindNextFocus(
    /* [in] */ IViewGroup* root,
    /* [in] */ IView* focused,
    /* [in] */ Int32 direction)
{
    if (focused != NULL) {
        // check for user specified next focus
        AutoPtr<IView> userSetNextFocus
            = ((View*)focused->Probe(EIID_View))->FindUserSetNextFocus(
                root, direction);

        if (userSetNextFocus != NULL) {
            View* temp = (View*)userSetNextFocus->Probe(EIID_View);
            if (temp->IsFocusable() && (!temp->IsInTouchMode()
                || temp->IsFocusableInTouchMode())) {
                return userSetNextFocus;
            }
        }

        // fill in interesting rect from focused
        focused->GetFocusedRect(mFocusedRect);
        root->OffsetDescendantRectToMyCoords(focused, mFocusedRect);
    }
    else {
        // make up a rect at top left or bottom right of root
        switch (direction) {
        case View::FOCUS_RIGHT:
        case View::FOCUS_DOWN:
            {
                Int32 rootTop, rootLeft;
                root->GetScrollY(&rootTop);
                root->GetScrollX(&rootLeft);
                mFocusedRect->Set(rootLeft, rootTop, rootLeft, rootTop);
            }
            break;
        case View::FOCUS_LEFT:
        case View::FOCUS_UP:
            {
                Int32 scrollX, scrollY, width, height;
                root->GetScrollX(&scrollX);
                root->GetScrollY(&scrollY);
                root->GetWidth(&width);
                root->GetHeight(&height);

                Int32 rootBottom = scrollY + height;
                Int32 rootRight = scrollX + width;
                mFocusedRect->Set(rootRight, rootBottom,
                    rootRight, rootBottom);
            }
            break;
        default:
            break;
        }
    }

    return FindNextFocus(root, focused, mFocusedRect, direction);
}

/**
 * Find the next view to take focus in root's descendants, searching from
 * a particular rectangle in root's coordinates.
 * @param root Contains focusedRect.
 * @param focusedRect The starting point of the search.
 * @param direction Direction to look.
 * @return The next focusable view, or NULL if none exists.
 */
AutoPtr<IView> FocusFinder::FindNextFocusFromRect(
    /* [in] */ IViewGroup* root,
    /* [in] */ IRect* focusedRect,
    /* [in] */ Int32 direction)
{
    return FindNextFocus(root, NULL, focusedRect, direction);
}

AutoPtr<IView> FocusFinder::FindNextFocus(
    /* [in] */ IViewGroup* root,
    /* [in] */ IView* focused,
    /* [in] */ IRect* focusedRect,
    /* [in] */ Int32 direction)
{
    AutoPtr<IObjectContainer> focusables;
    root->GetFocusables(direction, (IObjectContainer**)&focusables);
    assert(focusables != NULL);

    // initialize the best candidate to something impossible
    // (so the first plausible view will become the best choice)
    //
    mBestCandidateRect->SetEx(focusedRect);
    switch(direction) {
        case View::FOCUS_LEFT:
            mBestCandidateRect->Offset(
                ((CRect*)focusedRect)->GetWidth() + 1, 0);
            break;
        case View::FOCUS_RIGHT:
            mBestCandidateRect->Offset(
                -(((CRect*)focusedRect)->GetWidth() + 1), 0);
            break;
        case View::FOCUS_UP:
            mBestCandidateRect->Offset(
                0, ((CRect*)focusedRect)->GetHeight() + 1);
            break;
        case View::FOCUS_DOWN:
            mBestCandidateRect->Offset(
                0, -(((CRect*)focusedRect)->GetHeight() + 1));
            break;
        default:
            break;
    }

    AutoPtr<IView> closest;

    AutoPtr<IObjectEnumerator> objEmu;
    focusables->GetObjectEnumerator((IObjectEnumerator**)&objEmu);
    assert(objEmu != NULL);

    Boolean isSucceeded = FALSE;
    while (objEmu->MoveNext(&isSucceeded), isSucceeded) {
        AutoPtr<IInterface> obj;
        objEmu->Current((IInterface**)&obj);
        IView* focusable = (IView*)obj->Probe(EIID_IView);

        // only interested in other non-root views
        if (focusable == focused
            || focusable == (IView*)root->Probe(EIID_IView))
            continue;

        // get visible bounds of other view in same coordinate system
        focusable->GetDrawingRect(mOtherRect);
        root->OffsetDescendantRectToMyCoords(focusable, mOtherRect);

        if (IsBetterCandidate(direction, focusedRect, mOtherRect, mBestCandidateRect)) {
            mBestCandidateRect->SetEx(mOtherRect);
            closest = focusable;
        }
    }

    return closest;
}

/**
 * Is rect1 a better candidate than rect2 for a focus search in a particular
 * direction from a source rect?  This is the core routine that determines
 * the order of focus searching.
 * @param direction the direction (up, down, left, right)
 * @param source The source we are searching from
 * @param rect1 The candidate rectangle
 * @param rect2 The current best candidate.
 * @return Whether the candidate is the new best.
 */
Boolean FocusFinder::IsBetterCandidate(
    /* [in] */ Int32 direction,
    /* [in] */ IRect* source,
    /* [in] */ IRect* rect1,
    /* [in] */ IRect* rect2)
{
    // to be a better candidate, need to at least be a candidate in the first
    // place :)
    if (!IsCandidate(source, rect1, direction)) {
        return FALSE;
    }

    // we know that rect1 is a candidate.. if rect2 is not a candidate,
    // rect1 is better
    if (!IsCandidate(source, rect2, direction)) {
        return TRUE;
    }

    // if rect1 is better by beam, it wins
    if (BeamBeats(direction, source, rect1, rect2)) {
        return TRUE;
    }

    // if rect2 is better, then rect1 cant' be :)
    if (BeamBeats(direction, source, rect2, rect1)) {
        return FALSE;
    }

    // otherwise, do fudge-tastic comparison of the major and minor axis
    return (GetWeightedDistanceFor(
                MajorAxisDistance(direction, source, rect1),
                MinorAxisDistance(direction, source, rect1))
            < GetWeightedDistanceFor(
                MajorAxisDistance(direction, source, rect2),
                MinorAxisDistance(direction, source, rect2)));
}

/**
 * One rectangle may be another candidate than another by virtue of being
 * exclusively in the beam of the source rect.
 * @return Whether rect1 is a better candidate than rect2 by virtue of it being in src's
 *      beam
 */
Boolean FocusFinder::BeamBeats(
    /* [in] */ Int32 direction,
    /* [in] */ IRect* source,
    /* [in] */ IRect* rect1,
    /* [in] */ IRect* rect2)
{
    const Boolean rect1InSrcBeam = BeamsOverlap(direction, source, rect1);
    const Boolean rect2InSrcBeam = BeamsOverlap(direction, source, rect2);

    // if rect1 isn't exclusively in the src beam, it doesn't win
    if (rect2InSrcBeam || !rect1InSrcBeam) {
        return FALSE;
    }

    // we know rect1 is in the beam, and rect2 is not

    // if rect1 is to the direction of, and rect2 is not, rect1 wins.
    // for example, for direction left, if rect1 is to the left of the source
    // and rect2 is below, then we always prefer the in beam rect1, since rect2
    // could be reached by going down.
    if (!IsToDirectionOf(direction, source, rect2)) {
        return TRUE;
    }

    // for horizontal directions, being exclusively in beam always wins
    if ((direction == View::FOCUS_LEFT || direction == View::FOCUS_RIGHT)) {
        return TRUE;
    }

    // for vertical directions, beams only beat up to a point:
    // now, as long as rect2 isn't completely closer, rect1 wins
    // e.g for direction down, completely closer means for rect2's top
    // edge to be closer to the source's top edge than rect1's bottom edge.
    return (MajorAxisDistance(direction, source, rect1)
            < MajorAxisDistanceToFarEdge(direction, source, rect2));
}

/**
 * Fudge-factor opportunity: how to calculate distance given major and minor
 * axis distances.  Warning: this fudge factor is finely tuned, be sure to
 * run all focus tests if you dare tweak it.
 */
Int32 FocusFinder::GetWeightedDistanceFor(
    /* [in] */ Int32 majorAxisDistance,
    /* [in] */ Int32 minorAxisDistance)
{
    return 13 * majorAxisDistance * majorAxisDistance
            + minorAxisDistance * minorAxisDistance;
}

/**
 * Is destRect a candidate for the next focus given the direction?  This
 * checks whether the dest is at least partially to the direction of (e.g left of)
 * from source.
 *
 * Includes an edge case for an empty rect (which is used in some cases when
 * searching from a point on the screen).
 */
Boolean FocusFinder::IsCandidate(
    /* [in] */ IRect* srcRect,
    /* [in] */ IRect* destRect,
    /* [in] */ Int32 direction)
{
    assert(srcRect && destRect);
    CRect* src = (CRect*)srcRect;
    CRect* dest = (CRect*)destRect;
    switch (direction) {
    case View::FOCUS_LEFT:
        return (src->mRight > dest->mRight || src->mLeft >= dest->mRight)
                && src->mLeft > dest->mLeft;
    case View::FOCUS_RIGHT:
        return (src->mLeft < dest->mLeft || src->mRight <= dest->mLeft)
                && src->mRight < dest->mRight;
    case View::FOCUS_UP:
        return (src->mBottom > dest->mBottom || src->mTop >= dest->mBottom)
                && src->mTop > dest->mTop;
    case View::FOCUS_DOWN:
        return (src->mTop < dest->mTop|| src->mBottom <= dest->mTop)
                && src->mBottom < dest->mBottom;
    default:
        Logger::E("FocusFinder",
            "direction must be one of {FOCUS_UP, FOCUS_DOWN, FOCUS_LEFT, FOCUS_RIGHT}.");
        assert(0);
        break;
    }

    return FALSE;
}

/**
 * Do the "beams" w.r.t the given direcition's axos of rect1 and rect2 overlap?
 * @param direction the direction (up, down, left, right)
 * @param rect1 The first rectangle
 * @param rect2 The second rectangle
 * @return whether the beams overlap
 */
Boolean FocusFinder::BeamsOverlap(
    /* [in] */ Int32 direction,
    /* [in] */ IRect* rect1,
    /* [in] */ IRect* rect2)
{
    assert(rect1 && rect2);
    CRect* crect1 = (CRect*)rect1;
    CRect* crect2 = (CRect*)rect2;
    switch (direction) {
    case View::FOCUS_LEFT:
    case View::FOCUS_RIGHT:
        return (crect2->mBottom >= crect1->mTop) && (crect2->mTop <= crect1->mBottom);
    case View::FOCUS_UP:
    case View::FOCUS_DOWN:
        return (crect2->mRight >= crect1->mLeft) && (crect2->mLeft <= crect1->mRight);
    default:
        Logger::E("FocusFinder",
            "direction must be one of {FOCUS_UP, FOCUS_DOWN, FOCUS_LEFT, FOCUS_RIGHT}.");
        assert(0);
        break;
    }

    return FALSE;
}

/**
 * e.g for left, is 'to left of'
 */
Boolean FocusFinder::IsToDirectionOf(
    /* [in] */ Int32 direction,
    /* [in] */ IRect* src,
    /* [in] */ IRect* dest)
{
    assert(src && dest);
    CRect* srcRect = (CRect*)src;
    CRect* destRect = (CRect*)dest;
    switch (direction) {
    case View::FOCUS_LEFT:
        return srcRect->mLeft >= destRect->mRight;
    case View::FOCUS_RIGHT:
        return srcRect->mRight <= destRect->mLeft;
    case View::FOCUS_UP:
        return srcRect->mTop >= destRect->mBottom;
    case View::FOCUS_DOWN:
        return srcRect->mBottom <= destRect->mTop;
    default:
        Logger::E("FocusFinder",
            "direction must be one of {FOCUS_UP, FOCUS_DOWN, FOCUS_LEFT, FOCUS_RIGHT}.");
        assert(0);
        break;
    }

    return FALSE;
}

/**
 * @return The distance from the edge furthest in the given direction
 *   of source to the edge nearest in the given direction of dest.  If the
 *   dest is not in the direction from source, return 0.
 */
Int32 FocusFinder::MajorAxisDistance(
    /* [in] */ Int32 direction,
    /* [in] */ IRect* source,
    /* [in] */ IRect* dest)
{
    return Math::Max(0, MajorAxisDistanceRaw(direction, source, dest));
}

Int32 FocusFinder::MajorAxisDistanceRaw(
    /* [in] */ Int32 direction,
    /* [in] */ IRect* source,
    /* [in] */ IRect* dest)
{
    assert(source && dest);
    CRect* sourceRect = (CRect*)source;
    CRect* destRect = (CRect*)dest;
    switch (direction) {
    case View::FOCUS_LEFT:
        return sourceRect->mLeft - destRect->mRight;
    case View::FOCUS_RIGHT:
        return destRect->mLeft - sourceRect->mRight;
    case View::FOCUS_UP:
        return sourceRect->mTop - destRect->mBottom;
    case View::FOCUS_DOWN:
        return destRect->mTop - sourceRect->mBottom;
    default:
        Logger::E("FocusFinder",
            "direction must be one of {FOCUS_UP, FOCUS_DOWN, FOCUS_LEFT, FOCUS_RIGHT}.");
        assert(0);
        break;
    }

    return FALSE;
}

/**
 * @return The distance along the major axis w.r.t the direction from the
 *   edge of source to the far edge of dest. If the
 *   dest is not in the direction from source, return 1 (to break ties with
 *   {@link #majorAxisDistance}).
 */
Int32 FocusFinder::MajorAxisDistanceToFarEdge(
    /* [in] */ Int32 direction,
    /* [in] */ IRect* source,
    /* [in] */ IRect* dest)
{
    return Math::Max(1, MajorAxisDistanceToFarEdgeRaw(direction, source, dest));
}

Int32 FocusFinder::MajorAxisDistanceToFarEdgeRaw(
    /* [in] */ Int32 direction,
    /* [in] */ IRect* source,
    /* [in] */ IRect* dest)
{
    assert(source && dest);
    CRect* sourceRect = (CRect*)source;
    CRect* destRect = (CRect*)dest;
    switch (direction) {
    case View::FOCUS_LEFT:
        return sourceRect->mLeft - destRect->mLeft;
    case View::FOCUS_RIGHT:
        return destRect->mRight - sourceRect->mRight;
    case View::FOCUS_UP:
        return sourceRect->mTop - destRect->mTop;
    case View::FOCUS_DOWN:
        return destRect->mBottom - sourceRect->mBottom;
    default:
        Logger::E("FocusFinder",
            "direction must be one of {FOCUS_UP, FOCUS_DOWN, FOCUS_LEFT, FOCUS_RIGHT}.");
        assert(0);
        break;
    }

    return FALSE;
}

/**
 * Find the distance on the minor axis w.r.t the direction to the nearest
 * edge of the destination rectange.
 * @param direction the direction (up, down, left, right)
 * @param source The source rect.
 * @param dest The destination rect.
 * @return The distance.
 */
Int32 FocusFinder::MinorAxisDistance(
    /* [in] */ Int32 direction,
    /* [in] */ IRect* source,
    /* [in] */ IRect* dest)
{
    assert(source && dest);
    CRect* sourceRect = (CRect*)source;
    CRect* destRect = (CRect*)dest;
    switch (direction) {
    case View::FOCUS_LEFT:
    case View::FOCUS_RIGHT:
        // the distance between the center verticals
        return Math::Abs(
            (sourceRect->mTop + sourceRect->GetHeight() / 2) -
            (destRect->mTop + destRect->GetHeight() / 2));
    case View::FOCUS_UP:
    case View::FOCUS_DOWN:
        // the distance between the center horizontals
        return Math::Abs(
            (sourceRect->mLeft + sourceRect->GetWidth() / 2) -
            (destRect->mLeft + destRect->GetWidth() / 2));
    default:
        Logger::E("FocusFinder",
            "direction must be one of {FOCUS_UP, FOCUS_DOWN, FOCUS_LEFT, FOCUS_RIGHT}.");
        assert(0);
        break;
    }

    return FALSE;
}

/**
 * Find the nearest touchable view to the specified view.
 *
 * @param root The root of the tree in which to search
 * @param x X coordinate from which to start the search
 * @param y Y coordinate from which to start the search
 * @param direction Direction to look
 * @param deltas Offset from the <x, y> to the edge of the nearest view. Note that this array
 *        may already be populated with values.
 * @return The nearest touchable view, or NULL if none exists.
 */
AutoPtr<IView> FocusFinder::FindNearestTouchable(
    /* [in] */ IViewGroup* root,
    /* [in] */ Int32 x,
    /* [in] */ Int32 y,
    /* [in] */ Int32 direction,
    /* [in] */ Int32* deltas)
{
    AutoPtr<IObjectContainer> touchables;
    root->GetTouchables((IObjectContainer**)&touchables);

    Int32 minDistance = Math::INT32_MAX_VALUE;

    AutoPtr<IContext> context;
    root->GetContext((IContext**)&context);
    assert(context.Get());
    Int32 edgeSlop = ViewConfiguration::Get(context)->GetScaledEdgeSlop();

    AutoPtr<CRect> closestBounds;
    assert(SUCCEEDED(CRect::NewByFriend((CRect**)&closestBounds)));

    CRect* touchableBounds = (CRect*)mOtherRect.Get();

    AutoPtr<IView> closest;

    AutoPtr<IObjectEnumerator> objEmu;
    touchables->GetObjectEnumerator((IObjectEnumerator**)&objEmu);

    Boolean isSucceeded = FALSE;
    while (objEmu->MoveNext(&isSucceeded), isSucceeded) {
        AutoPtr<IInterface> obj;
        objEmu->Current((IInterface**)&obj);
        IView* touchable = (IView*)obj->Probe(EIID_IView);

        // get visible bounds of other view in same coordinate system
        touchable->GetDrawingRect((IRect*)touchableBounds);

        //root->OffsetRectBetweenParentAndChild(touchable, touchableBounds, TRUE, TRUE);

        if (!IsTouchCandidate(x, y, touchableBounds, direction)) {
            continue;
        }

        Int32 distance = Math::INT32_MAX_VALUE;

        switch (direction) {
        case View::FOCUS_LEFT:
            distance = x - touchableBounds->mRight + 1;
            break;
        case View::FOCUS_RIGHT:
            distance = touchableBounds->mLeft;
            break;
        case View::FOCUS_UP:
            distance = y - touchableBounds->mBottom + 1;
            break;
        case View::FOCUS_DOWN:
            distance = touchableBounds->mTop;
            break;
        }

        if (distance < edgeSlop) {
            Boolean isContains1, isContains2;
            closestBounds->ContainsEx2(touchableBounds, &isContains1);
            touchableBounds->ContainsEx2(closestBounds, &isContains2);
            // Give preference to innermost views
            if (closest == NULL || isContains1 ||
                (!isContains2 && distance < minDistance)) {
                minDistance = distance;
                closest = touchable;
                closestBounds->SetEx(touchableBounds);
                switch (direction) {
                case View::FOCUS_LEFT:
                    deltas[0] = -distance;
                    break;
                case View::FOCUS_RIGHT:
                    deltas[0] = distance;
                    break;
                case View::FOCUS_UP:
                    deltas[1] = -distance;
                    break;
                case View::FOCUS_DOWN:
                    deltas[1] = distance;
                    break;
                }
            }
        }
    }

    return closest;
}

/**
 * Is destRect a candidate for the next touch given the direction?
 */
Boolean FocusFinder::IsTouchCandidate(
    /* [in] */ Int32 x,
    /* [in] */ Int32 y,
    /* [in] */ IRect* dest,
    /* [in] */ Int32 direction)
{
    assert(dest);
    CRect* destRect = (CRect*)dest;
    switch (direction) {
    case View::FOCUS_LEFT:
        return destRect->mLeft <= x && destRect->mTop <= y && y <= destRect->mBottom;
    case View::FOCUS_RIGHT:
        return destRect->mLeft >= x && destRect->mTop <= y && y <= destRect->mBottom;
    case View::FOCUS_UP:
        return destRect->mTop <= y && destRect->mLeft <= x && x <= destRect->mRight;
    case View::FOCUS_DOWN:
        return destRect->mTop >= y && destRect->mLeft <= x && x <= destRect->mRight;
    default:
        Logger::E("FocusFinder",
            "direction must be one of {FOCUS_UP, FOCUS_DOWN, FOCUS_LEFT, FOCUS_RIGHT}.");
        assert(0);
        break;
    }

    return FALSE;
}
