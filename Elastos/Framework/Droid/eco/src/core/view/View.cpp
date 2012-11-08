
#include "view/View.h"
#include "view/ViewRoot.h"
#include "view/CDispatcherState.h"
#include "view/ViewTreeObserver.h"
#include "view/ViewConfiguration.h"
#include "view/CWindowManagerImpl.h"
#include "view/LayoutInflater.h"
#include "view//animation/Animation.h"
#include "view//animation/AnimationUtils.h"
#include "graphics/CCanvas.h"
#include "graphics/CPaint.h"
#include "graphics/CPoint.h"
#include "graphics/CMatrix.h"
#include "graphics/CLinearGradient.h"
#include "graphics/CPorterDuffXfermode.h"
#include "graphics/CBitmapFactory.h"
#include "graphics/ElPixelFormat.h"
#include "graphics/drawable/CColorDrawable.h"
#include "widget/CScrollBarDrawable.h"
#include "utils/CParcelableObjectContainer.h"
#include "utils/CDisplayMetrics.h"
#include "os/CApartment.h"
#include "os/SystemClock.h"
#include <elastos/Math.h>
#include <Logger.h>
#include <StringBuffer.h>

using namespace Elastos::Core;
using namespace Elastos::Utility::Logging;

extern "C" const InterfaceID EIID_View =
        {0xbbbbbbbb,0xbbbb,0xbbbb,{0xbb,0xbb,0xbb,0xbb,0xbb,0xbb,0xbb,0xbb}};

const Boolean View::DBG;
const Int32 View::NOT_FOCUSABLE;
const Int32 View::FOCUSABLE;
const Int32 View::FOCUSABLE_MASK;
const Int32 View::FITS_SYSTEM_WINDOWS;
const Int32 View::NO_ID;
const Int32 View::VISIBLE;
const Int32 View::INVISIBLE;
const Int32 View::GONE;
const Int32 View::VISIBILITY_MASK;
const Int32 View::ENABLED;
const Int32 View::DISABLED;
const Int32 View::ENABLED_MASK;
const Int32 View::WILL_NOT_DRAW;
const Int32 View::DRAW_MASK;
const Int32 View::SCROLLBARS_NONE;
const Int32 View::SCROLLBARS_HORIZONTAL;
const Int32 View::SCROLLBARS_VERTICAL;
const Int32 View::SCROLLBARS_MASK;
const Int32 View::FILTER_TOUCHES_WHEN_OBSCURED;
const Int32 View::FADING_EDGE_NONE;
const Int32 View::CLICKABLE;
const Int32 View::DRAWING_CACHE_ENABLED;
const Int32 View::SAVE_DISABLED;
const Int32 View::SAVE_DISABLED_MASK;
const Int32 View::WILL_NOT_CACHE_DRAWING;
const Int32 View::FOCUSABLE_IN_TOUCH_MODE;
const Int32 View::DRAWING_CACHE_QUALITY_LOW;
const Int32 View::DRAWING_CACHE_QUALITY_HIGH;
const Int32 View::DRAWING_CACHE_QUALITY_AUTO;
const Int32 View::DRAWING_CACHE_QUALITY_MASK;
const Int32 View::LONG_CLICKABLE;
const Int32 View::DUPLICATE_PARENT_STATE;
const Int32 View::FADING_EDGE_HORIZONTAL;
const Int32 View::FADING_EDGE_VERTICAL;
const Int32 View::FADING_EDGE_MASK;
const Int32 View::SCROLLBARS_INSIDE_OVERLAY;
const Int32 View::SCROLLBARS_INSIDE_INSET;
const Int32 View::SCROLLBARS_OUTSIDE_OVERLAY;
const Int32 View::SCROLLBARS_OUTSIDE_INSET;
const Int32 View::SCROLLBARS_INSET_MASK;
const Int32 View::SCROLLBARS_OUTSIDE_MASK;
const Int32 View::SCROLLBARS_STYLE_MASK;
const Int32 View::KEEP_SCREEN_ON;
const Int32 View::SOUND_EFFECTS_ENABLED;
const Int32 View::HAPTIC_FEEDBACK_ENABLED;
const Int32 View::FOCUSABLES_ALL;
const Int32 View::FOCUSABLES_TOUCH_MODE;
const Int32 View::FOCUS_BACKWARD;
const Int32 View::FOCUS_FORWARD;
const Int32 View::FOCUS_LEFT;
const Int32 View::FOCUS_UP;
const Int32 View::FOCUS_RIGHT;
const Int32 View::FOCUS_DOWN;
const Int32 View::WANTS_FOCUS;
const Int32 View::FOCUSED;
const Int32 View::SELECTED;
const Int32 View::IS_ROOT_NAMESPACE;
const Int32 View::HAS_BOUNDS;
const Int32 View::DRAWN;
const Int32 View::DRAW_ANIMATION;
const Int32 View::SKIP_DRAW;
const Int32 View::ONLY_DRAWS_BACKGROUND;
const Int32 View::REQUEST_TRANSPARENT_REGIONS;
const Int32 View::DRAWABLE_STATE_DIRTY;
const Int32 View::MEASURED_DIMENSION_SET;
const Int32 View::FORCE_LAYOUT;
const Int32 View::LAYOUT_REQUIRED;
const Int32 View::PRESSED;
const Int32 View::DRAWING_CACHE_VALID;
const Int32 View::ANIMATION_STARTED;
const Int32 View::SAVE_STATE_CALLED;
const Int32 View::ALPHA_SET;
const Int32 View::SCROLL_CONTAINER;
const Int32 View::SCROLL_CONTAINER_ADDED;
const Int32 View::DIRTY;
const Int32 View::DIRTY_OPAQUE;
const Int32 View::DIRTY_MASK;
const Int32 View::OPAQUE_BACKGROUND;
const Int32 View::OPAQUE_SCROLLBARS;
const Int32 View::OPAQUE_MASK;
const Int32 View::PREPRESSED;
const Int32 View::CANCEL_NEXT_UP_EVENT;
const Int32 View::AWAKEN_SCROLL_BARS_ON_ATTACH;
const Int32 View::OVER_SCROLL_ALWAYS;
const Int32 View::OVER_SCROLL_IF_CONTENT_SCROLLS;
const Int32 View::OVER_SCROLL_NEVER;

const Int32 View::MeasureSpec::MODE_SHIFT;
const Int32 View::MeasureSpec::MODE_MASK;
const Int32 View::MeasureSpec::UNSPECIFIED;
const Int32 View::MeasureSpec::EXACTLY;
const Int32 View::MeasureSpec::AT_MOST;

const Int32 View::ScrollabilityCache::OFF;
const Int32 View::ScrollabilityCache::ON;
const Int32 View::ScrollabilityCache::FADING;

const char* View::VIEW_LOG_TAG = "View";
Int64 View::sInstanceCount = 0;

const Int32 View::VISIBILITY_FLAGS[] = {VISIBLE, INVISIBLE, GONE};

const Int32 View::DRAWING_CACHE_QUALITY_FLAGS[] = {
    DRAWING_CACHE_QUALITY_AUTO, DRAWING_CACHE_QUALITY_LOW, DRAWING_CACHE_QUALITY_HIGH
};

const ArrayOf<ArrayOf<Int32>*>* View::VIEW_STATE_SETS = InitViewStateSets();

pthread_key_t View::sKey;

const ArrayOf<ArrayOf<Int32>*>* View::InitViewStateSets()
{
    pthread_key_create(&sKey, NULL);
    pthread_setspecific(sKey, NULL);

    const Int32 S_ENABLED = 0x0101009e; //R.attr.state_enabled
    const Int32 S_FOCUSED = 0x0101009c; //R.attr.state_focused
    const Int32 S_SELECTED = 0x010100a1; //R.attr.state_selected
    const Int32 S_PRESSED = 0x010100a7; //R.attr.state_pressed
    const Int32 S_WINDOW_FOCUSED = 0x0101009d; //R.attr.state_window_focused

    ArrayOf<ArrayOf<Int32>*>* stateSets = ArrayOf<ArrayOf<Int32>*>::Alloc(32);
    (*stateSets)[0] = ArrayOf<Int32>::Alloc(0);

    Int32 WINDOW_FOCUSED_STATE_SET[] = {S_WINDOW_FOCUSED};
    (*stateSets)[1] = ArrayOf<Int32>::Alloc(WINDOW_FOCUSED_STATE_SET, 1)->Clone();

    Int32 SELECTED_STATE_SET[] = {S_SELECTED};
    (*stateSets)[2] = ArrayOf<Int32>::Alloc(SELECTED_STATE_SET, 1)->Clone();

    Int32 SELECTED_WINDOW_FOCUSED_STATE_SET[] = {S_SELECTED, S_WINDOW_FOCUSED};
    (*stateSets)[3] = ArrayOf<Int32>::Alloc(SELECTED_WINDOW_FOCUSED_STATE_SET, 2)->Clone();

    Int32 FOCUSED_STATE_SET[] = {S_FOCUSED};
    (*stateSets)[4] = ArrayOf<Int32>::Alloc(FOCUSED_STATE_SET, 1)->Clone();

    Int32 FOCUSED_WINDOW_FOCUSED_STATE_SET[] = {S_FOCUSED, S_WINDOW_FOCUSED};
    (*stateSets)[5] = ArrayOf<Int32>::Alloc(FOCUSED_WINDOW_FOCUSED_STATE_SET, 2)->Clone();

    Int32 FOCUSED_SELECTED_STATE_SET[] = {S_FOCUSED, S_SELECTED};
    (*stateSets)[6] = ArrayOf<Int32>::Alloc(FOCUSED_SELECTED_STATE_SET, 2)->Clone();

    Int32 FOCUSED_SELECTED_WINDOW_FOCUSED_STATE_SET[] =
        {S_FOCUSED, S_SELECTED, S_WINDOW_FOCUSED};
    (*stateSets)[7] = ArrayOf<Int32>::Alloc(
        FOCUSED_SELECTED_WINDOW_FOCUSED_STATE_SET, 1)->Clone();

    Int32 ENABLED_STATE_SET[] = {S_ENABLED};
    (*stateSets)[8] = ArrayOf<Int32>::Alloc(ENABLED_STATE_SET, 1)->Clone();

    Int32 ENABLED_WINDOW_FOCUSED_STATE_SET[] = {S_ENABLED, S_WINDOW_FOCUSED};
    (*stateSets)[9] = ArrayOf<Int32>::Alloc(ENABLED_WINDOW_FOCUSED_STATE_SET, 2)->Clone();

    Int32 ENABLED_SELECTED_STATE_SET[] = {S_ENABLED, S_SELECTED};
    (*stateSets)[10] = ArrayOf<Int32>::Alloc(ENABLED_SELECTED_STATE_SET, 2)->Clone();

    Int32 ENABLED_SELECTED_WINDOW_FOCUSED_STATE_SET[] =
        {S_ENABLED, S_SELECTED, S_WINDOW_FOCUSED};
    (*stateSets)[11] = ArrayOf<Int32>::Alloc(
        ENABLED_SELECTED_WINDOW_FOCUSED_STATE_SET, 3)->Clone();

    Int32 ENABLED_FOCUSED_STATE_SET[] = {S_ENABLED, S_FOCUSED};
    (*stateSets)[12] = ArrayOf<Int32>::Alloc(ENABLED_FOCUSED_STATE_SET, 2)->Clone();

    Int32 ENABLED_FOCUSED_WINDOW_FOCUSED_STATE_SET[] =
        {S_ENABLED, S_FOCUSED, S_WINDOW_FOCUSED};
    (*stateSets)[13] = ArrayOf<Int32>::Alloc
        (ENABLED_FOCUSED_WINDOW_FOCUSED_STATE_SET, 3)->Clone();

    Int32 ENABLED_FOCUSED_SELECTED_STATE_SET[] = {S_ENABLED, S_FOCUSED, S_SELECTED};
    (*stateSets)[14] = ArrayOf<Int32>::Alloc(ENABLED_FOCUSED_SELECTED_STATE_SET, 3)->Clone();

    Int32 ENABLED_FOCUSED_SELECTED_WINDOW_FOCUSED_STATE_SET[] =
        {S_ENABLED, S_FOCUSED, S_SELECTED, S_WINDOW_FOCUSED};
    (*stateSets)[15] = ArrayOf<Int32>::Alloc(
        ENABLED_FOCUSED_SELECTED_WINDOW_FOCUSED_STATE_SET, 4)->Clone();

    Int32 PRESSED_STATE_SET[] = {S_PRESSED};
    (*stateSets)[16] = ArrayOf<Int32>::Alloc(PRESSED_STATE_SET, 1)->Clone();

    Int32 PRESSED_WINDOW_FOCUSED_STATE_SET[] = {S_PRESSED, S_WINDOW_FOCUSED};
    (*stateSets)[17] = ArrayOf<Int32>::Alloc(PRESSED_WINDOW_FOCUSED_STATE_SET, 2)->Clone();

    Int32 PRESSED_SELECTED_STATE_SET[] = {S_PRESSED, S_SELECTED};
    (*stateSets)[18] = ArrayOf<Int32>::Alloc(PRESSED_SELECTED_STATE_SET, 2)->Clone();

    Int32 PRESSED_SELECTED_WINDOW_FOCUSED_STATE_SET[] =
        {S_PRESSED, S_SELECTED, S_WINDOW_FOCUSED};
    (*stateSets)[19] = ArrayOf<Int32>::Alloc(
        PRESSED_SELECTED_WINDOW_FOCUSED_STATE_SET, 3)->Clone();

    Int32 PRESSED_FOCUSED_STATE_SET[] = {S_PRESSED, S_FOCUSED};
    (*stateSets)[20] = ArrayOf<Int32>::Alloc(PRESSED_FOCUSED_STATE_SET, 2)->Clone();

    Int32 PRESSED_FOCUSED_WINDOW_FOCUSED_STATE_SET[] =
        {S_PRESSED, S_FOCUSED, S_WINDOW_FOCUSED};
    (*stateSets)[21] = ArrayOf<Int32>::Alloc(
        PRESSED_FOCUSED_WINDOW_FOCUSED_STATE_SET, 3)->Clone();

    Int32 PRESSED_FOCUSED_SELECTED_STATE_SET[] = {S_PRESSED, S_FOCUSED, S_SELECTED};
    (*stateSets)[22] = ArrayOf<Int32>::Alloc(PRESSED_FOCUSED_SELECTED_STATE_SET, 3)->Clone();

    Int32 PRESSED_FOCUSED_SELECTED_WINDOW_FOCUSED_STATE_SET[] =
        {S_PRESSED, S_FOCUSED, S_SELECTED, S_WINDOW_FOCUSED};
    (*stateSets)[23] = ArrayOf<Int32>::Alloc(
        PRESSED_FOCUSED_SELECTED_WINDOW_FOCUSED_STATE_SET, 4)->Clone();

    Int32 PRESSED_ENABLED_STATE_SET[] = {S_PRESSED, S_ENABLED};
    (*stateSets)[24] = ArrayOf<Int32>::Alloc(PRESSED_ENABLED_STATE_SET, 2)->Clone();

    Int32 PRESSED_ENABLED_WINDOW_FOCUSED_STATE_SET[] =
        {S_PRESSED, S_ENABLED, S_WINDOW_FOCUSED};
    (*stateSets)[25] = ArrayOf<Int32>::Alloc(
        PRESSED_ENABLED_WINDOW_FOCUSED_STATE_SET, 3)->Clone();

    Int32 PRESSED_ENABLED_SELECTED_STATE_SET[] = {S_PRESSED, S_ENABLED, S_SELECTED};
    (*stateSets)[26] = ArrayOf<Int32>::Alloc(PRESSED_ENABLED_SELECTED_STATE_SET, 3)->Clone();

    Int32 PRESSED_ENABLED_SELECTED_WINDOW_FOCUSED_STATE_SET[] =
        {S_PRESSED, S_ENABLED, S_SELECTED, S_WINDOW_FOCUSED};
    (*stateSets)[27] = ArrayOf<Int32>::Alloc(
        PRESSED_ENABLED_SELECTED_WINDOW_FOCUSED_STATE_SET, 4)->Clone();

    Int32 PRESSED_ENABLED_FOCUSED_STATE_SET[] = {S_PRESSED, S_ENABLED, S_FOCUSED};
    (*stateSets)[28] = ArrayOf<Int32>::Alloc(PRESSED_ENABLED_FOCUSED_STATE_SET, 3)->Clone();

    Int32 PRESSED_ENABLED_FOCUSED_WINDOW_FOCUSED_STATE_SET[] =
        {S_PRESSED, S_ENABLED, S_FOCUSED, S_WINDOW_FOCUSED};
    (*stateSets)[29] = ArrayOf<Int32>::Alloc(
        PRESSED_ENABLED_FOCUSED_WINDOW_FOCUSED_STATE_SET, 4)->Clone();

    Int32 PRESSED_ENABLED_FOCUSED_SELECTED_STATE_SET[] =
        {S_PRESSED, S_ENABLED, S_FOCUSED, S_SELECTED};
    (*stateSets)[30] = ArrayOf<Int32>::Alloc(
        PRESSED_ENABLED_FOCUSED_SELECTED_STATE_SET, 4)->Clone();

    Int32 PRESSED_ENABLED_FOCUSED_SELECTED_WINDOW_FOCUSED_STATE_SET[] =
        {S_PRESSED, S_ENABLED, S_FOCUSED, S_SELECTED, S_WINDOW_FOCUSED};
    (*stateSets)[31] = ArrayOf<Int32>::Alloc(
        PRESSED_ENABLED_FOCUSED_SELECTED_WINDOW_FOCUSED_STATE_SET, 5)->Clone();

    return stateSets;
}

ECode View::CheckForLongPress::Run()
{
    View* view = (View*)mView->Probe(EIID_View);
    assert(view != NULL);
    if (view->IsPressed() && (view->mParent != NULL)
        && mOriginalWindowAttachCount == view->mWindowAttachCount) {
            if (view->PerformLongClick()) {
                view->mHasPerformedLongPress = TRUE;
            }
    }

    return NOERROR;
}

void View::CheckForLongPress::RememberWindowAttachCount()
{
    View* view = (View*)mView->Probe(EIID_View);
    assert(view != NULL);
    mOriginalWindowAttachCount = view->mWindowAttachCount;
}

ECode View::CheckForTap::Run()
{
    View* view = (View*)mView->Probe(EIID_View);
    assert(view != NULL);
    view->mPrivateFlags &= ~PREPRESSED;
    view->mPrivateFlags |= PRESSED;
    view->RefreshDrawableState();
    if ((view->mViewFlags & LONG_CLICKABLE) == LONG_CLICKABLE) {
        view->PostCheckForLongClick(ViewConfiguration::GetTapTimeout());
    }

    return NOERROR;
}

ECode View::ViewPerformClick::Run()
{
    Boolean result;
    return mView->PerformClick(&result);
}

ECode View::UnsetPressedState::Run()
{
    return mView->SetPressed(FALSE);
}

View::AttachInfo::AttachInfo(
    /* [in] */ IWindowSession* session,
    /* [in] */ IInnerWindow* window,
    /* [in] */ IApartment* handler,
    /* [in] */ Callbacks* effectPlayer)
    : mSession(session)
    , mWindow(window)
    , mWindowToken(IBinder::Probe(window))
    , mRootCallbacks(effectPlayer)
    , mHandler(handler)
{
    assert(SUCCEEDED(CRect::NewByFriend((CRect**)&mContentInsets)));
    assert(SUCCEEDED(CRect::NewByFriend((CRect**)&mVisibleInsets)));
    assert(SUCCEEDED(CRect::NewByFriend((CRect**)&mTmpInvalRect)));

    mTreeObserver = new ViewTreeObserver();
    mGivenInternalInsets = new ViewTreeObserver::InternalInsetsInfo();

    assert(SUCCEEDED(CDispatcherState::New((IDispatcherState**)&mKeyDispatchState)));
}

View::AttachInfo::~AttachInfo()
{
    delete mGivenInternalInsets;
}

View::ScrollabilityCache::ScrollabilityCache(
    /* [in] */ AutoPtr<ViewConfiguration>& configuration,
    /* [in] */ AutoPtr<IView>& host)
    : mState(OFF)
{
    mFadingEdgeLength = configuration->GetScaledFadingEdgeLength();
    mScrollBarSize = configuration->GetScaledScrollBarSize();
    mScrollBarDefaultDelayBeforeFade = ViewConfiguration::GetScrollDefaultDelay();
    mScrollBarFadeDuration = ViewConfiguration::GetScrollBarFadeDuration();

    CPaint::New((IPaint**)&mPaint);
    CMatrix::New((IMatrix**)&mMatrix);
    // use use a height of 1, and then wack the matrix each time we
    // actually use it.
    CLinearGradient::New(0, 0, 0, 1, 0xFF000000, 0, ShaderTileMode_CLAMP, (IShader**)&mShader);

    mPaint->SetShader(mShader.Get());
    AutoPtr<IXfermode> mode;
    CPorterDuffXfermode::New(PorterDuffMode_DST_OUT, (IXfermode**)&mode);
    mPaint->SetXfermode(mode.Get());
    mHost = host;
}

ECode View::ScrollabilityCache::Run()
{
    Int64 now = AnimationUtils::CurrentAnimationTimeMillis();
    if (now >= mFadeStartTime) {
        // the animation fades the scrollbars out by changing
        // the opacity (alpha) from fully opaque to fully
        // transparent
        Int32 nextFrame = (Int32)now;
        Int32 framesCount = 0;

        Interpolator* interpolator = mScrollBarInterpolator;

        // Start opaque
//        interpolator->SetKeyFrame(framesCount++, nextFrame, mHost->mOpaque);

        // End transparent
        nextFrame += mScrollBarFadeDuration;
//        interpolator->SetKeyFrame(framesCount, nextFrame, mHost->mTransparent);

        mState = FADING;

        // Kick off the fade animation
        mHost->InvalidateEx2();
    }
    return NOERROR;
}

void View::ScrollabilityCache::SetFadeColor(
    /* [in] */ Int32 color)
{
    if (color != 0 && color != mLastColor) {
        mLastColor = color;
        color |= 0xFF000000;

        CLinearGradient::New(0, 0, 0, 1, color | 0xFF000000,
                color & 0x00FFFFFF, ShaderTileMode_CLAMP, (IShader**)&mShader);

        mPaint->SetShader(mShader.Get());
        // Restore the default transfer mode (src_over)
        mPaint->SetXfermode(NULL);
    }
}

View::View() :
    mMeasuredWidth(0),
    mMeasuredHeight(0),
    mID(NO_ID),
    mOverScrollMode(0),
    mAttachInfo(NULL),
    mPrivateFlags(0),
    mWindowAttachCount(0),
    mViewFlags(0),
    mLeft(0),
    mRight(0),
    mTop(0),
    mBottom(0),
    mScrollX(0),
    mScrollY(0),
    mPaddingLeft(0),
    mPaddingRight(0),
    mPaddingTop(0),
    mPaddingBottom(0),
    mUserPaddingRight(0),
    mUserPaddingBottom(0),
    mOldWidthMeasureSpec(0),
    mOldHeightMeasureSpec(0),
    mContentDescription(NULL),
    mBackgroundResource(0),
    mBackgroundSizeChanged(FALSE),
    mDrawableState(NULL),
    mNextFocusLeftId(NO_ID),
    mNextFocusRightId(NO_ID),
    mNextFocusUpId(NO_ID),
    mNextFocusDownId(NO_ID),
    mHasPerformedLongPress(FALSE),
    mMinHeight(0),
    mMinWidth(0),
    mDrawingCacheBackgroundColor(0),
    mTouchSlop(0)
{
}

View::View(
    /* [in] */ IContext* context) :
    mMeasuredWidth(0),
    mMeasuredHeight(0),
    mID(NO_ID),
    mOverScrollMode(0),
    mAttachInfo(NULL),
    mPrivateFlags(0),
    mWindowAttachCount(0),
    mViewFlags(0),
    mLeft(0),
    mRight(0),
    mTop(0),
    mBottom(0),
    mScrollX(0),
    mScrollY(0),
    mPaddingLeft(0),
    mPaddingRight(0),
    mPaddingTop(0),
    mPaddingBottom(0),
    mUserPaddingRight(0),
    mUserPaddingBottom(0),
    mOldWidthMeasureSpec(0),
    mOldHeightMeasureSpec(0),
    mContentDescription(NULL),
    mBackgroundResource(0),
    mBackgroundSizeChanged(FALSE),
    mDrawableState(NULL),
    mNextFocusLeftId(NO_ID),
    mNextFocusRightId(NO_ID),
    mNextFocusUpId(NO_ID),
    mNextFocusDownId(NO_ID),
    mHasPerformedLongPress(FALSE),
    mMinHeight(0),
    mMinWidth(0),
    mDrawingCacheBackgroundColor(0),
    mTouchSlop(0)
{
    Init(context);
}

View::View(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs) :
    mMeasuredWidth(0),
    mMeasuredHeight(0),
    mID(NO_ID),
    mOverScrollMode(0),
    mAttachInfo(NULL),
    mPrivateFlags(0),
    mWindowAttachCount(0),
    mViewFlags(0),
    mLeft(0),
    mRight(0),
    mTop(0),
    mBottom(0),
    mScrollX(0),
    mScrollY(0),
    mPaddingLeft(0),
    mPaddingRight(0),
    mPaddingTop(0),
    mPaddingBottom(0),
    mUserPaddingRight(0),
    mUserPaddingBottom(0),
    mOldWidthMeasureSpec(0),
    mOldHeightMeasureSpec(0),
    mContentDescription(NULL),
    mBackgroundResource(0),
    mBackgroundSizeChanged(FALSE),
    mDrawableState(NULL),
    mNextFocusLeftId(NO_ID),
    mNextFocusRightId(NO_ID),
    mNextFocusUpId(NO_ID),
    mNextFocusDownId(NO_ID),
    mHasPerformedLongPress(FALSE),
    mMinHeight(0),
    mMinWidth(0),
    mDrawingCacheBackgroundColor(0),
    mTouchSlop(0)
{
    Init(context, attrs);
}

View::View(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs,
    /* [in] */ Int32 defStyle) :
    mMeasuredWidth(0),
    mMeasuredHeight(0),
    mID(NO_ID),
    mOverScrollMode(0),
    mAttachInfo(NULL),
    mPrivateFlags(0),
    mWindowAttachCount(0),
    mViewFlags(0),
    mLeft(0),
    mRight(0),
    mTop(0),
    mBottom(0),
    mScrollX(0),
    mScrollY(0),
    mPaddingLeft(0),
    mPaddingRight(0),
    mPaddingTop(0),
    mPaddingBottom(0),
    mUserPaddingRight(0),
    mUserPaddingBottom(0),
    mOldWidthMeasureSpec(0),
    mOldHeightMeasureSpec(0),
    mContentDescription(NULL),
    mBackgroundResource(0),
    mBackgroundSizeChanged(FALSE),
    mDrawableState(NULL),
    mNextFocusLeftId(NO_ID),
    mNextFocusRightId(NO_ID),
    mNextFocusUpId(NO_ID),
    mNextFocusDownId(NO_ID),
    mHasPerformedLongPress(FALSE),
    mMinHeight(0),
    mMinWidth(0),
    mDrawingCacheBackgroundColor(0),
    mTouchSlop(0)
{
    Init(context, attrs, defStyle);
}

View::~View()
{
    if (mDrawableState) {
        ArrayOf<Int32>::Free(mDrawableState);
    }
}

/**
 * <p>
 * Initializes the fading edges from a given set of styled attributes. This
 * method should be called by subclasses that need fading edges and when an
 * instance of these subclasses is created programmatically rather than
 * being inflated from XML. This method is automatically called when the XML
 * is inflated.
 * </p>
 *
 * @param a the styled attributes set to initialize the fading edges from
 */
void View::InitializeFadingEdge(
    /* [in] */ ITypedArray* a)
{
    InitScrollCache();

    ASSERT_SUCCEEDED(a->GetDimensionPixelSize(
            24/*R.styleable.View_fadingEdgeLength*/,
            ViewConfiguration::Get(mContext.Get())->GetScaledFadingEdgeLength(),
            &mScrollCache->mFadingEdgeLength));
}

/**
 * Returns the size of the vertical faded edges used to indicate that more
 * content in this view is visible.
 *
 * @return The size in pixels of the vertical faded edge or 0 if vertical
 *         faded edges are not enabled for this view.
 * @attr ref android.R.styleable#View_fadingEdgeLength
 */
Int32 View::GetVerticalFadingEdgeLength()
{
    if (IsVerticalFadingEdgeEnabled()) {
        AutoPtr<ScrollabilityCache> cache = mScrollCache;
        if (cache != NULL) {
            return cache->mFadingEdgeLength;
        }
    }
    return 0;
}

/**
 * Set the size of the faded edge used to indicate that more content in this
 * view is available.  Will not change whether the fading edge is enabled; use
 * {@link #setVerticalFadingEdgeEnabled} or {@link #setHorizontalFadingEdgeEnabled}
 * to enable the fading edge for the vertical or horizontal fading edges.
 *
 * @param length The size in pixels of the faded edge used to indicate that more
 *        content in this view is visible.
 */
ECode View::SetFadingEdgeLength(
    /* [in] */ Int32 length)
{
    InitScrollCache();
    mScrollCache->mFadingEdgeLength = length;

    return NOERROR;
}

/**
 * Returns the size of the horizontal faded edges used to indicate that more
 * content in this view is visible.
 *
 * @return The size in pixels of the horizontal faded edge or 0 if horizontal
 *         faded edges are not enabled for this view.
 * @attr ref android.R.styleable#View_fadingEdgeLength
 */
Int32 View::GetHorizontalFadingEdgeLength()
{
    if (IsHorizontalFadingEdgeEnabled()) {
        AutoPtr<ScrollabilityCache> cache = mScrollCache;
        if (cache != NULL) {
            return cache->mFadingEdgeLength;
        }
    }
    return 0;
}

/**
 * Returns the width of the vertical scrollbar.
 *
 * @return The width in pixels of the vertical scrollbar or 0 if there
 *         is no vertical scrollbar.
 */
Int32 View::GetVerticalScrollbarWidth()
{
    AutoPtr<ScrollabilityCache> cache = mScrollCache;
    if (cache != NULL) {
        AutoPtr<IScrollBarDrawable> scrollBar = cache->mScrollBar;
        if (scrollBar != NULL) {
            Int32 size;
            scrollBar->GetSize(TRUE, &size);
            if (size <= 0) {
                size = cache->mScrollBarSize;
            }
            return size;
        }
        return 0;
    }
    return 0;
}

/**
 * Returns the height of the horizontal scrollbar.
 *
 * @return The height in pixels of the horizontal scrollbar or 0 if
 *         there is no horizontal scrollbar.
 */
Int32 View::GetHorizontalScrollbarHeight()
{
    AutoPtr<ScrollabilityCache> cache = mScrollCache;
    if (cache != NULL) {
        AutoPtr<IScrollBarDrawable> scrollBar = cache->mScrollBar;
        if (scrollBar != NULL) {
            Int32 size;
            scrollBar->GetSize(FALSE, &size);
            if (size <= 0) {
                size = cache->mScrollBarSize;
            }
            return size;
        }
        return 0;
    }
    return 0;
}

/**
  * <p>
  * Initializes the scrollbars from a given set of styled attributes. This
  * method should be called by subclasses that need scrollbars and when an
  * instance of these subclasses is created programmatically rather than
  * being inflated from XML. This method is automatically called when the XML
  * is inflated.
  * </p>
  *
  * @param a the styled attributes set to initialize the scrollbars from
  */
void View::InitializeScrollbars(
    /* [in] */ ITypedArray* a)
{
    InitScrollCache();

    AutoPtr<ScrollabilityCache> scrollabilityCache = mScrollCache;

    if (scrollabilityCache->mScrollBar == NULL) {
        ASSERT_SUCCEEDED(CScrollBarDrawable::New(
                (IScrollBarDrawable**)&(scrollabilityCache->mScrollBar)));
    }

    Boolean fadeScrollbars;
    a->GetBoolean(44/*R.styleable.View_fadeScrollbars*/, TRUE, &fadeScrollbars);

    if (!fadeScrollbars) {
        scrollabilityCache->mState = ScrollabilityCache::ON;
    }
    scrollabilityCache->mFadeScrollBars = fadeScrollbars;


    a->GetInt32(42/*R.styleable.View_scrollbarFadeDuration*/,
            ViewConfiguration::GetScrollBarFadeDuration(),
            &(scrollabilityCache->mScrollBarFadeDuration));
    a->GetInt32(43/*R.styleable.View_scrollbarDefaultDelayBeforeFade*/,
            ViewConfiguration::GetScrollDefaultDelay(),
            &(scrollabilityCache->mScrollBarDefaultDelayBeforeFade));


    a->GetDimensionPixelSize(0/*com.android.internal.R.styleable.View_scrollbarSize*/,
            ViewConfiguration::Get(mContext)->GetScaledScrollBarSize(),
            &(scrollabilityCache->mScrollBarSize));

    AutoPtr<IDrawable> track;
    a->GetDrawable(3/*R.styleable.View_scrollbarTrackHorizontal*/, (IDrawable**)&track);
    scrollabilityCache->mScrollBar->SetHorizontalTrackDrawable(track.Get());

    AutoPtr<IDrawable> thumb;
    a->GetDrawable(1/*R.styleable.View_scrollbarThumbHorizontal*/, (IDrawable**)&thumb);
    if (thumb != NULL) {
        scrollabilityCache->mScrollBar->SetHorizontalThumbDrawable(thumb.Get());
    }

    Boolean alwaysDraw;
    a->GetBoolean(5/*R.styleable.View_scrollbarAlwaysDrawHorizontalTrack*/,
            FALSE, &alwaysDraw);
    if (alwaysDraw) {
        scrollabilityCache->mScrollBar->SetAlwaysDrawHorizontalTrack(TRUE);
    }

    track = NULL;
    a->GetDrawable(4/*R.styleable.View_scrollbarTrackVertical*/, (IDrawable**)&track);
    scrollabilityCache->mScrollBar->SetVerticalTrackDrawable(track.Get());

    thumb = NULL;
    a->GetDrawable(2/*R.styleable.View_scrollbarThumbVertical*/, (IDrawable**)&thumb);
    if (thumb != NULL) {
        scrollabilityCache->mScrollBar->SetVerticalThumbDrawable(thumb.Get());
    }

    a->GetBoolean(6/*R.styleable.View_scrollbarAlwaysDrawVerticalTrack*/,
            FALSE, &alwaysDraw);
    if (alwaysDraw) {
        scrollabilityCache->mScrollBar->SetAlwaysDrawVerticalTrack(TRUE);
    }

    // Re-apply user/background padding so that scrollbar(s) get added
    RecomputePadding();
}

/**
 * <p>
 * Initalizes the scrollability cache if necessary.
 * </p>
 */
void View::InitScrollCache()
{
    if (mScrollCache == NULL) {
        AutoPtr<ViewConfiguration> configuration = ViewConfiguration::Get(mContext);
        AutoPtr<IView> host = (IView*)this->Probe(EIID_IView);
        mScrollCache = new ScrollabilityCache(configuration, host);
    }
}

/**
 * Register a callback to be invoked when focus of this view changed.
 *
 * @param l The callback that will run.
 */
ECode View::SetOnFocusChangeListener(
    /* [in] */ IViewOnFocusChangeListener* l)
{
    mOnFocusChangeListener = l;

    return NOERROR;
}

/**
 * Returns the focus-change callback registered for this view.
 *
 * @return The callback, or NULL if one is not registered.
 */
ECode View::GetOnFocusChangeListener(
    /* [out] */ IViewOnFocusChangeListener** l)
{
    VALIDATE_NOT_NULL(l);
    *l = mOnFocusChangeListener;
    if (*l) (*l)->AddRef();

    return NOERROR;
}

/**
 * Register a callback to be invoked when this view is clicked. If this view is not
 * clickable, it becomes clickable.
 *
 * @param l The callback that will run
 *
 * @see #setClickable(Boolean)
 */
ECode View::SetOnClickListener(
    /* [in] */ IViewOnClickListener* l)
{
    if (!IsClickable()) {
        SetClickable(TRUE);
    }
    mOnClickListener = l;

    return NOERROR;
}

/**
 * Register a callback to be invoked when this view is clicked and held. If this view is not
 * long clickable, it becomes long clickable.
 *
 * @param l The callback that will run
 *
 * @see #setLongClickable(Boolean)
 */
ECode View::SetOnLongClickListener(
    /* [in] */ IViewOnLongClickListener* l)
{
    if (!IsLongClickable()) {
        SetLongClickable(TRUE);
    }
    mOnLongClickListener = l;

    return NOERROR;
}

/**
 * Register a callback to be invoked when the context menu for this view is
 * being built. If this view is not long clickable, it becomes long clickable.
 *
 * @param l The callback that will run
 *
 */
ECode View::SetOnCreateContextMenuListener(
    /* [in] */ IViewOnCreateContextMenuListener* l)
{
    if (!IsLongClickable()) {
        SetLongClickable(TRUE);
    }
    mOnCreateContextMenuListener = l;

    return NOERROR;
}

/**
 * Call this view's OnClickListener, if it is defined.
 *
 * @return True there was an assigned OnClickListener that was called, FALSE
 *         otherwise is returned.
 */
Boolean View::PerformClick()
{
    SendAccessibilityEvent(AccessibilityEvent_TYPE_VIEW_CLICKED);

    if (mOnClickListener != NULL) {
        PlaySoundEffect(SoundEffectConstants_CLICK);
        mOnClickListener->OnClick((IView*)this->Probe(EIID_IView));
        return TRUE;
    }

    return FALSE;
}

/**
 * Call this view's OnLongClickListener, if it is defined. Invokes the context menu if the
 * OnLongClickListener did not consume the event.
 *
 * @return True if one of the above receivers consumed the event, FALSE otherwise.
 */
Boolean View::PerformLongClick()
{
    SendAccessibilityEvent(AccessibilityEvent_TYPE_VIEW_LONG_CLICKED);

    Boolean handled = FALSE;
    if (mOnLongClickListener != NULL) {
        mOnLongClickListener->OnLongClick((IView*)this->Probe(EIID_IView), &handled);
    }

    if (!handled) {
        handled = ShowContextMenu();
    }

    if (handled) {
        PerformHapticFeedback(HapticFeedbackConstants_LONG_PRESS);
    }
    return handled;
}

/**
 * Bring up the context menu for this view.
 *
 * @return Whether a context menu was displayed.
 */
Boolean View::ShowContextMenu()
{
    Boolean result;
    GetParent()->ShowContextMenuForChild(
        (IView*)this->Probe(EIID_IView), &result);

    return result;
}

/***
 * Register a callback to be invoked when a key is pressed in this view.
 * @param l the key listener to attach to this view
 */
ECode View::SetOnKeyListener(
    /* [in] */ IViewOnKeyListener* l)
{
    mOnKeyListener = l;

    return NOERROR;
}

/**
 * Register a callback to be invoked when a touch event is sent to this view.
 * @param l the touch listener to attach to this view
 */
ECode View::SetOnTouchListener(
    /* [in] */ IViewOnTouchListener* l)
{
    mOnTouchListener = l;

    return NOERROR;
}

/**
 * Give this view focus. This will cause {@link #onFocusChanged} to be called.
 *
 * Note: this does not check whether this {@link View} should get focus, it just
 * gives it focus no matter what.  It should only be called internally by framework
 * code that knows what it is doing, namely {@link #requestFocus(int, Rect)}.
 *
 * @param direction values are View.FOCUS_UP, View.FOCUS_DOWN,
 *        View.FOCUS_LEFT or View.FOCUS_RIGHT. This is the direction which
 *        focus moved when requestFocus() is called. It may not always
 *        apply, in which case use the default View.FOCUS_DOWN.
 * @param previouslyFocusedRect The rectangle of the view that had focus
 *        prior in this View's coordinate system.
 */
ECode View::HandleFocusGainInternal(
    /* [in] */ Int32 direction,
    /* [in] */ IRect* previouslyFocusedRect)
{
    //if (DBG) {
    //    System.out.println(this + " requestFocus()");
    //}

    if ((mPrivateFlags & FOCUSED) == 0) {
        mPrivateFlags |= FOCUSED;
        if (mParent != NULL) {
            mParent->RequestChildFocus(
                    (IView*)this->Probe(EIID_IView),
                    (IView*)this->Probe(EIID_IView));
        }

        OnFocusChanged(TRUE, direction, previouslyFocusedRect);
        RefreshDrawableState();
    }

    return NOERROR;
}

/**
 * Request that a rectangle of this view be visible on the screen,
 * scrolling if necessary just enough.
 *
 * <p>A View should call this if it maintains some notion of which part
 * of its content is interesting.  For example, a text editing view
 * should call this when its cursor moves.
 *
 * @param rectangle The rectangle.
 * @return Whether any parent scrolled.
 */
Boolean View::RequestRectangleOnScreen(
    /* [in] */ IRect* rectangle)
{
    return RequestRectangleOnScreen(rectangle, FALSE);
}

/**
 * Request that a rectangle of this view be visible on the screen,
 * scrolling if necessary just enough.
 *
 * <p>A View should call this if it maintains some notion of which part
 * of its content is interesting.  For example, a text editing view
 * should call this when its cursor moves.
 *
 * <p>When <code>immediate</code> is set to TRUE, scrolling will not be
 * animated.
 *
 * @param rectangle The rectangle.
 * @param immediate True to forbid animated scrolling, FALSE otherwise
 * @return Whether any parent scrolled.
 */
Boolean View::RequestRectangleOnScreen(
    /* [in] */ IRect* rectangle,
    /* [in] */ Boolean immediate)
{
    View* child = this;
    AutoPtr<IViewParent> parent = mParent;
    Boolean scrolled = FALSE;
    while (parent != NULL) {
        Boolean result;

        parent->RequestChildRectangleOnScreen(
                (IView*)child->Probe(EIID_IView),
                rectangle, immediate, &result);

        scrolled |= result;

        // offset rect so next call has the rectangle in the
        // coordinate system of its direct child.
        rectangle->Offset(child->GetLeft(), child->GetTop());
        rectangle->Offset(-child->GetScrollX(), -child->GetScrollY());

        if (parent->Probe(EIID_View) == NULL) {
            break;
        }

        child = (View*)parent->Probe(EIID_View);
        parent = child->GetParent();
    }
    return scrolled;
}

/**
 * Called when this view wants to give up focus. This will cause
 * {@link #onFocusChanged} to be called.
 */
ECode View::ClearFocus()
{
//    if (DBG) {
//        System.out.println(this + " clearFocus()");
//    }

    if ((mPrivateFlags & FOCUSED) != 0) {
        mPrivateFlags &= ~FOCUSED;
        if (mParent != NULL) {
            mParent->ClearChildFocus((IView*)this->Probe(EIID_IView));
        }

        OnFocusChanged(FALSE, 0, NULL);
        RefreshDrawableState();
    }
    return NOERROR;
}

/**
 * Called to clear the focus of a view that is about to be removed.
 * Doesn't call clearChildFocus, which prevents this view from taking
 * focus again before it has been removed from the parent
 */
ECode View::ClearFocusForRemoval()
{
    if ((mPrivateFlags & FOCUSED) != 0) {
        mPrivateFlags &= ~FOCUSED;
        OnFocusChanged(FALSE, 0, NULL);
        RefreshDrawableState();
    }

    return NOERROR;
}

/**
 * Called internally by the view system when a new view is getting focus.
 * This is what clears the old focus.
 */
ECode View::UnFocus()
{
    //if (DBG) {
    //    System.out.println(this + " unFocus()");
    //}

    if ((mPrivateFlags & FOCUSED) != 0) {
        mPrivateFlags &= ~FOCUSED;
        OnFocusChanged(FALSE, 0, NULL);
        RefreshDrawableState();
    }

    return NOERROR;
}

/**
 * Returns TRUE if this view has focus iteself, or is the ancestor of the
 * view that has focus.
 *
 * @return True if this view has or contains focus, FALSE otherwise.
 */
Boolean View::HasFocus()
{
    return (mPrivateFlags & FOCUSED) != 0;
}

/**
 * Returns TRUE if this view is focusable or if it contains a reachable View
 * for which {@link #hasFocusable()} returns TRUE. A "reachable hasFocusable()"
 * is a View whose parents do not block descendants focus.
 *
 * Only {@link #VISIBLE} views are considered focusable.
 *
 * @return True if the view is focusable or if the view contains a focusable
 *         View, FALSE otherwise.
 *
 * @see ViewGroup#FOCUS_BLOCK_DESCENDANTS
 */
Boolean View::HasFocusable()
{
    return (mViewFlags & VISIBILITY_MASK) == VISIBLE && IsFocusable();
}

/**
 * Called by the view system when the focus state of this view changes.
 * When the focus change event is caused by directional navigation, direction
 * and previouslyFocusedRect provide insight into where the focus is coming from.
 * When overriding, be sure to call up through to the super class so that
 * the standard focus handling will occur.
 *
 * @param gainFocus True if the View has focus; FALSE otherwise.
 * @param direction The direction focus has moved when requestFocus()
 *                  is called to give this view focus. Values are
 *                  {@link #FOCUS_UP}, {@link #FOCUS_DOWN}, {@link #FOCUS_LEFT} or
 *                  {@link #FOCUS_RIGHT}. It may not always apply, in which
 *                  case use the default.
 * @param previouslyFocusedRect The rectangle, in this view's coordinate
 *        system, of the previously focused view.  If applicable, this will be
 *        passed in as finer grained information about where the focus is coming
 *        from (in addition to direction).  Will be <code>NULL</code> otherwise.
 */
void View::OnFocusChanged(
    /* [in] */ Boolean gainFocus,
    /* [in] */ Int32 direction,
    /* [in] */ IRect* previouslyFocusedRect)
{
    if (gainFocus) {
        SendAccessibilityEvent(AccessibilityEvent_TYPE_VIEW_FOCUSED);
    }

    //InputMethodManager imm = InputMethodManager.peekInstance();
    if (!gainFocus) {
        if (IsPressed()) {
            SetPressed(FALSE);
        }

        //if (imm != NULL && mAttachInfo != NULL
        //        && mAttachInfo->mHasWindowFocus) {
        //    imm.focusOut(this);
        //}

        OnFocusLost();
    }
    //else if (imm != NULL && mAttachInfo != NULL
    //        && mAttachInfo->mHasWindowFocus) {
    //    imm.focusIn(this);
    //}

    Invalidate();
    if (mOnFocusChangeListener != NULL) {
        mOnFocusChangeListener->OnFocusChange(
            (IView*)this->Probe(EIID_IView), gainFocus);
    }

    if (mAttachInfo != NULL) {
        mAttachInfo->mKeyDispatchState->ResetEx(this->Probe(EIID_IInterface));
    }
}

/**
 * {@inheritDoc}
 */
ECode View::SendAccessibilityEvent(
    /* [in] */ Int32 eventType)
{
    /*if (AccessibilityManager::GetInstance(mContext)->IsEnabled()) {
        SendAccessibilityEventUnchecked(AccessibilityEvent::Obtain(eventType));
    }*/

    return E_NOT_IMPLEMENTED;
}

/**
 * {@inheritDoc}
 */
ECode View::SendAccessibilityEventUnchecked(
    /* [in] */ IAccessibilityEvent* event)
{
//    event.setClassName(getClass().getName());
//    event.setPackageName(getContext().getPackageName());
//    event.setEnabled(isEnabled());
//    event.setContentDescription(mContentDescription);
//
//    if (event.getEventType() == AccessibilityEvent.TYPE_VIEW_FOCUSED && mAttachInfo != NULL) {
//        ArrayList<View> focusablesTempList = mAttachInfo->mFocusablesTempList;
//        getRootView().addFocusables(focusablesTempList, View.FOCUS_FORWARD, FOCUSABLES_ALL);
//        event.setItemCount(focusablesTempList.size());
//        event.setCurrentItemIndex(focusablesTempList.indexOf(this));
//        focusablesTempList.clear();
//    }
//
//    dispatchPopulateAccessibilityEvent(event);
//
//    AccessibilityManager.getInstance(mContext).sendAccessibilityEvent(event);
    return E_NOT_IMPLEMENTED;
}

/**
 * Dispatches an {@link AccessibilityEvent} to the {@link View} children
 * to be populated.
 *
 * @param event The event.
 *
 * @return True if the event population was completed.
 */
Boolean View::DispatchPopulateAccessibilityEvent(
    /* [in] */ IAccessibilityEvent* event)
{
    return FALSE;
}

/**
 * Gets the {@link View} description. It briefly describes the view and is
 * primarily used for accessibility support. Set this property to enable
 * better accessibility support for your application. This is especially
 * TRUE for views that do not have textual representation (For example,
 * ImageButton).
 *
 * @return The content descriptiopn.
 *
 * @attr ref android.R.styleable#View_contentDescription
 */
ArrayOf<Char8>* View::GetContentDescription()
{
    return mContentDescription;
}

/**
 * Sets the {@link View} description. It briefly describes the view and is
 * primarily used for accessibility support. Set this property to enable
 * better accessibility support for your application. This is especially
 * TRUE for views that do not have textual representation (For example,
 * ImageButton).
 *
 * @param contentDescription The content description.
 *
 * @attr ref android.R.styleable#View_contentDescription
 */
ECode View::SetContentDescription(
    /* [in] */ const ArrayOf<Char8>& contentDescription)
{
    mContentDescription = contentDescription.Clone();

    return NOERROR;
}

/**
 * Invoked whenever this view loses focus, either by losing window focus or by losing
 * focus within its window. This method can be used to clear any state tied to the
 * focus. For instance, if a button is held pressed with the trackball and the window
 * loses focus, this method can be used to cancel the press.
 *
 * Subclasses of View overriding this method should always call super.onFocusLost().
 *
 * @see #onFocusChanged(Boolean, int, android.graphics.Rect)
 * @see #onWindowFocusChanged(Boolean)
 *
 * @hide pending API council approval
 */
void View::OnFocusLost()
{
    ResetPressedState();
}

void View::ResetPressedState()
{
    if ((mViewFlags & ENABLED_MASK) == DISABLED) {
        return;
    }

    if (IsPressed()) {
        SetPressed(FALSE);

        if (!mHasPerformedLongPress) {
            RemoveLongPressCallback();
        }
    }
}

/**
 * Returns TRUE if this view has focus
 *
 * @return True if this view has focus, FALSE otherwise.
 */
Boolean View::IsFocused()
{
    return (mPrivateFlags & FOCUSED) != 0;
}

/**
 * Find the view in the hierarchy rooted at this view that currently has
 * focus.
 *
 * @return The view that currently has focus, or NULL if no focused view can
 *         be found.
 */
AutoPtr<IView> View::FindFocus()
{
    return (mPrivateFlags & FOCUSED) != 0 ?
            AutoPtr<IView>((IView*)this->Probe(EIID_IView)) : AutoPtr<IView>(NULL);
}

/**
 * Change whether this view is one of the set of scrollable containers in
 * its window.  This will be used to determine whether the window can
 * resize or must pan when a soft input area is open -- scrollable
 * containers allow the window to use resize mode since the container
 * will appropriately shrink.
 */
ECode View::SetScrollContainer(
    /* [in] */ Boolean isScrollContainer)
{
    if (isScrollContainer) {
        if (mAttachInfo != NULL && (mPrivateFlags & SCROLL_CONTAINER_ADDED) == 0) {
            mAttachInfo->mScrollContainers.PushBack((IView*)this->Probe(EIID_IView));
            mPrivateFlags |= SCROLL_CONTAINER_ADDED;
        }
        mPrivateFlags |= SCROLL_CONTAINER;
    }
    else {
        if ((mPrivateFlags & SCROLL_CONTAINER_ADDED) != 0) {
            mAttachInfo->mScrollContainers.Remove((IView*)this->Probe(EIID_IView));
        }
        mPrivateFlags &= ~(SCROLL_CONTAINER | SCROLL_CONTAINER_ADDED);
    }

    return NOERROR;
}

/**
 * Returns the quality of the drawing cache.
 *
 * @return One of {@link #DRAWING_CACHE_QUALITY_AUTO},
 *         {@link #DRAWING_CACHE_QUALITY_LOW}, or {@link #DRAWING_CACHE_QUALITY_HIGH}
 *
 * @see #setDrawingCacheQuality(int)
 * @see #setDrawingCacheEnabled(Boolean)
 * @see #isDrawingCacheEnabled()
 *
 * @attr ref android.R.styleable#View_drawingCacheQuality
 */
Int32 View::GetDrawingCacheQuality()
{
    return mViewFlags & DRAWING_CACHE_QUALITY_MASK;
}

/**
 * Set the drawing cache quality of this view. This value is used only when the
 * drawing cache is enabled
 *
 * @param quality One of {@link #DRAWING_CACHE_QUALITY_AUTO},
 *        {@link #DRAWING_CACHE_QUALITY_LOW}, or {@link #DRAWING_CACHE_QUALITY_HIGH}
 *
 * @see #getDrawingCacheQuality()
 * @see #setDrawingCacheEnabled(Boolean)
 * @see #isDrawingCacheEnabled()
 *
 * @attr ref android.R.styleable#View_drawingCacheQuality
 */
ECode View::SetDrawingCacheQuality(
    /* [in] */ Int32 quality)
{
    SetFlags(quality, DRAWING_CACHE_QUALITY_MASK);

    return NOERROR;
}

/**
 * Returns whether the screen should remain on, corresponding to the current
 * value of {@link #KEEP_SCREEN_ON}.
 *
 * @return Returns TRUE if {@link #KEEP_SCREEN_ON} is set.
 *
 * @see #setKeepScreenOn(Boolean)
 *
 * @attr ref android.R.styleable#View_keepScreenOn
 */
Boolean View::GetKeepScreenOn()
{
    return (mViewFlags & KEEP_SCREEN_ON) != 0;
}

/**
 * Controls whether the screen should remain on, modifying the
 * value of {@link #KEEP_SCREEN_ON}.
 *
 * @param keepScreenOn Supply TRUE to set {@link #KEEP_SCREEN_ON}.
 *
 * @see #getKeepScreenOn()
 *
 * @attr ref android.R.styleable#View_keepScreenOn
 */
ECode View::SetKeepScreenOn(
    /* [in] */ Boolean keepScreenOn)
{
    SetFlags(keepScreenOn ? KEEP_SCREEN_ON : 0, KEEP_SCREEN_ON);

    return NOERROR;
}

/**
 * @return The user specified next focus ID.
 *
 * @attr ref android.R.styleable#View_nextFocusLeft
 */
Int32 View::GetNextFocusLeftId()
{
    return mNextFocusLeftId;
}

/**
 * Set the id of the view to use for the next focus
 *
 * @param nextFocusLeftId
 *
 * @attr ref android.R.styleable#View_nextFocusLeft
 */
ECode View::SetNextFocusLeftId(
    /* [in] */ Int32 nextFocusLeftId)
{
    mNextFocusLeftId = nextFocusLeftId;

    return NOERROR;
}

/**
 * @return The user specified next focus ID.
 *
 * @attr ref android.R.styleable#View_nextFocusRight
 */
Int32 View::GetNextFocusRightId()
{
    return mNextFocusRightId;
}

/**
 * Set the id of the view to use for the next focus
 *
 * @param nextFocusRightId
 *
 * @attr ref android.R.styleable#View_nextFocusRight
 */
ECode View::SetNextFocusRightId(
    /* [in] */ Int32 nextFocusRightId)
{
    mNextFocusRightId = nextFocusRightId;

    return NOERROR;
}

/**
 * @return The user specified next focus ID.
 *
 * @attr ref android.R.styleable#View_nextFocusUp
 */
Int32 View::GetNextFocusUpId()
{
    return mNextFocusUpId;
}

/**
 * Set the id of the view to use for the next focus
 *
 * @param nextFocusUpId
 *
 * @attr ref android.R.styleable#View_nextFocusUp
 */
ECode View::SetNextFocusUpId(
    /* [in] */ Int32 nextFocusUpId)
{
    mNextFocusUpId = nextFocusUpId;

    return NOERROR;
}

/**
 * @return The user specified next focus ID.
 *
 * @attr ref android.R.styleable#View_nextFocusDown
 */
Int32 View::GetNextFocusDownId()
{
    return mNextFocusDownId;
}

/**
 * Set the id of the view to use for the next focus
 *
 * @param nextFocusDownId
 *
 * @attr ref android.R.styleable#View_nextFocusDown
 */
ECode View::SetNextFocusDownId(
    /* [in] */ Int32 nextFocusDownId)
{
    mNextFocusDownId = nextFocusDownId;

    return NOERROR;
}

/**
 * Returns the visibility of this view and all of its ancestors
 *
 * @return True if this view and all of its ancestors are {@link #VISIBLE}
 */
Boolean View::IsShown()
{
    View* current = this;
    //noinspection ConstantConditions
    do {
        if ((current->mViewFlags & VISIBILITY_MASK) != VISIBLE) {
            return FALSE;
        }
        AutoPtr<IViewParent> parent = current->mParent;
        if (parent == NULL) {
            return FALSE; // We are not attached to the view root
        }
        if (parent->Probe(EIID_View) == NULL) {
            return TRUE;
        }
        current = (View*)parent->Probe(EIID_View);
    } while (current != NULL);

    return FALSE;
}

/**
 * Apply the insets for system windows to this view, if the FITS_SYSTEM_WINDOWS flag
 * is set
 *
 * @param insets Insets for system windows
 *
 * @return True if this view applied the insets, FALSE otherwise
 */
Boolean View::FitSystemWindows(
    /* [in] */ IRect* _insets)
{
    assert(_insets != NULL);
    CRect* insets = (CRect*)_insets;

    if ((mViewFlags & FITS_SYSTEM_WINDOWS) == FITS_SYSTEM_WINDOWS) {
        mPaddingLeft = insets->mLeft;
        mPaddingTop = insets->mTop;
        mPaddingRight = insets->mRight;
        mPaddingBottom = insets->mBottom;
        RequestLayout();
        return TRUE;
    }
    return FALSE;
}

/**
 * Determine if this view has the FITS_SYSTEM_WINDOWS flag set.
 * @return True if window has FITS_SYSTEM_WINDOWS set
 *
 * @hide
 */
Boolean View::IsFitsSystemWindowsFlagSet()
{
    return (mViewFlags & FITS_SYSTEM_WINDOWS) == FITS_SYSTEM_WINDOWS;
}

Int32 View::GetVisibility()
{
    return mViewFlags & VISIBILITY_MASK;
}

/**
 * Set the enabled state of this view.
 *
 * @param visibility One of {@link #VISIBLE}, {@link #INVISIBLE}, or {@link #GONE}.
 * @attr ref android.R.styleable#View_visibility
 */
ECode View::SetVisibility(
    /* [in] */ Int32 visibility)
{
    SetFlags(visibility, VISIBILITY_MASK);
    if (mBGDrawable != NULL) {
        Boolean result;
        mBGDrawable->SetVisible(visibility == VISIBLE, FALSE, &result);
    }
    return NOERROR;
}

/**
 * Returns the enabled status for this view. The interpretation of the
 * enabled state varies by subclass.
 *
 * @return True if this view is enabled, FALSE otherwise.
 */
Boolean View::IsEnabled()
{
    return (mViewFlags & ENABLED_MASK) == ENABLED;
}

/**
 * Set the enabled state of this view. The interpretation of the enabled
 * state varies by subclass.
 *
 * @param enabled True if this view is enabled, FALSE otherwise.
 */
ECode View::SetEnabled(
    /* [in] */ Boolean enabled)
{
    if (enabled == IsEnabled()) return NOERROR;

    Int32 flags = DISABLED;
    if (enabled) flags = ENABLED;
    SetFlags(flags, ENABLED_MASK);

    /*
     * The View most likely has to change its appearance, so refresh
     * the drawable state.
     */
    RefreshDrawableState();

    // Invalidate too, since the default behavior for views is to be
    // be drawn at 50% alpha rather than to change the drawable.
    Invalidate();

    return NOERROR;
}

/**
 * Set whether this view can receive the focus.
 *
 * Setting this to FALSE will also ensure that this view is not focusable
 * in touch mode.
 *
 * @param focusable If TRUE, this view can receive the focus.
 *
 * @see #setFocusableInTouchMode(Boolean)
 * @attr ref android.R.styleable#View_focusable
 */
ECode View::SetFocusable(
    /* [in] */ Boolean focusable)
{
    if (!focusable) {
        SetFlags(0, FOCUSABLE_IN_TOUCH_MODE);
    }
    Int32 flags = NOT_FOCUSABLE;
    if (focusable) flags = FOCUSABLE;
    SetFlags(flags, FOCUSABLE_MASK);

    return NOERROR;
}

/**
 * Set whether this view can receive focus while in touch mode.
 *
 * Setting this to TRUE will also ensure that this view is focusable.
 *
 * @param focusableInTouchMode If TRUE, this view can receive the focus while
 *   in touch mode.
 *
 * @see #setFocusable(Boolean)
 * @attr ref android.R.styleable#View_focusableInTouchMode
 */
ECode View::SetFocusableInTouchMode(
    /* [in] */ Boolean focusableInTouchMode)
{
    // Focusable in touch mode should always be set before the focusable flag
    // otherwise, setting the focusable flag will trigger a focusableViewAvailable()
    // which, in touch mode, will not successfully request focus on this view
    // because the focusable in touch mode flag is not set
    SetFlags(focusableInTouchMode ? FOCUSABLE_IN_TOUCH_MODE : 0, FOCUSABLE_IN_TOUCH_MODE);
    if (focusableInTouchMode) {
        SetFlags(FOCUSABLE, FOCUSABLE_MASK);
    }

    return NOERROR;
}

/**
 * Set whether this view should have sound effects enabled for events such as
 * clicking and touching.
 *
 * <p>You may wish to disable sound effects for a view if you already play sounds,
 * for instance, a dial key that plays dtmf tones.
 *
 * @param soundEffectsEnabled whether sound effects are enabled for this view.
 * @see #isSoundEffectsEnabled()
 * @see #playSoundEffect(int)
 * @attr ref android.R.styleable#View_soundEffectsEnabled
 */
ECode View::SetSoundEffectsEnabled(
    /* [in] */ Boolean soundEffectsEnabled)
{
    SetFlags(soundEffectsEnabled ? SOUND_EFFECTS_ENABLED: 0, SOUND_EFFECTS_ENABLED);

    return NOERROR;
}

/**
 * @return whether this view should have sound effects enabled for events such as
 *     clicking and touching.
 *
 * @see #setSoundEffectsEnabled(Boolean)
 * @see #playSoundEffect(int)
 * @attr ref android.R.styleable#View_soundEffectsEnabled
 */
Boolean View::IsSoundEffectsEnabled()
{
    return SOUND_EFFECTS_ENABLED == (mViewFlags & SOUND_EFFECTS_ENABLED);
}

/**
 * Set whether this view should have haptic feedback for events such as
 * long presses.
 *
 * <p>You may wish to disable haptic feedback if your view already controls
 * its own haptic feedback.
 *
 * @param hapticFeedbackEnabled whether haptic feedback enabled for this view.
 * @see #isHapticFeedbackEnabled()
 * @see #performHapticFeedback(int)
 * @attr ref android.R.styleable#View_hapticFeedbackEnabled
 */
ECode View::SetHapticFeedbackEnabled(
    /* [in] */ Boolean hapticFeedbackEnabled)
{
    SetFlags(hapticFeedbackEnabled ? HAPTIC_FEEDBACK_ENABLED: 0, HAPTIC_FEEDBACK_ENABLED);

    return NOERROR;
}

/**
 * @return whether this view should have haptic feedback enabled for events
 * long presses.
 *
 * @see #setHapticFeedbackEnabled(Boolean)
 * @see #performHapticFeedback(int)
 * @attr ref android.R.styleable#View_hapticFeedbackEnabled
 */
Boolean View::IsHapticFeedbackEnabled()
{
    return HAPTIC_FEEDBACK_ENABLED == (mViewFlags & HAPTIC_FEEDBACK_ENABLED);
}

/**
 * If this view doesn't do any drawing on its own, set this flag to
 * allow further optimizations. By default, this flag is not set on
 * View, but could be set on some View subclasses such as ViewGroup.
 *
 * Typically, if you override {@link #onDraw} you should clear this flag.
 *
 * @param willNotDraw whether or not this View draw on its own
 */
ECode View::SetWillNotDraw(
    /* [in] */ Boolean willNotDraw)
{
    SetFlags(willNotDraw ? WILL_NOT_DRAW : 0, DRAW_MASK);

    return NOERROR;
}

/**
 * Returns whether or not this View draws on its own.
 *
 * @return TRUE if this view has nothing to draw, FALSE otherwise
 */
Boolean View::WillNotDraw()
{
    return (mViewFlags & DRAW_MASK) == WILL_NOT_DRAW;
}

/**
 * When a View's drawing cache is enabled, drawing is redirected to an
 * offscreen bitmap. Some views, like an ImageView, must be able to
 * bypass this mechanism if they already draw a single bitmap, to avoid
 * unnecessary usage of the memory.
 *
 * @param willNotCacheDrawing TRUE if this view does not cache its
 *        drawing, FALSE otherwise
 */
ECode View::SetWillNotCacheDrawing(
    /* [in] */ Boolean willNotCacheDrawing)
{
    SetFlags(willNotCacheDrawing ? WILL_NOT_CACHE_DRAWING : 0, WILL_NOT_CACHE_DRAWING);

    return NOERROR;
}

/**
 * Returns whether or not this View can cache its drawing or not.
 *
 * @return TRUE if this view does not cache its drawing, FALSE otherwise
 */
Boolean View::WillNotCacheDrawing()
{
    return (mViewFlags & WILL_NOT_CACHE_DRAWING) == WILL_NOT_CACHE_DRAWING;
}

/**
 * Indicates whether this view reacts to click events or not.
 *
 * @return TRUE if the view is clickable, FALSE otherwise
 *
 * @see #setClickable(Boolean)
 * @attr ref android.R.styleable#View_clickable
 */
Boolean View::IsClickable()
{
    return (mViewFlags & CLICKABLE) == CLICKABLE;
}

/**
 * Enables or disables click events for this view. When a view
 * is clickable it will change its state to "pressed" on every click.
 * Subclasses should set the view clickable to visually react to
 * user's clicks.
 *
 * @param clickable TRUE to make the view clickable, FALSE otherwise
 *
 * @see #isClickable()
 * @attr ref android.R.styleable#View_clickable
 */
ECode View::SetClickable(
    /* [in] */ Boolean clickable)
{
    SetFlags(clickable ? CLICKABLE : 0, CLICKABLE);

    return NOERROR;
}

/**
 * Indicates whether this view reacts to long click events or not.
 *
 * @return TRUE if the view is long clickable, FALSE otherwise
 *
 * @see #setLongClickable(Boolean)
 * @attr ref android.R.styleable#View_longClickable
 */
Boolean View::IsLongClickable()
{
    return (mViewFlags & LONG_CLICKABLE) == LONG_CLICKABLE;
}

/**
 * Enables or disables long click events for this view. When a view is long
 * clickable it reacts to the user holding down the button for a longer
 * duration than a tap. This event can either launch the listener or a
 * context menu.
 *
 * @param longClickable TRUE to make the view long clickable, FALSE otherwise
 * @see #isLongClickable()
 * @attr ref android.R.styleable#View_longClickable
 */
ECode View::SetLongClickable(
    /* [in] */ Boolean longClickable)
{
    SetFlags(longClickable ? LONG_CLICKABLE : 0, LONG_CLICKABLE);

    return NOERROR;
}

/**
 * Sets the pressed that for this view.
 *
 * @see #isClickable()
 * @see #setClickable(Boolean)
 *
 * @param pressed Pass TRUE to set the View's internal state to "pressed", or FALSE to reverts
 *        the View's internal state from a previously set "pressed" state.
 */
ECode View::SetPressed(
    /* [in] */ Boolean pressed)
{
    if (pressed) {
        mPrivateFlags |= PRESSED;
    }
    else {
        mPrivateFlags &= ~PRESSED;
    }

    RefreshDrawableState();
    DispatchSetPressed(pressed);

    return NOERROR;
}

/**
 * Dispatch setPressed to all of this View's children.
 *
 * @see #setPressed(Boolean)
 *
 * @param pressed The new pressed state
 */
void View::DispatchSetPressed(
    /* [in] */ Boolean pressed)
{
}

/**
 * Indicates whether the view is currently in pressed state. Unless
 * {@link #setPressed(Boolean)} is explicitly called, only clickable views can enter
 * the pressed state.
 *
 * @see #setPressed
 * @see #isClickable()
 * @see #setClickable(Boolean)
 *
 * @return TRUE if the view is currently pressed, FALSE otherwise
 */
Boolean View::IsPressed()
{
    return (mPrivateFlags & PRESSED) == PRESSED;
}

/**
 * Indicates whether this view will save its state (that is,
 * whether its {@link #onSaveInstanceState} method will be called).
 *
 * @return Returns TRUE if the view state saving is enabled, else FALSE.
 *
 * @see #setSaveEnabled(Boolean)
 * @attr ref android.R.styleable#View_saveEnabled
 */
Boolean View::IsSaveEnabled()
{
    return (mViewFlags & SAVE_DISABLED_MASK) != SAVE_DISABLED;
}

/**
 * Controls whether the saving of this view's state is
 * enabled (that is, whether its {@link #onSaveInstanceState} method
 * will be called).  Note that even if freezing is enabled, the
 * view still must have an id assigned to it (via {@link #setId setId()})
 * for its state to be saved.  This flag can only disable the
 * saving of this view; any child views may still have their state saved.
 *
 * @param enabled Set to FALSE to <em>disable</em> state saving, or TRUE
 * (the default) to allow it.
 *
 * @see #isSaveEnabled()
 * @see #setId(int)
 * @see #onSaveInstanceState()
 * @attr ref android.R.styleable#View_saveEnabled
 */
ECode View::SetSaveEnabled(
    /* [in] */ Boolean enabled)
{
    SetFlags(enabled ? 0 : SAVE_DISABLED, SAVE_DISABLED_MASK);

    return NOERROR;
}

/**
 * Gets whether the framework should discard touches when the view's
 * window is obscured by another visible window.
 * Refer to the {@link View} security documentation for more details.
 *
 * @return True if touch filtering is enabled.
 *
 * @see #setFilterTouchesWhenObscured(Boolean)
 * @attr ref android.R.styleable#View_filterTouchesWhenObscured
 */
Boolean View::GetFilterTouchesWhenObscured()
{
    return (mViewFlags & FILTER_TOUCHES_WHEN_OBSCURED) != 0;
}

/**
 * Sets whether the framework should discard touches when the view's
 * window is obscured by another visible window.
 * Refer to the {@link View} security documentation for more details.
 *
 * @param enabled True if touch filtering should be enabled.
 *
 * @see #getFilterTouchesWhenObscured
 * @attr ref android.R.styleable#View_filterTouchesWhenObscured
 */
ECode View::SetFilterTouchesWhenObscured(
    /* [in] */ Boolean enabled)
{
    SetFlags(enabled ? 0 : FILTER_TOUCHES_WHEN_OBSCURED,
            FILTER_TOUCHES_WHEN_OBSCURED);

    return NOERROR;
}

/**
 * Returns whether this View is able to take focus.
 *
 * @return True if this view can take focus, or FALSE otherwise.
 * @attr ref android.R.styleable#View_focusable
 */
Boolean View::IsFocusable()
{
    return FOCUSABLE == (mViewFlags & FOCUSABLE_MASK);
}

/**
 * When a view is focusable, it may not want to take focus when in touch mode.
 * For example, a button would like focus when the user is navigating via a D-pad
 * so that the user can click on it, but once the user starts touching the screen,
 * the button shouldn't take focus
 * @return Whether the view is focusable in touch mode.
 * @attr ref android.R.styleable#View_focusableInTouchMode
 */
Boolean View::IsFocusableInTouchMode()
{
    return FOCUSABLE_IN_TOUCH_MODE == (mViewFlags & FOCUSABLE_IN_TOUCH_MODE);
}

/**
 * Find the nearest view in the specified direction that can take focus.
 * This does not actually give focus to that view.
 *
 * @param direction One of FOCUS_UP, FOCUS_DOWN, FOCUS_LEFT, and FOCUS_RIGHT
 *
 * @return The nearest focusable in the specified direction, or NULL if none
 *         can be found.
 */
AutoPtr<IView> View::FocusSearch(
    /* [in] */ Int32 direction)
{
    if (mParent != NULL) {
        AutoPtr<IView> view;
        mParent->FocusSearchEx((IView*)this->Probe(EIID_IView), direction, (IView**)&view);
        return view;
    }
    else {
        return AutoPtr<IView>(NULL);
    }
}

/**
 * This method is the last chance for the focused view and its ancestors to
 * respond to an arrow key. This is called when the focused view did not
 * consume the key internally, nor could the view system find a new view in
 * the requested direction to give focus to.
 *
 * @param focused The currently focused view.
 * @param direction The direction focus wants to move. One of FOCUS_UP,
 *        FOCUS_DOWN, FOCUS_LEFT, and FOCUS_RIGHT.
 * @return True if the this view consumed this unhandled move.
 */
Boolean View::DispatchUnhandledMove(
    /* [in] */ IView* focused,
    /* [in] */ Int32 direction)
{
    return FALSE;
}

/**
 * If a user manually specified the next view id for a particular direction,
 * use the root to look up the view.  Once a view is found, it is cached
 * for future lookups.
 * @param root The root view of the hierarchy containing this view.
 * @param direction One of FOCUS_UP, FOCUS_DOWN, FOCUS_LEFT, and FOCUS_RIGHT
 * @return The user specified next view, or NULL if there is none.
 */
AutoPtr<IView> View::FindUserSetNextFocus(
    /* [in] */ IView* root,
    /* [in] */ Int32 direction)
{
    switch (direction) {
        case FOCUS_LEFT:
            if (mNextFocusLeftId == View::NO_ID) return NULL;
            return FindViewShouldExist(root, mNextFocusLeftId);
        case FOCUS_RIGHT:
            if (mNextFocusRightId == View::NO_ID) return NULL;
            return FindViewShouldExist(root, mNextFocusRightId);
        case FOCUS_UP:
            if (mNextFocusUpId == View::NO_ID) return NULL;
            return FindViewShouldExist(root, mNextFocusUpId);
        case FOCUS_DOWN:
            if (mNextFocusDownId == View::NO_ID) return NULL;
            return FindViewShouldExist(root, mNextFocusDownId);
    }
    return AutoPtr<IView>(NULL);
}

AutoPtr<IView> View::FindViewShouldExist(
    /* [in] */ IView* root,
    /* [in] */ Int32 childViewId)
{
    AutoPtr<IView> result;
    root->FindViewById(childViewId, (IView**)&result);
    if (result == NULL) {
        //Log.w(VIEW_LOG_TAG, "couldn't find next focus view specified "
        //        + "by user for id " + childViewId);
    }
    return result;
}

/**
 * Find and return all focusable views that are descendants of this view,
 * possibly including this view if it is focusable itself.
 *
 * @param direction The direction of the focus
 * @return A list of focusable views
 */
ECode View::GetFocusables(
    /* [in] */ Int32 direction,
    /* [out] */ IObjectContainer** views)
{
    assert(views != NULL);

    FAIL_RETURN(CParcelableObjectContainer::New(views));
    return AddFocusables(*views, direction);
}

/**
 * Add any focusable views that are descendants of this view (possibly
 * including this view if it is focusable itself) to views.  If we are in touch mode,
 * only add views that are also focusable in touch mode.
 *
 * @param views Focusable views found so far
 * @param direction The direction of the focus
 */
ECode View::AddFocusables(
    /* [in] */ IObjectContainer* views,
    /* [in] */ Int32 direction)
{
    return AddFocusables(views, direction, FOCUSABLES_TOUCH_MODE);
}

/**
 * Adds any focusable views that are descendants of this view (possibly
 * including this view if it is focusable itself) to views. This method
 * adds all focusable views regardless if we are in touch mode or
 * only views focusable in touch mode if we are in touch mode depending on
 * the focusable mode paramater.
 *
 * @param views Focusable views found so far or NULL if all we are interested is
 *        the number of focusables.
 * @param direction The direction of the focus.
 * @param focusableMode The type of focusables to be added.
 *
 * @see #FOCUSABLES_ALL
 * @see #FOCUSABLES_TOUCH_MODE
 */
ECode View::AddFocusables(
    /* [in, out] */ IObjectContainer* views,
    /* [in] */ Int32 direction,
    /* [in] */ Int32 focusableMode)
{
    if (!IsFocusable()) {
        return NOERROR;
    }

    if ((focusableMode & FOCUSABLES_TOUCH_MODE) == FOCUSABLES_TOUCH_MODE &&
            IsInTouchMode() && !IsFocusableInTouchMode()) {
        return NOERROR;
    }

    if (views != NULL) {
        views->Add((IView*)this->Probe(EIID_IView));
    }

    return NOERROR;
}

/**
 * Find and return all touchable views that are descendants of this view,
 * possibly including this view if it is touchable itself.
 *
 * @return A list of touchable views
 */
ECode View::GetTouchables(
    /* [out] */ IObjectContainer** views)
{
    assert(views != NULL);

    FAIL_RETURN(CParcelableObjectContainer::New(views));
    return AddTouchables(*views);
}

/**
 * Add any touchable views that are descendants of this view (possibly
 * including this view if it is touchable itself) to views.
 *
 * @param views Touchable views found so far
 */
ECode View::AddTouchables(
    /* [in, out] */ IObjectContainer* views)
{
    Int32 viewFlags = mViewFlags;

    if (((viewFlags & CLICKABLE) == CLICKABLE || (viewFlags & LONG_CLICKABLE) == LONG_CLICKABLE)
            && (viewFlags & ENABLED_MASK) == ENABLED) {
        views->Add((IView*)this->Probe(EIID_IView));
    }

    return NOERROR;
}

/**
 * Call this to try to give focus to a specific view or to one of its
 * descendants.
 *
 * A view will not actually take focus if it is not focusable ({@link #isFocusable} returns FALSE),
 * or if it is focusable and it is not focusable in touch mode ({@link #isFocusableInTouchMode})
 * while the device is in touch mode.
 *
 * See also {@link #focusSearch}, which is what you call to say that you
 * have focus, and you want your parent to look for the next one.
 *
 * This is equivalent to calling {@link #requestFocus(int, Rect)} with arguments
 * {@link #FOCUS_DOWN} and <code>NULL</code>.
 *
 * @return Whether this view or one of its descendants actually took focus.
 */
Boolean View::RequestFocus()
{
    return RequestFocus(View::FOCUS_DOWN);
}

/**
 * Call this to try to give focus to a specific view or to one of its
 * descendants and give it a hint about what direction focus is heading.
 *
 * A view will not actually take focus if it is not focusable ({@link #isFocusable} returns FALSE),
 * or if it is focusable and it is not focusable in touch mode ({@link #isFocusableInTouchMode})
 * while the device is in touch mode.
 *
 * See also {@link #focusSearch}, which is what you call to say that you
 * have focus, and you want your parent to look for the next one.
 *
 * This is equivalent to calling {@link #requestFocus(int, Rect)} with
 * <code>NULL</code> set for the previously focused rectangle.
 *
 * @param direction One of FOCUS_UP, FOCUS_DOWN, FOCUS_LEFT, and FOCUS_RIGHT
 * @return Whether this view or one of its descendants actually took focus.
 */
Boolean View::RequestFocus(
    /* [in] */ Int32 direction)
{
    return RequestFocus(direction, NULL);
}

/**
 * Call this to try to give focus to a specific view or to one of its descendants
 * and give it hints about the direction and a specific rectangle that the focus
 * is coming from.  The rectangle can help give larger views a finer grained hint
 * about where focus is coming from, and therefore, where to show selection, or
 * forward focus change internally.
 *
 * A view will not actually take focus if it is not focusable ({@link #isFocusable} returns FALSE),
 * or if it is focusable and it is not focusable in touch mode ({@link #isFocusableInTouchMode})
 * while the device is in touch mode.
 *
 * A View will not take focus if it is not visible.
 *
 * A View will not take focus if one of its parents has {@link android.view.ViewGroup#getDescendantFocusability()}
 * equal to {@link ViewGroup#FOCUS_BLOCK_DESCENDANTS}.
 *
 * See also {@link #focusSearch}, which is what you call to say that you
 * have focus, and you want your parent to look for the next one.
 *
 * You may wish to override this method if your custom {@link View} has an internal
 * {@link View} that it wishes to forward the request to.
 *
 * @param direction One of FOCUS_UP, FOCUS_DOWN, FOCUS_LEFT, and FOCUS_RIGHT
 * @param previouslyFocusedRect The rectangle (in this View's coordinate system)
 *        to give a finer grained hint about where focus is coming from.  May be NULL
 *        if there is no hint.
 * @return Whether this view or one of its descendants actually took focus.
 */
Boolean View::RequestFocus(
    /* [in] */ Int32 direction,
    /* [in] */IRect* previouslyFocusedRect)
{
    // need to be focusable
    if ((mViewFlags & FOCUSABLE_MASK) != FOCUSABLE ||
            (mViewFlags & VISIBILITY_MASK) != VISIBLE) {
        return FALSE;
    }

    // need to be focusable in touch mode if in touch mode
    if (IsInTouchMode() &&
            (FOCUSABLE_IN_TOUCH_MODE != (mViewFlags & FOCUSABLE_IN_TOUCH_MODE))) {
        return FALSE;
    }

    // need to not have any parents blocking us
    if (HasAncestorThatBlocksDescendantFocus()) {
        return FALSE;
    }

    HandleFocusGainInternal(direction, previouslyFocusedRect);

    return TRUE;
}

/**
 * Call this to try to give focus to a specific view or to one of its descendants. This is a
 * special variant of {@link #requestFocus() } that will allow views that are not focuable in
 * touch mode to request focus when they are touched.
 *
 * @return Whether this view or one of its descendants actually took focus.
 *
 * @see #isInTouchMode()
 *
 */
Boolean View::RequestFocusFromTouch()
{
    // Leave touch mode if we need to
    if (IsInTouchMode()) {
        AutoPtr<IView> root = GetRootView();
        if (root != NULL) {
            AutoPtr<IViewParent> parent;
            root->GetParent((IViewParent**)&parent);
            ViewRoot* viewRoot = (ViewRoot*)parent.Get();
            if (viewRoot != NULL) {
                viewRoot->EnsureTouchMode(FALSE);
            }
        }
    }
    return RequestFocus(View::FOCUS_DOWN);
}

/**
 * @return Whether any ancestor of this view blocks descendant focus.
 */
Boolean View::HasAncestorThatBlocksDescendantFocus()
{
    AutoPtr<IViewParent> ancestor = mParent;
    while (ancestor != NULL && ancestor->Probe(EIID_IViewGroup) != NULL) {
        AutoPtr<IViewGroup> vgAncestor = (IViewGroup*)ancestor->Probe(EIID_IViewGroup);
        Int32 focusability;
        vgAncestor->GetDescendantFocusability(&focusability);
        if (focusability == ViewGroup_FOCUS_BLOCK_DESCENDANTS) {
            return TRUE;
        }
        else {
            ancestor = NULL;
            vgAncestor->GetParent((IViewParent**)&ancestor);
        }
    }
    return FALSE;
}

/**
 * @hide
 */
ECode View::DispatchStartTemporaryDetach()
{
    return OnStartTemporaryDetach();
}

/**
 * This is called when a container is going to temporarily detach a child, with
 * {@link ViewGroup#detachViewFromParent(View) ViewGroup.detachViewFromParent}.
 * It will either be followed by {@link #onFinishTemporaryDetach()} or
 * {@link #onDetachedFromWindow()} when the container is done.
 */
ECode View::OnStartTemporaryDetach()
{
    RemoveUnsetPressCallback();
    mPrivateFlags |= CANCEL_NEXT_UP_EVENT;

    return NOERROR;
}

/**
 * @hide
 */
ECode View::DispatchFinishTemporaryDetach()
{
    return OnFinishTemporaryDetach();
}

/**
 * Called after {@link #onStartTemporaryDetach} when the container is done
 * changing the view.
 */
ECode View::OnFinishTemporaryDetach()
{
    return NOERROR;
}

/**
 * capture information of this view for later analysis: developement only
 * check dynamic switch to make sure we only dump view
 * when ViewDebug.SYSTEM_PROPERTY_CAPTURE_VIEW) is set
 */
void View::CaptureViewInfo(
    /* [in] */ const char* subTag,
    /* [in] */ IView* v)
{
    //if (v == NULL || SystemProperties.getInt(ViewDebug.SYSTEM_PROPERTY_CAPTURE_VIEW, 0) == 0) {
     //   return;
    //}
    //ViewDebug.dumpCapturedView(subTag, v);
}

/**
 * Return the global {@link KeyEvent.DispatcherState KeyEvent.DispatcherState}
 * for this view's window.  Returns NULL if the view is not currently attached
 * to the window.  Normally you will not need to use this directly, but
 * just use the standard high-level event callbacks like {@link #onKeyDown}.
 */
ECode View::GetKeyDispatcherState(
    /* [out] */ IDispatcherState** state)
{
    VALIDATE_NOT_NULL(state);
    *state = mAttachInfo != NULL ? mAttachInfo->mKeyDispatchState : NULL;
    if (*state) (*state)->AddRef();

    return NOERROR;
}

/**
 * Dispatch a key event before it is processed by any input method
 * associated with the view hierarchy.  This can be used to intercept
 * key events in special situations before the IME consumes them; a
 * typical example would be handling the BACK key to update the application's
 * UI instead of allowing the IME to see it and close itself.
 *
 * @param event The key event to be dispatched.
 * @return True if the event was handled, FALSE otherwise.
 */
Boolean View::DispatchKeyEventPreIme(
    /* [in] */ IKeyEvent* event)
{

    Int32 keyCode;
    event->GetKeyCode(&keyCode);
    return OnKeyPreIme(keyCode, event);
}

/**
 * Dispatch a key event to the next view on the focus path. This path runs
 * from the top of the view tree down to the currently focused view. If this
 * view has focus, it will dispatch to itself. Otherwise it will dispatch
 * the next node down the focus path. This method also fires any key
 * listeners.
 *
 * @param event The key event to be dispatched.
 * @return True if the event was handled, FALSE otherwise.
 */
Boolean View::DispatchKeyEvent(
    /* [in] */ IKeyEvent* event)
{
    // If any attached key listener a first crack at the event.
    //noinspection SimplifiableIfStatement

    if (DBG) {
        CaptureViewInfo("captureViewKeyEvent", (IView*)this->Probe(EIID_IView));
    }

    if (mOnKeyListener != NULL) {
        Int32 keyCode;
        event->GetKeyCode(&keyCode);
        Boolean result = FALSE;
        mOnKeyListener->OnKey(
            (IView*)this->Probe(EIID_IView), keyCode, event, &result);

        if ((mViewFlags & ENABLED_MASK) == ENABLED && result) {
            return TRUE;
        }
    }

    Boolean result;
    event->DispatchEx(
            (IKeyEventCallback*)this->Probe(EIID_IKeyEventCallback),
            mAttachInfo != NULL ? mAttachInfo->mKeyDispatchState : NULL,
            (IInterface*)this->Probe(EIID_IInterface), &result);

    return result;
}

/**
 * Dispatches a key shortcut event.
 *
 * @param event The key event to be dispatched.
 * @return True if the event was handled by the view, FALSE otherwise.
 */
Boolean View::DispatchKeyShortcutEvent(
    /* [in] */ IKeyEvent* event)
{
    Int32 keyCode;
    event->GetKeyCode(&keyCode);
    return OnKeyShortcut(keyCode, event);
}

/**
 * Pass the touch screen motion event down to the target view, or this
 * view if it is the target.
 *
 * @param event The motion event to be dispatched.
 * @return True if the event was handled by the view, FALSE otherwise.
 */
Boolean View::DispatchTouchEvent(
    /* [in] */ IMotionEvent* event)
{
    //printf("View::DispatchTouchEvent, ID = 0x%08x\n", mID);
    if (!OnFilterTouchEventForSecurity(event)) {
        return FALSE;
    }

    if (mOnTouchListener != NULL) {
        Boolean result = FALSE;
        mOnTouchListener->OnTouch((IView*)this->Probe(EIID_IView), event, &result);
        if ((mViewFlags & ENABLED_MASK) == ENABLED && result) {
            return TRUE;
        }
    }

    return OnTouchEvent(event);
}

/**
 * Filter the touch event to apply security policies.
 *
 * @param event The motion event to be filtered.
 * @return True if the event should be dispatched, FALSE if the event should be dropped.
 *
 * @see #getFilterTouchesWhenObscured
 */
Boolean View::OnFilterTouchEventForSecurity(
    /* [in] */ IMotionEvent* event)
{
    Int32 flag;
    event->GetFlags(&flag);
    if ((mViewFlags & FILTER_TOUCHES_WHEN_OBSCURED) != 0
        && (flag & MotionEvent_FLAG_WINDOW_IS_OBSCURED) != 0) {
        // Window is obscured, drop this touch.
        return FALSE;
    }
    return TRUE;
}

/**
 * Pass a trackball motion event down to the focused view.
 *
 * @param event The motion event to be dispatched.
 * @return True if the event was handled by the view, FALSE otherwise.
 */
Boolean View::DispatchTrackballEvent(
    /* [in] */ IMotionEvent* event)
{
    //Log.i("view", "view=" + this + ", " + event.toString());
    return OnTrackballEvent(event);
}

/**
 * Called when the window containing this view gains or loses window focus.
 * ViewGroups should override to route to their children.
 *
 * @param hasFocus True if the window containing this view now has focus,
 *        FALSE otherwise.
 */
ECode View::DispatchWindowFocusChanged(
    /* [in] */ Boolean hasFocus)
{
    return OnWindowFocusChanged(hasFocus);
}

/**
 * Called when the window containing this view gains or loses focus.  Note
 * that this is separate from view focus: to receive key events, both
 * your view and its window must have focus.  If a window is displayed
 * on top of yours that takes input focus, then your own window will lose
 * focus but the view focus will remain unchanged.
 *
 * @param hasWindowFocus True if the window containing this view now has
 *        focus, FALSE otherwise.
 */
ECode View::OnWindowFocusChanged(
    /* [in] */ Boolean hasWindowFocus)
{
    //InputMethodManager imm = InputMethodManager->PeekInstance();
    if (!hasWindowFocus) {
        if (IsPressed()) {
            SetPressed(FALSE);
        }

        //if (imm != NULL && (mPrivateFlags & FOCUSED) != 0) {
        //    imm->FocusOut(this);
        //}

        RemoveLongPressCallback();
        OnFocusLost();
    }
    //else if (imm != NULL && (mPrivateFlags & FOCUSED) != 0) {
    //    imm->FocusIn(this);
    //}
    RefreshDrawableState();

    return NOERROR;
}

/**
 * Returns TRUE if this view is in a window that currently has window focus.
 * Note that this is not the same as the view itself having focus.
 *
 * @return True if this view is in a window that currently has window focus.
 */
Boolean View::HasWindowFocus()
{
    return mAttachInfo != NULL && mAttachInfo->mHasWindowFocus;
}

/**
 * Dispatch a view visibility change down the view hierarchy.
 * ViewGroups should override to route to their children.
 * @param changedView The view whose visibility changed. Could be 'this' or
 * an ancestor view.
 * @param visibility The new visibility of changedView: {@link #VISIBLE},
 * {@link #INVISIBLE} or {@link #GONE}.
 */
void View::DispatchVisibilityChanged(
    /* [in] */ IView* changedView,
    /* [in] */ Int32 visibility)
{
    OnVisibilityChanged(changedView, visibility);
}

/**
 * Called when the visibility of the view or an ancestor of the view is changed.
 * @param changedView The view whose visibility changed. Could be 'this' or
 * an ancestor view.
 * @param visibility The new visibility of changedView: {@link #VISIBLE},
 * {@link #INVISIBLE} or {@link #GONE}.
 */
void View::OnVisibilityChanged(
    /* [in] */ IView* changedView,
    /* [in] */ Int32 visibility)
{
    if (visibility == VISIBLE) {
        if (mAttachInfo != NULL) {
            InitialAwakenScrollBars();
        }
        else {
            mPrivateFlags |= AWAKEN_SCROLL_BARS_ON_ATTACH;
        }
    }
}

/**
 * Dispatch a hint about whether this view is displayed. For instance, when
 * a View moves out of the screen, it might receives a display hint indicating
 * the view is not displayed. Applications should not <em>rely</em> on this hint
 * as there is no guarantee that they will receive one.
 *
 * @param hint A hint about whether or not this view is displayed:
 * {@link #VISIBLE} or {@link #INVISIBLE}.
 */
ECode View::DispatchDisplayHint(
    /* [in] */ Int32 hint)
{
    OnDisplayHint(hint);

    return NOERROR;
}

/**
 * Gives this view a hint about whether is displayed or not. For instance, when
 * a View moves out of the screen, it might receives a display hint indicating
 * the view is not displayed. Applications should not <em>rely</em> on this hint
 * as there is no guarantee that they will receive one.
 *
 * @param hint A hint about whether or not this view is displayed:
 * {@link #VISIBLE} or {@link #INVISIBLE}.
 */
void View::OnDisplayHint(
    /* [in] */ Int32 hint)
{
}

/**
 * Dispatch a window visibility change down the view hierarchy.
 * ViewGroups should override to route to their children.
 *
 * @param visibility The new visibility of the window.
 *
 * @see #onWindowVisibilityChanged
 */
ECode View::DispatchWindowVisibilityChanged(
    /* [in] */ Int32 visibility)
{
    OnWindowVisibilityChanged(visibility);

    return NOERROR;
}

/**
 * Called when the window containing has change its visibility
 * (between {@link #GONE}, {@link #INVISIBLE}, and {@link #VISIBLE}).  Note
 * that this tells you whether or not your window is being made visible
 * to the window manager; this does <em>not</em> tell you whether or not
 * your window is obscured by other windows on the screen, even if it
 * is itself visible.
 *
 * @param visibility The new visibility of the window.
 */
void View::OnWindowVisibilityChanged(
    /* [in] */ Int32 visibility)
{
    if (visibility == VISIBLE) {
        InitialAwakenScrollBars();
    }
}

/**
 * Returns the current visibility of the window this view is attached to
 * (either {@link #GONE}, {@link #INVISIBLE}, or {@link #VISIBLE}).
 *
 * @return Returns the current visibility of the view's window.
 */
Int32 View::GetWindowVisibility()
{
    return mAttachInfo != NULL ? mAttachInfo->mWindowVisibility : GONE;
}

/**
 * Retrieve the overall visible display size in which the window this view is
 * attached to has been positioned in.  This takes into account screen
 * decorations above the window, for both cases where the window itself
 * is being position inside of them or the window is being placed under
 * then and covered insets are used for the window to position its content
 * inside.  In effect, this tells you the available area where content can
 * be placed and remain visible to users.
 *
 * <p>This function requires an IPC back to the window manager to retrieve
 * the requested information, so should not be used in performance critical
 * code like drawing.
 *
 * @param outRect Filled in with the visible display frame.  If the view
 * is not attached to a window, this is simply the raw display size.
 */
ECode View::GetWindowVisibleDisplayFrame(
    /* [in, out] */ IRect* outRect)
{
    assert(outRect != NULL);

    if (mAttachInfo != NULL)
    {
        AutoPtr<IRect> r;
        FAIL_RETURN(mAttachInfo->mSession->GetDisplayFrame(
                mAttachInfo->mWindow.Get(), (IRect**)&r));
        // XXX This is really broken, and probably all needs to be done
        // in the window manager, and we need to know more about whether
        // we want the area behind or in front of the IME.
        CRect* insets = mAttachInfo->mVisibleInsets.Get();
        CRect* _r = (CRect*)r.Get();
        CRect* _outRect = (CRect*)outRect;
        _outRect->mLeft = _r->mLeft + insets->mLeft;
        _outRect->mTop = _r->mTop + insets->mTop;
        _outRect->mRight = _r->mRight - insets->mRight;
        _outRect->mBottom = _r->mBottom - insets->mBottom;
        return NOERROR;
    }
    AutoPtr<IDisplay> d;
    CWindowManagerImpl::GetDefault()->GetDefaultDisplay((IDisplay**)&d);
    Int32 width, height;
    d->GetWidth(&width);
    d->GetHeight(&height);
    outRect->Set(0, 0, width, height);

    return NOERROR;
}

/**
 * Dispatch a notification about a resource configuration change down
 * the view hierarchy.
 * ViewGroups should override to route to their children.
 *
 * @param newConfig The new resource configuration.
 *
 * @see #onConfigurationChanged
 */
ECode View::DispatchConfigurationChanged(
    /* [in] */ IConfiguration* newConfig)
{
    OnConfigurationChanged(newConfig);

    return NOERROR;
}

/**
 * Called when the current configuration of the resources being used
 * by the application have changed.  You can use this to decide when
 * to reload resources that can changed based on orientation and other
 * configuration characterstics.  You only need to use this if you are
 * not relying on the normal {@link android.app.Activity} mechanism of
 * recreating the activity instance upon a configuration change.
 *
 * @param newConfig The new resource configuration.
 */
void View::OnConfigurationChanged(
    /* [in] */ IConfiguration* newConfig)
{
}

/**
 * Private function to aggregate all per-view attributes in to the view
 * root.
 */
ECode View::DispatchCollectViewAttributes(
    /* [in] */ Int32 visibility)
{
    return PerformCollectViewAttributes(visibility);
}

ECode View::PerformCollectViewAttributes(
    /* [in] */ Int32 visibility)
{
    //noinspection PointlessBitwiseExpression
    if (((visibility | mViewFlags) & (VISIBILITY_MASK | KEEP_SCREEN_ON))
            == (VISIBLE | KEEP_SCREEN_ON)) {
        mAttachInfo->mKeepScreenOn = TRUE;
    }

    return NOERROR;
}

ECode View::NeedGlobalAttributesUpdate(
    /* [in] */ Boolean force)
{
    AttachInfo* ai = mAttachInfo;
    if (ai != NULL) {
        if (ai->mKeepScreenOn || force) {
            ai->mRecomputeGlobalAttributes = TRUE;
        }
    }

    return NOERROR;
}

/**
 * Returns whether the device is currently in touch mode.  Touch mode is entered
 * once the user begins interacting with the device by touch, and affects various
 * things like whether focus is always visible to the user.
 *
 * @return Whether the device is in touch mode.
 */
Boolean View::IsInTouchMode()
{
    if (mAttachInfo != NULL) {
        return mAttachInfo->mInTouchMode;
    }
    else {
        return FALSE;
        return ViewRoot::IsInTouchMode();
    }
}

AutoPtr<IContext> View::GetContext()
{
    return mContext;
}

/**
 * Handle a key event before it is processed by any input method
 * associated with the view hierarchy.  This can be used to intercept
 * key events in special situations before the IME consumes them; a
 * typical example would be handling the BACK key to update the application's
 * UI instead of allowing the IME to see it and close itself.
 *
 * @param keyCode The value in event.getKeyCode().
 * @param event Description of the key event.
 * @return If you handled the event, return TRUE. If you want to allow the
 *         event to be handled by the next receiver, return FALSE.
 */
Boolean View::OnKeyPreIme(
    /* [in] */ Int32 keyCode,
    /* [in] */ IKeyEvent* event)
{
    return FALSE;
}

/**
 * Default implementation of {@link KeyEvent.Callback#onKeyMultiple(int, int, KeyEvent)
 * KeyEvent.Callback.onKeyMultiple()}: perform press of the view
 * when {@link KeyEvent#KEYCODE_DPAD_CENTER} or {@link KeyEvent#KEYCODE_ENTER}
 * is released, if the view is enabled and clickable.
 *
 * @param keyCode A key code that represents the button pressed, from
 *                {@link android.view.KeyEvent}.
 * @param event   The KeyEvent object that defines the button action.
 */
Boolean View::OnKeyDown(
    /* [in] */ Int32 keyCode,
    /* [in] */ IKeyEvent* event)
{
    switch (keyCode) {
    case KeyEvent_KEYCODE_DPAD_CENTER:
    case KeyEvent_KEYCODE_ENTER:
        {
            if ((mViewFlags & ENABLED_MASK) == DISABLED) {
                return TRUE;
            }

            Int32 repeatCount;
            event->GetRepeatCount(&repeatCount);
            // Long clickable items don't necessarily have to be clickable
            if (((mViewFlags & CLICKABLE) == CLICKABLE ||
                (mViewFlags & LONG_CLICKABLE) == LONG_CLICKABLE) &&
                (repeatCount == 0)) {
                SetPressed(TRUE);

                if ((mViewFlags & LONG_CLICKABLE) == LONG_CLICKABLE) {
                    PostCheckForLongClick(0);
                }
                return TRUE;
            }
        }
        break;
    default:
        break;
    }

    return FALSE;
}

/**
 * Default implementation of {@link KeyEvent.Callback#onKeyLongPress(int, KeyEvent)
 * KeyEvent.Callback.onKeyLongPress()}: always returns FALSE (doesn't handle
 * the event).
 */
Boolean View::OnKeyLongPress(
    /* [in] */ Int32 keyCode,
    /* [in] */ IKeyEvent* event)
{
    return FALSE;
}

/**
 * Default implementation of {@link KeyEvent.Callback#onKeyMultiple(int, int, KeyEvent)
 * KeyEvent.Callback.onKeyMultiple()}: perform clicking of the view
 * when {@link KeyEvent#KEYCODE_DPAD_CENTER} or
 * {@link KeyEvent#KEYCODE_ENTER} is released.
 *
 * @param keyCode A key code that represents the button pressed, from
 *                {@link android.view.KeyEvent}.
 * @param event   The KeyEvent object that defines the button action.
 */
Boolean View::OnKeyUp(
    /* [in] */ Int32 keyCode,
    /* [in] */ IKeyEvent* event)
{
    Boolean result = FALSE;

    switch (keyCode) {
    case KeyEvent_KEYCODE_DPAD_CENTER:
    case KeyEvent_KEYCODE_ENTER:
        {
            if ((mViewFlags & ENABLED_MASK) == DISABLED) {
                return TRUE;
            }

            if ((mViewFlags & CLICKABLE) == CLICKABLE && IsPressed()) {
                SetPressed(FALSE);

                if (!mHasPerformedLongPress) {
                    // This is a tap, so remove the longpress check
                    RemoveLongPressCallback();

                    result = PerformClick();
                }
            }
        }
        break;
    default:
        break;
    }
    return result;
}

/**
 * Default implementation of {@link KeyEvent.Callback#onKeyMultiple(int, int, KeyEvent)
 * KeyEvent.Callback.onKeyMultiple()}: always returns FALSE (doesn't handle
 * the event).
 *
 * @param keyCode     A key code that represents the button pressed, from
 *                    {@link android.view.KeyEvent}.
 * @param repeatCount The number of times the action was made.
 * @param event       The KeyEvent object that defines the button action.
 */
Boolean View::OnKeyMultiple(
    /* [in] */ Int32 keyCode,
    /* [in] */ Int32 repeatCount,
    /* [in] */ IKeyEvent* event)
{
    return FALSE;
}

/**
 * Called when an unhandled key shortcut event occurs.
 *
 * @param keyCode The value in event.getKeyCode().
 * @param event Description of the key event.
 * @return If you handled the event, return TRUE. If you want to allow the
 *         event to be handled by the next receiver, return FALSE.
 */
Boolean View::OnKeyShortcut(
    /* [in] */ Int32 keyCode,
    /* [in] */ IKeyEvent* event)
{
    return FALSE;
}

/**
 * Check whether the called view is a text editor, in which case it
 * would make sense to automatically display a soft input window for
 * it.  Subclasses should override this if they implement
 * {@link #onCreateInputConnection(EditorInfo)} to return TRUE if
 * a call on that method would return a non-NULL InputConnection, and
 * they are really a first-class editor that the user would normally
 * start typing on when the go into a window containing your view.
 *
 * <p>The default implementation always returns FALSE.  This does
 * <em>not</em> mean that its {@link #onCreateInputConnection(EditorInfo)}
 * will not be called or the user can not otherwise perform edits on your
 * view; it is just a hint to the system that this is not the primary
 * purpose of this view.
 *
 * @return Returns TRUE if this view is a text editor, else FALSE.
 */
Boolean View::OnCheckIsTextEditor()
{
    return FALSE;
}

/**
 * Create a new InputConnection for an InputMethod to interact
 * with the view.  The default implementation returns NULL, since it doesn't
 * support input methods.  You can override this to implement such support.
 * This is only needed for views that take focus and text input.
 *
 * <p>When implementing this, you probably also want to implement
 * {@link #onCheckIsTextEditor()} to indicate you will return a
 * non-NULL InputConnection.
 *
 * @param outAttrs Fill in with attribute information about the connection.
 */
//IInputConnection* View::OnCreateInputConnection(
//    /* [in] */ IEditorInfo* outAttrs)
//{
//    return NULL;
//}

/**
 * Called by the {@link android.view.inputmethod.InputMethodManager}
 * when a view who is not the current
 * input connection target is trying to make a call on the manager.  The
 * default implementation returns FALSE; you can override this to return
 * TRUE for certain views if you are performing InputConnection proxying
 * to them.
 * @param view The View that is making the InputMethodManager call.
 * @return Return TRUE to allow the call, FALSE to reject.
 */
Boolean View::CheckInputConnectionProxy(
    /* [in] */ IView* view)
{
    return FALSE;
}

/**
 * Show the context menu for this view. It is not safe to hold on to the
 * menu after returning from this method.
 *
 * You should normally not overload this method. Overload
 * {@link #onCreateContextMenu(ContextMenu)} or define an
 * {@link OnCreateContextMenuListener} to add items to the context menu.
 *
 * @param menu The context menu to populate
 */
ECode View::CreateContextMenu(
    /* [in] */ IContextMenu* menu)
{
    //AutoPtr<IContextMenuInfo> menuInfo = GetContextMenuInfo();

    //// Sets the current menu info so all items added to menu will have
    //// my extra info set.
    //((MenuBuilder)menu)->SetCurrentMenuInfo(menuInfo);

    //OnCreateContextMenu(menu);
    //if (mOnCreateContextMenuListener != NULL) {
    //    mOnCreateContextMenuListener->OnCreateContextMenu(menu, this, menuInfo);
    //}

    //// Clear the extra information so subsequent items that aren't mine don't
    //// have my extra info.
    //((MenuBuilder)menu)->SetCurrentMenuInfo(NULL);

    //if (mParent != NULL) {
    //    mParent->CreateContextMenu(menu);
    //}
    return E_NOT_IMPLEMENTED;
}

/**
 * Views should implement this if they have extra information to associate
 * with the context menu. The return result is supplied as a parameter to
 * the {@link OnCreateContextMenuListener#onCreateContextMenu(ContextMenu, View, ContextMenuInfo)}
 * callback.
 *
 * @return Extra information about the item for which the context menu
 *         should be shown. This information will vary across different
 *         subclasses of View.
 */
AutoPtr<IContextMenuInfo> View::GetContextMenuInfo()
{
    return AutoPtr<IContextMenuInfo>(NULL);
}

/**
 * Views should implement this if the view itself is going to add items to
 * the context menu.
 *
 * @param menu the context menu to populate
 */
void View::OnCreateContextMenu(
    /* [in] */ IContextMenu* menu)
{
}

/**
 * Implement this method to handle trackball motion events.  The
 * <em>relative</em> movement of the trackball since the last event
 * can be retrieve with {@link MotionEvent#getX MotionEvent.getX()} and
 * {@link MotionEvent#getY MotionEvent.getY()}.  These are normalized so
 * that a movement of 1 corresponds to the user pressing one DPAD key (so
 * they will often be fractional values, representing the more fine-grained
 * movement information available from a trackball).
 *
 * @param event The motion event.
 * @return True if the event was handled, FALSE otherwise.
 */
Boolean View::OnTrackballEvent(
    /* [in] */ IMotionEvent* event)
{
    return FALSE;
}

/**
 * Implement this method to handle touch screen motion events.
 *
 * @param event The motion event.
 * @return True if the event was handled, FALSE otherwise.
 */
Boolean View::OnTouchEvent(
    /* [in] */ IMotionEvent* event)
{
    Int32 viewFlags = mViewFlags;

    if ((viewFlags & ENABLED_MASK) == DISABLED) {
        // A disabled view that is clickable still consumes the touch
        // events, it just doesn't respond to them.
        return (((viewFlags & CLICKABLE) == CLICKABLE ||
                (viewFlags & LONG_CLICKABLE) == LONG_CLICKABLE));
    }

    if (mTouchDelegate != NULL) {
        Boolean result;
        mTouchDelegate->OnTouchEvent(event, &result);
        if (result) {
            return TRUE;
        }
    }

    if (((viewFlags & CLICKABLE) == CLICKABLE ||
        (viewFlags & LONG_CLICKABLE) == LONG_CLICKABLE)) {
        Int32 action;
        event->GetAction(&action);
        switch (action) {
        case MotionEvent_ACTION_UP:
            {
                Boolean prepressed = (mPrivateFlags & PREPRESSED) != 0;
                if ((mPrivateFlags & PRESSED) != 0 || prepressed) {
                    // take focus if we don't have it already and we should in
                    // touch mode.
                    Boolean focusTaken = FALSE;
                    if (IsFocusable() && IsFocusableInTouchMode() && !IsFocused()) {
                        focusTaken = RequestFocus();
                    }

                    if (!mHasPerformedLongPress) {
                        // This is a tap, so remove the longpress check
                        RemoveLongPressCallback();

                        // Only perform take click actions if we were in the pressed state
                        if (!focusTaken) {
                            // Use a Runnable and post this rather than calling
                            // performClick directly. This lets other visual state
                            // of the view update before click actions start.
                            if (mPerformClick == NULL) {
                                AutoPtr<IView> view = (IView*)Probe(EIID_IView);
                                mPerformClick = new ViewPerformClick(view);
                            }

                            if (!Post(mPerformClick)) {
                                PerformClick();
                            }
                        }
                    }

                    if (mUnsetPressedState == NULL) {
                        AutoPtr<IView> view = (IView*)Probe(EIID_IView);
                        mUnsetPressedState = new UnsetPressedState(view);
                    }

                    if (prepressed) {
                        mPrivateFlags |= PRESSED;
                        RefreshDrawableState();
                        PostDelayed(mUnsetPressedState.Get(),
                            ViewConfiguration::GetPressedStateDuration());
                    }
                    else if (!Post(mUnsetPressedState.Get())) {
                        // If the post failed, unpress right now
                        mUnsetPressedState->Run();
                    }
                    RemoveTapCallback();
                }
            }
            break;
        case MotionEvent_ACTION_DOWN:
            {
                if (mPendingCheckForTap == NULL) {
                    AutoPtr<IView> view = (IView*)Probe(EIID_IView);
                    mPendingCheckForTap = new CheckForTap(view);
                }
                mPrivateFlags |= PREPRESSED;
                mHasPerformedLongPress = FALSE;
                PostDelayed(mPendingCheckForTap.Get(), ViewConfiguration::GetTapTimeout());
            }
            break;
        case MotionEvent_ACTION_CANCEL:
            {
                mPrivateFlags &= ~PRESSED;
                RefreshDrawableState();
                RemoveTapCallback();
            }
            break;
        case MotionEvent_ACTION_MOVE:
            {
                Float fx, fy;
                event->GetX(&fx);
                event->GetY(&fy);

                Int32 x = (Int32)fx;
                Int32 y = (Int32)fy;

                // Be lenient about moving outside of buttons
                Int32 slop = mTouchSlop;
                if ((x < 0 - slop) || (x >= GetWidth() + slop) ||
                    (y < 0 - slop) || (y >= GetHeight() + slop)) {
                    // Outside button
                    RemoveTapCallback();
                    if ((mPrivateFlags & PRESSED) != 0) {
                        // Remove any future long press/tap checks
                        RemoveLongPressCallback();

                        // Need to switch from pressed to not pressed
                        mPrivateFlags &= ~PRESSED;
                        RefreshDrawableState();
                    }
                }
            }
            break;
        default:
            break;
        }
        return TRUE;
    }

    return FALSE;
}

/**
 * Remove the longpress detection timer.
 */
void View::RemoveLongPressCallback()
{
    if (mPendingCheckForLongPress != NULL) {
        RemoveCallbacks(mPendingCheckForLongPress);
    }
}

/**
 * Remove the prepress detection timer.
 */
void View::RemoveUnsetPressCallback()
{
    if ((mPrivateFlags & PRESSED) != 0 && mUnsetPressedState != NULL) {
        SetPressed(FALSE);
        RemoveCallbacks(mUnsetPressedState);
    }
}

/**
 * Remove the tap detection timer.
 */
void View::RemoveTapCallback()
{
    if (mPendingCheckForTap != NULL) {
        mPrivateFlags &= ~PREPRESSED;
        RemoveCallbacks(mPendingCheckForTap);
    }
}

/**
 * Cancels a pending long press.  Your subclass can use this if you
 * want the context menu to come up if the user presses and holds
 * at the same place, but you don't want it to come up if they press
 * and then move around enough to cause scrolling.
 */
ECode View::CancelLongPress()
{
    RemoveLongPressCallback();

    /*
        * The prepressed state handled by the tap callback is a display
        * construct, but the tap callback will post a long press callback
        * less its own timeout. Remove it here.
        */
    RemoveTapCallback();

    return NOERROR;
}

/**
 * Sets the TouchDelegate for this View.
 */
ECode View::SetTouchDelegate(
    /* [in] */ ITouchDelegate* touchDelegate)
{
    mTouchDelegate = touchDelegate;

    return NOERROR;
}

/**
 * Gets the TouchDelegate for this View.
 */
ECode View::GetTouchDelegate(
    /* [out] */ ITouchDelegate** touchDelegate)
{
    VALIDATE_NOT_NULL(touchDelegate);
    *touchDelegate = mTouchDelegate;
    if (*touchDelegate) (*touchDelegate)->AddRef();

    return NOERROR;
}

/**
 * Set flags controlling behavior of this view.
 *
 * @param flags Constant indicating the value which should be set
 * @param mask Constant indicating the bit range that should be changed
 */
void View::SetFlags(
    /* [in] */ Int32 flags,
    /* [in] */ Int32 mask)
{
    Int32 old = mViewFlags;
    mViewFlags = (mViewFlags & ~mask) | (flags & mask);

    Int32 changed = mViewFlags ^ old;
    if (changed == 0) {
        return;
    }

    Int32 privateFlags = mPrivateFlags;

    /* Check if the FOCUSABLE bit has changed */
    if (((changed & FOCUSABLE_MASK) != 0) &&
            ((privateFlags & HAS_BOUNDS) !=0)) {
        if (((old & FOCUSABLE_MASK) == FOCUSABLE)
                && ((privateFlags & FOCUSED) != 0)) {
            /* Give up focus if we are no longer focusable */
            ClearFocus();
        }
        else if (((old & FOCUSABLE_MASK) == NOT_FOCUSABLE)
                && ((privateFlags & FOCUSED) == 0)) {
            /*
             * Tell the view system that we are now available to take focus
             * if no one else already has it.
             */
            if (mParent != NULL) {
                mParent->FocusableViewAvailable((IView*)this->Probe(EIID_IView));
            }
        }
    }

    if ((flags & VISIBILITY_MASK) == View_VISIBLE) {
        if ((changed & VISIBILITY_MASK) != 0) {
            /*
             * If this view is becoming visible, set the DRAWN flag so that
             * the next invalidate() will not be skipped.
             */
            mPrivateFlags |= DRAWN;

            NeedGlobalAttributesUpdate(TRUE);

            // a view becoming visible is worth notifying the parent
            // about in case nothing has focus.  even if this specific view
            // isn't focusable, it may contain something that is, so let
            // the root view try to give this focus if nothing else does.
            if ((mParent != NULL) && (mBottom > mTop) && (mRight > mLeft)) {
                mParent->FocusableViewAvailable((IView*)this->Probe(EIID_IView));
            }
        }
    }

    /* Check if the GONE bit has changed */
    if ((changed & View_GONE) != 0) {
        NeedGlobalAttributesUpdate(FALSE);
        RequestLayout();
        Invalidate();

        if (((mViewFlags & VISIBILITY_MASK) == View_GONE)) {
            if (HasFocus()) ClearFocus();
            DestroyDrawingCache();
        }
        if (mAttachInfo != NULL) {
            mAttachInfo->mViewVisibilityChanged = TRUE;
        }
    }

    /* Check if the VISIBLE bit has changed */
    if ((changed & View_INVISIBLE) != 0) {
        NeedGlobalAttributesUpdate(FALSE);
        Invalidate();

        if (((mViewFlags & VISIBILITY_MASK) == View_INVISIBLE) && HasFocus()) {
            // root view becoming invisible shouldn't clear focus
            if (GetRootView().Get() != (IView*)this->Probe(EIID_IView)) {
                ClearFocus();
            }
        }
        if (mAttachInfo != NULL) {
            mAttachInfo->mViewVisibilityChanged = TRUE;
        }
    }

    if ((changed & VISIBILITY_MASK) != 0) {
        DispatchVisibilityChanged(
            (IView*)this->Probe(EIID_IView), (flags & VISIBILITY_MASK));
    }

    if ((changed & WILL_NOT_CACHE_DRAWING) != 0) {
        DestroyDrawingCache();
    }

    if ((changed & DRAWING_CACHE_ENABLED) != 0) {
        DestroyDrawingCache();
        mPrivateFlags &= ~DRAWING_CACHE_VALID;
    }

    if ((changed & DRAWING_CACHE_QUALITY_MASK) != 0) {
        DestroyDrawingCache();
        mPrivateFlags &= ~DRAWING_CACHE_VALID;
    }

    if ((changed & DRAW_MASK) != 0) {
        if ((mViewFlags & WILL_NOT_DRAW) != 0) {
            if (mBGDrawable != NULL) {
                mPrivateFlags &= ~SKIP_DRAW;
                mPrivateFlags |= ONLY_DRAWS_BACKGROUND;
            }
            else {
                mPrivateFlags |= SKIP_DRAW;
            }
        }
        else {
            mPrivateFlags &= ~SKIP_DRAW;
        }
        RequestLayout();
        Invalidate();
    }

    if ((changed & KEEP_SCREEN_ON) != 0) {
        if (mParent != NULL) {
            mParent->RecomputeViewAttributes((IView*)this->Probe(EIID_IView));
        }
    }
}

/**
 * Change the view's z order in the tree, so it's on top of other sibling
 * views
 */
ECode View::BringToFront()
{
    if (mParent != NULL) {
        mParent->BringChildToFront((IView*)this->Probe(EIID_IView));
    }

    return NOERROR;
}

/**
 * This is called in response to an internal scroll in this view (i.e., the
 * view scrolled its own contents). This is typically as a result of
 * {@link #scrollBy(int, int)} or {@link #scrollTo(int, int)} having been
 * called.
 *
 * @param l Current horizontal scroll origin.
 * @param t Current vertical scroll origin.
 * @param oldl Previous horizontal scroll origin.
 * @param oldt Previous vertical scroll origin.
 */
void View::OnScrollChanged(
    /* [in] */ Int32 l,
    /* [in] */ Int32 t,
    /* [in] */ Int32 oldl,
    /* [in] */ Int32 oldt)
{
    mBackgroundSizeChanged = TRUE;

    AttachInfo* ai = mAttachInfo;
    if (ai != NULL) {
        ai->mViewScrollChanged = TRUE;
    }
}

/**
 * This is called during layout when the size of this view has changed. If
 * you were just added to the view hierarchy, you're called with the old
 * values of 0.
 *
 * @param w Current width of this view.
 * @param h Current height of this view.
 * @param oldw Old width of this view.
 * @param oldh Old height of this view.
 */
void View::OnSizeChanged(
    /* [in] */ Int32 w,
    /* [in] */ Int32 h,
    /* [in] */ Int32 oldw,
    /* [in] */ Int32 oldh)
{}

/**
 * Called by draw to draw the child views. This may be overridden
 * by derived classes to gain control just before its children are drawn
 * (but after its own view has been drawn).
 * @param canvas the canvas on which to draw the view
 */
void View::DispatchDraw(
    /* [in] */ ICanvas* canvas)
{}

/**
 * Gets the parent of this view. Note that the parent is a
 * ViewParent and not necessarily a View.
 *
 * @return Parent of this view.
 */
AutoPtr<IViewParent> View::GetParent()
{
    return mParent;
}

/**
 * Return the scrolled left position of this view. This is the left edge of
 * the displayed part of your view. You do not need to draw any pixels
 * farther left, since those are outside of the frame of your view on
 * screen.
 *
 * @return The left edge of the displayed part of your view, in pixels.
 */
Int32 View::GetScrollX()
{
    return mScrollX;
}

/**
 * Return the scrolled top position of this view. This is the top edge of
 * the displayed part of your view. You do not need to draw any pixels above
 * it, since those are outside of the frame of your view on screen.
 *
 * @return The top edge of the displayed part of your view, in pixels.
 */
Int32 View::GetScrollY()
{
    return mScrollY;
}

/**
 * Return the width of the your view.
 *
 * @return The width of your view, in pixels.
 */
Int32 View::GetWidth()
{
    return mRight - mLeft;
}

/**
 * Return the height of your view.
 *
 * @return The height of your view, in pixels.
 */
Int32 View::GetHeight()
{
    return mBottom - mTop;
}

/**
 * Return the visible drawing bounds of your view. Fills in the output
 * rectangle with the values from getScrollX(), getScrollY(),
 * getWidth(), and getHeight().
 *
 * @param outRect The (scrolled) drawing bounds of the view.
 */
ECode View::GetDrawingRect(
    /* [out] */ IRect* outRect)
{
    VALIDATE_NOT_NULL(outRect);
    return outRect->Set(
        mScrollX, mScrollY,
        mScrollX + (mRight - mLeft),
        mScrollY + (mBottom - mTop));
}

/**
 * The width of this view as measured in the most recent call to measure().
 * This should be used during measurement and layout calculations only. Use
 * {@link #getWidth()} to see how wide a view is after layout.
 *
 * @return The measured width of this view.
 */
Int32 View::GetMeasuredWidth()
{
    return mMeasuredWidth;
}

/**
 * The height of this view as measured in the most recent call to measure().
 * This should be used during measurement and layout calculations only. Use
 * {@link #getHeight()} to see how tall a view is after layout.
 *
 * @return The measured height of this view.
 */
Int32 View::GetMeasuredHeight()
{
    return mMeasuredHeight;
}

/**
 * Top position of this view relative to its parent.
 *
 * @return The top of this view, in pixels.
 */
Int32 View::GetTop()
{
    return mTop;
}

/**
 * Bottom position of this view relative to its parent.
 *
 * @return The bottom of this view, in pixels.
 */
Int32 View::GetBottom()
{
    return mBottom;
}

/**
 * Left position of this view relative to its parent.
 *
 * @return The left edge of this view, in pixels.
 */
Int32 View::GetLeft()
{
    return mLeft;
}

/**
 * Right position of this view relative to its parent.
 *
 * @return The right edge of this view, in pixels.
 */
Int32 View::GetRight()
{
    return mRight;
}

/**
 * Hit rectangle in parent's coordinates
 *
 * @param outRect The hit rectangle of the view.
 */
ECode View::GetHitRect(
    /* [in, out] */ IRect* outRect)
{
    assert(outRect != NULL);
    return outRect->Set(mLeft, mTop, mRight, mBottom);
}

/**
 * When a view has focus and the user navigates away from it, the next view is searched for
 * starting from the rectangle filled in by this method.
 *
 * By default, the rectange is the {@link #getDrawingRect})of the view.  However, if your
 * view maintains some idea of internal selection, such as a cursor, or a selected row
 * or column, you should override this method and fill in a more specific rectangle.
 *
 * @param r The rectangle to fill in, in this view's coordinates.
 */
ECode View::GetFocusedRect(
    /* [in, out] */ IRect* outRect)
{
    return GetDrawingRect(outRect);
}

/**
 * If some part of this view is not clipped by any of its parents, then
 * return that area in r in global (root) coordinates. To convert r to local
 * coordinates, offset it by -globalOffset (e.g. r.offset(-globalOffset.x,
 * -globalOffset.y)) If the view is completely clipped or translated out,
 * return FALSE.
 *
 * @param r If TRUE is returned, r holds the global coordinates of the
 *        visible portion of this view.
 * @param globalOffset If TRUE is returned, globalOffset holds the dx,dy
 *        between this view and its root. globalOffet may be NULL.
 * @return TRUE if r is non-empty (i.e. part of the view is visible at the
 *         root level.
 */
Boolean View::GetGlobalVisibleRect(
    /* [in, out] */ IRect* outRect,
    /* [in, out] */ IPoint* outGlobalOffset)
{
    Int32 width = mRight - mLeft;
    Int32 height = mBottom - mTop;
    if (width > 0 && height > 0) {
        outRect->Set(0, 0, width, height);
        if (outGlobalOffset != NULL) {
            outGlobalOffset->Set(-mScrollX, -mScrollY);
        }

        Boolean result;
        mParent->GetChildVisibleRect((IView*)this->Probe(EIID_IView),
                outRect, outGlobalOffset, &result);
        return mParent == NULL || result;
    }
    return FALSE;
}

Boolean View::GetGlobalVisibleRect(
    /* [in, out] */ IRect* outRect)
{
    return GetGlobalVisibleRect(outRect, NULL);
}

Boolean View::GetLocalVisibleRect(
    /* [in, out] */ IRect* outRect)
{
    AutoPtr<CPoint> offset;
    CPoint::NewByFriend((CPoint**)&offset);
    if (GetGlobalVisibleRect(outRect, (IPoint*)offset.Get())) {
        outRect->Offset(-offset->mX, -offset->mY); // make r local
        return TRUE;
    }
    return FALSE;
}

/**
 * Offset this view's vertical location by the specified number of pixels.
 *
 * @param offset the number of pixels to offset the view by
 */
ECode View::OffsetTopAndBottom(
    /* [in] */ Int32 offset)
{
    mTop += offset;
    mBottom += offset;

    return NOERROR;
}

/**
 * Offset this view's horizontal location by the specified amount of pixels.
 *
 * @param offset the numer of pixels to offset the view by
 */
ECode View::OffsetLeftAndRight(
    /* [in] */ Int32 offset)
{
    mLeft += offset;
    mRight += offset;

    return NOERROR;
}

/**
 * Get the LayoutParams associated with this view. All views should have
 * layout parameters. These supply parameters to the <i>parent</i> of this
 * view specifying how it should be arranged. There are many subclasses of
 * ViewGroup.LayoutParams, and these correspond to the different subclasses
 * of ViewGroup that are responsible for arranging their children.
 * @return The LayoutParams associated with this view
 */
AutoPtr<IViewGroupLayoutParams> View::GetLayoutParams()
{
    return mLayoutParams;
}

/**
 * Set the layout parameters associated with this view. These supply
 * parameters to the <i>parent</i> of this view specifying how it should be
 * arranged. There are many subclasses of ViewGroup.LayoutParams, and these
 * correspond to the different subclasses of ViewGroup that are responsible
 * for arranging their children.
 *
 * @param params the layout parameters for this view
 */
ECode View::SetLayoutParams(
    /* [in] */ IViewGroupLayoutParams* params)
{
    if (params == NULL) {
//        throw new NullPointerException("params == null");
        return E_NULL_POINTER_EXCEPTION;
    }
    mLayoutParams = params;
    RequestLayout();
    return NOERROR;
}

/**
 * Set the scrolled position of your view. This will cause a call to
 * {@link #onScrollChanged(int, int, int, int)} and the view will be
 * invalidated.
 * @param x the x position to scroll to
 * @param y the y position to scroll to
 */
ECode View::ScrollTo(
    /* [in] */ Int32 x,
    /* [in] */ Int32 y)
{
    if (mScrollX != x || mScrollY != y) {
        Int32 oldX = mScrollX;
        Int32 oldY = mScrollY;
        mScrollX = x;
        mScrollY = y;
        OnScrollChanged(mScrollX, mScrollY, oldX, oldY);
        if (!AwakenScrollBars()) {
            Invalidate();
        }
    }

    return NOERROR;
}

/**
 * Move the scrolled position of your view. This will cause a call to
 * {@link #onScrollChanged(int, int, int, int)} and the view will be
 * invalidated.
 * @param x the amount of pixels to scroll by horizontally
 * @param y the amount of pixels to scroll by vertically
 */
ECode View::ScrollBy(
    /* [in] */ Int32 x,
    /* [in] */ Int32 y)
{
    return ScrollTo(mScrollX + x, mScrollY + y);
}

/**
 * <p>Trigger the scrollbars to draw. When invoked this method starts an
 * animation to fade the scrollbars out after a default delay. If a subclass
 * provides animated scrolling, the start delay should equal the duration
 * of the scrolling animation.</p>
 *
 * <p>The animation starts only if at least one of the scrollbars is
 * enabled, as specified by {@link #isHorizontalScrollBarEnabled()} and
 * {@link #isVerticalScrollBarEnabled()}. When the animation is started,
 * this method returns TRUE, and FALSE otherwise. If the animation is
 * started, this method calls {@link #invalidate()}; in that case the
 * caller should not call {@link #invalidate()}.</p>
 *
 * <p>This method should be invoked every time a subclass directly updates
 * the scroll parameters.</p>
 *
 * <p>This method is automatically invoked by {@link #scrollBy(int, int)}
 * and {@link #scrollTo(int, int)}.</p>
 *
 * @return TRUE if the animation is played, FALSE otherwise
 *
 * @see #awakenScrollBars(int)
 * @see #scrollBy(int, int)
 * @see #scrollTo(int, int)
 * @see #isHorizontalScrollBarEnabled()
 * @see #isVerticalScrollBarEnabled()
 * @see #setHorizontalScrollBarEnabled(Boolean)
 * @see #setVerticalScrollBarEnabled(Boolean)
 */
Boolean View::AwakenScrollBars()
{
    return mScrollCache != NULL &&
            AwakenScrollBars(mScrollCache->mScrollBarDefaultDelayBeforeFade, TRUE);
}

/**
 * Trigger the scrollbars to draw.
 * This method differs from awakenScrollBars() only in its default duration.
 * initialAwakenScrollBars() will show the scroll bars for longer than
 * usual to give the user more of a chance to notice them.
 *
 * @return TRUE if the animation is played, FALSE otherwise.
 */
Boolean View::InitialAwakenScrollBars()
{
    return mScrollCache != NULL &&
            AwakenScrollBars(mScrollCache->mScrollBarDefaultDelayBeforeFade * 4, TRUE);
}

/**
 * <p>
 * Trigger the scrollbars to draw. When invoked this method starts an
 * animation to fade the scrollbars out after a fixed delay. If a subclass
 * provides animated scrolling, the start delay should equal the duration of
 * the scrolling animation.
 * </p>
 *
 * <p>
 * The animation starts only if at least one of the scrollbars is enabled,
 * as specified by {@link #isHorizontalScrollBarEnabled()} and
 * {@link #isVerticalScrollBarEnabled()}. When the animation is started,
 * this method returns TRUE, and FALSE otherwise. If the animation is
 * started, this method calls {@link #invalidate()}; in that case the caller
 * should not call {@link #invalidate()}.
 * </p>
 *
 * <p>
 * This method should be invoked everytime a subclass directly updates the
 * scroll parameters.
 * </p>
 *
 * @param startDelay the delay, in milliseconds, after which the animation
 *        should start; when the delay is 0, the animation starts
 *        immediately
 * @return TRUE if the animation is played, FALSE otherwise
 *
 * @see #scrollBy(int, int)
 * @see #scrollTo(int, int)
 * @see #isHorizontalScrollBarEnabled()
 * @see #isVerticalScrollBarEnabled()
 * @see #setHorizontalScrollBarEnabled(Boolean)
 * @see #setVerticalScrollBarEnabled(Boolean)
 */
Boolean View::AwakenScrollBars(
    /* [in] */ Int32 startDelay)
{
    return AwakenScrollBars(startDelay, TRUE);
}

/**
 * <p>
 * Trigger the scrollbars to draw. When invoked this method starts an
 * animation to fade the scrollbars out after a fixed delay. If a subclass
 * provides animated scrolling, the start delay should equal the duration of
 * the scrolling animation.
 * </p>
 *
 * <p>
 * The animation starts only if at least one of the scrollbars is enabled,
 * as specified by {@link #isHorizontalScrollBarEnabled()} and
 * {@link #isVerticalScrollBarEnabled()}. When the animation is started,
 * this method returns TRUE, and FALSE otherwise. If the animation is
 * started, this method calls {@link #invalidate()} if the invalidate parameter
 * is set to TRUE; in that case the caller
 * should not call {@link #invalidate()}.
 * </p>
 *
 * <p>
 * This method should be invoked everytime a subclass directly updates the
 * scroll parameters.
 * </p>
 *
 * @param startDelay the delay, in milliseconds, after which the animation
 *        should start; when the delay is 0, the animation starts
 *        immediately
 *
 * @param invalidate Wheter this method should call invalidate
 *
 * @return TRUE if the animation is played, FALSE otherwise
 *
 * @see #scrollBy(int, int)
 * @see #scrollTo(int, int)
 * @see #isHorizontalScrollBarEnabled()
 * @see #isVerticalScrollBarEnabled()
 * @see #setHorizontalScrollBarEnabled(Boolean)
 * @see #setVerticalScrollBarEnabled(Boolean)
 */
Boolean View::AwakenScrollBars(
    /* [in] */ Int32 startDelay,
    /* [in] */ Boolean invalidate)
{
    AutoPtr<ScrollabilityCache> scrollCache = mScrollCache;

    if (scrollCache == NULL || !scrollCache->mFadeScrollBars) {
        return FALSE;
    }

    if (scrollCache->mScrollBar == NULL) {
        CScrollBarDrawable::New((IScrollBarDrawable**)&(scrollCache->mScrollBar));
    }

    if (IsHorizontalScrollBarEnabled() || IsVerticalScrollBarEnabled()) {

        if (invalidate) {
            // Invalidate to show the scrollbars
            Invalidate();
        }

        if (scrollCache->mState == ScrollabilityCache::OFF) {
            // FIXME: this is copied from WindowManagerService.
            // We should get this value from the system when it
            // is possible to do so.
            Int32 KEY_REPEAT_FIRST_DELAY = 750;
            startDelay = Math::Max(KEY_REPEAT_FIRST_DELAY, startDelay);
        }

        // Tell mScrollCache when we should start fading. This may
        // extend the fade start time if one was already scheduled
        Int64 fadeStartTime = AnimationUtils::CurrentAnimationTimeMillis() + startDelay;
        scrollCache->mFadeStartTime = fadeStartTime;
        scrollCache->mState = ScrollabilityCache::ON;

        // Schedule our fader to run, unscheduling any old ones first
        if (mAttachInfo != NULL) {
            ECode (STDCALL IRunnable::*pHandlerFunc)();
            pHandlerFunc = &IRunnable::Run;

            //mAttachInfo->mHandler->RemoveCppCallbacks(
            //    (Handle32)scrollCache.Get(), *(Handle32*)&pHandlerFunc);
            //mAttachInfo->mHandler->PostCppCallbackAtTime(
            //    (Handle32)scrollCache.Get(), *(Handle32*)&pHandlerFunc,
            //    NULL, 0, fadeStartTime);
        }

        return TRUE;
    }

    return FALSE;
}

/**
 * Mark the the area defined by dirty as needing to be drawn. If the view is
 * visible, {@link #onDraw} will be called at some point in the future.
 * This must be called from a UI thread. To call from a non-UI thread, call
 * {@link #postInvalidate()}.
 *
 * WARNING: This method is destructive to dirty.
 * @param dirty the rectangle representing the bounds of the dirty region
 */
ECode View::Invalidate(
    /* [in] */ IRect* dirty)
{
//    if (ViewDebug.TRACE_HIERARCHY) {
//        ViewDebug.trace(this, ViewDebug.HierarchyTraceType.INVALIDATE);
//    }

    if ((mPrivateFlags & (DRAWN | HAS_BOUNDS)) == (DRAWN | HAS_BOUNDS)) {
        mPrivateFlags &= ~DRAWING_CACHE_VALID;

        AutoPtr<IViewParent> p = mParent;
        AttachInfo* ai = mAttachInfo;
        if (p != NULL && ai != NULL) {
            Int32 scrollX = mScrollX;
            Int32 scrollY = mScrollY;
            AutoPtr<IRect> tmpr = (IRect*)ai->mTmpInvalRect.Get();
            CRect* dirtyObj = (CRect*)dirty;
            tmpr->Set(dirtyObj->mLeft - scrollX, dirtyObj->mTop - scrollY,
                    dirtyObj->mRight - scrollX, dirtyObj->mBottom - scrollY);
            p->InvalidateChild((IView*)this->Probe(EIID_IView), tmpr);
        }
    }
    return NOERROR;
}

/**
 * Mark the the area defined by the rect (l,t,r,b) as needing to be drawn.
 * The coordinates of the dirty rect are relative to the view.
 * If the view is visible, {@link #onDraw} will be called at some point
 * in the future. This must be called from a UI thread. To call
 * from a non-UI thread, call {@link #postInvalidate()}.
 * @param l the left position of the dirty region
 * @param t the top position of the dirty region
 * @param r the right position of the dirty region
 * @param b the bottom position of the dirty region
 */
ECode View::Invalidate(
    /* [in] */ Int32 left,
    /* [in] */ Int32 top,
    /* [in] */ Int32 right,
    /* [in] */ Int32 bottom)
{
//    if (ViewDebug.TRACE_HIERARCHY) {
//        ViewDebug.trace(this, ViewDebug.HierarchyTraceType.INVALIDATE);
//    }

    if ((mPrivateFlags & (DRAWN | HAS_BOUNDS)) == (DRAWN | HAS_BOUNDS)) {
        mPrivateFlags &= ~DRAWING_CACHE_VALID;

        AutoPtr<IViewParent> p = mParent;
        AttachInfo* ai = mAttachInfo;
        if (p != NULL && ai != NULL && left < right && top < bottom) {
            Int32 scrollX = mScrollX;
            Int32 scrollY = mScrollY;
            AutoPtr<IRect> tmpr = (IRect*)ai->mTmpInvalRect.Get();
            tmpr->Set(left - scrollX, top - scrollY, right - scrollX, bottom - scrollY);
            p->InvalidateChild((IView*)this->Probe(EIID_IView), tmpr);
        }
    }
    return NOERROR;
}

/**
 * Invalidate the whole view. If the view is visible, {@link #onDraw} will
 * be called at some point in the future. This must be called from a
 * UI thread. To call from a non-UI thread, call {@link #postInvalidate()}.
 */
ECode View::Invalidate()
{
//    if (ViewDebug.TRACE_HIERARCHY) {
//        ViewDebug.trace(this, ViewDebug.HierarchyTraceType.INVALIDATE);
//    }

    if ((mPrivateFlags & (DRAWN | HAS_BOUNDS)) == (DRAWN | HAS_BOUNDS)) {
        mPrivateFlags &= ~DRAWN & ~DRAWING_CACHE_VALID;

        AutoPtr<IViewParent> p = mParent;
        AttachInfo* ai = mAttachInfo;
        if (p != NULL && ai != NULL) {
            AutoPtr<IRect> tmpr = (IRect*)ai->mTmpInvalRect.Get();
            tmpr->Set(0, 0, mRight - mLeft, mBottom - mTop);
            // Don't call invalidate -- we don't want to internally scroll
            // our own bounds
            p->InvalidateChild((IView*)this->Probe(EIID_IView), tmpr);
        }
    }
    return NOERROR;
}

/**
 * Indicates whether this View is opaque. An opaque View guarantees that it will
 * draw all the pixels overlapping its bounds using a fully opaque color.
 *
 * Subclasses of View should override this method whenever possible to indicate
 * whether an instance is opaque. Opaque Views are treated in a special way by
 * the View hierarchy, possibly allowing it to perform optimizations during
 * invalidate/draw passes.
 *
 * @return True if this View is guaranteed to be fully opaque, FALSE otherwise.
 */
Boolean View::IsOpaque()
{
    return (mPrivateFlags & OPAQUE_MASK) == OPAQUE_MASK;
}

void View::ComputeOpaqueFlags()
{
    // Opaque if:
    //   - Has a background
    //   - Background is opaque
    //   - Doesn't have scrollbars or scrollbars are inside overlay
    if (mBGDrawable != NULL) {
        Int32 opacity;
        mBGDrawable->GetOpacity(&opacity);
        if (opacity == ElPixelFormat::OPAQUE) {
            mPrivateFlags |= OPAQUE_BACKGROUND;
        }
        else {
            mPrivateFlags &= ~OPAQUE_BACKGROUND;
        }
    }
    else {
        mPrivateFlags &= ~OPAQUE_BACKGROUND;
    }

    Int32 flags = mViewFlags;
    if (((flags & SCROLLBARS_VERTICAL) == 0 && (flags & SCROLLBARS_HORIZONTAL) == 0) ||
            (flags & SCROLLBARS_STYLE_MASK) == SCROLLBARS_INSIDE_OVERLAY) {
        mPrivateFlags |= OPAQUE_SCROLLBARS;
    }
    else {
        mPrivateFlags &= ~OPAQUE_SCROLLBARS;
    }
}

/**
 * @hide
 */
Boolean View::HasOpaqueScrollbars()
{
    return (mPrivateFlags & OPAQUE_SCROLLBARS) == OPAQUE_SCROLLBARS;
}

/**
 * @return A handler associated with the thread running the View. This
 * handler can be used to pump events in the UI events queue.
 */
AutoPtr<IApartment> View::GetHandler()
{
    if (mAttachInfo != NULL) {
        return mAttachInfo->mHandler;
    }
    return AutoPtr<IApartment>(NULL);
}

/**
 * Causes the Runnable to be added to the message queue.
 * The runnable will be run on the user interface thread.
 *
 * @param action The Runnable that will be executed.
 *
 * @return Returns TRUE if the Runnable was successfully placed in to the
 *         message queue.  Returns FALSE on failure, usually because the
 *         looper processing the message queue is exiting.
 */
Boolean View::Post(
    /* [in] */ IRunnable* action)
{
    if (mAttachInfo != NULL) {
        ECode (STDCALL IRunnable::*pHandlerFunc)();
        pHandlerFunc = &IRunnable::Run;

        ECode ec = mAttachInfo->mHandler->PostCppCallback(
            (Handle32)action, *(Handle32*)&pHandlerFunc, NULL, 0);

        if (FAILED(ec)) {
            return FALSE;
        }
    }
    else {
        // Assume that post will succeed later
        ViewRoot::GetRunQueue()->Post(action);
    }

    return TRUE;
}

/**
 * Causes the Runnable to be added to the message queue, to be run
 * after the specified amount of time elapses.
 * The runnable will be run on the user interface thread.
 *
 * @param action The Runnable that will be executed.
 * @param delayMillis The delay (in milliseconds) until the Runnable
 *        will be executed.
 *
 * @return TRUE if the Runnable was successfully placed in to the
 *         message queue.  Returns FALSE on failure, usually because the
 *         looper processing the message queue is exiting.  Note that a
 *         result of TRUE does not mean the Runnable will be processed --
 *         if the looper is quit before the delivery time of the message
 *         occurs then the message will be dropped.
 */
Boolean View::PostDelayed(
    /* [in] */ IRunnable* action,
    /* [in] */ Int64 delayMillis)
{
    if (delayMillis <0 ) {
        delayMillis = 0;
    }

    if (mAttachInfo != NULL) {
        ECode (STDCALL IRunnable::*pHandlerFunc)();
        pHandlerFunc = &IRunnable::Run;

        ECode ec = mAttachInfo->mHandler->PostCppCallbackDelayed(
            (Handle32)action, *(Handle32*)&pHandlerFunc, NULL, 0, delayMillis);

        if (FAILED(ec)) {
            return FALSE;
        }

    }
    else {
        // Assume that post will succeed later
        ViewRoot::GetRunQueue()->PostDelayed(action, delayMillis);
    }

    return TRUE;
}

/**
 * Removes the specified Runnable from the message queue.
 *
 * @param action The Runnable to remove from the message handling queue
 *
 * @return TRUE if this view could ask the Handler to remove the Runnable,
 *         FALSE otherwise. When the returned value is TRUE, the Runnable
 *         may or may not have been actually removed from the message queue
 *         (for instance, if the Runnable was not in the queue already.)
 */
Boolean View::RemoveCallbacks(
    /* [in] */ IRunnable* action)
{
    if (mAttachInfo != NULL) {
        ECode ec = mAttachInfo->mHandler->RemoveCppCallbacks(
            (Handle32)action, NULL);

        if (FAILED(ec)) {
            return FALSE;
        }
    }
    else {
        // Assume that post will succeed later
        ViewRoot::GetRunQueue()->RemoveCallbacks(action);
    }

    return TRUE;
}

/**
 * Cause an invalidate to happen on a subsequent cycle through the event loop.
 * Use this to invalidate the View from a non-UI thread.
 *
 * @see #invalidate()
 */
ECode View::PostInvalidate()
{
    PostInvalidateDelayed(0);

    return NOERROR;
}

/**
 * Cause an invalidate of the specified area to happen on a subsequent cycle
 * through the event loop. Use this to invalidate the View from a non-UI thread.
 *
 * @param left The left coordinate of the rectangle to invalidate.
 * @param top The top coordinate of the rectangle to invalidate.
 * @param right The right coordinate of the rectangle to invalidate.
 * @param bottom The bottom coordinate of the rectangle to invalidate.
 *
 * @see #invalidate(int, int, int, int)
 * @see #invalidate(Rect)
 */
ECode View::PostInvalidate(
    /* [in] */ Int32 left,
    /* [in] */ Int32 top,
    /* [in] */ Int32 right,
    /* [in] */ Int32 bottom)
{
    return PostInvalidateDelayed(0, left, top, right, bottom);
}

/**
 * Cause an invalidate to happen on a subsequent cycle through the event
 * loop. Waits for the specified amount of time.
 *
 * @param delayMilliseconds the duration in milliseconds to delay the
 *         invalidation by
 */
ECode View::PostInvalidateDelayed(
    /* [in] */ Int64 delayMilliseconds)
{
    // We try only with the AttachInfo because there's no point in invalidating
    // if we are not attached to our window
    if (mAttachInfo != NULL) {
        ECode (STDCALL View::*pHandlerFunc)();
        pHandlerFunc = &View::HandleInvalidate;

        mAttachInfo->mHandler->PostCppCallbackDelayed(
            (Handle32)this, *(Handle32*)&pHandlerFunc,
            NULL, 0, delayMilliseconds);
    }
    return NOERROR;
}

/**
 * Cause an invalidate of the specified area to happen on a subsequent cycle
 * through the event loop. Waits for the specified amount of time.
 *
 * @param delayMilliseconds the duration in milliseconds to delay the
 *         invalidation by
 * @param left The left coordinate of the rectangle to invalidate.
 * @param top The top coordinate of the rectangle to invalidate.
 * @param right The right coordinate of the rectangle to invalidate.
 * @param bottom The bottom coordinate of the rectangle to invalidate.
 */
ECode View::PostInvalidateDelayed(
    /* [in] */ Int64 delayMilliseconds,
    /* [in] */ Int32 left,
    /* [in] */ Int32 top,
    /* [in] */ Int32 right,
    /* [in] */ Int32 bottom)
{
    // We try only with the AttachInfo because there's no point in invalidating
    // if we are not attached to our window
    if (mAttachInfo != NULL) {
        ECode (STDCALL View::*pHandlerFunc)(
            Int32, Int32, Int32, Int32);
        pHandlerFunc = &View::HandleInvalidateRect;

        AutoPtr<IParcel> params;
        CCallbackParcel::New((IParcel**)&params);
        params->WriteInt32(left);
        params->WriteInt32(top);
        params->WriteInt32(right);
        params->WriteInt32(bottom);

        mAttachInfo->mHandler->PostCppCallbackDelayed(
            (Handle32)this, *(Handle32*)&pHandlerFunc,
            params, 0, delayMilliseconds);
    }
    return NOERROR;
}

/**
 * Called by a parent to request that a child update its values for mScrollX
 * and mScrollY if necessary. This will typically be done if the child is
 * animating a scroll using a {@link android.widget.Scroller Scroller}
 * object.
 */
ECode View::ComputeScroll()
{
    return NOERROR;
}

/**
 * <p>Indicate whether the horizontal edges are faded when the view is
 * scrolled horizontally.</p>
 *
 * @return TRUE if the horizontal edges should are faded on scroll, FALSE
 *         otherwise
 *
 * @see #setHorizontalFadingEdgeEnabled(Boolean)
 * @attr ref android.R.styleable#View_fadingEdge
 */
Boolean View::IsHorizontalFadingEdgeEnabled()
{
    return (mViewFlags & FADING_EDGE_HORIZONTAL) == FADING_EDGE_HORIZONTAL;
}

/**
 * <p>Define whether the horizontal edges should be faded when this view
 * is scrolled horizontally.</p>
 *
 * @param horizontalFadingEdgeEnabled TRUE if the horizontal edges should
 *                                    be faded when the view is scrolled
 *                                    horizontally
 *
 * @see #isHorizontalFadingEdgeEnabled()
 * @attr ref android.R.styleable#View_fadingEdge
 */
ECode View::SetHorizontalFadingEdgeEnabled(
    /* [in] */ Boolean horizontalFadingEdgeEnabled)
{
    if (IsHorizontalFadingEdgeEnabled() != horizontalFadingEdgeEnabled) {
        if (horizontalFadingEdgeEnabled) {
            InitScrollCache();
        }

        mViewFlags ^= FADING_EDGE_HORIZONTAL;

    }
    return NOERROR;
}

/**
 * <p>Indicate whether the vertical edges are faded when the view is
 * scrolled horizontally.</p>
 *
 * @return TRUE if the vertical edges should are faded on scroll, FALSE
 *         otherwise
 *
 * @see #setVerticalFadingEdgeEnabled(Boolean)
 * @attr ref android.R.styleable#View_fadingEdge
 */
Boolean View::IsVerticalFadingEdgeEnabled()
{
    return (mViewFlags & FADING_EDGE_VERTICAL) == FADING_EDGE_VERTICAL;
}

/**
 * <p>Define whether the vertical edges should be faded when this view
 * is scrolled vertically.</p>
 *
 * @param verticalFadingEdgeEnabled TRUE if the vertical edges should
 *                                  be faded when the view is scrolled
 *                                  vertically
 *
 * @see #isVerticalFadingEdgeEnabled()
 * @attr ref android.R.styleable#View_fadingEdge
 */
ECode View::SetVerticalFadingEdgeEnabled(
    /* [in] */ Boolean verticalFadingEdgeEnabled)
{
    if (IsVerticalFadingEdgeEnabled() != verticalFadingEdgeEnabled) {
        if (verticalFadingEdgeEnabled) {
            InitScrollCache();
        }

        mViewFlags ^= FADING_EDGE_VERTICAL;
    }

    return NOERROR;
}

/**
 * Returns the strength, or intensity, of the top faded edge. The strength is
 * a value between 0.0 (no fade) and 1.0 (full fade). The default implementation
 * returns 0.0 or 1.0 but no value in between.
 *
 * Subclasses should override this method to provide a smoother fade transition
 * when scrolling occurs.
 *
 * @return the intensity of the top fade as a float between 0.0f and 1.0f
 */
Float View::GetTopFadingEdgeStrength()
{
    return ComputeVerticalScrollOffset() > 0 ? 1.0f : 0.0f;
}

/**
 * Returns the strength, or intensity, of the bottom faded edge. The strength is
 * a value between 0.0 (no fade) and 1.0 (full fade). The default implementation
 * returns 0.0 or 1.0 but no value in between.
 *
 * Subclasses should override this method to provide a smoother fade transition
 * when scrolling occurs.
 *
 * @return the intensity of the bottom fade as a float between 0.0f and 1.0f
 */
Float View::GetBottomFadingEdgeStrength()
{
    return ComputeVerticalScrollOffset() + ComputeVerticalScrollExtent() <
            ComputeVerticalScrollRange() ? 1.0f : 0.0f;
}

/**
 * Returns the strength, or intensity, of the left faded edge. The strength is
 * a value between 0.0 (no fade) and 1.0 (full fade). The default implementation
 * returns 0.0 or 1.0 but no value in between.
 *
 * Subclasses should override this method to provide a smoother fade transition
 * when scrolling occurs.
 *
 * @return the intensity of the left fade as a float between 0.0f and 1.0f
 */
Float View::GetLeftFadingEdgeStrength()
{
    return ComputeHorizontalScrollOffset() > 0 ? 1.0f : 0.0f;
}

/**
 * Returns the strength, or intensity, of the right faded edge. The strength is
 * a value between 0.0 (no fade) and 1.0 (full fade). The default implementation
 * returns 0.0 or 1.0 but no value in between.
 *
 * Subclasses should override this method to provide a smoother fade transition
 * when scrolling occurs.
 *
 * @return the intensity of the right fade as a float between 0.0f and 1.0f
 */
Float View::GetRightFadingEdgeStrength()
{
    return ComputeHorizontalScrollOffset() + ComputeHorizontalScrollExtent() <
            ComputeHorizontalScrollRange() ? 1.0f : 0.0f;
}

/**
 * <p>Indicate whether the horizontal scrollbar should be drawn or not. The
 * scrollbar is not drawn by default.</p>
 *
 * @return TRUE if the horizontal scrollbar should be painted, FALSE
 *         otherwise
 *
 * @see #setHorizontalScrollBarEnabled(Boolean)
 */
Boolean View::IsHorizontalScrollBarEnabled()
{
    return (mViewFlags & SCROLLBARS_HORIZONTAL) == SCROLLBARS_HORIZONTAL;
}

/**
 * <p>Define whether the horizontal scrollbar should be drawn or not. The
 * scrollbar is not drawn by default.</p>
 *
 * @param horizontalScrollBarEnabled TRUE if the horizontal scrollbar should
 *                                   be painted
 *
 * @see #isHorizontalScrollBarEnabled()
 */
ECode View::SetHorizontalScrollBarEnabled(
    /* [in] */ Boolean horizontalScrollBarEnabled)
{
    if (IsHorizontalScrollBarEnabled() != horizontalScrollBarEnabled) {
        mViewFlags ^= SCROLLBARS_HORIZONTAL;
        ComputeOpaqueFlags();
        RecomputePadding();
    }

    return NOERROR;
}

/**
 * <p>Indicate whether the vertical scrollbar should be drawn or not. The
 * scrollbar is not drawn by default.</p>
 *
 * @return TRUE if the vertical scrollbar should be painted, FALSE
 *         otherwise
 *
 * @see #setVerticalScrollBarEnabled(Boolean)
 */
Boolean View::IsVerticalScrollBarEnabled()
{
    return (mViewFlags & SCROLLBARS_VERTICAL) == SCROLLBARS_VERTICAL;
}

/**
 * <p>Define whether the vertical scrollbar should be drawn or not. The
 * scrollbar is not drawn by default.</p>
 *
 * @param verticalScrollBarEnabled TRUE if the vertical scrollbar should
 *                                 be painted
 *
 * @see #isVerticalScrollBarEnabled()
 */
ECode View::SetVerticalScrollBarEnabled(
    /* [in] */ Boolean verticalScrollBarEnabled)
{
    if (IsVerticalScrollBarEnabled() != verticalScrollBarEnabled) {
        mViewFlags ^= SCROLLBARS_VERTICAL;
        ComputeOpaqueFlags();
        RecomputePadding();
    }

    return NOERROR;
}

void View::RecomputePadding()
{
    SetPadding(mPaddingLeft, mPaddingTop, mUserPaddingRight, mUserPaddingBottom);
}

/**
 * Define whether scrollbars will fade when the view is not scrolling.
 *
 * @param fadeScrollbars wheter to enable fading
 *
 */
ECode View::SetScrollbarFadingEnabled(
    /* [in] */ Boolean fadeScrollbars)
{
    InitScrollCache();
    AutoPtr<ScrollabilityCache> scrollabilityCache = mScrollCache;
    scrollabilityCache->mFadeScrollBars = fadeScrollbars;
    if (fadeScrollbars) {
        scrollabilityCache->mState = ScrollabilityCache::OFF;
    }
    else {
        scrollabilityCache->mState = ScrollabilityCache::ON;
    }

    return NOERROR;
}

/**
 *
 * Returns TRUE if scrollbars will fade when this view is not scrolling
 *
 * @return TRUE if scrollbar fading is enabled
 */
Boolean View::IsScrollbarFadingEnabled()
{
    return mScrollCache != NULL && mScrollCache->mFadeScrollBars;
}

/**
 * <p>Specify the style of the scrollbars. The scrollbars can be overlaid or
 * inset. When inset, they add to the padding of the view. And the scrollbars
 * can be drawn inside the padding area or on the edge of the view. For example,
 * if a view has a background drawable and you want to draw the scrollbars
 * inside the padding specified by the drawable, you can use
 * SCROLLBARS_INSIDE_OVERLAY or SCROLLBARS_INSIDE_INSET. If you want them to
 * appear at the edge of the view, ignoring the padding, then you can use
 * SCROLLBARS_OUTSIDE_OVERLAY or SCROLLBARS_OUTSIDE_INSET.</p>
 * @param style the style of the scrollbars. Should be one of
 * SCROLLBARS_INSIDE_OVERLAY, SCROLLBARS_INSIDE_INSET,
 * SCROLLBARS_OUTSIDE_OVERLAY or SCROLLBARS_OUTSIDE_INSET.
 * @see #SCROLLBARS_INSIDE_OVERLAY
 * @see #SCROLLBARS_INSIDE_INSET
 * @see #SCROLLBARS_OUTSIDE_OVERLAY
 * @see #SCROLLBARS_OUTSIDE_INSET
 */
ECode View::SetScrollBarStyle(
    /* [in] */ Int32 style)
{
    if (style != (mViewFlags & SCROLLBARS_STYLE_MASK)) {
        mViewFlags = (mViewFlags & ~SCROLLBARS_STYLE_MASK) | (style & SCROLLBARS_STYLE_MASK);
        ComputeOpaqueFlags();
        RecomputePadding();
    }

    return NOERROR;
}

/**
 * <p>Returns the current scrollbar style.</p>
 * @return the current scrollbar style
 * @see #SCROLLBARS_INSIDE_OVERLAY
 * @see #SCROLLBARS_INSIDE_INSET
 * @see #SCROLLBARS_OUTSIDE_OVERLAY
 * @see #SCROLLBARS_OUTSIDE_INSET
 */
Int32 View::GetScrollBarStyle()
{
    return mViewFlags & SCROLLBARS_STYLE_MASK;
}

/**
 * <p>Compute the horizontal range that the horizontal scrollbar
 * represents.</p>
 *
 * <p>The range is expressed in arbitrary units that must be the same as the
 * units used by {@link #computeHorizontalScrollExtent()} and
 * {@link #computeHorizontalScrollOffset()}.</p>
 *
 * <p>The default range is the drawing width of this view.</p>
 *
 * @return the total horizontal range represented by the horizontal
 *         scrollbar
 *
 * @see #computeHorizontalScrollExtent()
 * @see #computeHorizontalScrollOffset()
 * @see android.widget.ScrollBarDrawable
 */
Int32 View::ComputeHorizontalScrollRange()
{
    return GetWidth();
}

/**
 * <p>Compute the horizontal offset of the horizontal scrollbar's thumb
 * within the horizontal range. This value is used to compute the position
 * of the thumb within the scrollbar's track.</p>
 *
 * <p>The range is expressed in arbitrary units that must be the same as the
 * units used by {@link #computeHorizontalScrollRange()} and
 * {@link #computeHorizontalScrollExtent()}.</p>
 *
 * <p>The default offset is the scroll offset of this view.</p>
 *
 * @return the horizontal offset of the scrollbar's thumb
 *
 * @see #computeHorizontalScrollRange()
 * @see #computeHorizontalScrollExtent()
 * @see android.widget.ScrollBarDrawable
 */
Int32 View::ComputeHorizontalScrollOffset()
{
    return mScrollX;
}

/**
 * <p>Compute the horizontal extent of the horizontal scrollbar's thumb
 * within the horizontal range. This value is used to compute the length
 * of the thumb within the scrollbar's track.</p>
 *
 * <p>The range is expressed in arbitrary units that must be the same as the
 * units used by {@link #computeHorizontalScrollRange()} and
 * {@link #computeHorizontalScrollOffset()}.</p>
 *
 * <p>The default extent is the drawing width of this view.</p>
 *
 * @return the horizontal extent of the scrollbar's thumb
 *
 * @see #computeHorizontalScrollRange()
 * @see #computeHorizontalScrollOffset()
 * @see android.widget.ScrollBarDrawable
 */
Int32 View::ComputeHorizontalScrollExtent()
{
    return GetWidth();
}

/**
 * <p>Compute the vertical range that the vertical scrollbar represents.</p>
 *
 * <p>The range is expressed in arbitrary units that must be the same as the
 * units used by {@link #computeVerticalScrollExtent()} and
 * {@link #computeVerticalScrollOffset()}.</p>
 *
 * @return the total vertical range represented by the vertical scrollbar
 *
 * <p>The default range is the drawing height of this view.</p>
 *
 * @see #computeVerticalScrollExtent()
 * @see #computeVerticalScrollOffset()
 * @see android.widget.ScrollBarDrawable
 */
Int32 View::ComputeVerticalScrollRange()
{
    return GetHeight();
}

/**
 * <p>Compute the vertical offset of the vertical scrollbar's thumb
 * within the horizontal range. This value is used to compute the position
 * of the thumb within the scrollbar's track.</p>
 *
 * <p>The range is expressed in arbitrary units that must be the same as the
 * units used by {@link #computeVerticalScrollRange()} and
 * {@link #computeVerticalScrollExtent()}.</p>
 *
 * <p>The default offset is the scroll offset of this view.</p>
 *
 * @return the vertical offset of the scrollbar's thumb
 *
 * @see #computeVerticalScrollRange()
 * @see #computeVerticalScrollExtent()
 * @see android.widget.ScrollBarDrawable
 */
Int32 View::ComputeVerticalScrollOffset()
{
    return mScrollY;
}

/**
 * <p>Compute the vertical extent of the horizontal scrollbar's thumb
 * within the vertical range. This value is used to compute the length
 * of the thumb within the scrollbar's track.</p>
 *
 * <p>The range is expressed in arbitrary units that must be the same as the
 * units used by {@link #computeVerticalScrollRange()} and
 * {@link #computeVerticalScrollOffset()}.</p>
 *
 * <p>The default extent is the drawing height of this view.</p>
 *
 * @return the vertical extent of the scrollbar's thumb
 *
 * @see #computeVerticalScrollRange()
 * @see #computeVerticalScrollOffset()
 * @see android.widget.ScrollBarDrawable
 */
Int32 View::ComputeVerticalScrollExtent()
{
    return GetHeight();
}

/**
 * <p>Request the drawing of the horizontal and the vertical scrollbar. The
 * scrollbars are painted only if they have been awakened first.</p>
 *
 * @param canvas the canvas on which to draw the scrollbars
 *
 * @see #awakenScrollBars(Int32)
 */
void View::OnDrawScrollBars(
    /* [in] */ ICanvas* canvas)
{
    // scrollbars are drawn only when the animation is running
    AutoPtr<ScrollabilityCache> cache = mScrollCache;
    if (cache != NULL) {

        Int32 state = cache->mState;

        if (state == ScrollabilityCache::OFF) {
            return;
        }

        Boolean invalidate = FALSE;

        if (state == ScrollabilityCache::FADING) {
            // We're fading -- get our fade interpolation
            if (cache->mInterpolatorValues == NULL) {
                cache->mInterpolatorValues = ArrayOf<Float>::Alloc(1);
            }

            ArrayOf<Float>* values = cache->mInterpolatorValues;

            // Stops the animation if we're done
            if (cache->mScrollBarInterpolator->TimeToValues(values) ==
                    Interpolator::FREEZE_END) {
                cache->mState = ScrollabilityCache::OFF;
            }
            else {
                cache->mScrollBar->SetAlpha(Math::Round((*values)[0]));
            }

            // This will make the scroll bars inval themselves after
            // drawing. We only want this when we're fading so that
            // we prevent excessive redraws
            invalidate = TRUE;
        }
        else {
            // We're just on -- but we may have been fading before so
            // reset alpha
            cache->mScrollBar->SetAlpha(255);
        }

        Int32 viewFlags = mViewFlags;

        Boolean drawHorizontalScrollBar =
            (viewFlags & SCROLLBARS_HORIZONTAL) == SCROLLBARS_HORIZONTAL;
        Boolean drawVerticalScrollBar =
            (viewFlags & SCROLLBARS_VERTICAL) == SCROLLBARS_VERTICAL
            && !IsVerticalScrollBarHidden();

        if (drawVerticalScrollBar || drawHorizontalScrollBar) {
            Int32 width = mRight - mLeft;
            Int32 height = mBottom - mTop;

            AutoPtr<IScrollBarDrawable> scrollBar = cache->mScrollBar;
            Int32 size;
            scrollBar->GetSize(FALSE, &size);
            if (size <= 0) {
                size = cache->mScrollBarSize;
            }

            Int32 scrollX = mScrollX;
            Int32 scrollY = mScrollY;
            Int32 inside = (viewFlags & SCROLLBARS_OUTSIDE_MASK) == 0 ? ~0 : 0;

            Int32 left, top, right, bottom;

            if (drawHorizontalScrollBar) {
                scrollBar->SetParameters(ComputeHorizontalScrollRange(),
                                         ComputeHorizontalScrollOffset(),
                                         ComputeHorizontalScrollExtent(), FALSE);
                Int32 verticalScrollBarGap = drawVerticalScrollBar ?
                        GetVerticalScrollbarWidth() : 0;
                top = scrollY + height - size - (mUserPaddingBottom & inside);
                left = scrollX + (mPaddingLeft & inside);
                right = scrollX + width - (mUserPaddingRight & inside) - verticalScrollBarGap;
                bottom = top + size;
                OnDrawHorizontalScrollBar(canvas, scrollBar, left, top, right, bottom);
                if (invalidate) {
                    Invalidate(left, top, right, bottom);
                }
            }

            if (drawVerticalScrollBar) {
                scrollBar->SetParameters(ComputeVerticalScrollRange(),
                                         ComputeVerticalScrollOffset(),
                                         ComputeVerticalScrollExtent(), TRUE);
                // TODO: Deal with RTL languages to position scrollbar on left
                left = scrollX + width - size - (mUserPaddingRight & inside);
                top = scrollY + (mPaddingTop & inside);
                right = left + size;
                bottom = scrollY + height - (mUserPaddingBottom & inside);
                OnDrawVerticalScrollBar(canvas, scrollBar, left, top, right, bottom);
                if (invalidate) {
                    Invalidate(left, top, right, bottom);
                }
            }
        }
    }
}

/**
 * Override this if the vertical scrollbar needs to be hidden in a subclass, like when
 * FastScroller is visible.
 * @return whether to temporarily hide the vertical scrollbar
 * @hide
 */
Boolean View::IsVerticalScrollBarHidden()
{
    return FALSE;
}

/**
 * <p>Draw the horizontal scrollbar if
 * {@link #isHorizontalScrollBarEnabled()} returns TRUE.</p>
 *
 * @param canvas the canvas on which to draw the scrollbar
 * @param scrollBar the scrollbar's drawable
 *
 * @see #isHorizontalScrollBarEnabled()
 * @see #computeHorizontalScrollRange()
 * @see #computeHorizontalScrollExtent()
 * @see #computeHorizontalScrollOffset()
 * @see android.widget.ScrollBarDrawable
 * @hide
 */
void View::OnDrawHorizontalScrollBar(
    /* [in] */ ICanvas* canvas,
    /* [in] */ IDrawable* scrollBar,
    /* [in] */ Int32 left,
    /* [in] */ Int32 top,
    /* [in] */ Int32 right,
    /* [in] */ Int32 bottom)
{
    scrollBar->SetBounds(left, top, right, bottom);
    scrollBar->Draw(canvas);
}

/**
 * <p>Draw the vertical scrollbar if {@link #isVerticalScrollBarEnabled()}
 * returns TRUE.</p>
 *
 * @param canvas the canvas on which to draw the scrollbar
 * @param scrollBar the scrollbar's drawable
 *
 * @see #isVerticalScrollBarEnabled()
 * @see #computeVerticalScrollRange()
 * @see #computeVerticalScrollExtent()
 * @see #computeVerticalScrollOffset()
 * @see android.widget.ScrollBarDrawable
 * @hide
 */
void View::OnDrawVerticalScrollBar(
    /* [in] */ ICanvas* canvas,
    /* [in] */ IDrawable* scrollBar,
    /* [in] */ Int32 left,
    /* [in] */ Int32 top,
    /* [in] */ Int32 right,
    /* [in] */ Int32 bottom)
{
    scrollBar->SetBounds(left, top, right, bottom);
    scrollBar->Draw(canvas);
}

/**
 * Implement this to do your drawing.
 *
 * @param canvas the canvas on which the background will be drawn
 */
void View::OnDraw(
    /* [in] */ ICanvas* canvas)
{}

ECode View::AssignParent(
    /* [in] */ IViewParent* parent)
{
    if (mParent == NULL) {
        mParent = parent;
    }
    else if (parent == NULL) {
        mParent = NULL;
    }
    else {
//          throw new RuntimeException("view " + this + " being added, but"
//                  + " it already has a parent");
        return E_RUNTIME_EXCEPTION;
    }
    return NOERROR;
}

/**
 * This is called when the view is attached to a window.  At this point it
 * has a Surface and will start drawing.  Note that this function is
 * guaranteed to be called before {@link #onDraw}, however it may be called
 * any time before the first onDraw -- including before or after
 * {@link #onMeasure}.
 *
 * @see #onDetachedFromWindow()
 */
void View::OnAttachedToWindow()
{
    if ((mPrivateFlags & REQUEST_TRANSPARENT_REGIONS) != 0) {
        mParent->RequestTransparentRegion((IView*)this->Probe(EIID_IView));
    }
    if ((mPrivateFlags & AWAKEN_SCROLL_BARS_ON_ATTACH) != 0) {
        InitialAwakenScrollBars();
        mPrivateFlags &= ~AWAKEN_SCROLL_BARS_ON_ATTACH;
    }
}

/**
 * This is called when the view is detached from a window.  At this point it
 * no longer has a surface for drawing.
 *
 * @see #onAttachedToWindow()
 */
void View::OnDetachedFromWindow()
{
    mPrivateFlags &= ~CANCEL_NEXT_UP_EVENT;
    RemoveUnsetPressCallback();
    RemoveLongPressCallback();
    DestroyDrawingCache();
}

/**
 * @return The number of times this view has been attached to a window
 */
Int32 View::GetWindowAttachCount()
{
    return mWindowAttachCount;
}

/**
 * Retrieve a unique token identifying the window this view is attached to.
 * @return Return the window's token for use in
 * {@link WindowManager.LayoutParams#token WindowManager.LayoutParams.token}.
 */
AutoPtr<IBinder> View::GetWindowToken()
{
    return mAttachInfo != NULL ? mAttachInfo->mWindowToken : AutoPtr<IBinder>(NULL);
}

/**
 * Retrieve a unique token identifying the top-level "real" window of
 * the window that this view is attached to.  That is, this is like
 * {@link #getWindowToken}, except if the window this view in is a panel
 * window (attached to another containing window), then the token of
 * the containing window is returned instead.
 *
 * @return Returns the associated window token, either
 * {@link #getWindowToken()} or the containing window's token.
 */
AutoPtr<IBinder> View::GetApplicationWindowToken()
{
    AttachInfo* ai = mAttachInfo;
    if (ai != NULL) {
        AutoPtr<IBinder> appWindowToken = ai->mPanelParentWindowToken;
        if (appWindowToken == NULL) {
            appWindowToken = ai->mWindowToken;
        }
        return appWindowToken;
    }
    return AutoPtr<IBinder>(NULL);
}

/**
 * Retrieve private session object this view hierarchy is using to
 * communicate with the window manager.
 * @return the session object to communicate with the window manager
 */
AutoPtr<IWindowSession> View::GetWindowSession()
{
    return mAttachInfo != NULL ? mAttachInfo->mSession : AutoPtr<IWindowSession>(NULL);
}

/**
 * @param info the {@link android.view.View.AttachInfo} to associated with
 *        this view
 */
void View::DispatchAttachedToWindow(
    /* [in] */ AttachInfo* info,
    /* [in] */ Int32 visibility)
{
    //System.out.println("Attached! " + this);
    mAttachInfo = info;
    mWindowAttachCount++;
    if (mFloatingTreeObserver != NULL) {
        info->mTreeObserver->Merge(mFloatingTreeObserver);
        mFloatingTreeObserver = NULL;
    }
    if ((mPrivateFlags&SCROLL_CONTAINER) != 0) {
        mAttachInfo->mScrollContainers.PushBack(
                AutoPtr<IView>((IView*)this->Probe(EIID_IView)));
        mPrivateFlags |= SCROLL_CONTAINER_ADDED;
    }

    PerformCollectViewAttributes(visibility);

    OnAttachedToWindow();
    Int32 vis = info->mWindowVisibility;
    if (vis != GONE) {
        OnWindowVisibilityChanged(vis);
    }
}

ECode View::DispatchDetachedFromWindow()
{
    //System.out.println("Detached! " + this);
    AttachInfo* info = mAttachInfo;
    if (info != NULL) {
        Int32 vis = info->mWindowVisibility;
        if (vis != GONE) {
            OnWindowVisibilityChanged(GONE);
        }
    }

    OnDetachedFromWindow();
    if ((mPrivateFlags & SCROLL_CONTAINER_ADDED) != 0) {
        mAttachInfo->mScrollContainers.Remove(
                AutoPtr<IView>((IView*)this->Probe(EIID_IView)));
        mPrivateFlags &= ~SCROLL_CONTAINER_ADDED;
    }
    mAttachInfo = NULL;

    return NOERROR;
}

/**
 * Store this view hierarchy's frozen state into the given container.
 *
 * @param container The SparseArray in which to save the view's state.
 *
 * @see #restoreHierarchyState
 * @see #dispatchSaveInstanceState
 * @see #onSaveInstanceState
 */
ECode View::SaveHierarchyState(
    /* [in, out] */ IObjectIntegerMap* container)
{
    return DispatchSaveInstanceState(container);
}

/**
 * Called by {@link #saveHierarchyState} to store the state for this view and its children.
 * May be overridden to modify how freezing happens to a view's children; for example, some
 * views may want to not store state for their children.
 *
 * @param container The SparseArray in which to save the view's state.
 *
 * @see #dispatchRestoreInstanceState
 * @see #saveHierarchyState
 * @see #onSaveInstanceState
 */
ECode View::DispatchSaveInstanceState(
    /* [in, out] */ IObjectIntegerMap* container)
{
    if (mID != NO_ID && (mViewFlags & SAVE_DISABLED_MASK) == 0) {
        mPrivateFlags &= ~SAVE_STATE_CALLED;
        AutoPtr<IParcelable> state = OnSaveInstanceState();
        if ((mPrivateFlags & SAVE_STATE_CALLED) == 0) {
            //throw new IllegalStateException(
              //      "Derived class did not call super.onSaveInstanceState()");
            return E_ILLEGAL_STATE_EXCEPTION;
        }
        if (state != NULL) {
            // Log.i("View", "Freezing #" + Integer.toHexString(mID)
            // + ": " + state);
            container->Put(mID, state.Get());
        }
    }
    return NOERROR;
}

/**
 * Hook allowing a view to generate a representation of its internal state
 * that can later be used to create a new instance with that same state.
 * This state should only contain information that is not persistent or can
 * not be reconstructed later. For example, you will never store your
 * current position on screen because that will be computed again when a
 * new instance of the view is placed in its view hierarchy.
 * <p>
 * Some examples of things you may store here: the current cursor position
 * in a text view (but usually not the text itself since that is stored in a
 * content provider or other persistent storage), the currently selected
 * item in a list view.
 *
 * @return Returns a Parcelable object containing the view's current dynamic
 *         state, or NULL if there is nothing interesting to save. The
 *         default implementation returns NULL.
 * @see #onRestoreInstanceState
 * @see #saveHierarchyState
 * @see #dispatchSaveInstanceState
 * @see #setSaveEnabled(Boolean)
 */
AutoPtr<IParcelable> View::OnSaveInstanceState()
{
    //mPrivateFlags |= SAVE_STATE_CALLED;
    //return BaseSavedState::EMPTY_STATE;
    return AutoPtr<IParcelable>(NULL);
}

/**
 * Restore this view hierarchy's frozen state from the given container.
 *
 * @param container The SparseArray which holds previously frozen states.
 *
 * @see #saveHierarchyState
 * @see #dispatchRestoreInstanceState
 * @see #onRestoreInstanceState
 */
ECode View::RestoreHierarchyState(
    /* [in] */ IObjectIntegerMap* container)
{
    return DispatchRestoreInstanceState(container);
}

/**
 * Called by {@link #restoreHierarchyState} to retrieve the state for this view and its
 * children. May be overridden to modify how restoreing happens to a view's children; for
 * example, some views may want to not store state for their children.
 *
 * @param container The SparseArray which holds previously saved state.
 *
 * @see #dispatchSaveInstanceState
 * @see #restoreHierarchyState
 * @see #onRestoreInstanceState
 */
ECode View::DispatchRestoreInstanceState(
    /* [in] */ IObjectIntegerMap* container)
{
    if (mID != NO_ID) {
        AutoPtr<IParcelable> state;
        container->Get(mID, (IInterface**)&state);
        if (state != NULL) {
            // Log.i("View", "Restoreing #" + Integer.toHexString(mID)
            // + ": " + state);
            mPrivateFlags &= ~SAVE_STATE_CALLED;
            OnRestoreInstanceState(state);
            if ((mPrivateFlags & SAVE_STATE_CALLED) == 0) {
                //throw new IllegalStateException(
                  //      "Derived class did not call super.onRestoreInstanceState()");
                return E_ILLEGAL_STATE_EXCEPTION;
            }
        }
    }
    return NOERROR;
}

/**
 * Hook allowing a view to re-apply a representation of its internal state that had previously
 * been generated by {@link #onSaveInstanceState}. This function will never be called with a
 * NULL state.
 *
 * @param state The frozen state that had previously been returned by
 *        {@link #onSaveInstanceState}.
 *
 * @see #onSaveInstanceState
 * @see #restoreHierarchyState
 * @see #dispatchRestoreInstanceState
 */
void View::OnRestoreInstanceState(
    /* [in] */ IParcelable* state)
{
    mPrivateFlags |= SAVE_STATE_CALLED;
    //if (state != BaseSavedState::EMPTY_STATE && state != NULL) {
        /*throw new IllegalArgumentException("Wrong state class, expecting View State but "
                + "received " + state.getClass().toString() + " instead. This usually happens "
                + "when two views of different type have the same id in the same hierarchy. "
                + "This view's id is " + ViewDebug.resolveId(mContext, getId()) + ". Make sure "
                + "other views do not use the same id.");*/
    //}
}

/**
 * <p>Return the time at which the drawing of the view hierarchy started.</p>
 *
 * @return the drawing start time in milliseconds
 */
Int64 View::GetDrawingTime()
{
    return mAttachInfo != NULL ? mAttachInfo->mDrawingTime : 0;
}

/**
 * <p>Enables or disables the duplication of the parent's state into this view. When
 * duplication is enabled, this view gets its drawable state from its parent rather
 * than from its own internal properties.</p>
 *
 * <p>Note: in the current implementation, setting this property to TRUE after the
 * view was added to a ViewGroup might have no effect at all. This property should
 * always be used from XML or set to TRUE before adding this view to a ViewGroup.</p>
 *
 * <p>Note: if this view's parent addStateFromChildren property is enabled and this
 * property is enabled, an exception will be thrown.</p>
 *
 * @param enabled True to enable duplication of the parent's drawable state, FALSE
 *                to disable it.
 *
 * @see #getDrawableState()
 * @see #isDuplicateParentStateEnabled()
 */
ECode View::SetDuplicateParentStateEnabled(
    /* [in] */ Boolean enabled)
{
    SetFlags(enabled ? DUPLICATE_PARENT_STATE : 0, DUPLICATE_PARENT_STATE);

    return NOERROR;
}

/**
 * <p>Indicates whether this duplicates its drawable state from its parent.</p>
 *
 * @return True if this view's drawable state is duplicated from the parent,
 *         FALSE otherwise
 *
 * @see #getDrawableState()
 * @see #setDuplicateParentStateEnabled(Boolean)
 */
Boolean View::IsDuplicateParentStateEnabled()
{
    return (mViewFlags & DUPLICATE_PARENT_STATE) == DUPLICATE_PARENT_STATE;
}

/**
 * <p>Enables or disables the drawing cache. When the drawing cache is enabled, the next call
 * to {@link #getDrawingCache()} or {@link #buildDrawingCache()} will draw the view in a
 * bitmap. Calling {@link #draw(android.graphics.Canvas)} will not draw from the cache when
 * the cache is enabled. To benefit from the cache, you must request the drawing cache by
 * calling {@link #getDrawingCache()} and draw it on screen if the returned bitmap is not
 * NULL.</p>
 *
 * @param enabled TRUE to enable the drawing cache, FALSE otherwise
 *
 * @see #isDrawingCacheEnabled()
 * @see #getDrawingCache()
 * @see #buildDrawingCache()
 */
ECode View::SetDrawingCacheEnabled(
    /* [in] */ Boolean enabled)
{
    SetFlags(enabled ? DRAWING_CACHE_ENABLED : 0, DRAWING_CACHE_ENABLED);

    return NOERROR;
}

/**
 * <p>Indicates whether the drawing cache is enabled for this view.</p>
 *
 * @return TRUE if the drawing cache is enabled
 *
 * @see #setDrawingCacheEnabled(Boolean)
 * @see #getDrawingCache()
 */
Boolean View::IsDrawingCacheEnabled()
{
    return (mViewFlags & DRAWING_CACHE_ENABLED) == DRAWING_CACHE_ENABLED;
}

/**
 * <p>Calling this method is equivalent to calling <code>getDrawingCache(FALSE)</code>.</p>
 *
 * @return A non-scaled bitmap representing this view or NULL if cache is disabled.
 *
 * @see #getDrawingCache(Boolean)
 */
AutoPtr<IBitmap> View::GetDrawingCache()
{
    return GetDrawingCache(FALSE);
}

/**
 * <p>Returns the bitmap in which this view drawing is cached. The returned bitmap
 * is NULL when caching is disabled. If caching is enabled and the cache is not ready,
 * this method will create it. Calling {@link #draw(android.graphics.Canvas)} will not
 * draw from the cache when the cache is enabled. To benefit from the cache, you must
 * request the drawing cache by calling this method and draw it on screen if the
 * returned bitmap is not NULL.</p>
 *
 * <p>Note about auto scaling in compatibility mode: When auto scaling is not enabled,
 * this method will create a bitmap of the same size as this view. Because this bitmap
 * will be drawn scaled by the parent ViewGroup, the result on screen might show
 * scaling artifacts. To avoid such artifacts, you should call this method by setting
 * the auto scaling to TRUE. Doing so, however, will generate a bitmap of a different
 * size than the view. This implies that your application must be able to handle this
 * size.</p>
 *
 * @param autoScale Indicates whether the generated bitmap should be scaled based on
 *        the current density of the screen when the application is in compatibility
 *        mode.
 *
 * @return A bitmap representing this view or NULL if cache is disabled.
 *
 * @see #setDrawingCacheEnabled(Boolean)
 * @see #isDrawingCacheEnabled()
 * @see #buildDrawingCache(Boolean)
 * @see #destroyDrawingCache()
 */
AutoPtr<IBitmap> View::GetDrawingCache(
    /* [in] */ Boolean autoScale)
{
    if ((mViewFlags & WILL_NOT_CACHE_DRAWING) == WILL_NOT_CACHE_DRAWING) {
        return AutoPtr<IBitmap>(NULL);
    }

    if ((mViewFlags & DRAWING_CACHE_ENABLED) == DRAWING_CACHE_ENABLED) {
        BuildDrawingCache(autoScale);
    }

    if (autoScale) {
        return (mDrawingCache == NULL ? AutoPtr<IBitmap>(NULL) : mDrawingCache);
    }
    else {
        return (mUnscaledDrawingCache == NULL ? AutoPtr<IBitmap>(NULL) : mUnscaledDrawingCache);
    }
}

/**
 * <p>Frees the resources used by the drawing cache. If you call
 * {@link #buildDrawingCache()} manually without calling
 * {@link #setDrawingCacheEnabled(Boolean) setDrawingCacheEnabled(TRUE)}, you
 * should cleanup the cache with this method afterwards.</p>
 *
 * @see #setDrawingCacheEnabled(Boolean)
 * @see #buildDrawingCache()
 * @see #getDrawingCache()
 */
ECode View::DestroyDrawingCache()
{
    if (mDrawingCache != NULL) {
        AutoPtr<IBitmap> bitmap = mDrawingCache;
        if (bitmap != NULL) bitmap->Recycle();
        mDrawingCache = NULL;
    }
    if (mUnscaledDrawingCache != NULL) {
        AutoPtr<IBitmap> bitmap = mUnscaledDrawingCache;
        if (bitmap != NULL) bitmap->Recycle();
        mUnscaledDrawingCache = NULL;
    }

    return NOERROR;
}

/**
 * Setting a solid background color for the drawing cache's bitmaps will improve
 * perfromance and memory usage. Note, though that this should only be used if this
 * view will always be drawn on top of a solid color.
 *
 * @param color The background color to use for the drawing cache's bitmap
 *
 * @see #setDrawingCacheEnabled(Boolean)
 * @see #buildDrawingCache()
 * @see #getDrawingCache()
 */
ECode View::SetDrawingCacheBackgroundColor(
    /* [in] */ Int32 color)
{
    if (color != mDrawingCacheBackgroundColor) {
        mDrawingCacheBackgroundColor = color;
        mPrivateFlags &= ~DRAWING_CACHE_VALID;
    }

    return NOERROR;
}

/**
 * @see #setDrawingCacheBackgroundColor(int)
 *
 * @return The background color to used for the drawing cache's bitmap
 */
Int32 View::GetDrawingCacheBackgroundColor()
{
    return mDrawingCacheBackgroundColor;
}

/**
 * <p>Calling this method is equivalent to calling <code>buildDrawingCache(FALSE)</code>.</p>
 *
 * @see #buildDrawingCache(Boolean)
 */
ECode View::BuildDrawingCache()
{
    BuildDrawingCache(FALSE);

    return NOERROR;
}

/**
 * <p>Forces the drawing cache to be built if the drawing cache is invalid.</p>
 *
 * <p>If you call {@link #buildDrawingCache()} manually without calling
 * {@link #setDrawingCacheEnabled(Boolean) setDrawingCacheEnabled(TRUE)}, you
 * should cleanup the cache by calling {@link #destroyDrawingCache()} afterwards.</p>
 *
 * <p>Note about auto scaling in compatibility mode: When auto scaling is not enabled,
 * this method will create a bitmap of the same size as this view. Because this bitmap
 * will be drawn scaled by the parent ViewGroup, the result on screen might show
 * scaling artifacts. To avoid such artifacts, you should call this method by setting
 * the auto scaling to TRUE. Doing so, however, will generate a bitmap of a different
 * size than the view. This implies that your application must be able to handle this
 * size.</p>
 *
 * @see #getDrawingCache()
 * @see #destroyDrawingCache()
 */
void View::BuildDrawingCache(
    /* [in] */ Boolean autoScale)
{
    if ((mPrivateFlags & DRAWING_CACHE_VALID) == 0 || (autoScale ?
            (mDrawingCache == NULL /*|| mDrawingCache->Get() == NULL*/) :
            (mUnscaledDrawingCache == NULL /*|| mUnscaledDrawingCache->Get() == NULL*/))) {

        /*if (ViewDebug::TRACE_HIERARCHY) {
            ViewDebug::Trace(this, ViewDebug::HierarchyTraceType::BUILD_CACHE);
        }*/
        /*if (Config::DEBUG && ViewDebug::profileDrawing) {
            EventLog->WriteEvent(60002, HashCode());
        }*/

        Int32 width = mRight - mLeft;
        Int32 height = mBottom - mTop;

        AttachInfo* attachInfo = mAttachInfo;
        Boolean scalingRequired = attachInfo != NULL && attachInfo->mScalingRequired;

        if (autoScale && scalingRequired) {
            width = (Int32)((width * attachInfo->mApplicationScale) + 0.5f);
            height = (Int32)((height * attachInfo->mApplicationScale) + 0.5f);
        }

        Int32 drawingCacheBackgroundColor = mDrawingCacheBackgroundColor;
        Boolean opaque = drawingCacheBackgroundColor != 0 || IsOpaque();
        Boolean use32BitCache = attachInfo != NULL && attachInfo->mUse32BitDrawingCache;

        if (width <= 0 || height <= 0 ||
                    // Projected bitmap size in bytes
                (width * height * (opaque && !use32BitCache ? 2 : 4) >
                        ViewConfiguration::Get(mContext)->GetScaledMaximumDrawingCacheSize())) {
            DestroyDrawingCache();
            return;
        }

        Boolean clear = TRUE;
        AutoPtr<IBitmap> bitmap = autoScale ? (mDrawingCache == NULL ? NULL : mDrawingCache) :
                (mUnscaledDrawingCache == NULL ? NULL : mUnscaledDrawingCache);

        Int32 bitmapWidth, bitmapHeight;
        if (bitmap == NULL || (bitmap->GetWidth(&bitmapWidth), bitmapWidth) != width
            || (bitmap->GetHeight(&bitmapHeight), bitmapHeight) != height) {
            BitmapConfig quality;
            if (!opaque) {
                switch (mViewFlags & DRAWING_CACHE_QUALITY_MASK) {
                    case DRAWING_CACHE_QUALITY_AUTO:
                        quality = BitmapConfig_ARGB_8888;
                        break;
                    case DRAWING_CACHE_QUALITY_LOW:
                        quality = BitmapConfig_ARGB_4444;
                        break;
                    case DRAWING_CACHE_QUALITY_HIGH:
                        quality = BitmapConfig_ARGB_8888;
                        break;
                    default:
                        quality = BitmapConfig_ARGB_8888;
                        break;
                }
            }
            else {
                // Optimization for translucent windows
                // If the window is translucent, use a 32 bits bitmap to benefit from memcpy()
                quality = use32BitCache ? BitmapConfig_ARGB_8888 : BitmapConfig_RGB_565;
            }

            // Try to cleanup memory
            if (bitmap != NULL) {
                bitmap->Recycle();
            }

            AutoPtr<IBitmapFactory> factory;
            CBitmapFactory::AcquireSingleton((IBitmapFactory**)&factory);
            bitmap = NULL;
            ECode ec = factory->CreateBitmapEx3(width, height, quality, (IBitmap**)&bitmap);
            if (FAILED(ec)) {
                if (ec == (ECode)E_OUT_OF_MEMORY_ERROR) {
                    // If there is not enough memory to create the bitmap cache, just
                    // ignore the issue as bitmap caches are not required to draw the
                    // view hierarchy
                    if (autoScale) {
                        mDrawingCache = NULL;
                    } else {
                        mUnscaledDrawingCache = NULL;
                    }
                }
                return;
            }

            AutoPtr<IResources> resource = GetResources();
            AutoPtr<IDisplayMetrics> metrics;
            resource->GetDisplayMetrics((IDisplayMetrics**)&metrics);
            bitmap->SetDensity(((CDisplayMetrics*)metrics.Get())->mDensityDpi);
            if (autoScale) {
                mDrawingCache = bitmap;
            }
            else {
                mUnscaledDrawingCache = bitmap;
            }
            if (opaque && use32BitCache) bitmap->SetHasAlpha(FALSE);

            clear = drawingCacheBackgroundColor != 0;
        }

        AutoPtr<ICanvas> canvas;
        if (attachInfo != NULL) {
            canvas = attachInfo->mCanvas;
            if (canvas == NULL) {
                CCanvas::New((ICanvas**)&canvas);
            }
            canvas->SetBitmap(bitmap);
            // Temporarily clobber the cached Canvas in case one of our children
            // is also using a drawing cache. Without this, the children would
            // steal the canvas by attaching their own bitmap to it and bad, bad
            // thing would happen (invisible views, corrupted drawings, etc.)
            attachInfo->mCanvas = NULL;
        }
        else {
            // This case should hopefully never or seldom happen
            CCanvas::New(bitmap, (ICanvas**)&canvas);
        }

        if (clear) {
            bitmap->EraseColor(drawingCacheBackgroundColor);
        }

        ComputeScroll();
        Int32 restoreCount;
        canvas->Save(&restoreCount);

        if (autoScale && scalingRequired) {
            Float scale = attachInfo->mApplicationScale;
            canvas->Scale(scale, scale);
        }

        canvas->Translate(-mScrollX, -mScrollY);

        mPrivateFlags |= DRAWN;
        mPrivateFlags |= DRAWING_CACHE_VALID;

        // Fast path for layouts with no backgrounds
        if ((mPrivateFlags & SKIP_DRAW) == SKIP_DRAW) {
            /*if (ViewDebug::TRACE_HIERARCHY) {
                ViewDebug::Trace(this, ViewDebug::HierarchyTraceType::DRAW);
            }*/
            mPrivateFlags &= ~DIRTY_MASK;
            DispatchDraw(canvas);
        }
        else {
            Draw(canvas);
            //TODO: delete
            //
            //AutoPtr<IPaintEx> paint;
            //CPaintEx::New((IPaintEx**)&paint);
            //paint->SetColor(0xFF000000 | Int32(this));
            //Int32 w, h;
            //canvas->GetWidth(&w);
            //canvas->GetHeight(&h);
            //printf("canvas w = %d, h = %d\n", w, h);
            ////canvas->DrawLine(0.0, 0.0, 200.0, 20.0, paint);
            ////canvas->DrawCircle(20.0, 20.0, 10.0, paint);
            //AutoPtr<IRect> rc;
            //CRect::New(0, 0, w/2, h/2, (IRect**)&rc);
            //canvas->DrawRectEx(rc, paint);
        }

        canvas->RestoreToCount(restoreCount);

        if (attachInfo != NULL) {
            // Restore the cached Canvas for our siblings
            attachInfo->mCanvas = canvas;
        }
    }
}

/**
 * Create a snapshot of the view into a bitmap.  We should probably make
 * some form of this public, but should think about the API.
 */
ECode View::CreateSnapshot(
    /* [in] */ BitmapConfig quality,
    /* [in] */ Int32 backgroundColor,
    /* [in] */ Boolean skipChildren,
    /* [out] */ IBitmap** bm)
{
    assert(bm != NULL);

    Int32 width = mRight - mLeft;
    Int32 height = mBottom - mTop;

    AttachInfo* attachInfo = mAttachInfo;
    Float scale = attachInfo != NULL ? attachInfo->mApplicationScale : 1.0f;
    width = (Int32) ((width * scale) + 0.5f);
    height = (Int32) ((height * scale) + 0.5f);

    AutoPtr<IBitmap> bitmap;
    AutoPtr<IBitmapFactory> factory;
    CBitmapFactory::AcquireSingleton((IBitmapFactory**)&factory);
    factory->CreateBitmapEx3(width > 0 ? width : 1, height > 0 ? height : 1, quality, (IBitmap**)&bitmap);
    if (bitmap == NULL) {
        //throw new OutOfMemoryError();
        return E_OUT_OF_MEMORY_ERROR;
    }

    AutoPtr<IResources> resource = GetResources();
    AutoPtr<IDisplayMetrics> metrics;
    resource->GetDisplayMetrics((IDisplayMetrics**)&metrics);
    bitmap->SetDensity(((CDisplayMetrics*)metrics.Get())->mDensityDpi);

    AutoPtr<ICanvas> canvas;
    if (attachInfo != NULL) {
        canvas = attachInfo->mCanvas;
        if (canvas == NULL) {
            CCanvas::New((ICanvas**)&canvas);
        }
        canvas->SetBitmap(bitmap.Get());
        // Temporarily clobber the cached Canvas in case one of our children
        // is also using a drawing cache. Without this, the children would
        // steal the canvas by attaching their own bitmap to it and bad, bad
        // things would happen (invisible views, corrupted drawings, etc.)
        attachInfo->mCanvas = NULL;
    }
    else {
        // This case should hopefully never or seldom happen
        CCanvas::New(bitmap.Get(), (ICanvas**)&canvas);
    }

    if ((backgroundColor & 0xff000000) != 0) {
        bitmap->EraseColor(backgroundColor);
    }

    ComputeScroll();
    Int32 restoreCount;
    canvas->Save(&restoreCount);
    canvas->Scale(scale, scale);
    canvas->Translate(-mScrollX, -mScrollY);

    // Temporarily remove the dirty mask
    Int32 flags = mPrivateFlags;
    mPrivateFlags &= ~DIRTY_MASK;

    // Fast path for layouts with no backgrounds
    if ((mPrivateFlags & SKIP_DRAW) == SKIP_DRAW) {
        DispatchDraw(canvas);
    }
    else {
        Draw(canvas);
    }

    mPrivateFlags = flags;

    canvas->RestoreToCount(restoreCount);

    if (attachInfo != NULL) {
        // Restore the cached Canvas for our siblings
        attachInfo->mCanvas = canvas;
    }

    *bm = bitmap;
    (*bm)->AddRef();
    return NOERROR;
}

/**
 * Indicates whether this View is currently in edit mode. A View is usually
 * in edit mode when displayed within a developer tool. For instance, if
 * this View is being drawn by a visual user interface builder, this method
 * should return TRUE.
 *
 * Subclasses should check the return value of this method to provide
 * different behaviors if their normal behavior might interfere with the
 * host environment. For instance: the class spawns a thread in its
 * constructor, the drawing code relies on device-specific features, etc.
 *
 * This method is usually checked in the drawing code of custom widgets.
 *
 * @return True if this View is in edit mode, FALSE otherwise.
 */
Boolean View::IsInEditMode()
{
    return FALSE;
}

/**
 * If the View draws content inside its padding and enables fading edges,
 * it needs to support padding offsets. Padding offsets are added to the
 * fading edges to extend the length of the fade so that it covers pixels
 * drawn inside the padding.
 *
 * Subclasses of this class should override this method if they need
 * to draw content inside the padding.
 *
 * @return True if padding offset must be applied, FALSE otherwise.
 *
 * @see #getLeftPaddingOffset()
 * @see #getRightPaddingOffset()
 * @see #getTopPaddingOffset()
 * @see #getBottomPaddingOffset()
 *
 * @since CURRENT
 */
Boolean View::IsPaddingOffsetRequired()
{
    return FALSE;
}

/**
 * Amount by which to extend the left fading region. Called only when
 * {@link #isPaddingOffsetRequired()} returns TRUE.
 *
 * @return The left padding offset in pixels.
 *
 * @see #isPaddingOffsetRequired()
 *
 * @since CURRENT
 */
Int32 View::GetLeftPaddingOffset()
{
    return 0;
}

/**
 * Amount by which to extend the right fading region. Called only when
 * {@link #isPaddingOffsetRequired()} returns TRUE.
 *
 * @return The right padding offset in pixels.
 *
 * @see #isPaddingOffsetRequired()
 *
 * @since CURRENT
 */
Int32 View::GetRightPaddingOffset()
{
    return 0;
}

/**
 * Amount by which to extend the top fading region. Called only when
 * {@link #isPaddingOffsetRequired()} returns TRUE.
 *
 * @return The top padding offset in pixels.
 *
 * @see #isPaddingOffsetRequired()
 *
 * @since CURRENT
 */
Int32 View::GetTopPaddingOffset()
{
    return 0;
}

/**
 * Amount by which to extend the bottom fading region. Called only when
 * {@link #isPaddingOffsetRequired()} returns TRUE.
 *
 * @return The bottom padding offset in pixels.
 *
 * @see #isPaddingOffsetRequired()
 *
 * @since CURRENT
 */
Int32 View::GetBottomPaddingOffset()
{
    return 0;
}

/**
 * Manually render this view (and all of its children) to the given Canvas.
 * The view must have already done a full layout before this function is
 * called.  When implementing a view, do not override this method; instead,
 * you should implement {@link #onDraw}.
 *
 * @param canvas The Canvas to which the View is rendered.
 */
ECode View::Draw(
    /* [in] */ ICanvas* canvas)
{
//    if (ViewDebug.TRACE_HIERARCHY) {
//        ViewDebug.trace(this, ViewDebug.HierarchyTraceType.DRAW);
//    }

    //printf("View::Draw ID = 0x%08x, this = 0x%08x----------------1\n", mID, this);
    Int32 privateFlags = mPrivateFlags;
    Boolean dirtyOpaque = (privateFlags & DIRTY_MASK) == DIRTY_OPAQUE &&
            (mAttachInfo == NULL || !mAttachInfo->mIgnoreDirtyState);
    mPrivateFlags = (privateFlags & ~DIRTY_MASK) | DRAWN;

    /*
     * Draw traversal performs several drawing steps which must be executed
     * in the appropriate order:
     *
     *      1. Draw the background
     *      2. If necessary, save the canvas' layers to prepare for fading
     *      3. Draw view's content
     *      4. Draw children
     *      5. If necessary, draw the fading edges and restore layers
     *      6. Draw decorations (scrollbars for instance)
     */

    // Step 1, draw the background, if needed
    Int32 saveCount;

    if (!dirtyOpaque) {
        AutoPtr<IDrawable> background = mBGDrawable;
        if (background != NULL) {
            Int32 scrollX = mScrollX;
            Int32 scrollY = mScrollY;

            if (mBackgroundSizeChanged) {
                background->SetBounds(0, 0,  mRight - mLeft, mBottom - mTop);
                mBackgroundSizeChanged = FALSE;
            }

            if ((scrollX | scrollY) == 0) {
                background->Draw(canvas);
            }
            else {
                canvas->Translate(scrollX, scrollY);
                background->Draw(canvas);
                canvas->Translate(-scrollX, -scrollY);
            }
        }
    }

    // skip step 2 & 5 if possible (common case)
    Int32 viewFlags = mViewFlags;
    Boolean horizontalEdges = (viewFlags & FADING_EDGE_HORIZONTAL) != 0;
    Boolean verticalEdges = (viewFlags & FADING_EDGE_VERTICAL) != 0;
    if (!verticalEdges && !horizontalEdges) {
        //printf("View::Draw ID = 0x%08x, this = 0x%08x----------------2\n", mID, this);
        // Step 3, draw the content
        if (!dirtyOpaque) {
            OnDraw(canvas);
        }

        //printf("View::Draw ID = 0x%08x, this = 0x%08x----------------3\n", mID, this);
        // Step 4, draw the children
        DispatchDraw(canvas);

        // Step 6, draw decorations (scrollbars)
        OnDrawScrollBars(canvas);

        //printf("View::Draw ID = 0x%08x, this = 0x%08x----------------4\n", mID, this);
        // we're done...
        return NOERROR;
    }

    /*
     * Here we do the full fledged routine...
     * (this is an uncommon case where speed matters less,
     * this is why we repeat some of the tests that have been
     * done above)
     */

    Boolean drawTop = FALSE;
    Boolean drawBottom = FALSE;
    Boolean drawLeft = FALSE;
    Boolean drawRight = FALSE;

    Float topFadeStrength = 0.0f;
    Float bottomFadeStrength = 0.0f;
    Float leftFadeStrength = 0.0f;
    Float rightFadeStrength = 0.0f;

    // Step 2, save the canvas' layers
    Int32 paddingLeft = mPaddingLeft;
    Int32 paddingTop = mPaddingTop;

    Boolean offsetRequired = IsPaddingOffsetRequired();
    if (offsetRequired) {
        paddingLeft += GetLeftPaddingOffset();
        paddingTop += GetTopPaddingOffset();
    }

    Int32 left = mScrollX + paddingLeft;
    Int32 right = left + mRight - mLeft - mPaddingRight - paddingLeft;
    Int32 top = mScrollY + paddingTop;
    Int32 bottom = top + mBottom - mTop - mPaddingBottom - paddingTop;

    if (offsetRequired) {
        right += GetRightPaddingOffset();
        bottom += GetBottomPaddingOffset();
    }

    AutoPtr<ScrollabilityCache> scrollabilityCache = mScrollCache;
    Int32 length = scrollabilityCache->mFadingEdgeLength;

    // clip the fade length if top and bottom fades overlap
    // overlapping fades produce odd-looking artifacts
    if (verticalEdges && (top + length > bottom - length)) {
        length = (bottom - top) / 2;
    }

    // also clip horizontal fades if necessary
    if (horizontalEdges && (left + length > right - length)) {
        length = (right - left) / 2;
    }

    if (verticalEdges) {
        topFadeStrength = Math::Max(0.0f, Math::Min(1.0f, GetTopFadingEdgeStrength()));
        drawTop = topFadeStrength >= 0.0f;
        bottomFadeStrength = Math::Max(0.0f, Math::Min(1.0f, GetBottomFadingEdgeStrength()));
        drawBottom = bottomFadeStrength >= 0.0f;
    }

    if (horizontalEdges) {
        leftFadeStrength = Math::Max(0.0f, Math::Min(1.0f, GetLeftFadingEdgeStrength()));
        drawLeft = leftFadeStrength >= 0.0f;
        rightFadeStrength = Math::Max(0.0f, Math::Min(1.0f, GetRightFadingEdgeStrength()));
        drawRight = rightFadeStrength >= 0.0f;
    }

    canvas->GetSaveCount(&saveCount);

    Int32 solidColor = GetSolidColor();
    if (solidColor == 0) {
        Int32 count;
        Int32 flags = CCanvas::HAS_ALPHA_LAYER_SAVE_FLAG;

        if (drawTop) {
            canvas->SaveLayerEx(left, top, right, top + length, NULL, flags, &count);
        }

        if (drawBottom) {
            canvas->SaveLayerEx(left, bottom - length, right, bottom, NULL, flags, &count);
        }

        if (drawLeft) {
            canvas->SaveLayerEx(left, top, left + length, bottom, NULL, flags, &count);
        }

        if (drawRight) {
            canvas->SaveLayerEx(right - length, top, right, bottom, NULL, flags, &count);
        }
    }
    else {
        scrollabilityCache->SetFadeColor(solidColor);
    }

    // Step 3, draw the content
    if (!dirtyOpaque) OnDraw(canvas);

    //printf("View::Draw ID = 0x%08x, this = 0x%08x----------------5\n", mID, this);
    // Step 4, draw the children
    DispatchDraw(canvas);
    //printf("View::Draw ID = 0x%08x, this = 0x%08x----------------6\n", mID, this);

    // Step 5, draw the fade effect and restore layers
    AutoPtr<IPaint> p = scrollabilityCache->mPaint;
    AutoPtr<IMatrix> matrix = scrollabilityCache->mMatrix;
    AutoPtr<IShader> fade = scrollabilityCache->mShader;
    Float fadeHeight = scrollabilityCache->mFadingEdgeLength;

    Boolean result;
    if (drawTop) {
        matrix->SetScaleEx(1, fadeHeight * topFadeStrength);
        matrix->PostTranslate(left, top, &result);
        fade->SetLocalMatrix(matrix);
        canvas->DrawRectEx2(left, top, right, top + length, p);
    }

    if (drawBottom) {
        matrix->SetScaleEx(1, fadeHeight * bottomFadeStrength);
        matrix->PostRotateEx(180, &result);
        matrix->PostTranslate(left, bottom, &result);
        fade->SetLocalMatrix(matrix);
        canvas->DrawRectEx2(left, bottom - length, right, bottom, p);
    }

    if (drawLeft) {
        matrix->SetScaleEx(1, fadeHeight * leftFadeStrength);
        matrix->PostRotateEx(-90, &result);
        matrix->PostTranslate(left, top, &result);
        fade->SetLocalMatrix(matrix);
        canvas->DrawRectEx2(left, top, left + length, bottom, p);
    }

    if (drawRight) {
        matrix->SetScaleEx(1, fadeHeight * rightFadeStrength);
        matrix->PostRotateEx(90, &result);
        matrix->PostTranslate(right, top, &result);
        fade->SetLocalMatrix(matrix);
        canvas->DrawRectEx2(right - length, top, right, bottom, p);
    }

    canvas->RestoreToCount(saveCount);

    //printf("View::Draw ID = 0x%08x, this = 0x%08x----------------7\n", mID, this);
    // Step 6, draw decorations (scrollbars)
    OnDrawScrollBars(canvas);
    //printf("View::Draw ID = 0x%08x, this = 0x%08x----------------8\n", mID, this);

    return NOERROR;
}

/**
 * Override this if your view is known to always be drawn on top of a solid color background,
 * and needs to draw fading edges. Returning a non-zero color enables the view system to
 * optimize the drawing of the fading edges. If you do return a non-zero color, the alpha
 * should be set to 0xFF.
 *
 * @see #setVerticalFadingEdgeEnabled
 * @see #setHorizontalFadingEdgeEnabled
 *
 * @return The known solid color background for this view, or 0 if the color may vary
 */
Int32 View::GetSolidColor()
{
    return 0;
}

/**
 * Build a human readable string representation of the specified view flags.
 *
 * @param flags the view flags to convert to a string
 * @return a String representing the supplied flags
 */
ECode View::PrintFlags(
    /* [in] */ Int32 flags,
    /* [out] */ String* output)
{
    VALIDATE_NOT_NULL(output);

    StringBuffer sb("");
    Int32 numFlags = 0;
    if ((flags & FOCUSABLE_MASK) == FOCUSABLE) {
        sb += "TAKES_FOCUS";
        numFlags++;
    }

    switch (flags & VISIBILITY_MASK) {
    case INVISIBLE:
        if (numFlags > 0) {
            sb += " ";
        }
        sb += "INVISIBLE";
        // USELESS HERE numFlags++;
        break;
    case GONE:
        if (numFlags > 0) {
            sb += " ";
        }
        sb += "GONE";
        // USELESS HERE numFlags++;
        break;
    default:
        break;
    }
    *output = (const char*)sb;
    return NOERROR;
}

/**
 * Build a human readable string representation of the specified private
 * view flags.
 *
 * @param privateFlags the private view flags to convert to a string
 * @return a String representing the supplied flags
 */
ECode View::PrintPrivateFlags(
    /* [in] */ Int32 privateFlags,
    /* [out] */ String* output)
{
    VALIDATE_NOT_NULL(output);

    StringBuffer sb("");
    Int32 numFlags = 0;

    if ((privateFlags & WANTS_FOCUS) == WANTS_FOCUS) {
        sb += "WANTS_FOCUS";
        numFlags++;
    }

    if ((privateFlags & FOCUSED) == FOCUSED) {
        if (numFlags > 0) {
            sb += " ";
        }
        sb += "FOCUSED";
        numFlags++;
    }

    if ((privateFlags & SELECTED) == SELECTED) {
        if (numFlags > 0) {
            sb += " ";
        }
        sb += "SELECTED";
        numFlags++;
    }

    if ((privateFlags & IS_ROOT_NAMESPACE) == IS_ROOT_NAMESPACE) {
        if (numFlags > 0) {
            sb += " ";
        }
        sb += "IS_ROOT_NAMESPACE";
        numFlags++;
    }

    if ((privateFlags & HAS_BOUNDS) == HAS_BOUNDS) {
        if (numFlags > 0) {
            sb += " ";
        }
        sb += "HAS_BOUNDS";
        numFlags++;
    }

    if ((privateFlags & DRAWN) == DRAWN) {
        if (numFlags > 0) {
            sb += " ";
        }
        sb += "DRAWN";
        // USELESS HERE numFlags++;
    }
    *output = (const char*)sb;
    return NOERROR;
}

/**
 * <p>Indicates whether or not this view's layout will be requested during
 * the next hierarchy layout pass.</p>
 *
 * @return TRUE if the layout will be forced during next layout pass
 */
Boolean View::IsLayoutRequested()
{
    return (mPrivateFlags & FORCE_LAYOUT) == FORCE_LAYOUT;
}

/**
 * Assign a size and position to a view and all of its
 * descendants
 *
 * <p>This is the second phase of the layout mechanism.
 * (The first is measuring). In this phase, each parent calls
 * layout on all of its children to position them.
 * This is typically done using the child measurements
 * that were stored in the measure pass().
 *
 * Derived classes with children should override
 * onLayout. In that method, they should
 * call layout on each of their their children.
 *
 * @param l Left position, relative to parent
 * @param t Top position, relative to parent
 * @param r Right position, relative to parent
 * @param b Bottom position, relative to parent
 */
ECode View::Layout(
    /* [in] */ Int32 l,
    /* [in] */ Int32 t,
    /* [in] */ Int32 r,
    /* [in] */ Int32 b)
{
    Boolean changed = SetFrame(l, t, r, b);
    if (changed || (mPrivateFlags & LAYOUT_REQUIRED) == LAYOUT_REQUIRED) {
        /*if (ViewDebug::TRACE_HIERARCHY) {
            ViewDebug::Trace(this, ViewDebug::HierarchyTraceType::ON_LAYOUT);
        }*/

        OnLayout(changed, l, t, r, b);
        mPrivateFlags &= ~LAYOUT_REQUIRED;
    }
    mPrivateFlags &= ~FORCE_LAYOUT;

    return NOERROR;
}

/**
 * Called from layout when this view should
 * assign a size and position to each of its children.
 *
 * Derived classes with children should override
 * this method and call layout on each of
 * their their children.
 * @param changed This is a new size or position for this view
 * @param left Left position, relative to parent
 * @param top Top position, relative to parent
 * @param right Right position, relative to parent
 * @param bottom Bottom position, relative to parent
 */
void View::OnLayout(
    /* [in] */ Boolean changed,
    /* [in] */ Int32 left,
    /* [in] */ Int32 top,
    /* [in] */ Int32 right,
    /* [in] */ Int32 bottom)
{
}

/**
 * Assign a size and position to this view.
 *
 * This is called from layout.
 *
 * @param left Left position, relative to parent
 * @param top Top position, relative to parent
 * @param right Right position, relative to parent
 * @param bottom Bottom position, relative to parent
 * @return TRUE if the new size and position are different than the
 *         previous ones
 * {@hide}
 */
Boolean View::SetFrame(
    /* [in] */ Int32 left,
    /* [in] */ Int32 top,
    /* [in] */ Int32 right,
    /* [in] */ Int32 bottom)
{
    //printf("ID=0x%08x, 0x%08x, View::SetFrame\n", mID, this);
    Boolean changed = FALSE;

    if (DBG) {
        Logger::D("View", /*this + */StringBuffer(" View.setFrame(")
            + left + "," + top + "," + right + "," + bottom + ")");
    }

    if (mLeft != left || mRight != right || mTop != top || mBottom != bottom) {
        //printf("left = %d, right = %d, top = %d, bottom = %d\n", left, right, top, bottom);
        changed = TRUE;

        // Remember our drawn bit
        Int32 drawn = mPrivateFlags & DRAWN;

        // Invalidate our old position
        Invalidate();

        Int32 oldWidth = mRight - mLeft;
        Int32 oldHeight = mBottom - mTop;

        mLeft = left;
        mTop = top;
        mRight = right;
        mBottom = bottom;

        mPrivateFlags |= HAS_BOUNDS;

        Int32 newWidth = right - left;
        Int32 newHeight = bottom - top;

        if (newWidth != oldWidth || newHeight != oldHeight) {
            OnSizeChanged(newWidth, newHeight, oldWidth, oldHeight);
        }

        if ((mViewFlags & VISIBILITY_MASK) == View_VISIBLE) {
            // If we are visible, force the DRAWN bit to on so that
            // this invalidate will go through (at least to our parent).
            // This is because someone may have invalidated this view
            // before this call to setFrame came in, therby clearing
            // the DRAWN bit.
            mPrivateFlags |= DRAWN;
            Invalidate();
        }

        // Reset drawn bit to original value (invalidate turns it off)
        mPrivateFlags |= drawn;

        mBackgroundSizeChanged = TRUE;
    }
    return changed;
}

/**
 * Finalize inflating a view from XML.  This is called as the last phase
 * of inflation, after all child views have been added.
 *
 * <p>Even if the subclass overrides onFinishInflate, they should always be
 * sure to call the super method, so that we get called.
 */
ECode View::OnFinishInflate()
{
    return NOERROR;
}

/**
 * Returns the resources associated with this view.
 *
 * @return Resources object.
 */
AutoPtr<IResources> View::GetResources()
{
    return mResources;
}

/**
 * Invalidates the specified Drawable.
 *
 * @param drawable the drawable to invalidate
 */
ECode View::InvalidateDrawable(
    /* [in] */ IDrawable* drawable)
{
    if (VerifyDrawable(drawable)) {
        AutoPtr<IRect> dirty;
        drawable->GetBounds((IRect**)&dirty);
        Int32 scrollX = mScrollX;
        Int32 scrollY = mScrollY;

        CRect* dirtyObj = (CRect*)dirty.Get();
        Invalidate(dirtyObj->mLeft + scrollX, dirtyObj->mTop + scrollY,
                dirtyObj->mRight + scrollX, dirtyObj->mBottom + scrollY);
    }
    return NOERROR;
}

/**
 * Schedules an action on a drawable to occur at a specified time.
 *
 * @param who the recipient of the action
 * @param what the action to run on the drawable
 * @param when the time at which the action must occur. Uses the
 *        {@link SystemClock#uptimeMillis} timebase.
 */
ECode View::ScheduleDrawable(
    /* [in] */ IDrawable* who,
    /* [in] */ IRunnable* what,
    /* [in] */ Int64 when)
{
    if (VerifyDrawable(who) && what != NULL && mAttachInfo != NULL) {
        ECode (STDCALL IRunnable::*pHandlerFunc)();
        pHandlerFunc = &IRunnable::Run;

        mAttachInfo->mHandler->PostCppCallbackAtTime(
            (Handle32)what, *(Handle32*)&pHandlerFunc, NULL, Int32(who), when);
    }
    return NOERROR;
}

/**
 * Cancels a scheduled action on a drawable.
 *
 * @param who the recipient of the action
 * @param what the action to cancel
 */
ECode View::UnscheduleDrawable(
    /* [in] */ IDrawable* who,
    /* [in] */ IRunnable* what)
{
    if (VerifyDrawable(who) && what != NULL && mAttachInfo != NULL) {
        ECode (STDCALL IRunnable::*pHandlerFunc)();
        pHandlerFunc = &IRunnable::Run;

        mAttachInfo->mHandler->RemoveCppCallbacksEx(
            (Handle32)what, *(Handle32*)&pHandlerFunc, Int32(who));
    }
    return NOERROR;
}

/**
 * Unschedule any events associated with the given Drawable.  This can be
 * used when selecting a new Drawable into a view, so that the previous
 * one is completely unscheduled.
 *
 * @param who The Drawable to unschedule.
 *
 * @see #drawableStateChanged
 */
ECode View::UnscheduleDrawable(
    /* [in] */ IDrawable* who)
{
    if (mAttachInfo != NULL) {
        mAttachInfo->mHandler->RemoveCppCallbacksEx(NULL, NULL, Int32(who));
    }
    return NOERROR;
}

/**
 * If your view subclass is displaying its own Drawable objects, it should
 * override this function and return TRUE for any Drawable it is
 * displaying.  This allows animations for those drawables to be
 * scheduled.
 *
 * <p>Be sure to call through to the super class when overriding this
 * function.
 *
 * @param who The Drawable to verify.  Return TRUE if it is one you are
 *            displaying, else return the result of calling through to the
 *            super class.
 *
 * @return Boolean If TRUE than the Drawable is being displayed in the
 *         view; else FALSE and it is not allowed to animate.
 *
 * @see #unscheduleDrawable
 * @see #drawableStateChanged
 */
Boolean View::VerifyDrawable(
    /* [in] */ IDrawable* who)
{
    return who == mBGDrawable;
}

/**
 * This function is called whenever the state of the view changes in such
 * a way that it impacts the state of drawables being shown.
 *
 * <p>Be sure to call through to the superclass when overriding this
 * function.
 *
 * @see Drawable#setState
 */
ECode View::DrawableStateChanged()
{
    AutoPtr<IDrawable> d = mBGDrawable;
    if (d != NULL) {
        Boolean stateful;
        d->IsStateful(&stateful);
        if (stateful) {
            d->SetState(GetDrawableState(), &stateful);
        }
    }
    return NOERROR;
}

/**
 * Call this to force a view to update its drawable state. This will cause
 * drawableStateChanged to be called on this view. Views that are interested
 * in the new state should call getDrawableState.
 *
 * @see #drawableStateChanged
 * @see #getDrawableState
 */
ECode View::RefreshDrawableState()
{
    mPrivateFlags |= DRAWABLE_STATE_DIRTY;
    DrawableStateChanged();

    AutoPtr<IViewParent> parent = mParent;
    if (parent != NULL) {
        parent->ChildDrawableStateChanged((IView*)this->Probe(EIID_IView));
    }
    return NOERROR;
}

/**
 * Return an array of resource IDs of the drawable states representing the
 * current state of the view.
 *
 * @return The current drawable state
 *
 * @see Drawable#setState
 * @see #drawableStateChanged
 * @see #onCreateDrawableState
 */
ArrayOf<Int32>* View::GetDrawableState()
{
    if ((mDrawableState != NULL) && ((mPrivateFlags & DRAWABLE_STATE_DIRTY) == 0)) {
        return mDrawableState;
    }
    else {
        if (mDrawableState) {
            ArrayOf<Int32>::Free(mDrawableState);
            mDrawableState = NULL;
        }
        OnCreateDrawableState(0, &mDrawableState);
        mPrivateFlags &= ~DRAWABLE_STATE_DIRTY;
        return mDrawableState;
    }
}

/**
 * Generate the new {@link android.graphics.drawable.Drawable} state for
 * this view. This is called by the view
 * system when the cached Drawable state is determined to be invalid.  To
 * retrieve the current state, you should use {@link #getDrawableState}.
 *
 * @param extraSpace if non-zero, this is the number of extra entries you
 * would like in the returned array in which you can place your own
 * states.
 *
 * @return Returns an array holding the current {@link Drawable} state of
 * the view.
 *
 * @see #mergeDrawableStates
 */
ECode View::OnCreateDrawableState(
    /* [in] */ Int32 extraSpace,
    /* [out] */ ArrayOf<Int32>** _drawableState)
{
    if ((mViewFlags & DUPLICATE_PARENT_STATE) == DUPLICATE_PARENT_STATE &&
            mParent->Probe(EIID_IView) != NULL) {
        return ((View*)mParent->Probe(EIID_View))->OnCreateDrawableState(
                extraSpace, _drawableState);
    }

    Int32 privateFlags = mPrivateFlags;

    Int32 viewStateIndex = (((privateFlags & PRESSED) != 0) ? 1 : 0);

    viewStateIndex = (viewStateIndex << 1)
            + (((mViewFlags & ENABLED_MASK) == ENABLED) ? 1 : 0);

    viewStateIndex = (viewStateIndex << 1) + (IsFocused() ? 1 : 0);

    viewStateIndex = (viewStateIndex << 1)
            + (((privateFlags & SELECTED) != 0) ? 1 : 0);

    viewStateIndex = (viewStateIndex << 1) + (HasWindowFocus() ? 1 : 0);

    const ArrayOf<Int32>* drawableState = (*VIEW_STATE_SETS)[viewStateIndex];

    //noinspection ConstantIfStatement
//    if (FALSE) {
//        Log.i("View", "drawableStateIndex=" + viewStateIndex);
//        Log.i("View", toString()
//                + " pressed=" + ((privateFlags & PRESSED) != 0)
//                + " en=" + ((mViewFlags & ENABLED_MASK) == ENABLED)
//                + " fo=" + hasFocus()
//                + " sl=" + ((privateFlags & SELECTED) != 0)
//                + " wf=" + hasWindowFocus
//                + ": " + Arrays.toString(drawableState));
//    }

    if (extraSpace == 0) {
        *_drawableState = drawableState->Clone();
        return NOERROR;
    }

    ArrayOf<Int32>* fullState;
    if (drawableState != NULL) {
        const Int32 len = drawableState->GetLength();
        fullState = ArrayOf<Int32>::Alloc(len + extraSpace);
        for (Int32 i = 0; i < len; i++) {
            (*fullState)[i] = (*drawableState)[i];
        }
    } else {
        fullState = ArrayOf<Int32>::Alloc(extraSpace);
    }

    *_drawableState = fullState;
    return NOERROR;
}

/**
 * Merge your own state values in <var>additionalState</var> into the base
 * state values <var>baseState</var> that were returned by
 * {@link #onCreateDrawableState}.
 *
 * @param baseState The base state values returned by
 * {@link #onCreateDrawableState}, which will be modified to also hold your
 * own additional state values.
 *
 * @param additionalState The additional state values you would like
 * added to <var>baseState</var>; this array is not modified.
 *
 * @return As a convenience, the <var>baseState</var> array you originally
 * passed into the function is returned.
 *
 * @see #onCreateDrawableState
 */
ArrayOf<Int32>* View::MergeDrawableStates(
    /* [in] */ ArrayOf<Int32>* baseState,
    /* [in] */ ArrayOf<Int32>* additionalState)
{
    const Int32 N = baseState->GetLength();
    Int32 i = N - 1;
    while (i >= 0 && (*baseState)[i] == 0) {
        i--;
    }
    i++;
    const Int32 len = i + additionalState->GetLength() <= N?
            i + additionalState->GetLength() : N;
    for (Int32 j = 0; i < len; ++i, ++j) {
        (*baseState)[i] = (*additionalState)[j];
    }
    return baseState;
}

/**
 * Sets the background color for this view.
 * @param color the color of the background
 */
ECode View::SetBackgroundColor(
    /* [in] */ Int32 color)
{
    AutoPtr<IColorDrawable> colorDrawable;
    CColorDrawable::New(color, (IColorDrawable**)&colorDrawable);
    SetBackgroundDrawable(colorDrawable.Get());

    return NOERROR;
}

/**
 * Set the background to a given resource. The resource should refer to
 * a Drawable object or 0 to remove the background.
 * @param resid The identifier of the resource.
 * @attr ref android.R.styleable#View_background
 */
ECode View::SetBackgroundResource(
    /* [in] */ Int32 resid)
{
    if (resid != 0 && resid == mBackgroundResource) {
        return NOERROR;
    }

    AutoPtr<IDrawable> d;
    if (resid != 0) {
        mResources->GetDrawable(resid, (IDrawable**)&d);
    }
    SetBackgroundDrawable(d.Get());

    mBackgroundResource = resid;

    return NOERROR;
}

/**
 * Set the background to a given Drawable, or remove the background. If the
 * background has padding, this View's padding is set to the background's
 * padding. However, when a background is removed, this View's padding isn't
 * touched. If setting the padding is desired, please use
 * {@link #setPadding(int, int, int, int)}.
 *
 * @param d The Drawable to use as the background, or NULL to remove the
 *        background
 */
ECode View::SetBackgroundDrawable(
    /* [in] */ IDrawable* d)
{
    Boolean requestLayout = FALSE;

    mBackgroundResource = 0;

    /*
     * Regardless of whether we're setting a new background or not, we want
     * to clear the previous drawable.
     */
    if (mBGDrawable != NULL) {
        mBGDrawable->SetCallback(NULL);
        UnscheduleDrawable(mBGDrawable);
    }

    if (d != NULL) {
        AutoPtr<CRect> padding = (CRect*)pthread_getspecific(sKey);

        if (padding == NULL) {
            CRect::NewByFriend((CRect**)&padding);
            pthread_setspecific(sKey, padding.Get());
            padding->AddRef();
        }

        Boolean result;
        d->GetPadding((IRect*)padding.Get(), &result);
        if (result) {
            SetPadding(padding->mLeft, padding->mTop, padding->mRight, padding->mBottom);
        }

        // Compare the minimum sizes of the old Drawable and the new.  If there isn't an old or
        // if it has a different minimum size, we should layout again
        if (mBGDrawable == NULL) {
            requestLayout = TRUE;
        }
        else {
            Int32 bWidth, dWidth, bHeight, dHeight;
            mBGDrawable->GetMinimumHeight(&bHeight);
            mBGDrawable->GetMinimumWidth(&bWidth);
            d->GetMinimumHeight(&dHeight);
            d->GetMinimumWidth(&dWidth);
            if (bHeight != dHeight || bWidth != dWidth) {
                requestLayout = TRUE;
            }
        }

        d->SetCallback((IDrawableCallback*)this->Probe(EIID_IDrawableCallback));
        d->IsStateful(&result);
        if (result) {
            Boolean stateful;
            d->SetState(GetDrawableState(), &stateful);
        }
        d->SetVisible(GetVisibility() == VISIBLE, FALSE, &result);
        mBGDrawable = d;

        if ((mPrivateFlags & SKIP_DRAW) != 0) {
            mPrivateFlags &= ~SKIP_DRAW;
            mPrivateFlags |= ONLY_DRAWS_BACKGROUND;
            requestLayout = TRUE;
        }
    }
    else {
        /* Remove the background */
        mBGDrawable = NULL;

        if ((mPrivateFlags & ONLY_DRAWS_BACKGROUND) != 0) {
            /*
                * This view ONLY drew the background before and we're removing
                * the background, so now it won't draw anything
                * (hence we SKIP_DRAW)
                */
            mPrivateFlags &= ~ONLY_DRAWS_BACKGROUND;
            mPrivateFlags |= SKIP_DRAW;
        }

        /*
         * When the background is set, we try to apply its padding to this
         * View. When the background is removed, we don't touch this View's
         * padding. This is noted in the Javadocs. Hence, we don't need to
         * requestLayout(), the invalidate() below is sufficient.
         */

        // The old background's minimum size could have affected this
        // View's layout, so let's requestLayout
        requestLayout = TRUE;
    }

    ComputeOpaqueFlags();

    if (requestLayout) {
        RequestLayout();
    }

    mBackgroundSizeChanged = TRUE;
    Invalidate();

    return NOERROR;
}


/**
 * Gets the background drawable
 * @return The drawable used as the background for this view, if any.
 */
AutoPtr<IDrawable> View::GetBackground()
{
    return mBGDrawable;
}

/**
 * Sets the padding. The view may add on the space required to display
 * the scrollbars, depending on the style and visibility of the scrollbars.
 * So the values returned from {@link #getPaddingLeft}, {@link #getPaddingTop},
 * {@link #getPaddingRight} and {@link #getPaddingBottom} may be different
 * from the values set in this call.
 *
 * @attr ref android.R.styleable#View_padding
 * @attr ref android.R.styleable#View_paddingBottom
 * @attr ref android.R.styleable#View_paddingLeft
 * @attr ref android.R.styleable#View_paddingRight
 * @attr ref android.R.styleable#View_paddingTop
 * @param left the left padding in pixels
 * @param top the top padding in pixels
 * @param right the right padding in pixels
 * @param bottom the bottom padding in pixels
 */
ECode View::SetPadding(
    /* [in] */ Int32 left,
    /* [in] */ Int32 top,
    /* [in] */ Int32 right,
    /* [in] */ Int32 bottom)
{
    Boolean changed = FALSE;

    mUserPaddingRight = right;
    mUserPaddingBottom = bottom;

    Int32 viewFlags = mViewFlags;

    // Common case is there are no scroll bars.
    if ((viewFlags & (SCROLLBARS_VERTICAL|SCROLLBARS_HORIZONTAL)) != 0) {
        // TODO: Deal with RTL languages to adjust left padding instead of right.
        if ((viewFlags & SCROLLBARS_VERTICAL) != 0) {
            right += (viewFlags & SCROLLBARS_INSET_MASK) == 0
                    ? 0 : GetVerticalScrollbarWidth();
        }
        if ((viewFlags & SCROLLBARS_HORIZONTAL) == 0) {
            bottom += (viewFlags & SCROLLBARS_INSET_MASK) == 0
                    ? 0 : GetHorizontalScrollbarHeight();
        }
    }

    if (mPaddingLeft != left) {
        changed = TRUE;
        mPaddingLeft = left;
    }
    if (mPaddingTop != top) {
        changed = TRUE;
        mPaddingTop = top;
    }
    if (mPaddingRight != right) {
        changed = TRUE;
        mPaddingRight = right;
    }
    if (mPaddingBottom != bottom) {
        changed = TRUE;
        mPaddingBottom = bottom;
    }

    if (changed) {
        RequestLayout();
    }

    return NOERROR;
}

/**
 * Returns the top padding of this view.
 *
 * @return the top padding in pixels
 */
Int32 View::GetPaddingTop()
{
    return mPaddingTop;
}

/**
 * Returns the bottom padding of this view. If there are inset and enabled
 * scrollbars, this value may include the space required to display the
 * scrollbars as well.
 *
 * @return the bottom padding in pixels
 */
Int32 View::GetPaddingBottom()
{
    return mPaddingBottom;
}

/**
 * Returns the left padding of this view. If there are inset and enabled
 * scrollbars, this value may include the space required to display the
 * scrollbars as well.
 *
 * @return the left padding in pixels
 */
Int32 View::GetPaddingLeft()
{
    return mPaddingLeft;
}

/**
 * Returns the right padding of this view. If there are inset and enabled
 * scrollbars, this value may include the space required to display the
 * scrollbars as well.
 *
 * @return the right padding in pixels
 */
Int32 View::GetPaddingRight()
{
    return mPaddingRight;
}

/**
 * Changes the selection state of this view. A view can be selected or not.
 * Note that selection is not the same as focus. Views are typically
 * selected in the context of an AdapterView like ListView or GridView;
 * the selected view is the view that is highlighted.
 *
 * @param selected TRUE if the view must be selected, FALSE otherwise
 */
ECode View::SetSelected(
    /* [in] */ Boolean selected)
{
    if (((mPrivateFlags & SELECTED) != 0) != selected) {
        mPrivateFlags = (mPrivateFlags & ~SELECTED) | (selected ? SELECTED : 0);
        if (!selected) ResetPressedState();
        Invalidate();
        RefreshDrawableState();
        DispatchSetSelected(selected);
    }
    return NOERROR;
}

/**
 * Dispatch setSelected to all of this View's children.
 *
 * @see #setSelected(Boolean)
 *
 * @param selected The new selected state
 */
void View::DispatchSetSelected(
    /* [in] */ Boolean selected)
{
}

/**
 * Indicates the selection state of this view.
 *
 * @return TRUE if the view is selected, FALSE otherwise
 */
Boolean View::IsSelected()
{
    return (mPrivateFlags & SELECTED) != 0;
}

/**
 * Returns the ViewTreeObserver for this view's hierarchy. The view tree
 * observer can be used to get notifications when global events, like
 * layout, happen.
 *
 * The returned ViewTreeObserver observer is not guaranteed to remain
 * valid for the lifetime of this View. If the caller of this method keeps
 * a long-lived reference to ViewTreeObserver, it should always check for
 * the return value of {@link ViewTreeObserver#isAlive()}.
 *
 * @return The ViewTreeObserver for this view's hierarchy.
 */
AutoPtr<IViewTreeObserver> View::GetViewTreeObserver()
{
    if (mAttachInfo != NULL) {
        return mAttachInfo->mTreeObserver;
    }
    if (mFloatingTreeObserver == NULL) {
        mFloatingTreeObserver = new ViewTreeObserver();
    }
    return mFloatingTreeObserver;
}

/**
 * <p>Finds the topmost view in the current view hierarchy.</p>
 *
 * @return the topmost view containing this view
 */
AutoPtr<IView> View::GetRootView()
{
    if (mAttachInfo != NULL) {
        AutoPtr<IView> v = mAttachInfo->mRootView.Get();
        if (v != NULL) {
            return v;
        }
    }

    AutoPtr<IView> me;
    AutoPtr<IViewParent> parent;
    me = (IView*)this->Probe(EIID_IView);
    me->GetParent((IViewParent**)&parent);
    while (parent != NULL && parent->Probe(EIID_IView) != NULL) {
        me = (IView*)parent->Probe(EIID_IView);
        if (parent != NULL) parent->Release();
        me->GetParent((IViewParent**)&parent);
    }

    return me;
}

/**
 * <p>Computes the coordinates of this view on the screen. The argument
 * must be an array of two integers. After the method returns, the array
 * contains the x and y location in that order.</p>
 *
 * @param location an array of two integers in which to hold the coordinates
 */
ECode View::GetLocationOnScreen(
    /* [out] */ Int32* x,
    /* [out] */ Int32* y)
{
    GetLocationInWindow(x, y);

    if (mAttachInfo != NULL) {
        *x += mAttachInfo->mWindowLeft;
        *y += mAttachInfo->mWindowTop;
    }

    return NOERROR;
}

/**
 * <p>Computes the coordinates of this view in its window. The argument
 * must be an array of two integers. After the method returns, the array
 * contains the x and y location in that order.</p>
 *
 * @param location an array of two integers in which to hold the coordinates
 */
ECode View::GetLocationInWindow(
    /* [out] */ Int32* x,
    /* [out] */ Int32* y)
{
    VALIDATE_NOT_NULL(x);
    VALIDATE_NOT_NULL(y);

    *x = mLeft;
    *y = mTop;

    AutoPtr<IViewParent> viewParent = mParent;
    while (viewParent && viewParent->Probe(EIID_View) != NULL) {
        View* view = (View*)viewParent->Probe(EIID_View);
        *x += view->mLeft - view->mScrollX;
        *y += view->mTop - view->mScrollY;
        viewParent = view->mParent;
    }

    if (viewParent) {
        ViewRoot* viewRoot = (ViewRoot*)viewParent->Probe(EIID_ViewRoot);
        if (viewRoot) {
            // *cough*
            *y -= viewRoot->mCurScrollY;
        }
    }

    return NOERROR;
}

/**
 * {@hide}
 * @param id the id of the view to be found
 * @return the view of the specified id, NULL if cannot be found
 */
AutoPtr<IView> View::FindViewTraversal(
    /* [in] */ Int32 id)
{
    if (id == mID) {
        return AutoPtr<IView>((IView*)this->Probe(EIID_IView));
    }
    return AutoPtr<IView>(NULL);
}

/**
 * {@hide}
 * @param tag the tag of the view to be found
 * @return the view of specified tag, NULL if cannot be found
 */
AutoPtr<IView> View::FindViewWithTagTraversal(
    /* [in] */ IInterface* tag)
{
    if (tag != NULL && tag == mTag.Get()) {
        return AutoPtr<IView>((IView*)this->Probe(EIID_IView));
    }
    return AutoPtr<IView>(NULL);
}

/**
 * Look for a child view with the given id.  If this view has the given
 * id, return this view.
 *
 * @param id The id to search for.
 * @return The view that has the given id in the hierarchy or NULL
 */
AutoPtr<IView> View::FindViewById(
    /* [in] */ Int32 id)
{
    if (id < 0) {
        return NULL;
    }
    return FindViewTraversal(id);
}

/**
    * Look for a child view with the given tag.  If this view has the given
    * tag, return this view.
    *
    * @param tag The tag to search for, using "tag.equals(getTag())".
    * @return The View that has the given tag in the hierarchy or NULL
    */
AutoPtr<IView> View::FindViewWithTag(
    /* [in] */ IInterface* tag)
{
    if (tag == NULL) {
        return AutoPtr<IView>(NULL);
    }
    return FindViewWithTagTraversal(tag);
}

/**
 * Sets the identifier for this view. The identifier does not have to be
 * unique in this view's hierarchy. The identifier should be a positive
 * number.
 *
 * @see #NO_ID
 * @see #getId
 * @see #findViewById
 *
 * @param id a number used to identify the view
 *
 * @attr ref android.R.styleable#View_id
 */
ECode View::SetId(
    /* [in] */ Int32 id)
{
    mID = id;

    return NOERROR;
}

/**
 * {@hide}
 *
 * @param isRoot TRUE if the view belongs to the root namespace, FALSE
 *        otherwise
 */
ECode View::SetIsRootNamespace(
    /* [in] */ Boolean isRoot)
{
    if (isRoot) {
        mPrivateFlags |= IS_ROOT_NAMESPACE;
    }
    else {
        mPrivateFlags &= ~IS_ROOT_NAMESPACE;
    }
    return NOERROR;
}

/**
 * {@hide}
 *
 * @return TRUE if the view belongs to the root namespace, FALSE otherwise
 */
Boolean View::IsRootNamespace()
{
    return (mPrivateFlags & IS_ROOT_NAMESPACE) != 0;
}

/**
 * Returns this view's identifier.
 *
 * @return a positive integer used to identify the view or {@link #NO_ID}
 *         if the view has no ID
 *
 * @see #setId
 * @see #findViewById
 * @attr ref android.R.styleable#View_id
 */
Int32 View::GetId()
{
    return mID;
}

/**
 * Returns this view's tag.
 *
 * @return the Object stored in this view as a tag
 *
 * @see #setTag(Object)
 * @see #getTag(int)
 */
AutoPtr<IInterface> View::GetTag()
{
    return mTag;
}

/**
 * Sets the tag associated with this view. A tag can be used to mark
 * a view in its hierarchy and does not have to be unique within the
 * hierarchy. Tags can also be used to store data within a view without
 * resorting to another data structure.
 *
 * @param tag an Object to tag the view with
 *
 * @see #getTag()
 * @see #setTag(int, Object)
 */
ECode View::SetTag(
    /* [in] */ IInterface* tag)
{
    mTag = tag;
    return NOERROR;
}

/**
 * Returns the tag associated with this view and the specified key.
 *
 * @param key The key identifying the tag
 *
 * @return the Object stored in this view as a tag
 *
 * @see #setTag(int, Object)
 * @see #getTag()
 */
//IObject* View::GetTag(
//    /* [in] */ Int32 key)
//{
//    SparseArray<Object> tags = NULL;
//    synchronized (sTagsLock) {
//        if (sTags != NULL) {
//            tags = sTags.get(this);
//        }
//    }
//
//    if (tags != NULL) return tags.get(key);
//    return NULL;
//}

/**
 * Sets a tag associated with this view and a key. A tag can be used
 * to mark a view in its hierarchy and does not have to be unique within
 * the hierarchy. Tags can also be used to store data within a view
 * without resorting to another data structure.
 *
 * The specified key should be an id declared in the resources of the
 * application to ensure it is unique (see the <a
 * href={@docRoot}guide/topics/resources/more-resources.html#Id">ID resource type</a>).
 * Keys identified as belonging to
 * the Android framework or not associated with any package will cause
 * an {@link IllegalArgumentException} to be thrown.
 *
 * @param key The key identifying the tag
 * @param tag An Object to tag the view with
 *
 * @throws IllegalArgumentException If they specified key is not valid
 *
 * @see #setTag(Object)
 * @see #getTag(int)
 */
//void View::SetTag(
//    /* [in] */ Int32 key,
//    /* [in] */ IObject* tag)
//{
//    // If the package id is 0x00 or 0x01, it's either an undefined package
//    // or a framework id
//    if ((key >> 24) < 2) {
//        return;
//        //throw new IllegalArgumentException("The key must be an application-specific "
//          //      + "resource id.");
//    }
//
//    SetTagInternal(this, key, tag);
//}

/**
 * Variation of {@link #setTag(int, Object)} that enforces the key to be a
 * framework id.
 *
 * @hide
 */
//void View::SetTagInternal(
//    /* [in] */ Int32 key,
//    /* [in] */ IObject* tag)
//{
//    if ((key >>> 24) != 0x1) {
//        return;
//        //throw new IllegalArgumentException("The key must be a framework-specific "
//          //      + "resource id.");
//    }
//
//    SetTagInternal(this, key, tag);
//}

//static void SetTagInternal(
//    /* [in] */ IView* view,
//    /* [in] */ Int32 key,
//    /* [in] */ IObject* tag)
//{
//    SparseArray<Object> tags = NULL;
//    synchronized (sTagsLock) {
//        if (sTags == NULL) {
//            sTags = new WeakHashMap<View, SparseArray<Object>>();
//        } else {
//            tags = sTags.get(view);
//        }
//    }
//
//    if (tags == NULL) {
//        tags = new SparseArray<Object>(2);
//        synchronized (sTagsLock) {
//            sTags.put(view, tags);
//        }
//    }
//
//    tags.put(key, tag);
//}

/**
 * @param consistency The type of consistency. See ViewDebug for more information.
 *
 * @hide
 */
Boolean View::DispatchConsistencyCheck(
    /* [in] */ Int32 consistency)
{
    return OnConsistencyCheck(consistency);
}

/**
 * Method that subclasses should implement to check their consistency. The type of
 * consistency check is indicated by the bit field passed as a parameter.
 *
 * @param consistency The type of consistency. See ViewDebug for more information.
 *
 * @throws IllegalStateException if the view is in an inconsistent state.
 *
 * @hide
 */
Boolean View::OnConsistencyCheck(
    /* [in] */ Int32 consistency)
{
    Boolean result = TRUE;

    Boolean checkLayout = FALSE/*= (consistency & ViewDebug::CONSISTENCY_LAYOUT) != 0*/;
    Boolean checkDrawing = FALSE/*= (consistency & ViewDebug::CONSISTENCY_DRAWING) != 0*/;

    if (checkLayout) {
        if (GetParent() == NULL) {
            result = FALSE;
            //android.util.Log.d(ViewDebug.CONSISTENCY_LOG_TAG,
              //      "View " + this + " does not have a parent.");
        }

        if (mAttachInfo == NULL) {
            result = FALSE;
            //android.util.Log.d(ViewDebug.CONSISTENCY_LOG_TAG,
              //      "View " + this + " is not attached to a window.");
        }
    }

    if (checkDrawing) {
        // Do not check the DIRTY/DRAWN flags because views can call invalidate()
        // from their draw() method

        if ((mPrivateFlags & DRAWN) != DRAWN &&
                (mPrivateFlags & DRAWING_CACHE_VALID) == DRAWING_CACHE_VALID) {
            result = FALSE;
            //android.util.Log.d(ViewDebug.CONSISTENCY_LOG_TAG,
              //      "View " + this + " was invalidated but its drawing cache is valid.");
        }
    }

    return result;
}

/**
 * Prints information about this view in the log output, with the tag
 * {@link #VIEW_LOG_TAG}.
 *
 * @hide
 */
void View::Debug()
{
    //Debug(0);
}

/**
 * Prints information about this view in the log output, with the tag
 * {@link #VIEW_LOG_TAG}. Each line in the output is preceded with an
 * indentation defined by the <code>depth</code>.
 *
 * @param depth the indentation level
 *
 * @hide
 */
//void View::Debug(
//    /* [in] */ int32 depth)
//{
//    String output = debugIndent(depth - 1);
//
//    output += "+ " + this;
//    int id = getId();
//    if (id != -1) {
//        output += " (id=" + id + ")";
//    }
//    Object tag = getTag();
//    if (tag != NULL) {
//        output += " (tag=" + tag + ")";
//    }
//    Log.d(VIEW_LOG_TAG, output);
//
//    if ((mPrivateFlags & FOCUSED) != 0) {
//        output = debugIndent(depth) + " FOCUSED";
//        Log.d(VIEW_LOG_TAG, output);
//    }
//
//    output = debugIndent(depth);
//    output += "frame={" + mLeft + ", " + mTop + ", " + mRight
//            + ", " + mBottom + "} scroll={" + mScrollX + ", " + mScrollY
//            + "} ";
//    Log.d(VIEW_LOG_TAG, output);
//
//    if (mPaddingLeft != 0 || mPaddingTop != 0 || mPaddingRight != 0
//            || mPaddingBottom != 0) {
//        output = debugIndent(depth);
//        output += "padding={" + mPaddingLeft + ", " + mPaddingTop
//                + ", " + mPaddingRight + ", " + mPaddingBottom + "}";
//        Log.d(VIEW_LOG_TAG, output);
//    }
//
//    output = debugIndent(depth);
//    output += "mMeasureWidth=" + mMeasuredWidth +
//            " mMeasureHeight=" + mMeasuredHeight;
//    Log.d(VIEW_LOG_TAG, output);
//
//    output = debugIndent(depth);
//    if (mLayoutParams == NULL) {
//        output += "BAD! no layout params";
//    } else {
//        output = mLayoutParams.debug(output);
//    }
//    Log.d(VIEW_LOG_TAG, output);
//
//    output = debugIndent(depth);
//    output += "flags={";
//    output += View.printFlags(mViewFlags);
//    output += "}";
//    Log.d(VIEW_LOG_TAG, output);
//
//    output = debugIndent(depth);
//    output += "privateFlags={";
//    output += View.printPrivateFlags(mPrivateFlags);
//    output += "}";
//    Log.d(VIEW_LOG_TAG, output);
//}

/**
 * Creates an string of whitespaces used for indentation.
 *
 * @param depth the indentation level
 * @return a String containing (depth * 2 + 3) * 2 white spaces
 *
 * @hide
 */
//static String debugIndent(
//    /* [in] */ Int32 depth)
//{
//    StringBuilder spaces = new StringBuilder((depth * 2 + 3) * 2);
//    for (int i = 0; i < (depth * 2) + 3; i++) {
//        spaces.append(' ').append(' ');
//    }
//    return spaces.toString();
//}

/**
 * <p>Return the offset of the widget's text baseline from the widget's top
 * boundary. If this widget does not support baseline alignment, this
 * method returns -1. </p>
 *
 * @return the offset of the baseline within the widget's bounds or -1
 *         if baseline alignment is not supported
 */
ECode View::GetBaseline(
    /* [out] */ Int32* baseline)
{
    assert(baseline);
    *baseline = -1;

    return NOERROR;
}

/**
 * Call this when something has changed which has invalidated the
 * layout of this view. This will schedule a layout pass of the view
 * tree.
 */
ECode View::RequestLayout()
{
//    if (ViewDebug.TRACE_HIERARCHY) {
//        ViewDebug.trace(this, ViewDebug.HierarchyTraceType.REQUEST_LAYOUT);
//    }
    mPrivateFlags |= FORCE_LAYOUT;

    if (mParent != NULL) {
        Boolean requested = FALSE;
        mParent->IsLayoutRequestedEx(&requested);
        if (!requested) {
            mParent->RequestLayoutEx();
        }
    }

    return NOERROR;
}

/**
 * Forces this view to be laid out during the next layout pass.
 * This method does not call requestLayout() or forceLayout()
 * on the parent.
 */
ECode View::ForceLayout()
{
    mPrivateFlags |= FORCE_LAYOUT;
    return NOERROR;
}

/**
 * <p>
 * This is called to find out how big a view should be. The parent
 * supplies constraint information in the width and height parameters.
 * </p>
 *
 * <p>
 * The actual mesurement work of a view is performed in
 * {@link #onMeasure(int, int)}, called by this method. Therefore, only
 * {@link #onMeasure(int, int)} can and must be overriden by subclasses.
 * </p>
 *
 *
 * @param widthMeasureSpec Horizontal space requirements as imposed by the
 *        parent
 * @param heightMeasureSpec Vertical space requirements as imposed by the
 *        parent
 *
 * @see #onMeasure(int, int)
 */
ECode View::Measure(
    /* [in] */ Int32 widthMeasureSpec,
    /* [in] */ Int32 heightMeasureSpec)
{
    if ((mPrivateFlags & FORCE_LAYOUT) == FORCE_LAYOUT ||
            widthMeasureSpec != mOldWidthMeasureSpec ||
            heightMeasureSpec != mOldHeightMeasureSpec) {

        // first clears the measured dimension flag
        mPrivateFlags &= ~MEASURED_DIMENSION_SET;

        /*if (ViewDebug::TRACE_HIERARCHY) {
            ViewDebug::Trace(this, ViewDebug::HierarchyTraceType::ON_MEASURE);
        }*/

        // measure ourselves, this should set the measured dimension flag back
        OnMeasure(widthMeasureSpec, heightMeasureSpec);

        // flag not set, setMeasuredDimension() was not invoked, we raise
        // an exception to warn the developer
        if ((mPrivateFlags & MEASURED_DIMENSION_SET) != MEASURED_DIMENSION_SET) {
            //throw new IllegalStateException("onMeasure() did not set the"
              //      + " measured dimension by calling"
                //    + " setMeasuredDimension()");
            return E_ILLEGAL_STATE_EXCEPTION;
        }

        mPrivateFlags |= LAYOUT_REQUIRED;
    }

    mOldWidthMeasureSpec = widthMeasureSpec;
    mOldHeightMeasureSpec = heightMeasureSpec;

    return NOERROR;
}

/**
 * <p>
 * Measure the view and its content to determine the measured width and the
 * measured height. This method is invoked by {@link #measure(int, int)} and
 * should be overriden by subclasses to provide accurate and efficient
 * measurement of their contents.
 * </p>
 *
 * <p>
 * <strong>CONTRACT:</strong> When overriding this method, you
 * <em>must</em> call {@link #setMeasuredDimension(int, int)} to store the
 * measured width and height of this view. Failure to do so will trigger an
 * <code>IllegalStateException</code>, thrown by
 * {@link #measure(int, int)}. Calling the superclass'
 * {@link #onMeasure(int, int)} is a valid use.
 * </p>
 *
 * <p>
 * The base class implementation of measure defaults to the background size,
 * unless a larger size is allowed by the MeasureSpec. Subclasses should
 * override {@link #onMeasure(int, int)} to provide better measurements of
 * their content.
 * </p>
 *
 * <p>
 * If this method is overridden, it is the subclass's responsibility to make
 * sure the measured height and width are at least the view's minimum height
 * and width ({@link #getSuggestedMinimumHeight()} and
 * {@link #getSuggestedMinimumWidth()}).
 * </p>
 *
 * @param widthMeasureSpec horizontal space requirements as imposed by the parent.
 *                         The requirements are encoded with
 *                         {@link android.view.View.MeasureSpec}.
 * @param heightMeasureSpec vertical space requirements as imposed by the parent.
 *                         The requirements are encoded with
 *                         {@link android.view.View.MeasureSpec}.
 *
 * @see #getMeasuredWidth()
 * @see #getMeasuredHeight()
 * @see #setMeasuredDimension(int, int)
 * @see #getSuggestedMinimumHeight()
 * @see #getSuggestedMinimumWidth()
 * @see android.view.View.MeasureSpec#getMode(int)
 * @see android.view.View.MeasureSpec#getSize(int)
 */
void View::OnMeasure(
    /* [in] */ Int32 widthMeasureSpec,
    /* [in] */ Int32 heightMeasureSpec)
{
    SetMeasuredDimension(GetDefaultSize(GetSuggestedMinimumWidth(), widthMeasureSpec),
        GetDefaultSize(GetSuggestedMinimumHeight(), heightMeasureSpec));
}

/**
 * <p>This mehod must be called by {@link #onMeasure(Int32, Int32)} to store the
 * measured width and measured height. Failing to do so will trigger an
 * exception at measurement time.</p>
 *
 * @param measuredWidth the measured width of this view
 * @param measuredHeight the measured height of this view
 */
void View::SetMeasuredDimension(
    /* [in] */ Int32 measuredWidth,
    /* [in] */ Int32 measuredHeight)
{
    mMeasuredWidth = measuredWidth;
    mMeasuredHeight = measuredHeight;

    mPrivateFlags |= MEASURED_DIMENSION_SET;
}

/**
 * Utility to reconcile a desired size with constraints imposed by a MeasureSpec.
 * Will take the desired size, unless a different size is imposed by the constraints.
 *
 * @param size How big the view wants to be
 * @param measureSpec Constraints imposed by the parent
 * @return The size this view should be.
 */
Int32 View::ResolveSize(
    /* [in] */ Int32 size,
    /* [in] */ Int32 measureSpec)
{
    Int32 result = size;
    Int32 specMode = MeasureSpec::GetMode(measureSpec);
    Int32 specSize =  MeasureSpec::GetSize(measureSpec);
    switch (specMode) {
    case MeasureSpec::UNSPECIFIED:
        result = size;
        break;
    case MeasureSpec::AT_MOST:
        result = Math::Min(size, specSize);
        break;
    case MeasureSpec::EXACTLY:
        result = specSize;
        break;
    }
    return result;
}

/**
 * Utility to return a default size. Uses the supplied size if the
 * MeasureSpec imposed no contraints. Will get larger if allowed
 * by the MeasureSpec.
 *
 * @param size Default size for this view
 * @param measureSpec Constraints imposed by the parent
 * @return The size this view should be.
 */
Int32 View::GetDefaultSize(
    /* [in] */ Int32 size,
    /* [in] */ Int32 measureSpec)
{
    Int32 result = size;
    Int32 specMode = MeasureSpec::GetMode(measureSpec);
    Int32 specSize =  MeasureSpec::GetSize(measureSpec);

    switch (specMode) {
    case MeasureSpec::UNSPECIFIED:
        result = size;
        break;
    case MeasureSpec::AT_MOST:
    case MeasureSpec::EXACTLY:
        result = specSize;
        break;
    }

    return result;
}

/**
 * Returns the suggested minimum height that the view should use. This
 * returns the maximum of the view's minimum height
 * and the background's minimum height
 * ({@link android.graphics.drawable.Drawable#getMinimumHeight()}).
 * <p>
 * When being used in {@link #onMeasure(Int32, Int32)}, the caller should still
 * ensure the returned height is within the requirements of the parent.
 *
 * @return The suggested minimum height of the view.
 */
Int32 View::GetSuggestedMinimumHeight()
{
    Int32 suggestedMinHeight = mMinHeight;

    if (mBGDrawable != NULL) {
        Int32 bgMinHeight;
        mBGDrawable->GetMinimumHeight(&bgMinHeight);
        if (suggestedMinHeight < bgMinHeight) {
            suggestedMinHeight = bgMinHeight;
        }
    }

    return suggestedMinHeight;
}

/**
 * Returns the suggested minimum width that the view should use. This
 * returns the maximum of the view's minimum width)
 * and the background's minimum width
 *  ({@link android.graphics.drawable.Drawable#getMinimumWidth()}).
 * <p>
 * When being used in {@link #onMeasure(Int32, Int32)}, the caller should still
 * ensure the returned width is within the requirements of the parent.
 *
 * @return The suggested minimum width of the view.
 */
Int32 View::GetSuggestedMinimumWidth()
{
    Int32 suggestedMinWidth = mMinWidth;

    if (mBGDrawable != NULL) {
        Int32 bgMinWidth;
        mBGDrawable->GetMinimumWidth(&bgMinWidth);
        if (suggestedMinWidth < bgMinWidth) {
            suggestedMinWidth = bgMinWidth;
        }
    }

    return suggestedMinWidth;
}

/**
 * Sets the minimum height of the view. It is not guaranteed the view will
 * be able to achieve this minimum height (for example, if its parent layout
 * constrains it with less available height).
 *
 * @param minHeight The minimum height the view will try to be.
 */
ECode View::SetMinimumHeight(
    /* [in] */ Int32 minHeight)
{
    mMinHeight = minHeight;

    return NOERROR;
}

/**
 * Sets the minimum width of the view. It is not guaranteed the view will
 * be able to achieve this minimum width (for example, if its parent layout
 * constrains it with less available width).
 *
 * @param minWidth The minimum width the view will try to be.
 */
ECode View::SetMinimumWidth(
    /* [in] */ Int32 minWidth)
{
    mMinWidth = minWidth;

    return NOERROR;
}


/**
 * Get the animation currently associated with this view.
 *
 * @return The animation that is currently playing or
 *         scheduled to play for this view.
 */
AutoPtr<IAnimation> View::GetAnimation()
{
    return mCurrentAnimation;
}

/**
 * Start the specified animation now.
 *
 * @param animation the animation to start now
 */
ECode View::StartAnimation(
    /* [in] */ IAnimation* animation)
{
    animation->SetStartTime(Animation_START_ON_FIRST_FRAME);
    SetAnimation(animation);
    Invalidate();

    return NOERROR;
}

/**
 * Cancels any animations for this view.
 */
ECode View::ClearAnimation()
{
    if (mCurrentAnimation != NULL) {
        ((Animation*)mCurrentAnimation->Probe(EIID_Animation))->Detach();
    }
    mCurrentAnimation = NULL;

    return NOERROR;
}

/**
 * Sets the next animation to play for this view.
 * If you want the animation to play immediately, use
 * startAnimation. This method provides allows fine-grained
 * control over the start time and invalidation, but you
 * must make sure that 1) the animation has a start time set, and
 * 2) the view will be invalidated when the animation is supposed to
 * start.
 *
 * @param animation The next animation, or NULL.
 */
ECode View::SetAnimation(
    /* [in] */ IAnimation* animation)
{
    mCurrentAnimation = animation;
    if (animation != NULL) {
        animation->Reset();
    }

    return NOERROR;
}

/**
 * Invoked by a parent ViewGroup to notify the start of the animation
 * currently associated with this view. If you override this method,
 * always call super.onAnimationStart();
 *
 * @see #setAnimation(android.view.animation.Animation)
 * @see #getAnimation()
 */
void View::OnAnimationStart()
{
    mPrivateFlags |= ANIMATION_STARTED;
}

/**
 * Invoked by a parent ViewGroup to notify the end of the animation
 * currently associated with this view. If you override this method,
 * always call super.onAnimationEnd();
 *
 * @see #setAnimation(android.view.animation.Animation)
 * @see #getAnimation()
 */
void View::OnAnimationEnd()
{
    mPrivateFlags &= ~ANIMATION_STARTED;
}

/**
 * Invoked if there is a Transform that involves alpha. Subclass that can
 * draw themselves with the specified alpha should return TRUE, and then
 * respect that alpha when their onDraw() is called. If this returns FALSE
 * then the view may be redirected to draw into an offscreen buffer to
 * fulfill the request, which will look fine, but may be slower than if the
 * subclass handles it internally. The default implementation returns FALSE.
 *
 * @param alpha The alpha (0..255) to apply to the view's drawing
 * @return TRUE if the view can draw with the specified alpha.
 */
Boolean View::OnSetAlpha(
    /* [in] */ Int32 alpha)
{
    return FALSE;
}

/**
 * This is used by the RootView to perform an optimization when
 * the view hierarchy contains one or several SurfaceView.
 * SurfaceView is always considered transparent, but its children are not,
 * therefore all View objects remove themselves from the global transparent
 * region (passed as a parameter to this function).
 *
 * @param region The transparent region for this ViewRoot (window).
 *
 * @return Returns TRUE if the effective visibility of the view at this
 * point is opaque, regardless of the transparent region; returns FALSE
 * if it is possible for underlying windows to be seen behind the view.
 *
 * {@hide}
 */
Boolean View::GatherTransparentRegion(
    /* [in] */ IRegion* region)
{
    AttachInfo* attachInfo = mAttachInfo;
    if (region != NULL && attachInfo != NULL) {
        Int32 pflags = mPrivateFlags;
        if ((pflags & SKIP_DRAW) == 0) {
            // The SKIP_DRAW flag IS NOT set, so this view draws. We need to
            // remove it from the transparent region.
            Int32* location = attachInfo->mTransparentLocation;
            GetLocationInWindow(location, location + 1);
            Boolean result;
            region->OpEx(location[0], location[1], location[0] + mRight - mLeft,
                    location[1] + mBottom - mTop, RegionOp_DIFFERENCE, &result);
        }
        else if ((pflags & ONLY_DRAWS_BACKGROUND) != 0 && mBGDrawable != NULL) {
            // The ONLY_DRAWS_BACKGROUND flag IS set and the background drawable
            // exists, so we remove the background drawable's non-transparent
            // parts from this transparent region.
            ApplyDrawableToTransparentRegion(mBGDrawable, region);
        }
    }
    return TRUE;
}

/**
 * Play a sound effect for this view.
 *
 * <p>The framework will play sound effects for some built in actions, such as
 * clicking, but you may wish to play these effects in your widget,
 * for instance, for internal navigation.
 *
 * <p>The sound effect will only be played if sound effects are enabled by the user, and
 * {@link #isSoundEffectsEnabled()} is TRUE.
 *
 * @param soundConstant One of the constants defined in {@link SoundEffectConstants}
 */
ECode View::PlaySoundEffect(
    /* [in] */ Int32 soundConstant)
{
    if (mAttachInfo == NULL || mAttachInfo->mRootCallbacks == NULL || !IsSoundEffectsEnabled()) {
        return NOERROR;
    }
    mAttachInfo->mRootCallbacks->PlaySoundEffect(soundConstant);

    return NOERROR;
}

/**
 * BZZZTT!!1!
 *
 * <p>Provide haptic feedback to the user for this view.
 *
 * <p>The framework will provide haptic feedback for some built in actions,
 * such as long presses, but you may wish to provide feedback for your
 * own widget.
 *
 * <p>The feedback will only be performed if
 * {@link #isHapticFeedbackEnabled()} is TRUE.
 *
 * @param feedbackConstant One of the constants defined in
 * {@link HapticFeedbackConstants}
 */
Boolean View::PerformHapticFeedback(
    /* [in] */ Int32 feedbackConstant)
{
    return PerformHapticFeedback(feedbackConstant, 0);
}

/**
 * BZZZTT!!1!
 *
 * <p>Like {@link #performHapticFeedback(int)}, with additional options.
 *
 * @param feedbackConstant One of the constants defined in
 * {@link HapticFeedbackConstants}
 * @param flags Additional flags as per {@link HapticFeedbackConstants}.
 */
Boolean View::PerformHapticFeedback(
    /* [in] */ Int32 feedbackConstant,
    /* [in] */ Int32 flags)
{
    if (mAttachInfo == NULL) {
        return FALSE;
    }
    if ((flags & HapticFeedbackConstants_FLAG_IGNORE_VIEW_SETTING) == 0
            && !IsHapticFeedbackEnabled()) {
        return FALSE;
    }

    Boolean result;

    return mAttachInfo->mRootCallbacks->PerformHapticFeedback(
            feedbackConstant,
            (flags & HapticFeedbackConstants_FLAG_IGNORE_GLOBAL_SETTING) != 0, &result);
}

/**
 * This needs to be a better API (NOT ON VIEW) before it is exposed.  If
 * it is ever exposed at all.
 * @hide
 */
ECode View::OnCloseSystemDialogs(
    /* [in] */ const String& reason)
{
    return NOERROR;
}

/**
 * Given a Drawable whose bounds have been set to draw into this view,
 * update a Region being computed for {@link #gatherTransparentRegion} so
 * that any non-transparent parts of the Drawable are removed from the
 * given transparent region.
 *
 * @param dr The Drawable whose transparency is to be applied to the region.
 * @param region A Region holding the current transparency information,
 * where any parts of the region that are set are considered to be
 * transparent.  On return, this region will be modified to have the
 * transparency information reduced by the corresponding parts of the
 * Drawable that are not transparent.
 * {@hide}
 */
ECode View::ApplyDrawableToTransparentRegion(
    /* [in] */ IDrawable* dr,
    /* [in] */ IRegion* region)
{
    if (DBG) {
        //Log.i("View", "Getting transparent region for: " + this);
    }
    AutoPtr<IRegion> r;
    dr->GetTransparentRegion((IRegion**)&r);
    AutoPtr<IRect> _db;
    dr->GetBounds((IRect**)&_db);
    AttachInfo* attachInfo = mAttachInfo;
    Boolean result;
    if (r != NULL && attachInfo != NULL) {
        Int32 w = GetRight() - GetLeft();
        Int32 h = GetBottom() - GetTop();
        CRect* db = (CRect*)_db.Get();
        if (db->mLeft > 0) {
            //Log.i("VIEW", "Drawable left " + db.left + " > view 0");
            r->OpEx(0, 0, db->mLeft, h, RegionOp_UNION, &result);
        }
        if (db->mRight < w) {
            //Log.i("VIEW", "Drawable right " + db.right + " < view " + w);
            r->OpEx(db->mRight, 0, w, h, RegionOp_UNION, &result);
        }
        if (db->mTop > 0) {
            //Log.i("VIEW", "Drawable top " + db.top + " > view 0");
            r->OpEx(0, 0, w, db->mTop, RegionOp_UNION, &result);
        }
        if (db->mBottom < h) {
            //Log.i("VIEW", "Drawable bottom " + db.bottom + " < view " + h);
            r->OpEx(0, db->mBottom, w, h, RegionOp_UNION, &result);
        }
        Int32* location = attachInfo->mTransparentLocation;
        GetLocationInWindow(location, location + 1);
        r->Translate(location[0], location[1]);
        region->OpEx2(r, RegionOp_INTERSECT, &result);
    }
    else {
        region->Op(_db, RegionOp_DIFFERENCE, &result);
    }

    return NOERROR;
}

void View::PostCheckForLongClick(
    /* [in] */ Int32 delayOffset)
{
    mHasPerformedLongPress = FALSE;

    if (mPendingCheckForLongPress == NULL) {
        AutoPtr<IView> view = AutoPtr<IView>((IView*)this->Probe(EIID_IView));
        mPendingCheckForLongPress = new CheckForLongPress(view);
    }
    mPendingCheckForLongPress->RememberWindowAttachCount();
    PostDelayed(mPendingCheckForLongPress,
            ViewConfiguration::GetLongPressTimeout() - delayOffset);
}

static Int32 R_Styleable_ViewDrawableStates[] = {
    0x0101009c, 0x0101009d, 0x0101009e, 0x010100a1,
    0x010100a7
};

ECode View::StateSetUnion(
    /* [in] */ ArrayOf<Int32>* stateSet1,
    /* [in] */ ArrayOf<Int32>* stateSet2,
    /* [out] */ ArrayOf<Int32>** newSet)
{
    assert(newSet != NULL);
    Int32 stateSet1Length = stateSet1->GetLength();
    Int32 stateSet2Length = stateSet2->GetLength();
    *newSet = ArrayOf<Int32>::Alloc(stateSet1Length + stateSet2Length);
    Int32 k = 0;
    Int32 i = 0;
    Int32 j = 0;
    // This is a merge of the two input state sets and assumes that the
    // input sets are sorted by the order imposed by ViewDrawableStates.
    for (Int32 m = 0; m < 5; m++) {
        Int32 viewState = R_Styleable_ViewDrawableStates[m];
        if (i < stateSet1Length && (*stateSet1)[i] == viewState) {
            (**newSet)[k++] = viewState;
            i++;
        } else if (j < stateSet2Length && (*stateSet2)[j] == viewState) {
            (**newSet)[k++] = viewState;
            j++;
        }
        if (k > 1) {
            assert((**newSet)[k - 1] > (**newSet)[k - 2]);
        }
    }
    return NOERROR;
}

/**
 * Inflate a view from an XML resource.  This convenience method wraps the {@link
 * LayoutInflater} class, which provides a full range of options for view inflation.
 *
 * @param context The Context object for your activity or application.
 * @param resource The resource ID to inflate
 * @param root A view group that will be the parent.  Used to properly inflate the
 * layout_* parameters.
 * @see LayoutInflater
 */
ECode View::Inflate(
    /* [in] */ IContext* context,
    /* [in] */ Int32 resource,
    /* [in] */ IViewGroup* root,
    /* [out] */ IView** view)
{
    AutoPtr<ILayoutInflater> factory;
    FAIL_RETURN(LayoutInflater::From(context, (ILayoutInflater**)&factory));
    return factory->Inflate(resource, root, view);
}

/**
 * Scroll the view with standard behavior for scrolling beyond the normal
 * content boundaries. Views that call this method should override
 * {@link #onOverScrolled(int, int, Boolean, Boolean)} to respond to the
 * results of an over-scroll operation.
 *
 * Views can use this method to handle any touch or fling-based scrolling.
 *
 * @param deltaX Change in X in pixels
 * @param deltaY Change in Y in pixels
 * @param scrollX Current X scroll value in pixels before applying deltaX
 * @param scrollY Current Y scroll value in pixels before applying deltaY
 * @param scrollRangeX Maximum content scroll range along the X axis
 * @param scrollRangeY Maximum content scroll range along the Y axis
 * @param maxOverScrollX Number of pixels to overscroll by in either direction
 *          along the X axis.
 * @param maxOverScrollY Number of pixels to overscroll by in either direction
 *          along the Y axis.
 * @param isTouchEvent TRUE if this scroll operation is the result of a touch event.
 * @return TRUE if scrolling was clamped to an over-scroll boundary along either
 *          axis, FALSE otherwise.
 */
Boolean View::OverScrollBy(
    /* [in] */ Int32 deltaX,
    /* [in] */ Int32 deltaY,
    /* [in] */ Int32 scrollX,
    /* [in] */ Int32 scrollY,
    /* [in] */ Int32 scrollRangeX,
    /* [in] */ Int32 scrollRangeY,
    /* [in] */ Int32 maxOverScrollX,
    /* [in] */ Int32 maxOverScrollY,
    /* [in] */ Boolean isTouchEvent)
{
    Int32 overScrollMode = mOverScrollMode;
    Boolean canScrollHorizontal =
            ComputeHorizontalScrollRange() > ComputeHorizontalScrollExtent();
    Boolean canScrollVertical =
            ComputeVerticalScrollRange() > ComputeVerticalScrollExtent();
    Boolean overScrollHorizontal = overScrollMode == OVER_SCROLL_ALWAYS ||
            (overScrollMode == OVER_SCROLL_IF_CONTENT_SCROLLS && canScrollHorizontal);
    Boolean overScrollVertical = overScrollMode == OVER_SCROLL_ALWAYS ||
            (overScrollMode == OVER_SCROLL_IF_CONTENT_SCROLLS && canScrollVertical);

    Int32 newScrollX = scrollX + deltaX;
    if (!overScrollHorizontal) {
        maxOverScrollX = 0;
    }

    Int32 newScrollY = scrollY + deltaY;
    if (!overScrollVertical) {
        maxOverScrollY = 0;
    }

    // Clamp values if at the limits and record
    Int32 left = -maxOverScrollX;
    Int32 right = maxOverScrollX + scrollRangeX;
    Int32 top = -maxOverScrollY;
    Int32 bottom = maxOverScrollY + scrollRangeY;

    Boolean clampedX = FALSE;
    if (newScrollX > right) {
        newScrollX = right;
        clampedX = TRUE;
    }
    else if (newScrollX < left) {
        newScrollX = left;
        clampedX = TRUE;
    }

    Boolean clampedY = FALSE;
    if (newScrollY > bottom) {
        newScrollY = bottom;
        clampedY = TRUE;
    }
    else if (newScrollY < top) {
        newScrollY = top;
        clampedY = TRUE;
    }

    OnOverScrolled(newScrollX, newScrollY, clampedX, clampedY);

    return clampedX || clampedY;
}

/**
 * Called by {@link #overScrollBy(int, int, int, int, int, int, int, int, Boolean)} to
 * respond to the results of an over-scroll operation.
 *
 * @param scrollX New X scroll value in pixels
 * @param scrollY New Y scroll value in pixels
 * @param clampedX True if scrollX was clamped to an over-scroll boundary
 * @param clampedY True if scrollY was clamped to an over-scroll boundary
 */
void View::OnOverScrolled(
    /* [in] */ Int32 scrollX,
    /* [in] */ Int32 scrollY,
    /* [in] */ Boolean clampedX,
    /* [in] */ Boolean clampedY)
{
    // Intentionally empty.
}

/**
 * Returns the over-scroll mode for this view. The result will be
 * one of {@link #OVER_SCROLL_ALWAYS} (default), {@link #OVER_SCROLL_IF_CONTENT_SCROLLS}
 * (allow over-scrolling only if the view content is larger than the container),
 * or {@link #OVER_SCROLL_NEVER}.
 *
 * @return This view's over-scroll mode.
 */
Int32 View::GetOverScrollMode()
{
    return mOverScrollMode;
}

/**
 * Set the over-scroll mode for this view. Valid over-scroll modes are
 * {@link #OVER_SCROLL_ALWAYS} (default), {@link #OVER_SCROLL_IF_CONTENT_SCROLLS}
 * (allow over-scrolling only if the view content is larger than the container),
 * or {@link #OVER_SCROLL_NEVER}.
 *
 * Setting the over-scroll mode of a view will have an effect only if the
 * view is capable of scrolling.
 *
 * @param overScrollMode The new over-scroll mode for this view.
 */
ECode View::SetOverScrollMode(
    /* [in] */ Int32 overScrollMode)
{
    if (overScrollMode != OVER_SCROLL_ALWAYS &&
            overScrollMode != OVER_SCROLL_IF_CONTENT_SCROLLS &&
            overScrollMode != OVER_SCROLL_NEVER) {
//        throw new IllegalArgumentException("Invalid overscroll mode " + overScrollMode);
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    mOverScrollMode = overScrollMode;

    return NOERROR;
}

ECode View::Init(
    /* [in] */ IContext* context)
{
    mContext = context;
    if (context != NULL) {
        context->GetResources((IResources**)&mResources);
    }
    mViewFlags = SOUND_EFFECTS_ENABLED | HAPTIC_FEEDBACK_ENABLED;
    // Used for debug only
    //++sInstanceCount;
    mTouchSlop = ViewConfiguration::Get(context)->GetScaledTouchSlop();
    SetOverScrollMode(OVER_SCROLL_IF_CONTENT_SCROLLS);
    mPrivateFlags = 0;

    return NOERROR;
}

ECode View::Init(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs)
{
    return Init(context, attrs, 0);
}

static Int32 R_Styleable_View[] = {
    0x01010063, 0x01010064, 0x01010065, 0x01010066,
    0x01010067, 0x01010068, 0x01010069, 0x0101007f,
    0x010100d0, 0x010100d1, 0x010100d2, 0x010100d3,
    0x010100d4, 0x010100d5, 0x010100d6, 0x010100d7,
    0x010100d8, 0x010100d9, 0x010100da, 0x010100db,
    0x010100dc, 0x010100dd, 0x010100de, 0x010100df,
    0x010100e0, 0x010100e1, 0x010100e2, 0x010100e3,
    0x010100e4, 0x010100e5, 0x010100e6, 0x010100e7,
    0x010100e8, 0x010100e9, 0x0101013f, 0x01010140,
    0x01010215, 0x01010216, 0x0101024e, 0x0101025e,
    0x0101026f, 0x01010273, 0x010102a8, 0x010102a9,
    0x010102aa, 0x010102c1, 0x010102c4
};

ECode View::Init(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs,
    /* [in] */ Int32 defStyle)
{
    Init(context);

    //printf("View::Init\n");
    AutoPtr<ITypedArray> a;
    ASSERT_SUCCEEDED(context->ObtainStyledAttributesEx3(
            attrs, ArrayOf<Int32>(R_Styleable_View, 47)/*com.android.internal.R.styleable.View*/,
            defStyle, 0, (ITypedArray**)&a));
    AutoPtr<IDrawable> background;

    Int32 leftPadding = -1;
    Int32 topPadding = -1;
    Int32 rightPadding = -1;
    Int32 bottomPadding = -1;

    Int32 padding = -1;

    Int32 viewFlagValues = 0;
    Int32 viewFlagMasks = 0;

    Boolean setScrollContainer = FALSE;

    Int32 x = 0;
    Int32 y = 0;

    Int32 scrollbarStyle = SCROLLBARS_INSIDE_OVERLAY;

    Int32 overScrollMode = mOverScrollMode;
    Boolean value;
    Int32 N;
    a->GetIndexCount(&N);
    for (Int32 i = 0; i < N; i++) {
        Int32 attr;
        a->GetIndex(i, &attr);
        //printf("View::Init---Index = %d, attr = %d\n", i, attr);
        switch (attr) {
            case 12/*com.android.internal.R.styleable.View_background*/:
                a->GetDrawable(attr, (IDrawable**)&background);
                //printf("Drawable = 0x%08x\n", (Int32)background.Get());
                break;
            case 13/*com.android.internal.R.styleable.View_padding*/:
                a->GetDimensionPixelSize(attr, -1, &padding);
                break;
             case 14/*com.android.internal.R.styleable.View_paddingLeft*/:
                a->GetDimensionPixelSize(attr, -1, &leftPadding);
                break;
            case 15/*com.android.internal.R.styleable.View_paddingTop*/:
                a->GetDimensionPixelSize(attr, -1, &topPadding);
                break;
            case 16/*com.android.internal.R.styleable.View_paddingRight*/:
                a->GetDimensionPixelSize(attr, -1, &rightPadding);
                break;
            case 17/*com.android.internal.R.styleable.View_paddingBottom*/:
                a->GetDimensionPixelSize(attr, -1, &bottomPadding);
                break;
            case 10/*com.android.internal.R.styleable.View_scrollX*/:
                a->GetDimensionPixelOffset(attr, 0, &x);
                break;
            case 11/*com.android.internal.R.styleable.View_scrollY*/:
                a->GetDimensionPixelOffset(attr, 0, &y);
                break;
            case 8/*com.android.internal.R.styleable.View_id*/:
                a->GetResourceId(attr, NO_ID, &mID);
                break;
            case 9/*com.android.internal.R.styleable.View_tag*/:
//                  mTag = a.getText(attr);
                break;
            case 21/*com.android.internal.R.styleable.View_fitsSystemWindows*/:
                a->GetBoolean(attr, FALSE, &value);
                if (value) {
                    viewFlagValues |= FITS_SYSTEM_WINDOWS;
                    viewFlagMasks |= FITS_SYSTEM_WINDOWS;
                }
                break;
            case 18/*com.android.internal.R.styleable.View_focusable*/:
                a->GetBoolean(attr, FALSE, &value);
                if (value) {
                    viewFlagValues |= FOCUSABLE;
                    viewFlagMasks |= FOCUSABLE_MASK;
                }
                break;
            case 19/*com.android.internal.R.styleable.View_focusableInTouchMode*/:
                a->GetBoolean(attr, FALSE, &value);
                if (value) {
                    viewFlagValues |= FOCUSABLE_IN_TOUCH_MODE | FOCUSABLE;
                    viewFlagMasks |= FOCUSABLE_IN_TOUCH_MODE | FOCUSABLE_MASK;
                }
                break;
            case 29/*com.android.internal.R.styleable.View_clickable*/:
                a->GetBoolean(attr, FALSE, &value);
                if (value) {
                    viewFlagValues |= CLICKABLE;
                    viewFlagMasks |= CLICKABLE;
                }
                break;
            case 30/*com.android.internal.R.styleable.View_longClickable*/:
                a->GetBoolean(attr, FALSE, &value);
                if (value) {
                    viewFlagValues |= LONG_CLICKABLE;
                    viewFlagMasks |= LONG_CLICKABLE;
                }
                break;
            case 31/*com.android.internal.R.styleable.View_saveEnabled*/:
                a->GetBoolean(attr, TRUE, &value);
                if (!value) {
                    viewFlagValues |= SAVE_DISABLED;
                    viewFlagMasks |= SAVE_DISABLED_MASK;
                }
                break;
            case 33/*com.android.internal.R.styleable.View_duplicateParentState*/:
                a->GetBoolean(attr, FALSE, &value);
                if (value) {
                    viewFlagValues |= DUPLICATE_PARENT_STATE;
                    viewFlagMasks |= DUPLICATE_PARENT_STATE;
                }
                break;
            case 20/*com.android.internal.R.styleable.View_visibility*/:
            {
                Int32 visibility;
                a->GetInt32(attr, 0, &visibility);
                if (visibility != 0) {
                    viewFlagValues |= VISIBILITY_FLAGS[visibility];
                    viewFlagMasks |= VISIBILITY_MASK;
                }
                break;
            }
            case 32/*com.android.internal.R.styleable.View_drawingCacheQuality*/:
            {
                Int32 cacheQuality;
                a->GetInt32(attr, 0, &cacheQuality);
                if (cacheQuality != 0) {
                    viewFlagValues |= DRAWING_CACHE_QUALITY_FLAGS[cacheQuality];
                    viewFlagMasks |= DRAWING_CACHE_QUALITY_MASK;
                }
                break;
            }
            case 41/*com.android.internal.R.styleable.View_contentDescription*/:
            {
                String str;
                a->GetString(attr, &str);
                if (!str.IsNull()) {
                    mContentDescription = ArrayOf<Char8>::Alloc(str.GetLength() + 1);
                    strcpy(mContentDescription->GetPayload(), (const char*)str);
                }
                else {
                    mContentDescription = NULL;
                }
                break;
            }
            case 36/*com.android.internal.R.styleable.View_soundEffectsEnabled*/:
                a->GetBoolean(attr, TRUE, &value);
                if (!value) {
                    viewFlagValues &= ~SOUND_EFFECTS_ENABLED;
                    viewFlagMasks |= SOUND_EFFECTS_ENABLED;
                }
                break;
            case 39/*com.android.internal.R.styleable.View_hapticFeedbackEnabled*/:
                a->GetBoolean(attr, TRUE, &value);
                if (!value) {
                    viewFlagValues &= ~HAPTIC_FEEDBACK_ENABLED;
                    viewFlagMasks |= HAPTIC_FEEDBACK_ENABLED;
                }
                break;
            case 22/*R.styleable.View_scrollbars*/:
            {
                Int32 scrollbars;
                a->GetInt32(attr, SCROLLBARS_NONE, &scrollbars);
                if (scrollbars != SCROLLBARS_NONE) {
                    viewFlagValues |= scrollbars;
                    viewFlagMasks |= SCROLLBARS_MASK;
                    InitializeScrollbars(a);
                }
                break;
            }
            case 23/*R.styleable.View_fadingEdge*/:
            {
                Int32 fadingEdge;
                a->GetInt32(attr, FADING_EDGE_NONE, &fadingEdge);
                if (fadingEdge != FADING_EDGE_NONE) {
                    viewFlagValues |= fadingEdge;
                    viewFlagMasks |= FADING_EDGE_MASK;
                    InitializeFadingEdge(a);
                }
                break;
            }
            case 7/*R.styleable.View_scrollbarStyle*/:
                a->GetInt32(attr, SCROLLBARS_INSIDE_OVERLAY, &scrollbarStyle);
                if (scrollbarStyle != SCROLLBARS_INSIDE_OVERLAY) {
                    viewFlagValues |= scrollbarStyle & SCROLLBARS_STYLE_MASK;
                    viewFlagMasks |= SCROLLBARS_STYLE_MASK;
                }
                break;
            case 38/*R.styleable.View_isScrollContainer*/:
                setScrollContainer = TRUE;
                a->GetBoolean(attr, FALSE, &value);
                if (value) {
                    SetScrollContainer(TRUE);
                }
                break;
            case 37/*com.android.internal.R.styleable.View_keepScreenOn*/:
                a->GetBoolean(attr, FALSE, &value);
                if (value) {
                    viewFlagValues |= KEEP_SCREEN_ON;
                    viewFlagMasks |= KEEP_SCREEN_ON;
                }
                break;
            case 46/*R.styleable.View_filterTouchesWhenObscured*/:
                a->GetBoolean(attr, FALSE, &value);
                if (value) {
                    viewFlagValues |= FILTER_TOUCHES_WHEN_OBSCURED;
                    viewFlagMasks |= FILTER_TOUCHES_WHEN_OBSCURED;
                }
                break;
            case 25/*R.styleable.View_nextFocusLeft*/:
                a->GetResourceId(attr, View::NO_ID, &mNextFocusLeftId);
                break;
            case 26/*R.styleable.View_nextFocusRight*/:
                a->GetResourceId(attr, View::NO_ID, &mNextFocusRightId);
                break;
            case 27/*R.styleable.View_nextFocusUp*/:
                a->GetResourceId(attr, View::NO_ID, &mNextFocusUpId);
                break;
            case 28/*R.styleable.View_nextFocusDown*/:
                a->GetResourceId(attr, View::NO_ID, &mNextFocusDownId);
                break;
            case 34/*R.styleable.View_minWidth*/:
                a->GetDimensionPixelSize(attr, 0, &mMinWidth);
                break;
            case 35/*R.styleable.View_minHeight*/:
                a->GetDimensionPixelSize(attr, 0, &mMinHeight);
                break;
            case 40/*R.styleable.View_onClick*/:
//                if (context.isRestricted()) {
//                    throw new IllegalStateException("The android:onClick attribute cannot "
//                            + "be used within a restricted context");
//                }
//
//                final String handlerName = a.getString(attr);
//                if (handlerName != NULL) {
//                    setOnClickListener(new OnClickListener() {
//                        private Method mHandler;
//
//                        public void onClick(View v) {
//                            if (mHandler == NULL) {
//                                try {
//                                    mHandler = getContext().getClass().getMethod(handlerName,
//                                            View.class);
//                                } catch (NoSuchMethodException e) {
//                                    int id = getId();
//                                    String idText = id == NO_ID ? "" : " with id '"
//                                            + getContext().getResources().getResourceEntryName(
//                                                id) + "'";
//                                    throw new IllegalStateException("Could not find a method " +
//                                            handlerName + "(View) in the activity "
//                                            + getContext().getClass() + " for onClick handler"
//                                            + " on view " + View.this.getClass() + idText, e);
//                                }
//                            }
//
//                             try {
//                                 mHandler.invoke(getContext(), View.this);
//                             } catch (IllegalAccessException e) {
//                                 throw new IllegalStateException("Could not execute non "
//                                         + "public method of the activity", e);
//                             } catch (InvocationTargetException e) {
//                                 throw new IllegalStateException("Could not execute "
//                                         + "method of the activity", e);
//                             }
//                         }
//                     });
//                }
                break;
            case 45/*R.styleable.View_overScrollMode*/:
                a->GetInt32(attr, OVER_SCROLL_IF_CONTENT_SCROLLS, &overScrollMode);
                break;
            default:
                break;
        }
    }

    SetOverScrollMode(overScrollMode);

    if (background != NULL) {
        SetBackgroundDrawable(background);
    }

    if (padding >= 0) {
        leftPadding = padding;
        topPadding = padding;
        rightPadding = padding;
        bottomPadding = padding;
    }

    // If the user specified the padding (either with android:padding or
    // android:paddingLeft/Top/Right/Bottom), use this padding, otherwise
    // use the default padding or the padding from the background drawable
    // (stored at this point in mPadding*)
    SetPadding(leftPadding >= 0 ? leftPadding : mPaddingLeft,
            topPadding >= 0 ? topPadding : mPaddingTop,
            rightPadding >= 0 ? rightPadding : mPaddingRight,
            bottomPadding >= 0 ? bottomPadding : mPaddingBottom);

    if (viewFlagMasks != 0) {
        SetFlags(viewFlagValues, viewFlagMasks);
    }

    // Needs to be called after mViewFlags is set
    if (scrollbarStyle != SCROLLBARS_INSIDE_OVERLAY) {
        RecomputePadding();
    }

    if (x != 0 || y != 0) {
        ScrollTo(x, y);
    }

    if (!setScrollContainer && (viewFlagValues&SCROLLBARS_VERTICAL) != 0) {
        SetScrollContainer(TRUE);
    }

    ComputeOpaqueFlags();

    a->Recycle();

    //printf("View::Init----END 0x%08x, ID = 0x%08x\n", (Int32)this->Probe(EIID_IView), mID);
    return NOERROR;
}

ECode View::HandleInvalidate()
{
    return Invalidate();
}

ECode View::HandleInvalidateRect(
    /* [in] */ Int32 left,
    /* [in] */ Int32 top,
    /* [in] */ Int32 right,
    /* [in] */ Int32 bottom)
{
    return Invalidate(left, top, right, bottom);
}
