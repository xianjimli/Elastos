
#include "view/ViewGroup.h"
#include "view/CViewGroupLayoutParams.h"
#include "view/CMotionEvent.h"
#include "view/ViewRoot.h"
#include "view/FocusFinder.h"
#include "view/animation/Animation.h"
#include "view/animation/AnimationUtils.h"
#include "view/animation/CTransformation.h"
#include "view/animation/CLayoutAnimationController.h"
#include "graphics/CPaint.h"
#include "graphics/CPoint.h"
#include "os/SystemClock.h"
#include <Logger.h>
#include <StringBuffer.h>
#include <elastos/Math.h>

using namespace Elastos::Core;
using namespace Elastos::Utility::Logging;

const Int32 ViewGroup::FOCUS_BEFORE_DESCENDANTS;
const Int32 ViewGroup::FOCUS_AFTER_DESCENDANTS;
const Int32 ViewGroup::FOCUS_BLOCK_DESCENDANTS;
const Int32 ViewGroup::PERSISTENT_NO_CACHE;
const Int32 ViewGroup::PERSISTENT_ANIMATION_CACHE;
const Int32 ViewGroup::PERSISTENT_SCROLLING_CACHE;
const Int32 ViewGroup::PERSISTENT_ALL_CACHES;
const Boolean ViewGroup::DBG;
const char* ViewGroup::VG_TAG = "ViewGroup";
const Int32 ViewGroup::FLAG_CLIP_CHILDREN;
const Int32 ViewGroup::FLAG_CLIP_TO_PADDING;
const Int32 ViewGroup::FLAG_INVALIDATE_REQUIRED;
const Int32 ViewGroup::FLAG_RUN_ANIMATION;
const Int32 ViewGroup::FLAG_ANIMATION_DONE;
const Int32 ViewGroup::FLAG_PADDING_NOT_NULL;
const Int32 ViewGroup::FLAG_ANIMATION_CACHE;
const Int32 ViewGroup::FLAG_OPTIMIZE_INVALIDATE;
const Int32 ViewGroup::FLAG_CLEAR_TRANSFORMATION;
const Int32 ViewGroup::FLAG_NOTIFY_ANIMATION_LISTENER;
const Int32 ViewGroup::FLAG_ALPHA_LOWER_THAN_ONE;
const Int32 ViewGroup::FLAG_ADD_STATES_FROM_CHILDREN;
const Int32 ViewGroup::FLAG_ALWAYS_DRAWN_WITH_CACHE;
const Int32 ViewGroup::FLAG_CHILDREN_DRAWN_WITH_CACHE;
const Int32 ViewGroup::FLAG_NOTIFY_CHILDREN_ON_DRAWABLE_STATE_CHANGE;
const Int32 ViewGroup::FLAG_MASK_FOCUSABILITY;
const Int32 ViewGroup::DESCENDANT_FOCUSABILITY_FLAGS[] = {
    FOCUS_BEFORE_DESCENDANTS, FOCUS_AFTER_DESCENDANTS,
    FOCUS_BLOCK_DESCENDANTS
};
const Int32 ViewGroup::ARRAY_INITIAL_CAPACITY;
const Int32 ViewGroup::ARRAY_CAPACITY_INCREMENT;
const Int32 ViewGroup::FLAG_USE_CHILD_DRAWING_ORDER;
const Int32 ViewGroup::FLAG_SUPPORT_STATIC_TRANSFORMATIONS;
const Int32 ViewGroup::FLAG_DISALLOW_INTERCEPT;
const Int32 ViewGroup::CLIP_TO_PADDING_MASK;
const Int32 ViewGroup::CHILD_LEFT_INDEX;
const Int32 ViewGroup::CHILD_TOP_INDEX;

ViewGroup::ViewGroup()
    : mDisappearingChildren(NULL)
{
}

ViewGroup::ViewGroup(
    /* [in] */ IContext* context)
    : View(context)
    , mDisappearingChildren(NULL)
{
    InitViewGroup();
}

ViewGroup::ViewGroup(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs)
    : View(context, attrs)
    , mDisappearingChildren(NULL)
{
    InitViewGroup();
    InitFromAttributes(context, attrs);
}

ViewGroup::ViewGroup(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs,
    /* [in] */ Int32 defStyle)
    : View(context, attrs, defStyle)
    , mDisappearingChildren(NULL)
{
    InitViewGroup();
    InitFromAttributes(context, attrs);
}

void ViewGroup::InitViewGroup()
{
    assert(SUCCEEDED(CRect::NewByFriend((CRect**)&mTempRect)));
    assert(SUCCEEDED(CPaint::New((IPaint**)&mCachePaint)));
    // ViewGroup doesn't draw by default
    SetFlags(WILL_NOT_DRAW, DRAW_MASK);
    mGroupFlags |= FLAG_CLIP_CHILDREN;
    mGroupFlags |= FLAG_CLIP_TO_PADDING;
    mGroupFlags |= FLAG_ANIMATION_DONE;
    mGroupFlags |= FLAG_ANIMATION_CACHE;
    mGroupFlags |= FLAG_ALWAYS_DRAWN_WITH_CACHE;

    SetDescendantFocusability(FOCUS_BEFORE_DESCENDANTS);


    mCachePaint->SetDither(FALSE);

    mPersistentDrawingCache = PERSISTENT_SCROLLING_CACHE;
}

static Int32 R_Styleable_ViewGroup[] = {
    0x010100ea, 0x010100eb, 0x010100ec, 0x010100ed,
    0x010100ee, 0x010100ef, 0x010100f0, 0x010100f1
};

void ViewGroup::InitFromAttributes(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs)
{
    AutoPtr<ITypedArray> a;
    ASSERT_SUCCEEDED(context->ObtainStyledAttributesEx2(attrs,
            ArrayOf<Int32>(R_Styleable_ViewGroup, 8)/*com.android.internal.R.styleable.ViewGroup*/,
            (ITypedArray**)&a));

    Int32 N;
    a->GetIndexCount(&N);
    for (Int32 i = 0; i < N; i++) {
        Int32 attr;
        a->GetIndex(i, &attr);
        switch (attr) {
            case 0/*R.styleable.ViewGroup_clipChildren*/:
            {
                Boolean value;
                a->GetBoolean(attr, TRUE, &value);
                SetClipChildren(value);
                break;
            }
            case 1/*R.styleable.ViewGroup_clipToPadding*/:
            {
                Boolean value;
                a->GetBoolean(attr, TRUE, &value);
                SetClipToPadding(value);
                break;
            }
            case 3/*R.styleable.ViewGroup_animationCache*/:
            {
                Boolean value;
                a->GetBoolean(attr, TRUE, &value);
                SetAnimationCacheEnabled(value);
                break;
            }
            case 4/*R.styleable.ViewGroup_persistentDrawingCache*/:
            {
                Int32 value;
                a->GetInt32(attr, PERSISTENT_SCROLLING_CACHE, &value);
                SetPersistentDrawingCache(value);
                break;
            }
            case 6/*R.styleable.ViewGroup_addStatesFromChildren*/:
            {
                Boolean value;
                a->GetBoolean(attr, FALSE, &value);
                SetAddStatesFromChildren(value);
                break;
            }
            case 5/*R.styleable.ViewGroup_alwaysDrawnWithCache*/:
            {
                Boolean value;
                a->GetBoolean(attr, TRUE, &value);
                SetAlwaysDrawnWithCacheEnabled(value);
                break;
            }
            case 2/*R.styleable.ViewGroup_layoutAnimation*/:
            {
                Int32 id;
                a->GetResourceId(attr, -1, &id);
                if (id > 0) {
                    AutoPtr<ILayoutAnimationController> controller;
                    AnimationUtils::LoadLayoutAnimation(
                        mContext, id, (ILayoutAnimationController**)&controller);
                    SetLayoutAnimation(controller);
                }
                break;
            }
            case 7/*R.styleable.ViewGroup_descendantFocusability*/:
            {
                Int32 value;
                a->GetInt32(attr, 0, &value);
                SetDescendantFocusability(DESCENDANT_FOCUSABILITY_FLAGS[value]);
                break;
            }
            default:
                break;
        }
    }

    a->Recycle();
}

Int32 ViewGroup::GetDescendantFocusability()
{
    return mGroupFlags & FLAG_MASK_FOCUSABILITY;
}

ECode ViewGroup::SetDescendantFocusability(
    /* [in] */ Int32 focusability)
{
    switch (focusability) {
        case FOCUS_BEFORE_DESCENDANTS:
        case FOCUS_AFTER_DESCENDANTS:
        case FOCUS_BLOCK_DESCENDANTS:
            break;
        default:
//            throw new IllegalArgumentException("must be one of FOCUS_BEFORE_DESCENDANTS, "
//                    + "FOCUS_AFTER_DESCENDANTS, FOCUS_BLOCK_DESCENDANTS");
            return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    mGroupFlags &= ~FLAG_MASK_FOCUSABILITY;
    mGroupFlags |= (focusability & FLAG_MASK_FOCUSABILITY);
    return NOERROR;
}

ECode ViewGroup::HandleFocusGainInternal(
    /* [in] */ Int32 direction,
    /* [in] */ IRect* previouslyFocusedRect)
{
    if (mFocused != NULL) {
        ((View*)mFocused->Probe(EIID_View))->UnFocus();
        mFocused = NULL;
    }

    return View::HandleFocusGainInternal(direction, previouslyFocusedRect);
}

ECode ViewGroup::RequestChildFocus(
    /* [in] */ IView* child,
    /* [in] */ IView* focused)
{
    if (DBG) {
        Logger::D(VG_TAG, /*this + */" requestChildFocus()");
    }

    if (GetDescendantFocusability() == FOCUS_BLOCK_DESCENDANTS) {
        return NOERROR;
    }

    // Unfocus us, if necessary
    ViewGroup::UnFocus();

    // We had a previous notion of who had focus. Clear it.
    if (mFocused.Get() != child) {
        if (mFocused != NULL) {
            ((View*)mFocused->Probe(EIID_View))->UnFocus();
        }

        mFocused = child;
    }

    if (mParent != NULL) {
        mParent->RequestChildFocus((IView*)this->Probe(EIID_IView), focused);
    }
    return NOERROR;
}

ECode ViewGroup::FocusableViewAvailable(
    /* [in] */ IView* v)
{
    if (mParent != NULL
        // shortcut: don't report a new focusable view if we block our descendants from
        // getting focus
        && (GetDescendantFocusability() != FOCUS_BLOCK_DESCENDANTS)
        // shortcut: don't report a new focusable view if we already are focused
        // (and we don't prefer our descendants)
        //
        // note: knowing that mFocused is non-NULL is not a good enough reason
        // to break the traversal since in that case we'd actually have to find
        // the focused view and make sure it wasn't FOCUS_AFTER_DESCENDANTS and
        // an ancestor of v; this will get checked for at ViewRoot
        && !(IsFocused() && GetDescendantFocusability() != FOCUS_AFTER_DESCENDANTS)) {
        mParent->FocusableViewAvailable(v);
    }
    return NOERROR;
}

Boolean ViewGroup::ShowContextMenuForChild(
    /* [in] */ IView* originalView)
{
    Boolean result = FALSE;
    if (mParent != NULL) {
        mParent->ShowContextMenuForChild(originalView, &result);
    }

    return result;
}

AutoPtr<IView> ViewGroup::FocusSearch(
     /* [in] */ IView* focused,
      /* [in] */ Int32 direction)
{
    if (IsRootNamespace()) {
        // root namespace means we should consider ourselves the top of the
        // tree for focus searching; otherwise we could be focus searching
        // into other tabs.  see LocalActivityManager and TabHost for more info
        //
        return FocusFinder::GetInstance()->FindNextFocus(
            (IViewGroup*)this->Probe(EIID_IViewGroup), focused, direction);
    }
    else if (mParent != NULL) {
        AutoPtr<IView> result;
        mParent->FocusSearchEx(focused, direction, (IView**)&result);
        return result;
    }

    return AutoPtr<IView>(NULL);
}

Boolean ViewGroup::RequestChildRectangleOnScreen(
    /* [in] */ IView* child,
    /* [in] */ IRect* rectangle,
    /* [in] */ Boolean immediate)
{
    return FALSE;
}

Boolean ViewGroup::DispatchUnhandledMove(
     /* [in] */ IView* focused,
     /* [in] */ Int32 direction)
{
    Boolean result = FALSE;
    if (mFocused != NULL) {
        mFocused->DispatchUnhandledMove(focused, direction, &result);
    }

    return result;
}

ECode ViewGroup::ClearChildFocus(
    /* [in] */ IView* child)
{
    if (DBG) {
        Logger::D(VG_TAG, /*this + */" clearChildFocus()");
    }

    mFocused = NULL;
    if (mParent != NULL) {
        mParent->ClearChildFocus((IView*)this->Probe(EIID_IView));
    }

    return NULL;
}

ECode ViewGroup::ClearFocus()
{
    View::ClearFocus();

    // clear any child focus if it exists
    if (mFocused != NULL) {
        mFocused->ClearFocus();
    }

    return NOERROR;
}

ECode ViewGroup::UnFocus()
{
    if (DBG) {
        Logger::D(VG_TAG, /*this + */" unFocus()");
    }

    View::UnFocus();
    if (mFocused != NULL) {
        ((View*)mFocused->Probe(EIID_View))->UnFocus();
    }
    mFocused = NULL;

    return NOERROR;
}

AutoPtr<IView> ViewGroup::GetFocusedChild()
{
    return mFocused;
}

Boolean ViewGroup::HasFocus()
{
    return (mPrivateFlags & FOCUSED) != 0 || mFocused != NULL;
}

AutoPtr<IView> ViewGroup::FindFocus()
{
    if (DBG) {
        //System.out.println("Find focus in " + this + ": flags="
        //        + isFocused() + ", child=" + mFocused);
    }

    if (IsFocused()) {
        return AutoPtr<IView>((IView*)this->Probe(EIID_IView));
    }

    if (mFocused != NULL) {
        return ((View*)mFocused->Probe(EIID_View))->FindFocus();
    }
    return AutoPtr<IView>(NULL);
}

Boolean ViewGroup::HasFocusable()
{
    if ((mViewFlags & VISIBILITY_MASK) != VISIBLE) {
        return FALSE;
    }

    if (IsFocusable()) {
        return TRUE;
    }

    if (GetDescendantFocusability() != FOCUS_BLOCK_DESCENDANTS) {
        Vector<AutoPtr<IView> >::Iterator iter = mChildren.Begin();
        for (; iter != mChildren.End(); ++iter) {
            Boolean hasFocusable;
            (*iter)->HasFocusable(&hasFocusable);
            if (hasFocusable) {
                return TRUE;
            }
        }
    }

    return FALSE;
}

ECode ViewGroup::AddFocusables(
    /* [in] */ IObjectContainer* views,
    /* [in] */ Int32 direction)
{
    return AddFocusables(views, direction, FOCUSABLES_TOUCH_MODE);
}

ECode ViewGroup::AddFocusables(
    /* [in] */ IObjectContainer* views,
    /* [in] */ Int32 direction,
    /* [in] */ Int32 focusableMode)
{
    assert(views);

    Int32 focusableCount;
    views->GetObjectCount(&focusableCount);

    Int32 descendantFocusability = GetDescendantFocusability();
    if (descendantFocusability != FOCUS_BLOCK_DESCENDANTS) {
        Vector<AutoPtr<IView> >::Iterator iter = mChildren.Begin();
        for (; iter != mChildren.End(); ++iter) {
            View* temp = (View*)(*iter)->Probe(EIID_View);
            if ((temp->mViewFlags & VISIBILITY_MASK) == VISIBLE) {
                temp->AddFocusables(views, direction, focusableMode);
            }
        }
    }

    // we add ourselves (if focusable) in all cases except for when we are
    // FOCUS_AFTER_DESCENDANTS and there are some descendants focusable.  this is
    // to avoid the focus search finding layouts when a more precise search
    // among the focusable children would be more interesting.
    //
    Int32 size;
    views->GetObjectCount(&size);
    if (descendantFocusability != FOCUS_AFTER_DESCENDANTS ||
        // No focusable descendants
        (focusableCount == size)) {
            View::AddFocusables(views, direction, focusableMode);
    }

    return NOERROR;
}

ECode ViewGroup::DispatchWindowFocusChanged(
    /* [in] */ Boolean hasFocus)
{
    View::DispatchWindowFocusChanged(hasFocus);

    Vector<AutoPtr<IView> >::Iterator iter = mChildren.Begin();
    for (; iter != mChildren.End(); ++iter) {
        (*iter)->DispatchWindowFocusChanged(hasFocus);
    }

    return NOERROR;
}

ECode ViewGroup::AddTouchables(
    /* [in] */ IObjectContainer* views)
{
    View::AddTouchables(views);

    Vector<AutoPtr<IView> >::Iterator iter = mChildren.Begin();
    for (; iter != mChildren.End(); ++iter) {
        View* temp = (View*)(*iter)->Probe(EIID_View);
        if ((temp->mViewFlags & VISIBILITY_MASK) == VISIBLE) {
            temp->AddTouchables(views);
        }
    }

    return NOERROR;
}

ECode ViewGroup::DispatchDisplayHint(
    /* [in] */ Int32 hint)
{
    View::DispatchDisplayHint(hint);

    Vector<AutoPtr<IView> >::Iterator iter = mChildren.Begin();
    for (; iter != mChildren.End(); ++iter) {
        (*iter)->DispatchDisplayHint(hint);
    }

    return NOERROR;
}

void ViewGroup::DispatchVisibilityChanged(
    /* [in] */ IView* changedView,
    /* [in] */ Int32 visibility)
{
    View::DispatchVisibilityChanged(changedView, visibility);

    Vector<AutoPtr<IView> >::Iterator iter = mChildren.Begin();
    for (; iter != mChildren.End(); ++iter) {
        ((View*)(*iter)->Probe(EIID_View))->DispatchVisibilityChanged(
            changedView, visibility);
    }
}

ECode ViewGroup::DispatchWindowVisibilityChanged(
    /* [in] */ Int32 visibility)
{
    View::DispatchWindowVisibilityChanged(visibility);

    Vector<AutoPtr<IView> >::Iterator iter = mChildren.Begin();
    for (; iter != mChildren.End(); ++iter) {
        (*iter)->DispatchWindowVisibilityChanged(visibility);
    }

    return NOERROR;
}

ECode ViewGroup::DispatchConfigurationChanged(
    /* [in] */ IConfiguration* newConfig)
{
    View::DispatchConfigurationChanged(newConfig);

    Vector<AutoPtr<IView> >::Iterator iter = mChildren.Begin();
    for (; iter != mChildren.End(); ++iter) {
        (*iter)->DispatchConfigurationChanged(newConfig);
    }

    return NOERROR;
}

ECode ViewGroup::RecomputeViewAttributes(
    /* [in] */ IView* child)
{
    if (mParent != NULL) {
        return mParent->RecomputeViewAttributes((IView*)this->Probe(EIID_IView));
    }

    return NOERROR;
}

ECode ViewGroup::DispatchCollectViewAttributes(
    /* [in] */ Int32 visibility)
{
    visibility |= mViewFlags&VISIBILITY_MASK;

    View::DispatchCollectViewAttributes(visibility);

    Vector<AutoPtr<IView> >::Iterator iter = mChildren.Begin();
    for (; iter != mChildren.End(); ++iter) {
        ((View*)(*iter)->Probe(EIID_View))->DispatchCollectViewAttributes(visibility);
    }

    return NOERROR;
}

ECode ViewGroup::BringChildToFront(
    /* [in] */ IView* child)
{
    Int32 index = IndexOfChild(child);
    if (index >= 0) {
        RemoveFromArray(index);
        AddInArray(child, mChildren.GetSize());
        ((View*)child->Probe(EIID_View))->mParent =
            (IViewParent*)this->Probe(EIID_IViewParent);
    }
    return NOERROR;
}

Boolean ViewGroup::DispatchKeyEventPreIme(
    /* [in] */ IKeyEvent* event)
{
    if ((mPrivateFlags & (FOCUSED | HAS_BOUNDS)) == (FOCUSED | HAS_BOUNDS)) {
        return View::DispatchKeyEventPreIme(event);
    }
    else if (mFocused != NULL) {
        View* focused = (View*)mFocused->Probe(EIID_View);
        assert(focused);
        if ((focused->mPrivateFlags & HAS_BOUNDS) == HAS_BOUNDS) {
            return focused->DispatchKeyEventPreIme(event);
        }
    }

    return FALSE;
}

Boolean ViewGroup::DispatchKeyEvent(
    /* [in] */ IKeyEvent* event)
{
    if ((mPrivateFlags & (FOCUSED | HAS_BOUNDS)) == (FOCUSED | HAS_BOUNDS)) {
        return View::DispatchKeyEvent(event);
    }
    else if (mFocused != NULL) {
        View* focused = (View*)mFocused->Probe(EIID_View);
        assert(focused);
        if ((focused->mPrivateFlags & HAS_BOUNDS) == HAS_BOUNDS) {
            return focused->DispatchKeyEvent(event);
        }
    }

    return FALSE;
}

Boolean ViewGroup::DispatchKeyShortcutEvent(
    /* [in] */ IKeyEvent* event)
{
    if ((mPrivateFlags & (FOCUSED | HAS_BOUNDS)) == (FOCUSED | HAS_BOUNDS)) {
        return View::DispatchKeyShortcutEvent(event);
    }
    else if (mFocused != NULL) {
        View* focused = (View*)mFocused->Probe(EIID_IView);
        assert(focused);
        if ((focused->mPrivateFlags & HAS_BOUNDS) == HAS_BOUNDS) {
            return focused->DispatchKeyShortcutEvent(event);
        }
    }

    return FALSE;
}

Boolean ViewGroup::DispatchTrackballEvent(
    /* [in] */ IMotionEvent* event)
{
    if ((mPrivateFlags & (FOCUSED | HAS_BOUNDS)) == (FOCUSED | HAS_BOUNDS)) {
        return View::DispatchTrackballEvent(event);
    }
    else if (mFocused != NULL) {
        View* focused = (View*)mFocused->Probe(EIID_View);
        assert(focused);
        if ((focused->mPrivateFlags & HAS_BOUNDS) == HAS_BOUNDS) {
            return focused->DispatchTrackballEvent(event);
        }
    }

    return FALSE;
}

Boolean ViewGroup::DispatchTouchEvent(
    /* [in] */ IMotionEvent* ev)
{
    if (!OnFilterTouchEventForSecurity(ev)) {
        return FALSE;
    }

    Int32 action;
    ev->GetAction(&action);
    Float xf;
    ev->GetX(&xf);
    Float yf;
    ev->GetY(&yf);
    Float scrolledXFloat = xf + mScrollX;
    Float scrolledYFloat = yf + mScrollY;
    CRect* frame = mTempRect.Get();

    Boolean disallowIntercept = (mGroupFlags & FLAG_DISALLOW_INTERCEPT) != 0;

    if (action == MotionEvent_ACTION_DOWN) {
        if (mMotionTarget != NULL) {
            // this is weird, we got a pen down, but we thought it was
            // already down!
            // XXX: We should probably send an ACTION_UP to the current
            // target.
            mMotionTarget = NULL;
        }

        // If we're disallowing intercept or if we're allowing and we didn't
        // intercept
        if (disallowIntercept || !OnInterceptTouchEvent(ev)) {
            // reset this event's action (just to protect ourselves)
            ev->SetAction(MotionEvent_ACTION_DOWN);

            // We know we want to dispatch the event down, find a child
            // who can handle it, start with the front-most child.
            Int32 scrolledXInt = (Int32)scrolledXFloat;
            Int32 scrolledYInt = (Int32)scrolledYFloat;

            Vector<AutoPtr<IView> >::Iterator iter = mChildren.Begin();
            for (; iter != mChildren.End(); ++iter) {
                View* child = (View*)(*iter).Get()->Probe(EIID_View);
                if ((child->mViewFlags & VISIBILITY_MASK) == VISIBLE
                    || child->GetAnimation() != NULL) {
                    child->GetHitRect(frame);
                    Boolean isContains;
                    frame->Contains(scrolledXInt, scrolledYInt, &isContains);
                    //printf("hitrect:%d, %d, %d, %d scrolledXInt = %d, scrolledYInt =%d\n",
                    //    frame->mLeft, frame->mRight, frame->mTop, frame->mBottom,
                    //    scrolledXInt, scrolledYInt);

                    if (isContains) {
                        // offset the event to the view's coordinate system
                        Float xc = scrolledXFloat - child->mLeft;
                        Float yc = scrolledYFloat - child->mTop;
                        ev->SetLocation(xc, yc);
                        child->mPrivateFlags &= ~CANCEL_NEXT_UP_EVENT;
                        if (child->DispatchTouchEvent(ev))  {
                            // Event handled, we have a target now.
                            mMotionTarget = *iter;
                            return TRUE;
                        }
                        // The event didn't get handled, try the next view.
                        // Don't reset the event's location, it's not
                        // necessary here.
                    }
                }
            }
        }
    }

    Boolean isUpOrCancel = (action == MotionEvent_ACTION_UP) ||
            (action == MotionEvent_ACTION_CANCEL);

    if (isUpOrCancel) {
        // Note, we've already copied the previous state to our local
        // variable, so this takes effect on the next event
        mGroupFlags &= ~FLAG_DISALLOW_INTERCEPT;
    }

    // The event wasn't an ACTION_DOWN, dispatch it to our target if
    // we have one.
    if (mMotionTarget == NULL) {
        // We don't have a target, this means we're handling the
        // event as a regular view.
        ev->SetLocation(xf, yf);
        if ((mPrivateFlags & CANCEL_NEXT_UP_EVENT) != 0) {
            ev->SetAction(MotionEvent_ACTION_CANCEL);
            mPrivateFlags &= ~CANCEL_NEXT_UP_EVENT;
        }

        return View::DispatchTouchEvent(ev);
    }
    View* target = (View*)mMotionTarget->Probe(EIID_View);

    // if have a target, see if we're allowed to and want to intercept its
    // events
    if (!disallowIntercept && OnInterceptTouchEvent(ev)) {
        Float xc = scrolledXFloat - (Float)target->mLeft;
        Float yc = scrolledYFloat - (Float)target->mTop;
        mPrivateFlags &= ~CANCEL_NEXT_UP_EVENT;
        ev->SetAction(MotionEvent_ACTION_CANCEL);
        ev->SetLocation(xc, yc);
        if (!target->DispatchTouchEvent(ev)) {
            // target didn't handle ACTION_CANCEL. not much we can do
            // but they should have.
        }
        // clear the target
        mMotionTarget = NULL;
        // Don't dispatch this event to our own view, because we already
        // saw it when intercepting; we just want to give the following
        // event to the normal onTouchEvent().
        return TRUE;
    }

    if (isUpOrCancel) {
        mMotionTarget = NULL;
    }

    // finally offset the event to the target's coordinate system and
    // dispatch the event.
    Float xc = scrolledXFloat - (Float)target->mLeft;
    Float yc = scrolledYFloat - (Float)target->mTop;
    ev->SetLocation(xc, yc);

    if ((target->mPrivateFlags & CANCEL_NEXT_UP_EVENT) != 0) {
        ev->SetAction(MotionEvent_ACTION_CANCEL);
        target->mPrivateFlags &= ~CANCEL_NEXT_UP_EVENT;
        mMotionTarget = NULL;
    }

    return target->DispatchTouchEvent(ev);
}

ECode ViewGroup::RequestDisallowInterceptTouchEvent(
    /* [in] */ Boolean disallowIntercept)
{
    if (disallowIntercept == ((mGroupFlags & FLAG_DISALLOW_INTERCEPT) != 0)) {
        // We're already in this state, assume our ancestors are too
        return NOERROR;
    }

    if (disallowIntercept) {
        mGroupFlags |= FLAG_DISALLOW_INTERCEPT;
    }
    else {
        mGroupFlags &= ~FLAG_DISALLOW_INTERCEPT;
    }

    // Pass it up to our parent
    if (mParent != NULL) {
        return mParent->RequestDisallowInterceptTouchEvent(disallowIntercept);
    }

    return NOERROR;
}

/**
 * Implement this method to intercept all touch screen motion events.  This
 * allows you to watch events as they are dispatched to your children, and
 * take ownership of the current gesture at any point.
 *
 * <p>Using this function takes some care, as it has a fairly complicated
 * interaction with {@link View#onTouchEvent(MotionEvent)
 * View.onTouchEvent(MotionEvent)}, and using it requires implementing
 * that method as well as this one in the correct way.  Events will be
 * received in the following order:
 *
 * <ol>
 * <li> You will receive the down event here.
 * <li> The down event will be handled either by a child of this view
 * group, or given to your own onTouchEvent() method to handle; this means
 * you should implement onTouchEvent() to return TRUE, so you will
 * continue to see the rest of the gesture (instead of looking for
 * a parent view to handle it).  Also, by returning TRUE from
 * onTouchEvent(), you will not receive any following
 * events in onInterceptTouchEvent() and all touch processing must
 * happen in onTouchEvent() like normal.
 * <li> For as long as you return FALSE from this function, each following
 * event (up to and including the final up) will be delivered first here
 * and then to the target's onTouchEvent().
 * <li> If you return TRUE from here, you will not receive any
 * following events: the target view will receive the same event but
 * with the action {@link MotionEvent#ACTION_CANCEL}, and all further
 * events will be delivered to your onTouchEvent() method and no longer
 * appear here.
 * </ol>
 *
 * @param ev The motion event being dispatched down the hierarchy.
 * @return Return TRUE to steal motion events from the children and have
 * them dispatched to this ViewGroup through onTouchEvent().
 * The current target will receive an ACTION_CANCEL event, and no further
 * messages will be delivered here.
 */
Boolean ViewGroup::OnInterceptTouchEvent(
    /* [in] */ IMotionEvent* ev)
{
    return FALSE;
}

/**
 *
 * Looks for a view to give focus to respecting the setting specified by
 * {@link #getDescendantFocusability()}.
 *
 * Uses {@link #onRequestFocusInDescendants(Int32, android.graphics.Rect)} to
 * find focus within the children of this group when appropriate.
 *
 * @see #FOCUS_BEFORE_DESCENDANTS
 * @see #FOCUS_AFTER_DESCENDANTS
 * @see #FOCUS_BLOCK_DESCENDANTS
 * @see #onRequestFocusInDescendants
 */
Boolean ViewGroup::RequestFocus(
    /* [in] */ Int32 direction,
    /* [in] */ IRect* previouslyFocusedRect)
{
    if (DBG) {
        Logger::D(VG_TAG, /*this + */
            StringBuffer("ViewGroup.requestFocus direction = ") + direction);
    }

    Int32 descendantFocusability = GetDescendantFocusability();

    switch (descendantFocusability) {
    case FOCUS_BLOCK_DESCENDANTS:
        return View::RequestFocus(direction, previouslyFocusedRect);
    case FOCUS_BEFORE_DESCENDANTS:
        {
            Boolean took = View::RequestFocus(direction, previouslyFocusedRect);
            return took ? took : OnRequestFocusInDescendants(
                direction, previouslyFocusedRect);
        }
    case FOCUS_AFTER_DESCENDANTS:
        {
            Boolean took = OnRequestFocusInDescendants(
                direction, previouslyFocusedRect);
            return took ? took : View::RequestFocus(
                direction, previouslyFocusedRect);
        }
    default:
        {
            Logger::E(VG_TAG, StringBuffer("descendant focusability must be ") +
                "one of FOCUS_BEFORE_DESCENDANTS, FOCUS_AFTER_DESCENDANTS" +
                ", FOCUS_BLOCK_DESCENDANTS but is " + descendantFocusability);

        }
    }
    return FALSE;
}

/**
  * Look for a descendant to call {@link View#requestFocus} on.
  * Called by {@link ViewGroup#requestFocus(Int32, android.graphics.Rect)}
  * when it wants to request focus within its children.  Override this to
  * customize how your {@link ViewGroup} requests focus within its children.
  * @param direction One of FOCUS_UP, FOCUS_DOWN, FOCUS_LEFT, and FOCUS_RIGHT
  * @param previouslyFocusedRect The rectangle (in this View's coordinate system)
  *        to give a finer grained hint about where focus is coming from.  May be NULL
  *        if there is no hint.
  * @return Whether focus was taken.
  */
Boolean ViewGroup::OnRequestFocusInDescendants(
    /* [in] */ Int32 direction,
    /* [in] */ IRect* previouslyFocusedRect)
{
    Int32 index;
    Int32 increment;
    Int32 end;
    Int32 count = mChildren.GetSize();
    if (count == 0) {
        return FALSE;
    }

    if ((direction & FOCUS_FORWARD) != 0) {
        index = 0;
        increment = 1;
        end = count;
    }
    else {
        index = count - 1;
        increment = -1;
        end = -1;
    }

    for (Int32 i = index; i != end; i += increment) {
        View* child = (View*)mChildren[i]->Probe(EIID_View);
        if ((child->mViewFlags & VISIBILITY_MASK) == VISIBLE) {
            if (child->RequestFocus(direction, previouslyFocusedRect)) {
                return TRUE;
            }
        }
    }

    return FALSE;
}

ECode ViewGroup::DispatchStartTemporaryDetach()
{
    View::DispatchStartTemporaryDetach();

    Vector<AutoPtr<IView> >::Iterator iter = mChildren.Begin();
    for (; iter != mChildren.End(); ++iter) {
        (*iter)->DispatchStartTemporaryDetach();
    }

    return NOERROR;
}

ECode ViewGroup::DispatchFinishTemporaryDetach()
{
    View::DispatchFinishTemporaryDetach();

    Vector<AutoPtr<IView> >::Iterator iter = mChildren.Begin();
    for (; iter != mChildren.End(); ++iter) {
        (*iter)->DispatchFinishTemporaryDetach();
    }

    return NOERROR;
}

void ViewGroup::DispatchAttachedToWindow(
    /* [in] */ AttachInfo* info,
    /* [in] */ Int32 visibility)
{
    View::DispatchAttachedToWindow(info, visibility);
    visibility |= mViewFlags & VISIBILITY_MASK;

    Vector<AutoPtr<IView> >::Iterator iter = mChildren.Begin();
    for (; iter != mChildren.End(); ++iter) {
        View* temp = (View*)(*iter)->Probe(EIID_View);
        temp->DispatchAttachedToWindow(info, visibility);
    }
}

Boolean ViewGroup::DispatchPopulateAccessibilityEvent(
    /* [in] */ IAccessibilityEvent* event)
{
    Boolean populated = FALSE;
    for (Int32 i = 0, count = GetChildCount(); i < count; i++) {
        View* tmp = (View*)GetChildAt(i)->Probe(EIID_View);
        populated |= tmp->DispatchPopulateAccessibilityEvent(event);
    }
    return populated;
}

ECode ViewGroup::DispatchDetachedFromWindow()
{
    // If we still have a motion target, we are still in the process of
    // dispatching motion events to a child; we need to get rid of that
    // child to avoid dispatching events to it after the window is torn
    // down. To make sure we keep the child in a consistent state, we
    // first send it an ACTION_CANCEL motion event.
    if (mMotionTarget != NULL) {
        const Int64 now = SystemClock::GetUptimeMillis();
        AutoPtr<CMotionEvent> event;
        ASSERT_SUCCEEDED(CMotionEvent::Obtain(now, now,
                MotionEvent_ACTION_CANCEL, 0.0f, 0.0f, 0, (CMotionEvent**)&event));
        Boolean result;
        mMotionTarget->DispatchTouchEvent((IMotionEvent*)event.Get(), &result);
        event->Recycle();
        mMotionTarget = NULL;
    }

    Vector<AutoPtr<IView> >::Iterator it;
    for (it = mChildren.Begin(); it != mChildren.End(); it++) {
        View* tmp = (View*)(*it)->Probe(EIID_View);
        FAIL_RETURN(tmp->DispatchDetachedFromWindow());
    }
    return View::DispatchDetachedFromWindow();
}

ECode ViewGroup::SetPadding(
    /* [in] */ Int32 left,
    /* [in] */ Int32 top,
    /* [in] */ Int32 right,
    /* [in] */ Int32 bottom)
{
    View::SetPadding(left, top, right, bottom);

    if ((mPaddingLeft | mPaddingTop | mPaddingRight | mPaddingRight) != 0) {
        mGroupFlags |= FLAG_PADDING_NOT_NULL;
    } else {
        mGroupFlags &= ~FLAG_PADDING_NOT_NULL;
    }

    return NOERROR;
}

ECode ViewGroup::DispatchSaveInstanceState(
    /* [in] */ IObjectIntegerMap* container)
{
    FAIL_RETURN(View::DispatchSaveInstanceState(container));
    Vector<AutoPtr<IView> >::Iterator it;
    for (it = mChildren.Begin(); it != mChildren.End(); it++) {
        View* tmp = (View*)(*it)->Probe(EIID_View);
        FAIL_RETURN(tmp->DispatchSaveInstanceState(container));
    }
    return NOERROR;
}

ECode ViewGroup::DispatchFreezeSelfOnly(
    /* [in] */ IObjectIntegerMap* container)
{
    return View::DispatchSaveInstanceState(container);
}

ECode ViewGroup::DispatchRestoreInstanceState(
    /* [in] */ IObjectIntegerMap* container)
{
    FAIL_RETURN(View::DispatchRestoreInstanceState(container));
    Vector<AutoPtr<IView> >::Iterator it;
    for (it = mChildren.Begin(); it != mChildren.End(); it++) {
        View* tmp = (View*)(*it)->Probe(EIID_View);
        FAIL_RETURN(tmp->DispatchRestoreInstanceState(container));
    }
    return NOERROR;
}

ECode ViewGroup::DispatchThawSelfOnly(
    /* [in] */ IObjectIntegerMap* container)
{
    return View::DispatchRestoreInstanceState(container);
}

/**
  * Enables or disables the drawing cache for each child of this view group.
  *
  * @param enabled TRUE to enable the cache, FALSE to dispose of it
  */
void ViewGroup::SetChildrenDrawingCacheEnabled(
    /* [in] */ Boolean enabled)
{
    if (enabled || (mPersistentDrawingCache & PERSISTENT_ALL_CACHES) != PERSISTENT_ALL_CACHES) {
        Vector<AutoPtr<IView> >::Iterator it;
        for (it = mChildren.Begin(); it != mChildren.End(); it++) {
            (*it)->SetDrawingCacheEnabled(enabled);
        }
    }
}

void ViewGroup::OnAnimationStart()
{
    View::OnAnimationStart();

    // When this ViewGroup's animation starts, build the cache for the children
    if ((mGroupFlags & FLAG_ANIMATION_CACHE) == FLAG_ANIMATION_CACHE) {
        Vector<AutoPtr<IView> >::Iterator it;
        for (it = mChildren.Begin(); it != mChildren.End(); it++) {
            View* child = (View*)(*it)->Probe(EIID_View);
            if ((child->mViewFlags & VISIBILITY_MASK) == VISIBLE) {
                child->SetDrawingCacheEnabled(TRUE);
                child->BuildDrawingCache(TRUE);
            }
        }

        mGroupFlags |= FLAG_CHILDREN_DRAWN_WITH_CACHE;
    }
}

void ViewGroup::OnAnimationEnd()
{
    View::OnAnimationEnd();

    // When this ViewGroup's animation ends, destroy the cache of the children
    if ((mGroupFlags & FLAG_ANIMATION_CACHE) == FLAG_ANIMATION_CACHE) {
        mGroupFlags &= ~FLAG_CHILDREN_DRAWN_WITH_CACHE;

        if ((mPersistentDrawingCache & PERSISTENT_ANIMATION_CACHE) == 0) {
            SetChildrenDrawingCacheEnabled(FALSE);
        }
    }
}

ECode ViewGroup::CreateSnapshot(
    /* [in] */ BitmapConfig quality,
    /* [in] */ Int32 backgroundColor,
    /* [in] */ Boolean skipChildren,
    /* [out] */ IBitmap** bitmap)
{
    Int32 count = mChildren.GetSize();
    Int32* visibilities = NULL;

    if (skipChildren) {
        visibilities = new Int32[count];
        for (Int32 i = 0; i < count; i++) {
            AutoPtr<IView> child = GetChildAt(i);
            child->GetVisibility(&visibilities[i]);
            if (visibilities[i] == View_VISIBLE) {
                child->SetVisibility(INVISIBLE);
            }
        }
    }

    AutoPtr<IBitmap> b;
    FAIL_RETURN(View::CreateSnapshot(quality, backgroundColor,
            skipChildren, (IBitmap**)&b));

    if (skipChildren) {
        for (Int32 i = 0; i < count; i++) {
            GetChildAt(i)->SetVisibility(visibilities[i]);
        }
    }

    delete[] visibilities;
    return NOERROR;;
}

void ViewGroup::DispatchDraw(
    /* [in] */ ICanvas* canvas)
{
    Int32 count = mChildren.GetSize();
    Vector<AutoPtr<IView> >& children = mChildren;
    Int32 flags = mGroupFlags;

    if ((flags & FLAG_RUN_ANIMATION) != 0 && CanAnimate()) {
        const Boolean cache = (mGroupFlags & FLAG_ANIMATION_CACHE) == FLAG_ANIMATION_CACHE;

        for (Int32 i = 0; i < count; i++) {
            View* child = (View*)children[i]->Probe(EIID_View);
            if ((child->mViewFlags & VISIBILITY_MASK) == VISIBLE) {
                const AutoPtr<IViewGroupLayoutParams> params = child->GetLayoutParams();
                AttachLayoutAnimationParameters((IView*)child->Probe(EIID_IView), params, i, count);
                BindLayoutAnimation((IView*)child->Probe(EIID_IView));
                if (cache) {
                    child->SetDrawingCacheEnabled(TRUE);
                    child->BuildDrawingCache(TRUE);
                }
            }
        }

        Boolean willOverlap;
        mLayoutAnimationController->WillOverlap(&willOverlap);
        if (willOverlap) {
            mGroupFlags |= FLAG_OPTIMIZE_INVALIDATE;
        }

        mLayoutAnimationController->Start();

        mGroupFlags &= ~FLAG_RUN_ANIMATION;
        mGroupFlags &= ~FLAG_ANIMATION_DONE;

        if (cache) {
            mGroupFlags |= FLAG_CHILDREN_DRAWN_WITH_CACHE;
        }

        if (mAnimationListener != NULL) {
            AutoPtr<IAnimation> animation;
            mLayoutAnimationController->GetAnimation((IAnimation**)&animation);
            mAnimationListener->OnAnimationStart(animation);
        }
    }

    Int32 saveCount = 0;
    const Boolean clipToPadding = (flags & CLIP_TO_PADDING_MASK) == CLIP_TO_PADDING_MASK;
    if (clipToPadding) {
        canvas->Save(&saveCount);
        Boolean isNonEmpty;
        canvas->ClipRectEx6(mScrollX + mPaddingLeft, mScrollY + mPaddingTop,
                mScrollX + mRight - mLeft - mPaddingRight,
                mScrollY + mBottom - mTop - mPaddingBottom, &isNonEmpty);

    }

    // We will draw our child's animation, let's reset the flag
    mPrivateFlags &= ~DRAW_ANIMATION;
    mGroupFlags &= ~FLAG_INVALIDATE_REQUIRED;

    Boolean more = FALSE;
    const Int64 drawingTime = GetDrawingTime();

    if ((flags & FLAG_USE_CHILD_DRAWING_ORDER) == 0) {
        for (Int32 i = 0; i < count; i++) {
            View* child = (View*)children[i]->Probe(EIID_View);
            if ((child->mViewFlags & VISIBILITY_MASK) == VISIBLE || child->GetAnimation() != NULL) {
                more |= DrawChild(canvas, (IView*)child->Probe(EIID_IView), drawingTime);
            }
        }
    }
    else {
        for (Int32 i = 0; i < count; i++) {
            View* child = (View*)children[GetChildDrawingOrder(count, i)]->Probe(EIID_View);
            if ((child->mViewFlags & VISIBILITY_MASK) == VISIBLE || child->GetAnimation() != NULL) {
                more |= DrawChild(canvas, (IView*)child->Probe(EIID_IView), drawingTime);
            }
        }
    }

    // Draw any disappearing views that have animations
    if (mDisappearingChildren != NULL) {
        List<AutoPtr<IView> >::ReverseIterator rit = mDisappearingChildren->RBegin();
        // Go backwards -- we may delete as animations finish
        for (; rit != mDisappearingChildren->REnd(); ++rit) {
            more |= DrawChild(canvas, *rit, drawingTime);
        }
    }

    if (clipToPadding) {
        canvas->RestoreToCount(saveCount);
    }

    // mGroupFlags might have been updated by drawChild()
    flags = mGroupFlags;

    if ((flags & FLAG_INVALIDATE_REQUIRED) == FLAG_INVALIDATE_REQUIRED) {
        Invalidate();
    }
//      if ((flags & FLAG_ANIMATION_DONE) == 0 && (flags & FLAG_NOTIFY_ANIMATION_LISTENER) == 0 &&
//              mLayoutAnimationController.isDone() && !more) {
//          // We want to erase the drawing cache and notify the listener after the
//          // next frame is drawn because one extra invalidate() is caused by
//          // drawChild() after the animation is over
//          mGroupFlags |= FLAG_NOTIFY_ANIMATION_LISTENER;
//          final Runnable end = new Runnable() {
//              public void run() {
//                  notifyAnimationListener();
//              }
//          };
//          post(end);
//      }
}

/**
  * Returns the index of the child to draw for this iteration. Override this
  * if you want to change the drawing order of children. By default, it
  * returns i.
  * <p>
  * NOTE: In order for this method to be called, you must enable child ordering
  * first by calling {@link #setChildrenDrawingOrderEnabled(Boolean)}.
  *
  * @param i The current iteration.
  * @return The index of the child to draw this iteration.
  *
  * @see #setChildrenDrawingOrderEnabled(Boolean)
  * @see #isChildrenDrawingOrderEnabled()
  */
Int32 ViewGroup::GetChildDrawingOrder(
    /* [in] */ Int32 childCount,
    /* [in] */ Int32 i)
{
    return i;
}

//private void notifyAnimationListener() {
//    mGroupFlags &= ~FLAG_NOTIFY_ANIMATION_LISTENER;
//    mGroupFlags |= FLAG_ANIMATION_DONE;
//
//    if (mAnimationListener != NULL) {
//        final Runnable end = new Runnable() {
//            public void run() {
//                mAnimationListener.onAnimationEnd(mLayoutAnimationController.getAnimation());
//            }
//        };
//        post(end);
//    }
//
//    if ((mGroupFlags & FLAG_ANIMATION_CACHE) == FLAG_ANIMATION_CACHE) {
//        mGroupFlags &= ~FLAG_CHILDREN_DRAWN_WITH_CACHE;
//        if ((mPersistentDrawingCache & PERSISTENT_ANIMATION_CACHE) == 0) {
//            setChildrenDrawingCacheEnabled(FALSE);
//        }
//    }
//
//    invalidate();
//}
//
/**
  * Draw one child of this View Group. This method is responsible for getting
  * the canvas in the right state. This includes clipping, translating so
  * that the child's scrolled origin is at 0, 0, and applying any animation
  * transformations.
  *
  * @param canvas The canvas on which to draw the child
  * @param child Who to draw
  * @param drawingTime The time at which draw is occuring
  * @return True if an invalidate() was issued
  */
Boolean ViewGroup::DrawChild(
    /* [in] */ ICanvas* canvas,
    /* [in] */ IView* _child,
    /* [in] */ Int64 drawingTime)
{
    View* child = (View*)_child->Probe(EIID_View);
    Boolean more = FALSE;

    const Int32 cl = child->mLeft;
    const Int32 ct = child->mTop;
    const Int32 cr = child->mRight;
    const Int32 cb = child->mBottom;

    //printf("child = 0x%08x, top = %d, bottom = %d\n", child, ct, cb);
    const Int32 flags = mGroupFlags;

    if ((flags & FLAG_CLEAR_TRANSFORMATION) == FLAG_CLEAR_TRANSFORMATION) {
        if (mChildTransformation != NULL) {
            mChildTransformation->Clear();
        }
        mGroupFlags &= ~FLAG_CLEAR_TRANSFORMATION;
    }

    AutoPtr<ITransformation> transformToApply;
    AutoPtr<IAnimation> anim = child->GetAnimation();
    Boolean concatMatrix = FALSE;

    if (anim != NULL) {
        Animation* a = (Animation*)anim->Probe(EIID_Animation);
        if (mInvalidateRegion == NULL) {
            ASSERT_SUCCEEDED(CRectF::NewByFriend((CRectF**)&mInvalidateRegion));
        }
        CRectF* region = mInvalidateRegion.Get();

        Boolean initialized = a->IsInitialized();
        if (!initialized) {
            a->Initialize(cr - cl, cb - ct, GetWidth(), GetHeight());
            a->InitializeInvalidateRegion(0, 0, cr - cl, cb - ct);
            child->OnAnimationStart();
        }

        if (mChildTransformation == NULL) {
            ASSERT_SUCCEEDED(CTransformation::New(
                (ITransformation**)&mChildTransformation));
        }
        more = a->GetTransformation(drawingTime, mChildTransformation);
        transformToApply = mChildTransformation;

        concatMatrix = a->WillChangeTransformationMatrix();

        if (more) {
            if (!a->WillChangeBounds()) {
                if ((flags & (FLAG_OPTIMIZE_INVALIDATE | FLAG_ANIMATION_DONE)) ==
                        FLAG_OPTIMIZE_INVALIDATE) {
                    mGroupFlags |= FLAG_INVALIDATE_REQUIRED;
                }
                else if ((flags & FLAG_INVALIDATE_REQUIRED) == 0) {
                    // The child need to draw an animation, potentially offscreen, so
                    // make sure we do not cancel invalidate requests
                    mPrivateFlags |= DRAW_ANIMATION;
                    Invalidate(cl, ct, cr, cb);
                }
            }
            else {
                a->GetInvalidateRegion(0, 0, cr - cl, cb - ct, region, transformToApply);

                // The child need to draw an animation, potentially offscreen, so
                // make sure we do not cancel invalidate requests
                mPrivateFlags |= DRAW_ANIMATION;

                Int32 left = cl + (Int32)region->mLeft;
                Int32 top = ct + (Int32)region->mTop;
                Invalidate(left, top, left + (Int32)region->GetWidth(),
                    top + (Int32)region->GetHeight());
            }
        }
    }
    else if ((flags & FLAG_SUPPORT_STATIC_TRANSFORMATIONS) ==
        FLAG_SUPPORT_STATIC_TRANSFORMATIONS) {
        if (mChildTransformation == NULL) {
            ASSERT_SUCCEEDED(CTransformation::New(
                (ITransformation**)&mChildTransformation));
        }
        Boolean hasTransform = GetChildStaticTransformation(_child, mChildTransformation);
        if (hasTransform) {
            Int32 transformType;
            mChildTransformation->GetTransformationType(&transformType);
            transformToApply = transformType != Transformation_TYPE_IDENTITY ?
                    mChildTransformation : NULL;
            concatMatrix = (transformType & Transformation_TYPE_MATRIX) != 0;
        }
    }

    // Sets the flag as early as possible to allow draw() implementations
    // to call invalidate() successfully when doing animations
    child->mPrivateFlags |= DRAWN;

    Boolean res;
    if (!concatMatrix && (canvas->QuickReject(cl, ct, cr, cb, CanvasEdgeType_BW, &res), res)
        && (child->mPrivateFlags & DRAW_ANIMATION) == 0) {
        return more;
    }

    child->ComputeScroll();

    const Int32 sx = child->mScrollX;
    const Int32 sy = child->mScrollY;

    Boolean scalingRequired = FALSE;
    AutoPtr<IBitmap> cache;
    if ((flags & FLAG_CHILDREN_DRAWN_WITH_CACHE) == FLAG_CHILDREN_DRAWN_WITH_CACHE ||
            (flags & FLAG_ALWAYS_DRAWN_WITH_CACHE) == FLAG_ALWAYS_DRAWN_WITH_CACHE) {
        cache = child->GetDrawingCache(TRUE);
        if (mAttachInfo != NULL) {
            scalingRequired = mAttachInfo->mScalingRequired;
        }
    }

    //TODO:
    const Boolean hasNoCache = TRUE;//cache == NULL;

    Int32 restoreTo;
    canvas->Save(&restoreTo);
    if (hasNoCache) {
        canvas->Translate(cl - sx, ct - sy);
    }
    else {
        canvas->Translate(cl, ct);
        if (scalingRequired) {
            // mAttachInfo cannot be NULL, otherwise scalingRequired == FALSE
            const Float scale = 1.0f / mAttachInfo->mApplicationScale;
            canvas->Scale(scale, scale);
        }
    }

    Float alpha = 1.0f;

    if (transformToApply != NULL) {
        if (concatMatrix) {
            Int32 transX = 0;
            Int32 transY = 0;
            if (hasNoCache) {
                transX = -sx;
                transY = -sy;
            }
            // Undo the scroll translation, apply the transformation matrix,
            // then redo the scroll translate to get the correct result.
            canvas->Translate(-transX, -transY);
            AutoPtr<IMatrix> matrix;
            transformToApply->GetMatrix((IMatrix**)&matrix);
            canvas->Concat(matrix);
            canvas->Translate(transX, transY);
            mGroupFlags |= FLAG_CLEAR_TRANSFORMATION;
        }

        transformToApply->GetAlpha(&alpha);
        if (alpha < 1.0f) {
            mGroupFlags |= FLAG_CLEAR_TRANSFORMATION;
        }

        if (alpha < 1.0f && hasNoCache) {
            Int32 multipliedAlpha = (Int32)(255 * alpha);
            if (!child->OnSetAlpha(multipliedAlpha)) {
                Int32 count;
                canvas->SaveLayerAlphaEx(sx, sy, sx + cr - cl, sy + cb - ct, multipliedAlpha,
                    Canvas_HAS_ALPHA_LAYER_SAVE_FLAG | Canvas_CLIP_TO_LAYER_SAVE_FLAG, &count);
            }
            else {
                child->mPrivateFlags |= ALPHA_SET;
            }
        }
    }
    else if ((child->mPrivateFlags & ALPHA_SET) == ALPHA_SET) {
        child->OnSetAlpha(255);
    }

    if ((flags & FLAG_CLIP_CHILDREN) == FLAG_CLIP_CHILDREN) {
        if (hasNoCache) {
            Boolean result;
            canvas->ClipRectEx6(sx, sy, sx + (cr - cl), sy + (cb - ct), &result);
        }
        else {
            Boolean result;
            if (!scalingRequired) {
                canvas->ClipRectEx6(0, 0, cr - cl, cb - ct, &result);
            }
            else {
                Int32 w, h;
                cache->GetWidth(&w);
                cache->GetHeight(&h);
                canvas->ClipRectEx6(0, 0, w, h, &result);
            }
        }
    }

    if (hasNoCache) {
        // Fast path for layouts with no backgrounds
        if ((child->mPrivateFlags & SKIP_DRAW) == SKIP_DRAW) {
//            if (ViewDebug.TRACE_HIERARCHY) {
//                ViewDebug.trace(this, ViewDebug.HierarchyTraceType.DRAW);
//            }
            child->mPrivateFlags &= ~DIRTY_MASK;
            child->DispatchDraw(canvas);
        }
        else {
            child->Draw(canvas);
        }
    }
    else {
        AutoPtr<IPaint> cachePaint = mCachePaint;
        if (alpha < 1.0f) {
            cachePaint->SetAlpha((Int32)(alpha * 255));
            mGroupFlags |= FLAG_ALPHA_LOWER_THAN_ONE;
        }
        else if  ((flags & FLAG_ALPHA_LOWER_THAN_ONE) == FLAG_ALPHA_LOWER_THAN_ONE) {
            cachePaint->SetAlpha(255);
            mGroupFlags &= ~FLAG_ALPHA_LOWER_THAN_ONE;
        }
//        if (Config.DEBUG && ViewDebug.profileDrawing) {
//            EventLog.writeEvent(60003, hashCode());
//        }
        canvas->DrawBitmap(cache, 0.0f, 0.0f, cachePaint);
    }

    canvas->RestoreToCount(restoreTo);

    if (anim != NULL && !more) {
        child->OnSetAlpha(255);
        FinishAnimatingView(_child, anim);
    }

    return more;
}

/**
  * By default, children are clipped to their bounds before drawing. This
  * allows view groups to override this behavior for animations, etc.
  *
  * @param clipChildren TRUE to clip children to their bounds,
  *        FALSE otherwise
  * @attr ref android.R.styleable#ViewGroup_clipChildren
  */
void ViewGroup::SetClipChildren(
    /* [in] */ Boolean clipChildren)
{
    SetBooleanFlag(FLAG_CLIP_CHILDREN, clipChildren);
}

/**
  * By default, children are clipped to the padding of the ViewGroup. This
  * allows view groups to override this behavior
  *
  * @param clipToPadding TRUE to clip children to the padding of the
  *        group, FALSE otherwise
  * @attr ref android.R.styleable#ViewGroup_clipToPadding
  */
void ViewGroup::SetClipToPadding(
    /* [in] */ Boolean clipToPadding)
{
    SetBooleanFlag(FLAG_CLIP_TO_PADDING, clipToPadding);
}

/**
  * {@inheritDoc}
  */
//@Override
void ViewGroup::DispatchSetSelected(
    /* [in] */ Boolean selected)
{
    Vector<AutoPtr<IView> >::Iterator it ;
    for (it = mChildren.Begin(); it != mChildren.End(); ++it) {
        (*it)->SetSelected(selected);
    }
}

//@Override
void ViewGroup::DispatchSetPressed(
    /* [in] */ Boolean pressed)
{
    Vector<AutoPtr<IView> >::Iterator it ;
    for (it = mChildren.Begin(); it != mChildren.End(); ++it) {
        (*it)->SetPressed(pressed);
    }
}

/**
  * When this property is set to TRUE, this ViewGroup supports static transformations on
  * children; this causes
  * {@link #getChildStaticTransformation(View, android.view.animation.Transformation)} to be
  * invoked when a child is drawn.
  *
  * Any subclass overriding
  * {@link #getChildStaticTransformation(View, android.view.animation.Transformation)} should
  * set this property to TRUE.
  *
  * @param enabled True to enable static transformations on children, FALSE otherwise.
  *
  * @see #FLAG_SUPPORT_STATIC_TRANSFORMATIONS
  */
void ViewGroup::SetStaticTransformationsEnabled(
    /* [in] */ Boolean enabled)
{
    SetBooleanFlag(FLAG_SUPPORT_STATIC_TRANSFORMATIONS, enabled);
}

Boolean ViewGroup::GetChildStaticTransformation(
    /* [in] */ IView* child,
    /* [in] */ ITransformation* t)
{
    return FALSE;
}

AutoPtr<IView> ViewGroup::FindViewTraversal(
    /* [in] */ Int32 id)
{
    if (id == mID) {
        return AutoPtr<IView>((IView*)this->Probe(EIID_IView));
    }

    Vector<AutoPtr<IView> >::Iterator iter = mChildren.Begin();
    for (; iter != mChildren.End(); ++iter) {
        Boolean isRootNS;
        (*iter)->IsRootNamespace(&isRootNS);
        if (!isRootNS) {
            AutoPtr<IView> vv;
            (*iter)->FindViewById(id, (IView**)&vv);
            if (vv != NULL) {
                return vv;
            }
        }
    }

    return AutoPtr<IView>(NULL);
}

///**
//    * {@hide}
//    */
//@Override
//protected View findViewWithTagTraversal(Object tag) {
//    if (tag != NULL && tag.equals(mTag)) {
//        return this;
//    }
//
//    final View[] where = mChildren;
//    final Int32 len = mChildrenCount;
//
//    for (Int32 i = 0; i < len; i++) {
//        View v = where[i];
//
//        if ((v.mPrivateFlags & IS_ROOT_NAMESPACE) == 0) {
//            v = v.findViewWithTag(tag);
//
//            if (v != NULL) {
//                return v;
//            }
//        }
//    }
//
//    return NULL;
//}
//
/**
 * Adds a child view. If no layout parameters are already set on the child, the
 * default parameters for this ViewGroup are set on the child.
 *
 * @param child the child view to add
 *
 * @see #generateDefaultLayoutParams()
 */
ECode ViewGroup::AddView(
    /* [in] */ IView* child)
{
    return AddView(child, -1);
}

/**
 * Adds a child view. If no layout parameters are already set on the child, the
 * default parameters for this ViewGroup are set on the child.
 *
 * @param child the child view to add
 * @param index the position at which to add the child
 *
 * @see #generateDefaultLayoutParams()
 */
ECode ViewGroup::AddView(
    /* [in] */ IView* child,
    /* [in] */ Int32 index)
{
    AutoPtr<IViewGroupLayoutParams> params;
    child->GetLayoutParams((IViewGroupLayoutParams**)&params);
    if (params == NULL) {
        GenerateDefaultLayoutParams((IViewGroupLayoutParams**)&params);
        if (params == NULL) {
//            throw new IllegalArgumentException("generateDefaultLayoutParams() cannot return NULL");
            return E_ILLEGAL_ARGUMENT_EXCEPTION;
        }
    }
    return AddView(child, index, params.Get());
}

/**
 * Adds a child view with this ViewGroup's default layout parameters and the
 * specified width and height.
 *
 * @param child the child view to add
 */
ECode ViewGroup::AddView(
    /* [in] */ IView* child,
    /* [in] */ Int32 width,
    /* [in] */ Int32 height)
{
    AutoPtr<IViewGroupLayoutParams> params;
    GenerateDefaultLayoutParams((IViewGroupLayoutParams**)&params);
    ((CViewGroupLayoutParams*)params.Get())->mWidth = width;
    ((CViewGroupLayoutParams*)params.Get())->mHeight = height;
    return AddView(child, -1, params.Get());
}

/**
 * Adds a child view with the specified layout parameters.
 *
 * @param child the child view to add
 * @param params the layout parameters to set on the child
 */
ECode ViewGroup::AddView(
    /* [in] */ IView* child,
    /* [in] */ IViewGroupLayoutParams* params)
{
    return AddView(child, -1, params);
}

/**
 * Adds a child view with the specified layout parameters.
 *
 * @param child the child view to add
 * @param index the position at which to add the child
 * @param params the layout parameters to set on the child
 */
ECode ViewGroup::AddView(
    /* [in] */ IView* child,
    /* [in] */ Int32 index,
    /* [in] */ IViewGroupLayoutParams* params)
{
    //if (DBG) {
    //    System.out.println(this + " addView");
    //}

    // addViewInner() will call child.requestLayout() when setting the new LayoutParams
    // therefore, we call requestLayout() on ourselves before, so that the child's request
    // will be blocked at our level
    RequestLayout();
    Invalidate();
    return AddViewInner(child, index, params, FALSE);
}

/**
  * {@inheritDoc}
  */
ECode ViewGroup::UpdateViewLayout(
    /* [in] */ IView* view,
    /* [in] */ IViewGroupLayoutParams* params)
{
    if (!CheckLayoutParams(params)) {
//        throw new IllegalArgumentException("Invalid LayoutParams supplied to " + this);
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    AutoPtr<IViewParent> parent;
    view->GetParent((IViewParent**)&parent);
    if (parent.Get() != (IViewParent*)this->Probe(EIID_IViewParent)) {
//        throw new IllegalArgumentException("Given view not a child of " + this);
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    return view->SetLayoutParams(params);
}

Boolean ViewGroup::CheckLayoutParams(
    /* [in] */ IViewGroupLayoutParams* p)
{
    return  p != NULL;
}

ECode ViewGroup::SetOnHierarchyChangeListener(
    /* [in] */ IViewGroupOnHierarchyChangeListener* listener)
{
    mOnHierarchyChangeListener = listener;
    return NOERROR;
}

Boolean ViewGroup::AddViewInLayout(
    /* [in] */ IView* child,
    /* [in] */ Int32 index,
    /* [in] */ IViewGroupLayoutParams* params)
{
    return AddViewInLayout(child, index, params, FALSE);
}

Boolean ViewGroup::AddViewInLayout(
    /* [in] */ IView* child,
    /* [in] */ Int32 index,
    /* [in] */ IViewGroupLayoutParams* params,
    /* [in] */ Boolean preventRequestLayout)
{
    View* v = (View*)child->Probe(EIID_View);
    v->mParent = NULL;
    AddViewInner(child, index, params, preventRequestLayout);
    v->mPrivateFlags = (v->mPrivateFlags & ~DIRTY_MASK) | DRAWN;
    return TRUE;
}


void ViewGroup::CleanupLayoutState(
    /* [in] */ IView* child)
{
    View* v = (View*)child->Probe(EIID_View);
    v->mPrivateFlags &= ~View::FORCE_LAYOUT;
}

ECode ViewGroup::AddViewInner(
    /* [in] */ IView* child,
    /* [in] */ Int32 index,
    /* [in] */ IViewGroupLayoutParams* _params,
    /* [in] */ Boolean preventRequestLayout)
{
    AutoPtr<IViewGroupLayoutParams> params = _params;
    View* v = (View*)child->Probe(EIID_View);

    if (v->GetParent() != NULL) {
//        throw new IllegalStateException("The specified child already has a parent. " +
//                "You must call removeView() on the child's parent first.");
        return E_ILLEGAL_STATE_EXCEPTION;
    }

    if (!CheckLayoutParams(params)) {
        params = GenerateLayoutParams(params);
    }

    if (preventRequestLayout) {
        v->mLayoutParams = params;
    } else {
        v->SetLayoutParams(params);
    }

    if (index < 0) {
        index = mChildren.GetSize();
    }

    AddInArray(child, index);

    // tell our children
    if (preventRequestLayout) {
        v->AssignParent((IViewParent*)this->Probe(EIID_IViewParent));
    } else {
        v->mParent = (IViewParent*)this->Probe(EIID_IViewParent);
    }

    if (v->HasFocus()) {
        RequestChildFocus(child, v->FindFocus());
    }

    AttachInfo* ai = mAttachInfo;
    if (ai != NULL) {
        Boolean lastKeepOn = ai->mKeepScreenOn;
        ai->mKeepScreenOn = FALSE;
        v->DispatchAttachedToWindow(mAttachInfo, (mViewFlags & VISIBILITY_MASK));
        if (ai->mKeepScreenOn) {
            NeedGlobalAttributesUpdate(TRUE);
        }
        ai->mKeepScreenOn = lastKeepOn;
    }

    if (mOnHierarchyChangeListener != NULL) {
        mOnHierarchyChangeListener->OnChildViewAdded((IView*)this->Probe(EIID_IView), child);
    }

    if ((v->mViewFlags & DUPLICATE_PARENT_STATE) == DUPLICATE_PARENT_STATE) {
        mGroupFlags |= FLAG_NOTIFY_CHILDREN_ON_DRAWABLE_STATE_CHANGE;
    }
    return NOERROR;
}

ECode ViewGroup::AddInArray(
    /* [in] */ IView* child,
    /* [in] */ Int32 index)
{
    if (child == NULL) {
        return E_INVALID_ARGUMENT;
    }

    Int32 count = mChildren.GetSize();

    if (index <= count) {
        mChildren.Insert(mChildren.Begin() + index, child);
    }
    else {
        Logger::E(VG_TAG, StringBuffer("IndexOutOfBoundsException index = ")
            + index + ", count = " + count);

        return E_INDEX_OUT_OF_BOUNDS_EXCEPTION;
    }

    return NOERROR;
}

// This method also sets the child's mParent to NULL
ECode ViewGroup::RemoveFromArray(
    /* [in] */ Int32 index)
{
    if (index < (Int32)mChildren.GetSize()) {
        View* temp = (View*)mChildren[index]->Probe(EIID_View);
        temp->mParent = NULL;
        mChildren.Erase(mChildren.Begin() + index);
    }
    else {
        return E_INDEX_OUT_OF_BOUNDS_EXCEPTION;
    }

    return NOERROR;
}

// This method also sets the children's mParent to NULL
ECode ViewGroup::RemoveFromArray(
    /* [in] */ Int32 start,
    /* [in] */ Int32 count)
{
    start = Math::Max(0, start);
    const Int32 end = Math::Min(mChildren.GetSize(), start + count);

    if (start == end) {
        return NOERROR;
    }

    Vector<AutoPtr<IView> >::Iterator it = mChildren.Begin() + start;
    Vector<AutoPtr<IView> >::Iterator endPos = mChildren.Begin() + end;
    for (; it != endPos; ++it) {
        View* v = (View*)(*it)->Probe(EIID_View);
        v->mParent = NULL;
    }

    mChildren.Erase(mChildren.Begin() + start, mChildren.Begin() + end);

    return NOERROR;
}

void ViewGroup::BindLayoutAnimation(
    /* [in] */ IView* child)
{
//    Animation a = mLayoutAnimationController.getAnimationForView(child);
//    child.setAnimation(a);
}


ECode ViewGroup::AttachLayoutAnimationParameters(
    /* [in] */ IView* child,
    /* [in] */ IViewGroupLayoutParams* params,
    /* [in] */ Int32 index,
    /* [in] */ Int32 count)
{
    LayoutAnimationController::AnimationParameters* animationParams =
                ((CViewGroupLayoutParams*)params)->mLayoutAnimationParameters;
    if (animationParams == NULL) {
        animationParams = new LayoutAnimationController::AnimationParameters();
        ((CViewGroupLayoutParams*)params)->mLayoutAnimationParameters = animationParams;
    }

    animationParams->mCount = count;
    animationParams->mIndex = index;

    return NOERROR;
}

ECode ViewGroup::RemoveView(
    /* [in] */ IView* view)
{
    RemoveViewInternal(view);
    RequestLayout();
    Invalidate();
    return NOERROR;
}

ECode ViewGroup::RemoveViewInLayout(
    /* [in] */ IView* view)
{
    RemoveViewInternal(view);

    return NOERROR;
}

ECode ViewGroup::RemoveViewsInLayout(
    /* [in] */ Int32 start,
    /* [in] */ Int32 count)
{
    RemoveViewsInternal(start, count);

    return NOERROR;
}


ECode ViewGroup::RemoveViewAt(
    /* [in] */ Int32 index)
{
    RemoveViewInternal(index, GetChildAt(index));
    RequestLayout();
    Invalidate();
    return NOERROR;
}

ECode ViewGroup::RemoveViews(
    /* [in] */ Int32 start,
    /* [in] */ Int32 count)
{
    RemoveViewsInternal(start, count);
    RequestLayout();
    Invalidate();

    return NOERROR;
}

void ViewGroup::RemoveViewInternal(
    /* [in] */ IView* view)
{
    const Int32 index = IndexOfChild(view);
    if (index >= 0) {
        RemoveViewInternal(index, view);
    }
}

void ViewGroup::RemoveViewInternal(
    /* [in] */ Int32 index,
    /* [in] */ IView* view)
{
    View* v = (View*)view->Probe(EIID_View);
    Boolean clearChildFocus = FALSE;
    if (view == mFocused) {
        v->ClearFocusForRemoval();
        clearChildFocus = TRUE;
    }

    if (v->GetAnimation() != NULL) {
        AddDisappearingView(view);
    }
    else if (v->mAttachInfo != NULL) {
        v->DispatchDetachedFromWindow();
    }

    if (mOnHierarchyChangeListener != NULL) {
        mOnHierarchyChangeListener->OnChildViewRemoved(
                (IView*)this->Probe(EIID_IView), view);
    }

    NeedGlobalAttributesUpdate(FALSE);

    RemoveFromArray(index);

    if (clearChildFocus) {
        ClearChildFocus(view);
    }
}

void ViewGroup::RemoveViewsInternal(
    /* [in] */ Int32 start,
    /* [in] */ Int32 count)
{
    const Boolean notifyListener = mOnHierarchyChangeListener != NULL;
    const IView* focused = mFocused;
    const Boolean detach = mAttachInfo != NULL;
    IView* clearChildFocus = NULL;

    const Int32 end = start + count;

    Vector<AutoPtr<IView> >::Iterator startPos = mChildren.Begin() + start;
    Vector<AutoPtr<IView> >::Iterator endPos = mChildren.Begin() + end;
    Vector<AutoPtr<IView> >::Iterator it;
    for (it = startPos; it != endPos; it++) {
        IView* view = *it;
        View* v = (View*)view->Probe(EIID_View);

        if (view == focused) {
            v->ClearFocusForRemoval();
            clearChildFocus = view;
        }

        if (v->GetAnimation() != NULL) {
            AddDisappearingView(view);
        }
        else if (detach) {
            v->DispatchDetachedFromWindow();
        }

        NeedGlobalAttributesUpdate(FALSE);

        if (notifyListener) {
            mOnHierarchyChangeListener->OnChildViewRemoved(
                    (IView*)this->Probe(EIID_IView), view);
        }
    }

    RemoveFromArray(start, count);

    if (clearChildFocus != NULL) {
        ClearChildFocus(clearChildFocus);
    }
}

ECode ViewGroup::RemoveAllViews()
{
    RemoveAllViewsInLayout();
    RequestLayout();
    Invalidate();
    return NOERROR;
}


ECode ViewGroup::RemoveAllViewsInLayout()
{
    if (mChildren.GetSize() <= 0) {
        return NOERROR;
    }

    mChildrenCount = 0;

    const Boolean notify = mOnHierarchyChangeListener != NULL;
    const IView* focused = mFocused;
    const Boolean detach = mAttachInfo != NULL;
    IView* clearChildFocus = NULL;

    NeedGlobalAttributesUpdate(FALSE);

    Vector<AutoPtr<IView> >::ReverseIterator rit= mChildren.RBegin();
    while (rit != mChildren.REnd()) {
        IView* view = *rit;
        View* v = (View*)view->Probe(EIID_View);

        if (view == focused) {
            v->ClearFocusForRemoval();
            clearChildFocus = view;
        }

        if (v->GetAnimation() != NULL) {
            AddDisappearingView(view);
        }
        else if (detach) {
            v->DispatchDetachedFromWindow();
        }

        if (notify) {
            mOnHierarchyChangeListener->OnChildViewRemoved(
                (IView*)this->Probe(EIID_IView), view);
        }

        v->mParent = NULL;
        Vector<AutoPtr<IView> >::Iterator it = (++rit).GetBase();
        it = mChildren.Erase(it);
        rit = Vector<AutoPtr<IView> >::ReverseIterator(it);
    }

    if (clearChildFocus != NULL) {
        ClearChildFocus(clearChildFocus);
    }

    return NOERROR;
}

void ViewGroup::RemoveDetachedView(
    /* [in] */ IView* child,
    /* [in] */ Boolean animate)
{
    assert(child != NULL);
    View* v = (View*)child->Probe(EIID_View);

    if (child == mFocused) {
        v->ClearFocus();
    }

    if (animate && v->GetAnimation() != NULL) {
        AddDisappearingView(child);
    }
    else if (v->mAttachInfo != NULL) {
        v->DispatchDetachedFromWindow();
    }

    if (mOnHierarchyChangeListener != NULL) {
        mOnHierarchyChangeListener->OnChildViewRemoved(
            (IView*)this->Probe(EIID_IView), child);
    }
}

void ViewGroup::AttachViewToParent(
    /* [in] */ IView* child,
    /* [in] */ Int32 index,
    /* [in] */ IViewGroupLayoutParams* params)
{
    assert(child != NULL);
    View* v = (View*)child->Probe(EIID_View);

    v->mLayoutParams = params;

    if (index < 0) {
        index = mChildren.GetSize();
    }

    AddInArray(child, index);

    v->mParent = (IViewParent*)this->Probe(EIID_IViewParent);
    v->mPrivateFlags = (v->mPrivateFlags & ~DIRTY_MASK & ~DRAWING_CACHE_VALID) | DRAWN;

    if (v->HasFocus()) {
        RequestChildFocus(child, v->FindFocus());
    }
}

void ViewGroup::DetachViewFromParent(
    /* [in] */ IView* child)
{
    RemoveFromArray(IndexOfChild(child));
}

void ViewGroup::DetachViewFromParent(
    /* [in] */ Int32 index)
{
    RemoveFromArray(index);
}

void ViewGroup::DetachViewsFromParent(
    /* [in] */ Int32 start,
    /* [in] */ Int32 count)
{
    RemoveFromArray(start, count);
}

void ViewGroup::DetachAllViewsFromParent()
{
    if (mChildren.GetSize() <= 0) {
        return;
    }

    Vector<AutoPtr<IView> >::ReverseIterator rit= mChildren.RBegin();
    while (rit != mChildren.REnd()) {
        View* v = (View*)(*rit)->Probe(EIID_View);
        v->mParent = NULL;
        Vector<AutoPtr<IView> >::Iterator it = mChildren.Erase((++rit).GetBase());
        rit = Vector<AutoPtr<IView> >::ReverseIterator(it);
    }
}

ECode ViewGroup::InvalidateChild(
    /* [in] */ IView* child,
    /* [in] */ IRect* dirty)
{
    assert(child != NULL);
    View* v = (View*)child->Probe(EIID_View);

    //if (ViewDebug.TRACE_HIERARCHY) {
    //    ViewDebug.trace(this, ViewDebug.HierarchyTraceType.INVALIDATE_CHILD);
    //}

    AutoPtr<IViewParent> parent = (IViewParent*)this->Probe(EIID_IViewParent);

    AttachInfo* attachInfo = mAttachInfo;
    if (attachInfo != NULL) {
        ArrayOf<Int32> location(attachInfo->mInvalidateChildLocation, 2);
        location[CHILD_LEFT_INDEX] = v->mLeft;
        location[CHILD_TOP_INDEX] = v->mTop;

        // If the child is drawing an animation, we want to copy this flag onto
        // ourselves and the parent to make sure the invalidate request goes
        // through
        const Boolean drawAnimation = (v->mPrivateFlags & DRAW_ANIMATION) == DRAW_ANIMATION;

        // Check whether the child that requests the invalidate is fully opaque
        const Boolean isOpaque = v->IsOpaque() && !drawAnimation &&
                v->GetAnimation() != NULL;
        // Mark the child as dirty, using the appropriate flag
        // Make sure we do not set both flags at the same time
        const Int32 opaqueFlag = isOpaque ? DIRTY_OPAQUE : DIRTY;

        do {
            View* view = NULL;
            if (parent->Probe(EIID_View) != NULL) {
                view = (View*)parent->Probe(EIID_View);
            }

            if (drawAnimation) {
                if (view != NULL) {
                    view->mPrivateFlags |= DRAW_ANIMATION;
                }
                else if (parent->Probe(EIID_ViewRoot) != NULL) {
                    ViewRoot* vr = reinterpret_cast<ViewRoot*>(parent->Probe(EIID_ViewRoot));
                    vr->mIsAnimating = TRUE;
                }
            }

            // If the parent is dirty opaque or not dirty, mark it dirty with the opaque
            // flag coming from the child that initiated the invalidate
            if (view != NULL && (view->mPrivateFlags & DIRTY_MASK) != DIRTY) {
                view->mPrivateFlags = (view->mPrivateFlags & ~DIRTY_MASK) | opaqueFlag;
            }

            AutoPtr<IViewParent> tmp;
            parent->InvalidateChildInParent(&location, dirty, (IViewParent**)&tmp);
            parent = tmp;
        } while (parent != NULL);
    }
    return NOERROR;
}

AutoPtr<IViewParent> ViewGroup::InvalidateChildInParent(
        /* [in] */ ArrayOf<Int32>* location,
        /* [in] */ IRect* dirty)
{
//    if (ViewDebug.TRACE_HIERARCHY) {
//        ViewDebug.trace(this, ViewDebug.HierarchyTraceType.INVALIDATE_CHILD_IN_PARENT);
//    }

    if ((mPrivateFlags & DRAWN) == DRAWN) {
        if ((mGroupFlags & (FLAG_OPTIMIZE_INVALIDATE | FLAG_ANIMATION_DONE)) !=
                    FLAG_OPTIMIZE_INVALIDATE) {
            dirty->Offset((*location)[CHILD_LEFT_INDEX] - mScrollX,
                        (*location)[CHILD_TOP_INDEX] - mScrollY);

            const Int32 left = mLeft;
            const Int32 top = mTop;

            Boolean result;
            dirty->Intersect(0, 0, mRight - left, mBottom - top, &result);
            if (result || (mPrivateFlags & DRAW_ANIMATION) == DRAW_ANIMATION) {
                mPrivateFlags &= ~DRAWING_CACHE_VALID;

                (*location)[CHILD_LEFT_INDEX] = left;
                (*location)[CHILD_TOP_INDEX] = top;

                return mParent;
            }
        }
        else {
            mPrivateFlags &= ~DRAWN & ~DRAWING_CACHE_VALID;

            (*location)[CHILD_LEFT_INDEX] = mLeft;
            (*location)[CHILD_TOP_INDEX] = mTop;

            dirty->Set(0, 0, mRight - (*location)[CHILD_LEFT_INDEX],
                    mBottom - (*location)[CHILD_TOP_INDEX]);

            return mParent;
        }
    }

    return NULL;
}

ECode ViewGroup::OffsetDescendantRectToMyCoords(
    /* [in] */ IView* descendant,
    /* [in] */ IRect* rect)
{
    return OffsetRectBetweenParentAndChild(descendant, rect, TRUE, FALSE);
}

ECode ViewGroup::OffsetRectIntoDescendantCoords(
    /* [in] */ IView* descendant,
    /* [in] */ IRect* rect)
{
    return OffsetRectBetweenParentAndChild(descendant, rect, FALSE, FALSE);
}

ECode ViewGroup::OffsetRectBetweenParentAndChild(
    /* [in] */ IView* descendant,
    /* [in] */ IRect* rect,
    /* [in] */ Boolean offsetFromChildToParent,
    /* [in] */ Boolean clipToBounds)
{
    assert(descendant != NULL);

    // already in the same coord system :)
    if (descendant == (IView*)this->Probe(EIID_IView)) {
        return NOERROR;
    }

    View* v = (View*)descendant->Probe(EIID_View);
    AutoPtr<IViewParent> theParent = v->mParent;

    // search and offset up to the parent
    while ((theParent != NULL)
            && (theParent->Probe(EIID_View) != NULL)
            && (theParent.Get() != (IViewParent*)this->Probe(EIID_IViewParent))) {

        if (offsetFromChildToParent) {
            rect->Offset(v->mLeft - v->mScrollX, v->mTop - v->mScrollY);
            if (clipToBounds) {
                View* p = (View*)theParent->Probe(EIID_View);
                Boolean result;
                rect->Intersect(0, 0, p->mRight - p->mLeft, p->mBottom - p->mTop, &result);
            }
        }
        else {
            if (clipToBounds) {
                View* p = (View*)theParent->Probe(EIID_View);
                Boolean result;
                rect->Intersect(0, 0, p->mRight - p->mLeft, p->mBottom - p->mTop, &result);
            }
            rect->Offset(v->mScrollX - v->mLeft, v->mScrollY - v->mTop);
        }

        v = (View*)theParent->Probe(EIID_View);
        theParent = v->mParent;
    }

    // now that we are up to this view, need to offset one more time
    // to get into our coordinate space
    if (theParent.Get() == (IViewParent*)this->Probe(EIID_IViewParent)) {
        if (offsetFromChildToParent) {
            rect->Offset(v->mLeft - v->mScrollX, v->mTop - v->mScrollY);
        } else {
            rect->Offset(v->mScrollX - v->mLeft, v->mScrollY - v->mTop);
        }
    } else {
//        throw new IllegalArgumentException("parameter must be a descendant of this view");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    return NOERROR;
}

ECode ViewGroup::OffsetChildrenTopAndBottom(
    /* [in] */ Int32 offset)
{
    Vector<AutoPtr<IView> >::Iterator it;
    for (it = mChildren.Begin(); it != mChildren.End(); ++it) {
        View* v = (View*)(*it)->Probe(EIID_View);
        v->mTop += offset;
        v->mBottom += offset;
    }
    return NOERROR;
}

ECode ViewGroup::GetChildVisibleRect(
    /* [in] */ IView* child,
    /* [in] */ IRect* r,
    /* [in] */ IPoint* offset,
    /* [out] */ Boolean* result)
{
    assert(child != NULL);
    assert(result != NULL);
    View* v = (View*)child->Probe(EIID_View);

    Int32 dx = v->mLeft - mScrollX;
    Int32 dy = v->mTop - mScrollY;
    if (offset != NULL) {
        ((CPoint*)offset)->mX += dx;
        ((CPoint*)offset)->mY += dy;
    }
    r->Offset(dx, dy);

    ASSERT_SUCCEEDED(r->Intersect(0, 0, mRight - mLeft, mBottom - mTop, result));
    if (!*result || mParent == NULL) return NOERROR;

    return mParent->GetChildVisibleRect(
            (IView*)this->Probe(EIID_IView), r, offset, result);
}

Boolean ViewGroup::CanAnimate()
{
    return mLayoutAnimationController != NULL;
}

ECode ViewGroup::StartLayoutAnimation()
{
    if (mLayoutAnimationController != NULL) {
        mGroupFlags |= FLAG_RUN_ANIMATION;
        RequestLayout();
    }
    return NOERROR;
}

ECode ViewGroup::ScheduleLayoutAnimation()
{
    mGroupFlags |= FLAG_RUN_ANIMATION;
    return NOERROR;
}

ECode ViewGroup::SetLayoutAnimation(
    /* [in] */ ILayoutAnimationController* controller)
{
    mLayoutAnimationController = controller;
    if (mLayoutAnimationController != NULL) {
        mGroupFlags |= FLAG_RUN_ANIMATION;
    }
    return NOERROR;
}

AutoPtr<ILayoutAnimationController> ViewGroup::GetLayoutAnimation()
{
    return mLayoutAnimationController;
}

Boolean ViewGroup::IsAnimationCacheEnabled()
{
    return (mGroupFlags & FLAG_ANIMATION_CACHE) == FLAG_ANIMATION_CACHE;
}

ECode ViewGroup::SetAnimationCacheEnabled(
    /* [in] */ Boolean enabled)
{
    SetBooleanFlag(FLAG_ANIMATION_CACHE, enabled);
    return NOERROR;
}

Boolean ViewGroup::IsAlwaysDrawnWithCacheEnabled()
{
    return (mGroupFlags & FLAG_ALWAYS_DRAWN_WITH_CACHE) == FLAG_ALWAYS_DRAWN_WITH_CACHE;
}

ECode ViewGroup::SetAlwaysDrawnWithCacheEnabled(
    /* [in] */ Boolean always)
{
    SetBooleanFlag(FLAG_ALWAYS_DRAWN_WITH_CACHE, always);
    return NOERROR;
}

Boolean ViewGroup::IsChildrenDrawnWithCacheEnabled()
{
    return (mGroupFlags & FLAG_CHILDREN_DRAWN_WITH_CACHE) == FLAG_CHILDREN_DRAWN_WITH_CACHE;
}

void ViewGroup::SetChildrenDrawnWithCacheEnabled(
    /* [in] */ Boolean enabled)
{
    SetBooleanFlag(FLAG_CHILDREN_DRAWN_WITH_CACHE, enabled);
}

Boolean ViewGroup::IsChildrenDrawingOrderEnabled()
{
    return (mGroupFlags & FLAG_USE_CHILD_DRAWING_ORDER) == FLAG_USE_CHILD_DRAWING_ORDER;
}

void ViewGroup::SetChildrenDrawingOrderEnabled(
    /* [in] */ Boolean enabled)
{
    SetBooleanFlag(FLAG_USE_CHILD_DRAWING_ORDER, enabled);
}

void ViewGroup::SetBooleanFlag(
    /* [in] */ Int32 flag,
    /* [in] */ Boolean value)
{
    if (value) {
        mGroupFlags |= flag;
    } else {
        mGroupFlags &= ~flag;
    }
}

Int32 ViewGroup::GetPersistentDrawingCache()
{
    return mPersistentDrawingCache;
}

ECode ViewGroup::SetPersistentDrawingCache(
    /* [in] */ Int32 drawingCacheToKeep)
{
    mPersistentDrawingCache = drawingCacheToKeep & PERSISTENT_ALL_CACHES;
    return NOERROR;
}

AutoPtr<IViewGroupLayoutParams> ViewGroup::GenerateLayoutParams(
    /* [in] */ IAttributeSet* attrs)
{
    AutoPtr<IViewGroupLayoutParams> params;
    ASSERT_SUCCEEDED(CViewGroupLayoutParams::New(
            GetContext(), attrs, (IViewGroupLayoutParams**)&params));
    return params;
}

AutoPtr<IViewGroupLayoutParams> ViewGroup::GenerateLayoutParams(
    /* [in] */ IViewGroupLayoutParams* p)
{
    return AutoPtr<IViewGroupLayoutParams>(p);
}

/**
 * Returns a set of default layout parameters. These parameters are requested
 * when the View passed to {@link #addView(View)} has no layout parameters
 * already set. If NULL is returned, an exception is thrown from addView.
 *
 * @return a set of default layout parameters or NULL
 */
ECode ViewGroup::GenerateDefaultLayoutParams(
    /* [out] */ IViewGroupLayoutParams** params)
{
    return CViewGroupLayoutParams::New(
            ViewGroupLayoutParams_WRAP_CONTENT,
            ViewGroupLayoutParams_WRAP_CONTENT, params);
}

Boolean ViewGroup::DispatchConsistencyCheck(
    /* [in] */ Int32 consistency)
{
    Boolean result = View::DispatchConsistencyCheck(consistency);

    Vector<AutoPtr<IView> >::Iterator it;
    for (it = mChildren.Begin(); it != mChildren.End(); it++) {
        View* v = (View*)(*it)->Probe(EIID_View);
        if (!v->DispatchConsistencyCheck(consistency)) result = FALSE;
    }

    return result;
}

Boolean ViewGroup::OnConsistencyCheck(
    /* [in] */ Int32 consistency)
{
    Boolean result = View::OnConsistencyCheck(consistency);

    const Boolean checkLayout = (consistency/* & ViewDebug.CONSISTENCY_LAYOUT*/) != 0;
    const Boolean checkDrawing = (consistency/* & ViewDebug.CONSISTENCY_DRAWING*/) != 0;

    if (checkLayout) {
        Vector<AutoPtr<IView> >::Iterator it;
        for (it = mChildren.Begin(); it != mChildren.End(); it++) {
            View* v = (View*)(*it)->Probe(EIID_View);
            if ((v->GetParent()).Get() != (IViewParent*)this->Probe(EIID_IViewParent)) {
                result = FALSE;
//                android.util.Log.d(ViewDebug.CONSISTENCY_LOG_TAG,
//                        "View " + children[i] + " has no parent/a parent that is not " + this);
            }
        }
    }

    if (checkDrawing) {
        // If this group is dirty, check that the parent is dirty as well
        if ((mPrivateFlags & DIRTY_MASK) != 0) {
            AutoPtr<IViewParent> parent = GetParent();
            if (parent != NULL && parent->Probe(EIID_ViewRoot) == NULL) {
                if ((((View*)parent->Probe(EIID_View))->mPrivateFlags & DIRTY_MASK) == 0) {
                    result = FALSE;
//                    android.util.Log.d(ViewDebug.CONSISTENCY_LOG_TAG,
//                            "ViewGroup " + this + " is dirty but its parent is not: " + this);
                }
            }
        }
    }

    return result;
}

///**
//    * {@inheritDoc}
//    */
//@Override
//protected void debug(Int32 depth) {
//    super.debug(depth);
//    String output;
//
//    if (mFocused != NULL) {
//        output = debugIndent(depth);
//        output += "mFocused";
//        Log.d(VIEW_LOG_TAG, output);
//    }
//    if (mChildrenCount != 0) {
//        output = debugIndent(depth);
//        output += "{";
//        Log.d(VIEW_LOG_TAG, output);
//    }
//    Int32 count = mChildrenCount;
//    for (Int32 i = 0; i < count; i++) {
//        View child = mChildren[i];
//        child.debug(depth + 1);
//    }
//
//    if (mChildrenCount != 0) {
//        output = debugIndent(depth);
//        output += "}";
//        Log.d(VIEW_LOG_TAG, output);
//    }
//}

/**
  * Returns the position in the group of the specified child view.
  *
  * @param child the view for which to get the position
  * @return a positive integer representing the position of the view in the
  *         group, or -1 if the view does not exist in the group
  */
Int32 ViewGroup::IndexOfChild(
    /* [in] */ IView* child)
{
    Vector<AutoPtr<IView> >::Iterator iter = mChildren.Begin();
    for (Int32 i=0; iter != mChildren.End(); ++iter, ++i) {
        if ((*iter).Get() == child) {
            return i;
        }
    }

    return -1;
}

/**
 * Returns the number of children in the group.
 *
 * @return a positive integer representing the number of children in
 *         the group
 */
Int32 ViewGroup::GetChildCount()
{
    return mChildren.GetSize();
}

/**
 * Returns the view at the specified position in the group.
 *
 * @param index the position at which to get the view from
 * @return the view at the specified position or NULL if the position
 *         does not exist within the group
 */
AutoPtr<IView> ViewGroup::GetChildAt(
    /* [in] */ Int32 index)
{
    if (index >= 0 && index < (Int32)mChildren.GetSize()) {
        return mChildren[index];
    }

    return AutoPtr<IView>(NULL);
}

void ViewGroup::MeasureChildren(
    /* [in] */ Int32 widthMeasureSpec,
    /* [in] */ Int32 heightMeasureSpec)
{
    Vector<AutoPtr<IView> >::Iterator it;
    for (it = mChildren.Begin(); it != mChildren.End(); it++) {
        View* v = (View*)(*it)->Probe(EIID_View);
        if ((v->mViewFlags & VISIBILITY_MASK) != GONE) {
            MeasureChild(*it, widthMeasureSpec, heightMeasureSpec);
        }
    }
}

void ViewGroup::MeasureChild(
    /* [in] */ IView* child,
    /* [in] */ Int32 parentWidthMeasureSpec,
    /* [in] */ Int32 parentHeightMeasureSpec)
{
    assert(child != NULL);

    AutoPtr<IViewGroupLayoutParams> lp;
    child->GetLayoutParams((IViewGroupLayoutParams**)&lp);

    Int32 width, height;
    lp->GetWidth(&width);
    lp->GetHeight(&height);

    const Int32 childWidthMeasureSpec = GetChildMeasureSpec(parentWidthMeasureSpec,
            mPaddingLeft + mPaddingRight, width);
    const Int32 childHeightMeasureSpec = GetChildMeasureSpec(parentHeightMeasureSpec,
            mPaddingTop + mPaddingBottom, height);

    child->Measure(childWidthMeasureSpec, childHeightMeasureSpec);
}

/**
 * Ask one of the children of this view to measure itself, taking into
 * account both the MeasureSpec requirements for this view and its padding
 * and margins. The child must have MarginLayoutParams The heavy lifting is
 * done in getChildMeasureSpec.
 *
 * @param child The child to measure
 * @param parentWidthMeasureSpec The width requirements for this view
 * @param widthUsed Extra space that has been used up by the parent
 *        horizontally (possibly by other children of the parent)
 * @param parentHeightMeasureSpec The height requirements for this view
 * @param heightUsed Extra space that has been used up by the parent
 *        vertically (possibly by other children of the parent)
 */
void ViewGroup::MeasureChildWithMargins(
    /* [in] */ IView* child,
    /* [in] */ Int32 parentWidthMeasureSpec,
    /* [in] */ Int32 widthUsed,
    /* [in] */ Int32 parentHeightMeasureSpec,
    /* [in] */ Int32 heightUsed)
{
    AutoPtr<IViewGroupLayoutParams> lv;
    child->GetLayoutParams((IViewGroupLayoutParams**)&lv);

    IViewGroupMarginLayoutParams* lp = (IViewGroupMarginLayoutParams*)lv.Get();

    Int32 width, height, left, top, right, bottom;
    lp->GetWidth(&width);
    lp->GetHeight(&height);
    lp->GetMargins(&left, &top, &right, &bottom);

    Int32 childWidthMeasureSpec = GetChildMeasureSpec(parentWidthMeasureSpec,
            mPaddingLeft + mPaddingRight + left + right + widthUsed, width);
    Int32 childHeightMeasureSpec = GetChildMeasureSpec(parentHeightMeasureSpec,
            mPaddingTop + mPaddingBottom + top + bottom + heightUsed, height);

    child->Measure(childWidthMeasureSpec, childHeightMeasureSpec);
}

/**
 * Does the hard part of measureChildren: figuring out the MeasureSpec to
 * pass to a particular child. This method figures out the right MeasureSpec
 * for one dimension (height or width) of one child view.
 *
 * The goal is to combine information from our MeasureSpec with the
 * LayoutParams of the child to get the best possible results. For example,
 * if the this view knows its size (because its MeasureSpec has a mode of
 * EXACTLY), and the child has indicated in its LayoutParams that it wants
 * to be the same size as the parent, the parent should ask the child to
 * layout given an exact size.
 *
 * @param spec The requirements for this view
 * @param padding The padding of this view for the current dimension and
 *        margins, if applicable
 * @param childDimension How big the child wants to be in the current
 *        dimension
 * @return a MeasureSpec integer for the child
 */
Int32 ViewGroup::GetChildMeasureSpec(
    /* [in] */ Int32 spec,
    /* [in] */ Int32 padding,
    /* [in] */ Int32 childDimension)
{
    Int32 specMode = MeasureSpec::GetMode(spec);
    Int32 specSize = MeasureSpec::GetSize(spec);

    Int32 size = Math::Max(0, specSize - padding);

    Int32 resultSize = 0;
    Int32 resultMode = 0;

    switch (specMode) {
    // Parent has imposed an exact size on us
    case MeasureSpec::EXACTLY:
        if (childDimension >= 0) {
            resultSize = childDimension;
            resultMode = MeasureSpec::EXACTLY;
        }
        else if (childDimension == ViewGroupLayoutParams::MATCH_PARENT) {
            // Child wants to be our size. So be it.
            resultSize = size;
            resultMode = MeasureSpec::EXACTLY;
        }
        else if (childDimension == ViewGroupLayoutParams::WRAP_CONTENT) {
            // Child wants to determine its own size. It can't be
            // bigger than us.
            resultSize = size;
            resultMode = MeasureSpec::AT_MOST;
        }
        break;

    // Parent has imposed a maximum size on us
    case MeasureSpec::AT_MOST:
        if (childDimension >= 0) {
            // Child wants a specific size... so be it
            resultSize = childDimension;
            resultMode = MeasureSpec::EXACTLY;
        }
        else if (childDimension == ViewGroupLayoutParams::MATCH_PARENT) {
            // Child wants to be our size, but our size is not fixed.
            // Constrain child to not be bigger than us.
            resultSize = size;
            resultMode = MeasureSpec::AT_MOST;
        }
        else if (childDimension == ViewGroupLayoutParams::WRAP_CONTENT) {
            // Child wants to determine its own size. It can't be
            // bigger than us.
            resultSize = size;
            resultMode = MeasureSpec::AT_MOST;
        }
        break;

    // Parent asked to see how big we want to be
    case MeasureSpec::UNSPECIFIED:
        if (childDimension >= 0) {
            // Child wants a specific size... let him have it
            resultSize = childDimension;
            resultMode = MeasureSpec::EXACTLY;
        }
        else if (childDimension == ViewGroupLayoutParams::MATCH_PARENT) {
            // Child wants to be our size... find out how big it should
            // be
            resultSize = 0;
            resultMode = MeasureSpec::UNSPECIFIED;
        }
        else if (childDimension == ViewGroupLayoutParams::WRAP_CONTENT) {
            // Child wants to determine its own size.... find out how
            // big it should be
            resultSize = 0;
            resultMode = MeasureSpec::UNSPECIFIED;
        }
        break;
    }
    return MeasureSpec::MakeMeasureSpec(resultSize, resultMode);
}

ECode ViewGroup::ClearDisappearingChildren()
{
    if (mDisappearingChildren != NULL) {
        mDisappearingChildren->Clear();
    }
    return NOERROR;
}

void ViewGroup::AddDisappearingView(
    /* [in] */ IView* v)
{
    if (mDisappearingChildren == NULL) {
        mDisappearingChildren = new List<AutoPtr<IView> >();
    }

    mDisappearingChildren->PushBack(AutoPtr<IView>(v));
}

void ViewGroup::FinishAnimatingView(
    /* [in] */ IView* view,
    /* [in] */ IAnimation* animation)
{
    assert(view != NULL);
    View* v = (View*)view->Probe(EIID_View);

    if (mDisappearingChildren != NULL) {
        List<AutoPtr<IView> >::Iterator it = Find(
                mDisappearingChildren->Begin(), mDisappearingChildren->End(),
                AutoPtr<IView>(view));
        if (it != mDisappearingChildren->End()) {
            if (v->mAttachInfo != NULL) {
                v->DispatchDetachedFromWindow();
            }

            v->ClearAnimation();
            mGroupFlags |= FLAG_INVALIDATE_REQUIRED;

            mDisappearingChildren->Erase(it);
        }
    }


    if (animation != NULL) {
        Boolean result;
        animation->GetFillAfter(&result);
        if (!result) {
            v->ClearAnimation();
        }
    }

    if ((v->mPrivateFlags & ANIMATION_STARTED) == ANIMATION_STARTED) {
        v->OnAnimationEnd();
        // Should be performed by onAnimationEnd() but this avoid an infinite loop,
        // so we'd rather be safe than sorry
        v->mPrivateFlags &= ~ANIMATION_STARTED;
        // Draw one more frame after the animation is done
        mGroupFlags |= FLAG_INVALIDATE_REQUIRED;
    }
}

Boolean ViewGroup::GatherTransparentRegion(
    /* [in, out] */ IRegion* region)
{
    // If no transparent regions requested, we are always opaque.
    const Boolean meOpaque = (mPrivateFlags & View::REQUEST_TRANSPARENT_REGIONS) == 0;
    if (meOpaque && region == NULL) {
        // The caller doesn't care about the region, so stop now.
        return TRUE;
    }
    View::GatherTransparentRegion(region);
    Boolean noneOfTheChildrenAreTransparent = TRUE;
    Vector<AutoPtr<IView> >::Iterator it;
    for (it = mChildren.Begin(); it != mChildren.End(); it++) {
        View* v = (View*)(*it)->Probe(EIID_View);
        if ((v->mViewFlags & VISIBILITY_MASK) == VISIBLE || v->GetAnimation() != NULL) {
            if (!v->GatherTransparentRegion(region)) {
                noneOfTheChildrenAreTransparent = FALSE;
            }
        }
    }
    return meOpaque || noneOfTheChildrenAreTransparent;
}

ECode ViewGroup::RequestTransparentRegion(
    /* [in] */ IView* child)
{
    if (child != NULL) {
        View* v = (View*)child->Probe(EIID_View);
        v->mPrivateFlags |= View::REQUEST_TRANSPARENT_REGIONS;
        if (mParent != NULL) {
            mParent->RequestTransparentRegion((IView*)this->Probe(EIID_IView));
        }
    }
    return NOERROR;
}

Boolean ViewGroup::FitSystemWindows(
    /* [in] */ IRect* insets)
{
    Boolean done = View::FitSystemWindows(insets);
    if (!done) {
        Vector<AutoPtr<IView> >::Iterator it;
        for (it = mChildren.Begin(); it != mChildren.End(); it++) {
            View* v = (View*)(*it)->Probe(EIID_View);
            done = v->FitSystemWindows(insets);
            if (done) {
                break;
            }
        }
    }
    return done;
}

AutoPtr<IAnimationListener> ViewGroup::GetLayoutAnimationListener()
{
    return mAnimationListener;
}

ECode ViewGroup::DrawableStateChanged()
{
    View::DrawableStateChanged();

    if ((mGroupFlags & FLAG_NOTIFY_CHILDREN_ON_DRAWABLE_STATE_CHANGE) != 0) {
        if ((mGroupFlags & FLAG_ADD_STATES_FROM_CHILDREN) != 0) {
//            throw new IllegalStateException("addStateFromChildren cannot be enabled if a"
//                    + " child has duplicateParentState set to TRUE");
            return E_ILLEGAL_STATE_EXCEPTION;
        }

        Vector<AutoPtr<IView> >::Iterator it;
        for (it = mChildren.Begin(); it != mChildren.End(); it++) {
            View* v = (View*)(*it)->Probe(EIID_View);
            if ((v->mViewFlags & DUPLICATE_PARENT_STATE) != 0) {
                v->RefreshDrawableState();
            }
        }
    }
    return NOERROR;
}

ECode ViewGroup::OnCreateDrawableState(
    /* [in] */ Int32 extraSpace,
    /* [out] */ ArrayOf<Int32>** drawableState)
{
    if ((mGroupFlags & FLAG_ADD_STATES_FROM_CHILDREN) == 0) {
        return View::OnCreateDrawableState(extraSpace, drawableState);
    }

    Int32 need = 0;
    Int32 n = GetChildCount();
    for (Int32 i = 0; i < n; i++) {
        View* v = (View*)GetChildAt(i)->Probe(EIID_View);
        ArrayOf<Int32>* childState = v->GetDrawableState();

        if (childState != NULL) {
            need += childState->GetLength();
        }
    }

    ArrayOf<Int32>* state;
    ASSERT_SUCCEEDED(View::OnCreateDrawableState(extraSpace + need, &state));

    for (Int32 i = 0; i < n; i++) {
        View* v = (View*)GetChildAt(i)->Probe(EIID_View);
        ArrayOf<Int32>* childState = v->GetDrawableState();

        if (childState != NULL) {
            state = MergeDrawableStates(state, childState);
        }
    }

    *drawableState = state;
    return NOERROR;
}

ECode ViewGroup::SetAddStatesFromChildren(
    /* [in] */ Boolean addsStates)
{
    if (addsStates) {
        mGroupFlags |= FLAG_ADD_STATES_FROM_CHILDREN;
    } else {
        mGroupFlags &= ~FLAG_ADD_STATES_FROM_CHILDREN;
    }

    return RefreshDrawableState();
}

Boolean ViewGroup::AddStatesFromChildren()
{
    return (mGroupFlags & FLAG_ADD_STATES_FROM_CHILDREN) != 0;
}

ECode ViewGroup::ChildDrawableStateChanged(
    /* [in] */ IView* child)
{
    if ((mGroupFlags & FLAG_ADD_STATES_FROM_CHILDREN) != 0) {
        return RefreshDrawableState();
    }
    return NOERROR;
}

ECode ViewGroup::SetLayoutAnimationListener(
    /* [in] */ IAnimationListener* animationListener)
{
    mAnimationListener = animationListener;
    return NOERROR;
}

ECode ViewGroup::Init(
    /* [in] */ IContext* context)
{
    ASSERT_SUCCEEDED(View::Init(context));
    InitViewGroup();
    return NOERROR;
}

ECode ViewGroup::Init(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs)
{
    ASSERT_SUCCEEDED(View::Init(context, attrs));
    InitViewGroup();
    InitFromAttributes(context, attrs);
    return NOERROR;
}

ECode ViewGroup::Init(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs,
    /* [in] */ Int32 defStyle)
{
    ASSERT_SUCCEEDED(View::Init(context, attrs, defStyle));
    InitViewGroup();
    InitFromAttributes(context, attrs);
    return NOERROR;
}
