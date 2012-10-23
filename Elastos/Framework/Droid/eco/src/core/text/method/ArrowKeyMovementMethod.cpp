
#include "text/method/ArrowKeyMovementMethod.h"
#include "text/method/MetaKeyKeyListener.h"
#include "text/Selection.h"
#include "view/View.h"
#include "os/ElObject.h"

const AutoPtr<IInterface> ArrowKeyMovementMethod::LAST_TAP_DOWN = new ElObject();

Boolean ArrowKeyMovementMethod::IsCap(
    /* [in] */ ISpannable* buffer)
{
    return ((MetaKeyKeyListener::GetMetaState(buffer, KeyEvent_META_SHIFT_ON) == 1) ||
        (MetaKeyKeyListener::GetMetaState(buffer, MetaKeyKeyListener::META_SELECTING) != 0));
}

Boolean ArrowKeyMovementMethod::IsAlt(
    /* [in] */ ISpannable* buffer)
{
    return MetaKeyKeyListener::GetMetaState(buffer, KeyEvent_META_ALT_ON) == 1;
}

Boolean ArrowKeyMovementMethod::Up(
    /* [in] */ ITextView* widget,
    /* [in] */ ISpannable* buffer)
{
    Boolean cap = IsCap(buffer);
    Boolean alt = IsAlt(buffer);
    AutoPtr<ILayout> layout;
    widget->GetLayout((ILayout**)&layout);

    if (cap) {
        if (alt) {
            Selection::ExtendSelection(buffer, 0);
            return TRUE;
        }
        else {
            return Selection::ExtendUp(buffer, layout);
        }
    }
    else {
        if (alt) {
            Selection::SetSelection(buffer, 0);
            return TRUE;
        }
        else {
            return Selection::MoveUp(buffer, layout);
        }
    }
}

Boolean ArrowKeyMovementMethod::Down(
    /* [in] */ ITextView* widget,
    /* [in] */ ISpannable* buffer)
{
    Boolean cap = IsCap(buffer);
    Boolean alt = IsAlt(buffer);
    AutoPtr<ILayout> layout;
    widget->GetLayout((ILayout**)&layout);

    if (cap) {
        if (alt) {
            Int32 len;
            buffer->GetLength(&len);
            Selection::ExtendSelection(buffer, len);
            return TRUE;
        }
        else {
            return Selection::ExtendDown(buffer, layout);
        }
    }
    else {
        if (alt) {
            Int32 len;
            buffer->GetLength(&len);
            Selection::SetSelection(buffer, len);
            return TRUE;
        }
        else {
            return Selection::MoveDown(buffer, layout);
        }
    }
}

Boolean ArrowKeyMovementMethod::Left(
    /* [in] */ ITextView* widget,
    /* [in] */ ISpannable* buffer)
{
    Boolean cap = IsCap(buffer);
    Boolean alt = IsAlt(buffer);
    AutoPtr<ILayout> layout;
    widget->GetLayout((ILayout**)&layout);

    if (cap) {
        if (alt) {
            return Selection::ExtendToLeftEdge(buffer, layout);
        }
        else {
            return Selection::ExtendLeft(buffer, layout);
        }
    }
    else {
        if (alt) {
            return Selection::MoveToLeftEdge(buffer, layout);
        }
        else {
            return Selection::MoveLeft(buffer, layout);
        }
    }
}

Boolean ArrowKeyMovementMethod::Right(
    /* [in] */ ITextView* widget,
    /* [in] */ ISpannable* buffer)
{
    Boolean cap = IsCap(buffer);
    Boolean alt = IsAlt(buffer);
    AutoPtr<ILayout> layout;
    widget->GetLayout((ILayout**)&layout);

    if (cap) {
        if (alt) {
            return Selection::ExtendToRightEdge(buffer, layout);
        }
        else {
            return Selection::ExtendRight(buffer, layout);
        }
    }
    else {
        if (alt) {
            return Selection::MoveToRightEdge(buffer, layout);
        }
        else {
            return Selection::MoveRight(buffer, layout);
        }
    }
}

Boolean ArrowKeyMovementMethod::OnKeyDown(
    /* [in] */ ITextView* widget,
    /* [in] */ ISpannable* buffer,
    /* [in] */ Int32 keyCode,
    /* [in] */ IKeyEvent* event)
{
    if (ExecuteDown(widget, buffer, keyCode)) {
        MetaKeyKeyListener::AdjustMetaAfterKeypress(buffer);
        MetaKeyKeyListener::ResetLockedMeta(buffer);
        return TRUE;
    }

    return FALSE;
}

Boolean ArrowKeyMovementMethod::ExecuteDown(
    /* [in] */ ITextView* widget,
    /* [in] */ ISpannable* buffer,
    /* [in] */ Int32 keyCode)
{
    Boolean handled = FALSE;

    switch (keyCode) {
    case KeyEvent_KEYCODE_DPAD_UP:
        handled |= Up(widget, buffer);
        break;

    case KeyEvent_KEYCODE_DPAD_DOWN:
        handled |= Down(widget, buffer);
        break;

    case KeyEvent_KEYCODE_DPAD_LEFT:
        handled |= Left(widget, buffer);
        break;

    case KeyEvent_KEYCODE_DPAD_RIGHT:
        handled |= Right(widget, buffer);
        break;

    case KeyEvent_KEYCODE_DPAD_CENTER:
        Boolean res;
        if ((MetaKeyKeyListener::GetMetaState(buffer, MetaKeyKeyListener::META_SELECTING) != 0) &&
            (widget->ShowContextMenu(&res), res)) {
                handled = TRUE;
        }
    }

    if (handled) {
        MetaKeyKeyListener::AdjustMetaAfterKeypress(buffer);
        MetaKeyKeyListener::ResetLockedMeta(buffer);
    }

    return handled;
}

Boolean ArrowKeyMovementMethod::OnKeyUp(
    /* [in] */ ITextView* widget,
    /* [in] */ ISpannable* buffer,
    /* [in] */ Int32 keyCode,
    /* [in] */ IKeyEvent* event)
{
    return FALSE;
}

Boolean ArrowKeyMovementMethod::OnKeyOther(
    /* [in] */ ITextView* view,
    /* [in] */ ISpannable* text,
    /* [in] */ IKeyEvent* event)
{
    Int32 code, action;
    event->GetKeyCode(&code);
    event->GetAction(&action);
    if (code != KeyEvent_KEYCODE_UNKNOWN && action == KeyEvent_ACTION_MULTIPLE) {
        Int32 repeat;
        event->GetRepeatCount(&repeat);
        Boolean handled = FALSE;
        while ((--repeat) > 0) {
            handled |= ExecuteDown(view, text, code);
        }
        return handled;
    }

    return FALSE;
}

Boolean ArrowKeyMovementMethod::OnTrackballEvent(
    /* [in] */ ITextView* widget,
    /* [in] */ ISpannable* text,
    /* [in] */ IMotionEvent* event)
{
    return FALSE;
}

Boolean ArrowKeyMovementMethod::OnTouchEvent(
    /* [in] */ ITextView* widget,
    /* [in] */ ISpannable* buffer,
    /* [in] */ IMotionEvent* event)
{
    Int32 initialScrollX = -1;
    Int32 initialScrollY = -1;
    Int32 action;
    event->GetAction(&action);
    if (action == MotionEvent_ACTION_UP) {
        //initialScrollX = Touch::GetInitialScrollX(widget, buffer);
        //initialScrollY = Touch::GetInitialScrollY(widget, buffer);
    }

    Boolean handled = FALSE; //Touch::OnTouchEvent(widget, buffer, event);

    Boolean isFocused, r;
    if ((widget->IsFocused(&isFocused), isFocused) &&
        !(widget->DidTouchFocusSelect(&r), r)) {
        Float fx, fy;
        event->GetX(&fx);
        event->GetY(&fy);
        Int32 offset;
        widget->GetOffset((Int32)fx, (Int32)fy, &offset);

        if (action == MotionEvent_ACTION_DOWN) {
            Boolean cap = IsCap(buffer);
            if (cap) {
                buffer->SetSpan(LAST_TAP_DOWN, offset, offset, Spanned_SPAN_POINT_POINT);

                // Disallow Int32ercepting of the touch events, so that
                // users can scroll and select at the same time.
                // without this, users would get booted out of select
                // mode once the view detected it needed to scroll.
                AutoPtr<IViewParent> parent;
                widget->GetParent((IViewParent**)&parent);
                parent->RequestDisallowInterceptTouchEvent(TRUE);
            }
        }
        else if (action == MotionEvent_ACTION_MOVE) {
            Boolean cap = IsCap(buffer);

            if (cap && handled) {
                // Before selecting, make sure we've moved out of the "slop".
                // handled will be TRUE, if we're in select mode AND we're
                // OUT of the slop

                // Turn long press off while we're selecting. User needs to
                // re-tap on the selection to enable long press
                widget->CancelLongPress();

                // Update selection as we're moving the selection area.

                Selection::ExtendSelection(buffer, offset);
                return TRUE;
            }
        }
        else if (action == MotionEvent_ACTION_UP) {
            // If we have scrolled, then the up shouldn't move the cursor,
            // but we do need to make sure the cursor is still visible at
            // the current scroll offset to avoid the scroll jumping later
            // to show it.
            Int32 scrollX, scrollY;
            widget->GetScrollX(&scrollX);
            widget->GetScrollY(&scrollY);
            if ((initialScrollY >= 0 && initialScrollY != scrollY) ||
                (initialScrollX >= 0 && initialScrollX != scrollX)) {
                Boolean res;
                widget->MoveCursorToVisibleOffset(&res);
                return TRUE;
            }

            if (IsCap(buffer)) {
                buffer->RemoveSpan(LAST_TAP_DOWN);
                Selection::ExtendSelection(buffer, offset);
            }
            else {
                Selection::SetSelection(buffer, offset);
            }

            MetaKeyKeyListener::AdjustMetaAfterKeypress(buffer);
            MetaKeyKeyListener::ResetLockedMeta(buffer);

            return TRUE;
        }
    }

    return handled;
}

Boolean ArrowKeyMovementMethod::CanSelectArbitrarily()
{
    return TRUE;
}

ECode ArrowKeyMovementMethod::Initialize(
    /* [in] */ ITextView* widget,
    /* [in] */ ISpannable* text)
{
    Selection::SetSelection(text, 0);
    return NOERROR;
}

ECode ArrowKeyMovementMethod::OnTakeFocus(
    /* [in] */ ITextView* view,
    /* [in] */ ISpannable* text,
    /* [in] */ Int32 dir)
{
    if ((dir & (View::FOCUS_FORWARD | View::FOCUS_DOWN)) != 0) {
        AutoPtr<ILayout> layout;
        view->GetLayout((ILayout**)&layout);
        if (layout == NULL) {
            // This shouldn't be null, but do something sensible if it is.
            Int32 len;
            text->GetLength(&len);
            Selection::SetSelection(text, len);
        }
    }
    else {
        Int32 len;
        text->GetLength(&len);
        Selection::SetSelection(text, len);
    }

    return NOERROR;
}
