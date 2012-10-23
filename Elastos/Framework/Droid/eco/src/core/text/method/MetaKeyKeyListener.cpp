
#include "text/method/MetaKeyKeyListener.h"
#include "text/CNoCopySpan.h"

const Int32 MetaKeyKeyListener::META_SHIFT_ON;
const Int32 MetaKeyKeyListener::META_ALT_ON;
const Int32 MetaKeyKeyListener::META_SYM_ON;
const Int32 MetaKeyKeyListener::LOCKED_SHIFT;
const Int32 MetaKeyKeyListener::META_CAP_LOCKED;
const Int32 MetaKeyKeyListener::META_ALT_LOCKED;
const Int32 MetaKeyKeyListener::META_SYM_LOCKED;
const Int32 MetaKeyKeyListener::META_SELECTING;

const Int32 MetaKeyKeyListener::USED_SHIFT;
const Int64 MetaKeyKeyListener::META_CAP_USED;
const Int64 MetaKeyKeyListener::META_ALT_USED;
const Int64 MetaKeyKeyListener::META_SYM_USED;
const Int32 MetaKeyKeyListener::PRESSED_SHIFT;
const Int64 MetaKeyKeyListener::META_CAP_PRESSED;
const Int64 MetaKeyKeyListener::META_ALT_PRESSED;
const Int64 MetaKeyKeyListener::META_SYM_PRESSED;
const Int32 MetaKeyKeyListener::RELEASED_SHIFT;
const Int64 MetaKeyKeyListener::META_CAP_RELEASED;
const Int64 MetaKeyKeyListener::META_ALT_RELEASED;
const Int64 MetaKeyKeyListener::META_SYM_RELEASED;
const Int64 MetaKeyKeyListener::META_SHIFT_MASK;
const Int64 MetaKeyKeyListener::META_ALT_MASK;
const Int64 MetaKeyKeyListener::META_SYM_MASK;

const AutoPtr<IInterface> MetaKeyKeyListener::CAP = MetaKeyKeyListener::NewNoCopySpan();
const AutoPtr<IInterface> MetaKeyKeyListener::ALT = MetaKeyKeyListener::NewNoCopySpan();
const AutoPtr<IInterface> MetaKeyKeyListener::SYM = MetaKeyKeyListener::NewNoCopySpan();
const AutoPtr<IInterface> MetaKeyKeyListener::SELECTING = MetaKeyKeyListener::NewNoCopySpan();

const Int32 MetaKeyKeyListener::PRESSED;
const Int32 MetaKeyKeyListener::RELEASED;
const Int32 MetaKeyKeyListener::USED;
const Int32 MetaKeyKeyListener::LOCKED;

AutoPtr<IInterface> MetaKeyKeyListener::NewNoCopySpan()
{
    AutoPtr<CNoCopySpan> noCopySpan;
    CNoCopySpan::NewByFriend((CNoCopySpan**)&noCopySpan);

    return noCopySpan->Probe(EIID_IInterface);
}

/**
 * Resets all meta state to inactive.
 */
void MetaKeyKeyListener::ResetMetaState(
    /* [in] */ ISpannable* text)
{
    text->RemoveSpan(CAP);
    text->RemoveSpan(ALT);
    text->RemoveSpan(SYM);
    text->RemoveSpan(SELECTING);
}

/**
 * Gets the state of the meta keys.
 *
 * @param text the buffer in which the meta key would have been pressed.
 *
 * @return an integer in which each bit set to one represents a pressed
 *         or locked meta key.
 */
Int32 MetaKeyKeyListener::GetMetaState(
    /* [in] */ ICharSequence* text)
{
    return GetActive(text, CAP, META_SHIFT_ON, META_CAP_LOCKED) |
           GetActive(text, ALT, META_ALT_ON, META_ALT_LOCKED) |
           GetActive(text, SYM, META_SYM_ON, META_SYM_LOCKED) |
           GetActive(text, SELECTING, META_SELECTING, META_SELECTING);
}

/**
 * Gets the state of a particular meta key.
 *
 * @param meta META_SHIFT_ON, META_ALT_ON, META_SYM_ON, or META_SELECTING
 * @param text the buffer in which the meta key would have been pressed.
 *
 * @return 0 if inactive, 1 if active, 2 if locked.
 */
Int32 MetaKeyKeyListener::GetMetaState(
    /* [in] */ ICharSequence* text,
     /* [in] */ Int32 meta)
{
    switch (meta) {
        case META_SHIFT_ON:
            return GetActive(text, CAP, 1, 2);

        case META_ALT_ON:
            return GetActive(text, ALT, 1, 2);

        case META_SYM_ON:
            return GetActive(text, SYM, 1, 2);

        case META_SELECTING:
            return GetActive(text, SELECTING, 1, 2);

        default:
            return 0;
    }
}

Int32 MetaKeyKeyListener::GetActive(
    /* [in] */ ICharSequence* text,
    /* [in] */ IInterface* meta,
    /* [in] */ Int32 on,
    /* [in] */ Int32 lock)
{
    if (!(ISpanned::Probe(text))) {
        return 0;
    }

    AutoPtr<ISpanned> sp = ISpanned::Probe(text);
    Int32 flag;
    sp->GetSpanFlags(meta, &flag);

    if (flag == LOCKED) {
        return lock;
    }
    else if (flag != 0) {
        return on;
    }
    else {
        return 0;
    }
}

/**
 * Call this method after you handle a keypress so that the meta
 * state will be reset to unshifted (if it is not still down)
 * or primed to be reset to unshifted (once it is released).
 */
void MetaKeyKeyListener::AdjustMetaAfterKeypress(
    /* [in] */ ISpannable* content)
{
    Adjust(content, CAP);
    Adjust(content, ALT);
    Adjust(content, SYM);
}

/**
 * Returns TRUE if this object is one that this class would use to
 * keep track of any meta state in the specified text->
 */
Boolean MetaKeyKeyListener::IsMetaTracker(
    /* [in] */ ICharSequence* text,
    /* [in] */ IInterface* what)
{
    return what == CAP || what == ALT || what == SYM ||
           what == SELECTING;
}

/**
 * Returns TRUE if this object is one that this class would use to
 * keep track of the selecting meta state in the specified text->
 */
Boolean MetaKeyKeyListener::IsSelectingMetaTracker(
    /* [in] */ ICharSequence* text,
    /* [in] */ IInterface* what)
{
    return what == SELECTING;
}

void MetaKeyKeyListener::Adjust(
    /* [in] */ ISpannable* content,
    /* [in] */ IInterface* what)
{
    Int32 current;
    content->GetSpanFlags(what, &current);

    if (current == PRESSED)
        content->SetSpan(what, 0, 0, USED);
    else if (current == RELEASED)
        content->RemoveSpan(what);
}

/**
 * Call this if you are a method that ignores the locked meta state
 * (arrow keys, for example) and you handle a key.
 */
void MetaKeyKeyListener::ResetLockedMeta(
    /* [in] */ ISpannable* content)
{
    ResetLock(content, CAP);
    ResetLock(content, ALT);
    ResetLock(content, SYM);
    ResetLock(content, SELECTING);
}

void MetaKeyKeyListener::ResetLock(
    /* [in] */ ISpannable* content,
    /* [in] */ IInterface* what)
{
    Int32 current;
    content->GetSpanFlags(what, &current);

    if (current == LOCKED)
        content->RemoveSpan(what);
}

/**
 * Handles presses of the meta keys.
 */
Boolean MetaKeyKeyListener::OnKeyDown(
    /* [in] */ IView* view,
    /* [in] */ IEditable* content,
    /* [in] */ Int32 keyCode,
    /* [in] */ IKeyEvent* event)
{
    if (keyCode == KeyEvent_KEYCODE_SHIFT_LEFT || keyCode == KeyEvent_KEYCODE_SHIFT_RIGHT) {
        Press(content, CAP);
        return TRUE;
    }

    if (keyCode == KeyEvent_KEYCODE_ALT_LEFT || keyCode == KeyEvent_KEYCODE_ALT_RIGHT
            || keyCode == KeyEvent_KEYCODE_NUM) {
        Press(content, ALT);
        return TRUE;
    }

    if (keyCode == KeyEvent_KEYCODE_SYM) {
        Press(content, SYM);
        return TRUE;
    }

    return FALSE; // no super to call through to
}

void MetaKeyKeyListener::Press(
    /* [in] */ IEditable* content,
    /* [in] */ IInterface* what)
{
    Int32 state;
    content->GetSpanFlags(what, &state);

    if (state == PRESSED)
        ; // repeat before use
    else if (state == RELEASED)
        content->SetSpan(what, 0, 0, LOCKED);
    else if (state == USED)
        ; // repeat after use
    else if (state == LOCKED)
        content->RemoveSpan(what);
    else
        content->SetSpan(what, 0, 0, PRESSED);
}

/**
 * Start selecting text->
 * @hide pending API review
 */
void MetaKeyKeyListener::StartSelecting(
    /* [in] */ IView* view,
    /* [in] */ ISpannable* content)
{
    content->SetSpan(SELECTING, 0, 0, PRESSED);
}

/**
 * Stop selecting text->  This does not actually collapse the selection;
 * call {@link android.text->Selection#setSelection} too.
 * @hide pending API review
 */
void MetaKeyKeyListener::StopSelecting(
    /* [in] */ IView* view,
    /* [in] */ ISpannable* content)
{
    content->RemoveSpan(SELECTING);
}

/**
 * Handles Release of the meta keys.
 */
Boolean MetaKeyKeyListener::OnKeyUp(
    /* [in] */ IView* view,
    /* [in] */ IEditable* content,
    /* [in] */ Int32 keyCode,
    /* [in] */ IKeyEvent* event)
{
    if (keyCode == KeyEvent_KEYCODE_SHIFT_LEFT || keyCode == KeyEvent_KEYCODE_SHIFT_RIGHT) {
        Release(content, CAP);
        return TRUE;
    }

    if (keyCode == KeyEvent_KEYCODE_ALT_LEFT || keyCode == KeyEvent_KEYCODE_ALT_RIGHT
            || keyCode == KeyEvent_KEYCODE_NUM) {
        Release(content, ALT);
        return TRUE;
    }

    if (keyCode == KeyEvent_KEYCODE_SYM) {
        Release(content, SYM);
        return TRUE;
    }

    return FALSE; // no super to call through to
}

void MetaKeyKeyListener::Release(
    /* [in] */ IEditable* content,
    /* [in] */ IInterface* what)
{
    Int32 current;
    content->GetSpanFlags(what, &current);

    if (current == USED)
        content->RemoveSpan(what);
    else if (current == PRESSED)
        content->SetSpan(what, 0, 0, RELEASED);
}

ECode MetaKeyKeyListener::ClearMetaKeyState(
    /* [in] */ IView* view,
    /* [in] */ IEditable* content,
    /* [in] */ Int32 states)
{
    ClearMetaKeyState(content, states);
    return NOERROR;
}

void MetaKeyKeyListener::ClearMetaKeyState(
    /* [in] */ IEditable* content,
    /* [in] */ Int32 states)
{
    if ((states&META_SHIFT_ON) != 0)
        content->RemoveSpan(CAP);
    if ((states&META_ALT_ON) != 0)
        content->RemoveSpan(ALT);
    if ((states&META_SYM_ON) != 0)
        content->RemoveSpan(SYM);
    if ((states&META_SELECTING) != 0)
        content->RemoveSpan(SELECTING);
}

/**
 * Call this if you are a method that ignores the locked meta state
 * (arrow keys, for example) and you handle a key.
 */
Int64 MetaKeyKeyListener::ResetLockedMeta(
    /* [in] */ Int64 state)
{
    state = ResetLock(state, META_SHIFT_ON, META_SHIFT_MASK);
    state = ResetLock(state, META_ALT_ON, META_ALT_MASK);
    state = ResetLock(state, META_SYM_ON, META_SYM_MASK);
    return state;
}

Int64 MetaKeyKeyListener::ResetLock(
    /* [in] */ Int64 state,
    /* [in] */ Int32 what,
    /* [in] */ Int64 mask)
{
    if ((state&(((Int64)what)<<LOCKED_SHIFT)) != 0) {
        state &= ~mask;
    }
    return state;
}

// ---------------------------------------------------------------------
// Version of API that operates on a state bit mask
// ---------------------------------------------------------------------

/**
 * Gets the state of the meta keys.
 *
 * @param state the current meta state bits.
 *
 * @return an integer in which each bit set to one represents a pressed
 *         or locked meta key.
 */
Int32 MetaKeyKeyListener::GetMetaState(
    /* [in] */ Int64 state)
{
    return GetActive(state, META_SHIFT_ON, META_SHIFT_ON, META_CAP_LOCKED) |
           GetActive(state, META_ALT_ON, META_ALT_ON, META_ALT_LOCKED) |
           GetActive(state, META_SYM_ON, META_SYM_ON, META_SYM_LOCKED);
}

/**
 * Gets the state of a particular meta key.
 *
 * @param state the current state bits.
 * @param meta META_SHIFT_ON, META_ALT_ON, or META_SYM_ON
 *
 * @return 0 if inactive, 1 if active, 2 if locked.
 */
Int32 MetaKeyKeyListener::GetMetaState(
    /* [in] */ Int64 state,
    /* [in] */ Int32 meta)
{
    switch (meta) {
        case META_SHIFT_ON:
            return GetActive(state, meta, 1, 2);

        case META_ALT_ON:
            return GetActive(state, meta, 1, 2);

        case META_SYM_ON:
            return GetActive(state, meta, 1, 2);

        default:
            return 0;
    }
}

Int32 MetaKeyKeyListener::GetActive(
    /* [in] */ Int64 state,
    /* [in] */ Int32 meta,
    /* [in] */ Int32 on,
    /* [in] */ Int32 lock)
{
    if ((state&(meta<<LOCKED_SHIFT)) != 0) {
        return lock;
    }
    else if ((state&meta) != 0) {
        return on;
    }
    else {
        return 0;
    }
}

/**
 * Call this method after you handle a keypress so that the meta
 * state will be reset to unshifted (if it is not still down)
 * or primed to be reset to unshifted (once it is released).  Takes
 * the current state, returns the new state.
 */
Int64 MetaKeyKeyListener::AdjustMetaAfterKeypress(
    /* [in] */ Int64 state)
{
    state = Adjust(state, META_SHIFT_ON, META_SHIFT_MASK);
    state = Adjust(state, META_ALT_ON, META_ALT_MASK);
    state = Adjust(state, META_SYM_ON, META_SYM_MASK);
    return state;
}

Int64 MetaKeyKeyListener::Adjust(
    /* [in] */ Int64 state,
    /* [in] */ Int32 what,
    /* [in] */ Int64 mask)
{
    if ((state&(((Int64)what)<<PRESSED_SHIFT)) != 0)
        return (state&~mask) | what | ((Int64)what)<<USED_SHIFT;
    else if ((state&(((Int64)what)<<RELEASED_SHIFT)) != 0)
        return state & ~mask;
    return state;
}

/**
 * Handles presses of the meta keys.
 */
Int64 MetaKeyKeyListener::HandleKeyDown(
    /* [in] */ Int64 state,
    /* [in] */ Int32 keyCode,
    /* [in] */ IKeyEvent* event)
{
    if (keyCode == KeyEvent_KEYCODE_SHIFT_LEFT || keyCode == KeyEvent_KEYCODE_SHIFT_RIGHT) {
        return Press(state, META_SHIFT_ON, META_SHIFT_MASK);
    }

    if (keyCode == KeyEvent_KEYCODE_ALT_LEFT || keyCode == KeyEvent_KEYCODE_ALT_RIGHT
            || keyCode == KeyEvent_KEYCODE_NUM) {
        return Press(state, META_ALT_ON, META_ALT_MASK);
    }

    if (keyCode == KeyEvent_KEYCODE_SYM) {
        return Press(state, META_SYM_ON, META_SYM_MASK);
    }

    return state;
}

Int64 MetaKeyKeyListener::Press(
    /* [in] */ Int64 state,
    /* [in] */ Int32 what,
    /* [in] */ Int64 mask)
{
    if ((state&(((Int64)what)<<PRESSED_SHIFT)) != 0)
        ; // repeat before use
    else if ((state&(((Int64)what)<<RELEASED_SHIFT)) != 0)
        state = (state&~mask) | what | (((Int64)what) << LOCKED_SHIFT);
    else if ((state&(((Int64)what)<<USED_SHIFT)) != 0)
        ; // repeat after use
    else if ((state&(((Int64)what)<<LOCKED_SHIFT)) != 0)
        state = state&~mask;
    else
        state = state | what | (((Int64)what)<<PRESSED_SHIFT);
    return state;
}

/**
 * Handles Release of the meta keys.
 */
Int64 MetaKeyKeyListener::HandleKeyUp(
    /* [in] */ Int64 state, Int32 keyCode,
    /* [in] */ IKeyEvent* event)
{
    if (keyCode == KeyEvent_KEYCODE_SHIFT_LEFT || keyCode == KeyEvent_KEYCODE_SHIFT_RIGHT) {
        return Release(state, META_SHIFT_ON, META_SHIFT_MASK);
    }

    if (keyCode == KeyEvent_KEYCODE_ALT_LEFT || keyCode == KeyEvent_KEYCODE_ALT_RIGHT
            || keyCode == KeyEvent_KEYCODE_NUM) {
        return Release(state, META_ALT_ON, META_ALT_MASK);
    }

    if (keyCode == KeyEvent_KEYCODE_SYM) {
        return Release(state, META_SYM_ON, META_SYM_MASK);
    }

    return state;
}

Int64 MetaKeyKeyListener::Release(
    /* [in] */ Int64 state,
    /* [in] */ Int32 what,
    /* [in] */ Int64 mask)
{
    if ((state&(((Int64)what)<<USED_SHIFT)) != 0)
        state = state&~mask;
    else if ((state&(((Int64)what)<<PRESSED_SHIFT)) != 0)
        state = state | what | (((Int64)what)<<RELEASED_SHIFT);
    return state;
}

Int64 MetaKeyKeyListener::ClearMetaKeyState(
    /* [in] */ Int64 state,
    /* [in] */ Int32 which)
{
    if ((which&META_SHIFT_ON) != 0)
        state = ResetLock(state, META_SHIFT_ON, META_SHIFT_MASK);
    if ((which&META_ALT_ON) != 0)
        state = ResetLock(state, META_ALT_ON, META_ALT_MASK);
    if ((which&META_SYM_ON) != 0)
        state = ResetLock(state, META_SYM_ON, META_SYM_MASK);
    return state;
}
