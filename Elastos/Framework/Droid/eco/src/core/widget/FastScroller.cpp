
#include "widget/FastScroller.h"
#include "graphics/CRectF.h"
#include "graphics/CPaint.h"
#include "os/CApartment.h"
#include "os/SystemClock.h"
#include "view/CMotionEvent.h"
#include "widget/AbsListView.h"
#include <elastos/Math.h>

using namespace Elastos::Core;

Int32 FastScroller::MIN_PAGES = 4;
const Int32 FastScroller::STATE_NONE;
const Int32 FastScroller::STATE_ENTER;
const Int32 FastScroller::STATE_VISIBLE;
const Int32 FastScroller::STATE_DRAGGING;
const Int32 FastScroller::STATE_EXIT;

const Int32 FastScroller::ScrollFade::ALPHA_MAX;
const Int64 FastScroller::ScrollFade::FADE_DURATION;

FastScroller::ScrollFade::ScrollFade(
    /* [in] */ FastScroller* host)
    : mHost(host)
{}

void FastScroller::ScrollFade::StartFade()
{
    mFadeDuration = FADE_DURATION;
    mStartTime = SystemClock::GetUptimeMillis();
    mHost->SetState(STATE_EXIT);
}

Int32 FastScroller::ScrollFade::GetAlpha()
{
    if (mHost->GetState() != mHost->STATE_EXIT) {
        return ALPHA_MAX;
    }
    Int32 alpha;
    Int64 now = SystemClock::GetUptimeMillis();
    if (now > mStartTime + mFadeDuration) {
        alpha = 0;
    }
    else {
        alpha = (Int32)(ALPHA_MAX - ((now - mStartTime) * ALPHA_MAX) / mFadeDuration);
    }

    return alpha;
}

ECode FastScroller::ScrollFade::Run()
{
    if (mHost->GetState() != mHost->STATE_EXIT) {
        StartFade();
        return NOERROR;
    }

    if (GetAlpha() > 0) {
        mHost->mList->Invalidate();
    }
    else {
        mHost->SetState(mHost->STATE_NONE);
    }

    return NOERROR;
}

FastScroller::FastScroller(
    /* [in] */ IContext* context,
    /* [in] */ AbsListView* listView)
    : mList(listView)
    , mScrollCompleted(FALSE)
    , mItemCount(-1)
    , mLongList(FALSE)
    , mDrawOverlay(FALSE)
    , mChangedBounds(FALSE)
{
    Init(context);
}

void FastScroller::SetState(
    /* [in] */ Int32 state)
{
    switch (state) {
    case STATE_NONE:
        //mHandler->RemoveCallbacks(mScrollFade);
        mList->Invalidate();
        break;
    case STATE_VISIBLE:
        if (mState != STATE_VISIBLE) { // Optimization
            ResetThumbPos();
        }
        // Fall through
    case STATE_DRAGGING:
        //mHandler->RemoveCallbacks(mScrollFade);
        break;
    case STATE_EXIT:
        {
            Int32 viewWidth = mList->GetWidth();
            mList->Invalidate(
                viewWidth - mThumbW, mThumbY, viewWidth, mThumbY + mThumbH);
        }
        break;
    default:
        break;
    }
    mState = state;
}

Int32 FastScroller::GetState()
{
    return mState;
}

void FastScroller::ResetThumbPos()
{
    Int32 viewWidth = mList->GetWidth();
    // Bounds are always top right. Y coordinate get's translated during draw
    mThumbDrawable->SetBounds(viewWidth - mThumbW, 0, viewWidth, mThumbH);
    mThumbDrawable->SetAlpha(ScrollFade::ALPHA_MAX);
}

void FastScroller::UseThumbDrawable(
    /* [in] */ IContext* context,
    /* [in] */ IDrawable* drawable)
{
    mThumbDrawable = drawable;
    AutoPtr<IResources> resources;
    context->GetResources((IResources**)&resources);
    resources->GetDimensionPixelSize(
        0x01050008/*com.android.internal.R.dimen.fastscroll_thumb_width*/,
        &mThumbW);
    resources->GetDimensionPixelSize(
        0x01050009/*com.android.internal.R.dimen.fastscroll_thumb_height*/,
        &mThumbH);
    mChangedBounds = TRUE;
}

void FastScroller::Init(
    /* [in] */ IContext* context)
{
    ASSERT_SUCCEEDED(CApartment::GetMainApartment((IApartment**)&mApartment));

    // Get both the scrollbar states drawables
    AutoPtr<IResources> res;
    context->GetResources((IResources**)&res);
    AutoPtr<IDrawable> drawable;
    //com.android.internal.R.drawable.scrollbar_handle_accelerated_anim2
    res->GetDrawable(0x01080273, (IDrawable**)&drawable);
    UseThumbDrawable(context, drawable);

    mOverlayDrawable = NULL;
    //com.android.internal.R.drawable.menu_submenu_background
    res->GetDrawable(0x0108022e, (IDrawable**)&mOverlayDrawable);

    mScrollCompleted = TRUE;

    GetSectionsFromIndexer();

    //com.android.internal.R.dimen.fastscroll_overlay_size
    res->GetDimensionPixelSize(0x01050007, &mOverlaySize);
    ASSERT_SUCCEEDED(CRectF::New((IRectF**)&mOverlayPos));
    mScrollFade = new ScrollFade(this);
    ASSERT_SUCCEEDED(CPaint::New((IPaint**)&mPaint));
    mPaint->SetAntiAlias(TRUE);
    mPaint->SetTextAlign(PaintAlign_CENTER);
    mPaint->SetTextSize(mOverlaySize / 2);

    AutoPtr<ITypedArray> ta;
    AutoPtr<ITheme> theme;
    context->GetTheme((ITheme**)&theme);

    ArrayOf<Int32>* attrs = ArrayOf<Int32>::Alloc(1);
    (*attrs)[0] = 0x01010036;//android.R.attr.textColorPrimary
    theme->ObtainStyledAttributes(*attrs, (ITypedArray**)&ta);
    ArrayOf<Int32>::Free(attrs);

    Int32 data;
    ta->GetIndex(0, &data);
    AutoPtr<IColorStateList> textColor;
    ta->GetColorStateList(data, (IColorStateList**)&textColor);
    Int32 textColorNormal;
    textColor->GetDefaultColor(&textColorNormal);
    mPaint->SetColor(textColorNormal);
    mPaint->SetStyle(PaintStyle_FILL_AND_STROKE);

    mState = STATE_NONE;
}

void FastScroller::Stop()
{
    SetState(STATE_NONE);
}

Boolean FastScroller::IsVisible()
{
    return !(mState == STATE_NONE);
}

void FastScroller::Draw(
    /* [in] */ ICanvas* canvas)
{
    if (mState == STATE_NONE) {
        // No need to draw anything
        return;
    }

    Int32 y = mThumbY;
    Int32 viewWidth = mList->GetWidth();

    Int32 alpha = -1;
    if (mState == STATE_EXIT) {
        alpha = mScrollFade->GetAlpha();
        if (alpha < ScrollFade::ALPHA_MAX / 2) {
            mThumbDrawable->SetAlpha(alpha * 2);
        }

        Int32 left = viewWidth - (mThumbW * alpha) / ScrollFade::ALPHA_MAX;
        mThumbDrawable->SetBounds(left, 0, viewWidth, mThumbH);
        mChangedBounds = TRUE;
    }

    canvas->Translate(0, y);
    mThumbDrawable->Draw(canvas);
    canvas->Translate(0, -y);

    // If user is dragging the scroll bar, draw the alphabet overlay
    if (mState == STATE_DRAGGING && mDrawOverlay) {
        mOverlayDrawable->Draw(canvas);
        Float descent;
        mPaint->Descent(&descent);
        CRectF* rectF = (CRectF*)mOverlayPos.Get();
        canvas->DrawTextInString(mSectionText, (Int32)(rectF->mLeft + rectF->mRight) / 2,
            (Int32)(rectF->mBottom + rectF->mTop) / 2 + mOverlaySize / 4 - descent, mPaint);
    }
    else if (mState == STATE_EXIT) {
        if (alpha == 0) { // Done with exit
            SetState(STATE_NONE);
        }
        else {
            mList->Invalidate(viewWidth - mThumbW, y, viewWidth, y + mThumbH);
        }
    }
}

void FastScroller::OnSizeChanged(
    /* [in] */ Int32 w,
    /* [in] */ Int32 h,
    /* [in] */ Int32 oldw,
    /* [in] */ Int32 oldh)
{
    if (mThumbDrawable != NULL) {
        mThumbDrawable->SetBounds(w - mThumbW, 0, w, mThumbH);
    }

    CRectF* pos = (CRectF*)mOverlayPos.Get();
    pos->mLeft = (w - mOverlaySize) / 2;
    pos->mRight = pos->mLeft + mOverlaySize;
    pos->mTop = h / 10; // 10% from top
    pos->mBottom = pos->mTop + mOverlaySize;
    if (mOverlayDrawable != NULL) {
        mOverlayDrawable->SetBounds(
            (Int32)pos->mLeft, (Int32)pos->mTop,
            (Int32)pos->mRight, (Int32)pos->mBottom);
    }
}

void FastScroller::OnScroll(
    /* [in] */ IAbsListView* view,
    /* [in] */ Int32 firstVisibleItem,
    /* [in] */ Int32 visibleItemCount,
    /* [in] */ Int32 totalItemCount)
{
    // Are there enough pages to require fast scroll? Recompute only if total count changes
    if (mItemCount != totalItemCount && visibleItemCount > 0) {
        mItemCount = totalItemCount;
        mLongList = mItemCount / visibleItemCount >= MIN_PAGES;
    }

    if (!mLongList) {
        if (mState != STATE_NONE) {
            SetState(STATE_NONE);
        }
        return;
    }

    if (totalItemCount - visibleItemCount > 0 && mState != STATE_DRAGGING ) {
        mThumbY = ((mList->GetHeight() - mThumbH) * firstVisibleItem)
                / (totalItemCount - visibleItemCount);
        if (mChangedBounds) {
            ResetThumbPos();
            mChangedBounds = FALSE;
        }
    }
    mScrollCompleted = TRUE;
    if (firstVisibleItem == mVisibleItem) {
        return;
    }
    mVisibleItem = firstVisibleItem;
    if (mState != STATE_DRAGGING) {
        SetState(STATE_VISIBLE);
        PostDelayed(1500);
    }
}

AutoPtr<ISectionIndexer> FastScroller::GetSectionIndexer()
{
    return mSectionIndexer;
}

Vector<AutoPtr<IInterface> >& FastScroller::GetSections()
{
    if (mListAdapter == NULL && mList != NULL) {
        GetSectionsFromIndexer();
    }

    return mSections;
}

void FastScroller::GetSectionsFromIndexer()
{
    AutoPtr<IAdapter> adapter = mList->GetAdapter();
    mSectionIndexer = NULL;
    IHeaderViewListAdapter* headerAdapter =
        IHeaderViewListAdapter::Probe(adapter);
    if (headerAdapter) {
        headerAdapter->GetHeadersCount(&mListOffset);
        adapter = NULL;
        headerAdapter->GetWrappedAdapter((IListAdapter**)&adapter);
    }

    //if (adapter instanceof ExpandableListConnector) {
    //    ExpandableListAdapter expAdapter = ((ExpandableListConnector)adapter).getAdapter();
    //    if (expAdapter instanceof SectionIndexer) {
    //        mSectionIndexer = (SectionIndexer) expAdapter;
    //        mListAdapter = (BaseAdapter) adapter;
    //        mSections = mSectionIndexer.getSections();
    //    }
    //}
    //else {
        mListAdapter = IBaseAdapter::Probe(adapter);
        mSections.Clear();
        ISectionIndexer* sectionIndexer = ISectionIndexer::Probe(adapter);
        if (sectionIndexer) {
            mListAdapter = IBaseAdapter::Probe(adapter);
            mSectionIndexer = sectionIndexer;

            AutoPtr<IObjectContainer> sections;
            mSectionIndexer->GetSections((IObjectContainer**)&sections);

            AutoPtr<IObjectEnumerator> objEmu;
            sections->GetObjectEnumerator((IObjectEnumerator**)&objEmu);

            Boolean isSucceeded;
            objEmu->MoveNext(&isSucceeded);
            while (isSucceeded) {
                AutoPtr<IInterface> obj;
                objEmu->Current((IInterface**)&obj);
                mSections.PushBack(obj);
                objEmu->MoveNext(&isSucceeded);
            }
        }
        else {
            //mSections = new String[] { " " };
        }
    //}
}

void FastScroller::ScrollTo(
    /* [in] */ Float position)
{
    Int32 count = mList->GetCount();
    mScrollCompleted = FALSE;
    Float fThreshold = (1.0f / count) / 8;
    Int32 sectionIndex;
    if (mSections.GetSize() > 1) {
        Int32 nSections = mSections.GetSize();
        Int32 section = (Int32)(position * nSections);
        if (section >= nSections) {
            section = nSections - 1;
        }

        Int32 exactSection = section;
        sectionIndex = section;
        Int32 index;
        mSectionIndexer->GetPositionForSection(section, &index);

        // Given the expected section and index, the following code will
        // try to account for missing sections (no names starting with..)
        // It will compute the scroll space of surrounding empty sections
        // and interpolate the currently visible letter's range across the
        // available space, so that there is always some list movement while
        // the user moves the thumb.
        //
        Int32 nextIndex = count;
        Int32 prevIndex = index;
        Int32 prevSection = section;
        Int32 nextSection = section + 1;
        // Assume the next section is unique
        if (section < nSections - 1) {
            mSectionIndexer->GetPositionForSection(section + 1, &nextIndex);
        }

        // Find the previous index if we're slicing the previous section
        if (nextIndex == index) {
            // Non-existent letter
            while (section > 0) {
                section--;
                mSectionIndexer->GetPositionForSection(section, &prevIndex);
                if (prevIndex != index) {
                    prevSection = section;
                    sectionIndex = section;
                    break;
                }
                else if (section == 0) {
                    // When section reaches 0 here, sectionIndex must follow it.
                    // Assuming mSectionIndexer.getPositionForSection(0) == 0.
                    sectionIndex = 0;
                    break;
                }
            }
        }

        // Find the next index, in case the assumed next index is not
        // unique. For instance, if there is no P, then request for P's
        // position actually returns Q's. So we need to look ahead to make
        // sure that there is really a Q at Q's position. If not, move
        // further down...
        Int32 nextNextSection = nextSection + 1;
        Int32 nextNextPosition;
        mSectionIndexer->GetPositionForSection(nextNextSection, &nextNextPosition);
        while (nextNextSection < nSections &&
               nextNextPosition == nextIndex) {
            nextNextSection++;
            nextSection++;
            mSectionIndexer->GetPositionForSection(nextNextSection, &nextNextPosition);
        }
        // Compute the beginning and ending scroll range percentage of the
        // currently visible letter. This could be equal to or greater than
        // (1 / nSections).
        Float fPrev = (Float)prevSection / nSections;
        Float fNext = (Float)nextSection / nSections;
        if (prevSection == exactSection && position - fPrev < fThreshold) {
            index = prevIndex;
        }
        else {
            index = prevIndex + (Int32)((nextIndex - prevIndex) * (position - fPrev)
                / (fNext - fPrev));
        }
        // Don't overflow
        if (index > count - 1) {
            index = count - 1;
        }

        //if (mList instanceof ExpandableListView) {
        //    ExpandableListView expList = (ExpandableListView) mList;
        //    expList.setSelectionFromTop(expList.getFlatListPosition(
        //            ExpandableListView.getPackedPositionForGroup(index + mListOffset)), 0);
        //}
        //else if (mList instanceof ListView) {
        //    ((ListView)mList).setSelectionFromTop(index + mListOffset, 0);
        //}
        //else {
            mList->SetSelection(index + mListOffset);
        //}
    }
    else {
        Int32 index = (Int32)(position * count);
        //if (mList instanceof ExpandableListView) {
        //    ExpandableListView expList = (ExpandableListView) mList;
        //    expList.setSelectionFromTop(expList.getFlatListPosition(
        //            ExpandableListView.getPackedPositionForGroup(index + mListOffset)), 0);
        //}
        //else if (mList instanceof ListView) {
        //    ((ListView)mList).setSelectionFromTop(index + mListOffset, 0);
        //}
        //else {
            mList->SetSelection(index + mListOffset);
        //}
        sectionIndex = -1;
    }

    if (sectionIndex >= 0) {
        //String text = mSectionText = mSections[sectionIndex].toString();
        //mDrawOverlay = (text.length() != 1 || text.charAt(0) != ' ') &&
        //        sectionIndex < mSections.GetSize();
    }
    else {
        mDrawOverlay = FALSE;
    }
}

void FastScroller::CancelFling()
{
    // Cancel the list fling
    AutoPtr<CMotionEvent> cancelFling;
    CMotionEvent::Obtain(
        0, 0, MotionEvent_ACTION_CANCEL, 0, 0, 0,
        (CMotionEvent**)&cancelFling);
    mList->OnTouchEvent(cancelFling);
    cancelFling->Recycle();
}

Boolean FastScroller::OnInterceptTouchEvent(
    /* [in] */ IMotionEvent* ev)
{
    Int32 action;
    ev->GetAction(&action);
    if (mState > STATE_NONE && action == MotionEvent_ACTION_DOWN) {
        Float fx, fy;
        ev->GetX(&fx);
        ev->GetY(&fy);
        if (IsPointInside(fx, fy)) {
            SetState(STATE_DRAGGING);
            return TRUE;
        }
    }

    return FALSE;
}

Boolean FastScroller::OnTouchEvent(
    /* [in] */ IMotionEvent* me)
{
    if (mState == STATE_NONE) {
        return FALSE;
    }

    Int32 action;
    me->GetAction(&action);

    if (action == MotionEvent_ACTION_DOWN) {
        Float fx, fy;
        me->GetX(&fx);
        me->GetY(&fy);
        if (IsPointInside(fx, fy)) {
            SetState(STATE_DRAGGING);
            if (mListAdapter == NULL && mList != NULL) {
                GetSectionsFromIndexer();
            }

            if (mList != NULL) {
                mList->RequestDisallowInterceptTouchEvent(TRUE);
                mList->ReportScrollStateChange(OnScrollListener_SCROLL_STATE_TOUCH_SCROLL);
            }

            CancelFling();
            return TRUE;
        }
    }
    else if (action == MotionEvent_ACTION_UP) { // don't add ACTION_CANCEL here
        if (mState == STATE_DRAGGING) {
            if (mList != NULL) {
                // ViewGroup does the right thing already, but there might
                // be other classes that don't properly reset on touch-up,
                // so do this explicitly just in case.
                mList->RequestDisallowInterceptTouchEvent(FALSE);
                mList->ReportScrollStateChange(OnScrollListener_SCROLL_STATE_IDLE);
            }

            SetState(STATE_VISIBLE);
            //mHandler->RemoveCallbacks(mScrollFade);
            PostDelayed(1000);
            return TRUE;
        }
    }
    else if (action == MotionEvent_ACTION_MOVE) {
        if (mState == STATE_DRAGGING) {
            Int32 viewHeight = mList->GetHeight();
            // Jitter
            Float fy;
            me->GetY(&fy);
            Int32 newThumbY = (Int32)fy - mThumbH + 10;
            if (newThumbY < 0) {
                newThumbY = 0;
            }
            else if (newThumbY + mThumbH > viewHeight) {
                newThumbY = viewHeight - mThumbH;
            }

            if (Math::Abs(mThumbY - newThumbY) < 2) {
                return TRUE;
            }
            mThumbY = newThumbY;
            // If the previous scrollTo is still pending
            if (mScrollCompleted) {
                ScrollTo((Float)mThumbY / (viewHeight - mThumbH));
            }
            return TRUE;
        }
    }
    return FALSE;
}

Boolean FastScroller::IsPointInside(
    /* [in] */ Float x,
    /* [in] */ Float y)
{
    return x > mList->GetWidth() - mThumbW
        && y >= mThumbY && y <= mThumbY + mThumbH;
}

Boolean FastScroller::PostDelayed(
    /* [in] */ Int64 delayMillis)
{
    if (delayMillis <0 ) {
        delayMillis = 0;
    }

    ECode (STDCALL IRunnable::*pHandlerFunc)();
    pHandlerFunc = &IRunnable::Run;

    ECode ec = mApartment->PostCppCallbackDelayed(
        (Handle32)mScrollFade.Get(), *(Handle32*)&pHandlerFunc, NULL, 0, delayMillis);

    if (FAILED(ec)) {
        return FALSE;
    }

    return TRUE;
}
