
#ifndef __METAKEYKEYLISTENER_H__
#define __METAKEYKEYLISTENER_H__

#include "ext/frameworkext.h"
#include <elastos/AutoPtr.h>

class MetaKeyKeyListener
{
public:
    static CARAPI_(AutoPtr<IInterface>) NewNoCopySpan();

    static CARAPI_(void) ResetMetaState(
        /* [in] */ ISpannable* text);

    static CARAPI_(Int32) GetMetaState(
        /* [in] */ ICharSequence* text);

    static CARAPI_(Int32) GetMetaState(
        /* [in] */ ICharSequence* text,
        /* [in] */ Int32 meta);

    static CARAPI_(void) AdjustMetaAfterKeypress(
        /* [in] */ ISpannable* content);

    static CARAPI_(Boolean) IsMetaTracker(
        /* [in] */ ICharSequence* text,
        /* [in] */ IInterface* what);

    static CARAPI_(Boolean) IsSelectingMetaTracker(
        /* [in] */ ICharSequence* text,
        /* [in] */ IInterface* what);

    static CARAPI_(void) StartSelecting(
        /* [in] */ IView* view,
        /* [in] */ ISpannable* content);

    static CARAPI_(void) StopSelecting(
        /* [in] */ IView* view,
        /* [in] */ ISpannable* content);

    static CARAPI_(void) ClearMetaKeyState(
        /* [in] */ IEditable* content,
        /* [in] */ Int32 states);;

    static CARAPI_(Int64) ResetLockedMeta(
        /* [in] */ Int64 state);

    static CARAPI_(Int32) GetMetaState(
        /* [in] */ Int64 state);

    static CARAPI_(Int32) GetMetaState(
        /* [in] */ Int64 state,
        /* [in] */ Int32 meta);

    static CARAPI_(Int64) AdjustMetaAfterKeypress(
        /* [in] */ Int64 state);

    static CARAPI_(Int64) HandleKeyDown(
        /* [in] */ Int64 state,
        /* [in] */ Int32 keyCode,
        /* [in] */ IKeyEvent* event);

    static CARAPI_(Int64) HandleKeyUp(
        /* [in] */ Int64 state, Int32 keyCode,
        /* [in] */ IKeyEvent* event);

    static CARAPI_(void) ResetLockedMeta(
        /* [in] */ ISpannable* content);

public:
    virtual CARAPI_(Boolean) OnKeyDown(
        /* [in] */ IView* view,
        /* [in] */ IEditable* content,
        /* [in] */ Int32 keyCode,
        /* [in] */ IKeyEvent* event);

    virtual CARAPI_(Boolean) OnKeyUp(
        /* [in] */ IView* view,
        /* [in] */ IEditable* content,
        /* [in] */ Int32 keyCode,
        /* [in] */ IKeyEvent* event);

    virtual CARAPI ClearMetaKeyState(
        /* [in] */ IView* view,
        /* [in] */ IEditable* content,
        /* [in] */ Int32 states);

    virtual CARAPI_(Int64) ClearMetaKeyState(
        /* [in] */ Int64 state,
        /* [in] */ Int32 which);

private:
    static CARAPI_(Int32) GetActive(
        /* [in] */ ICharSequence* text,
        /* [in] */ IInterface* meta,
        /* [in] */ Int32 on,
        /* [in] */ Int32 lock);

    static CARAPI_(void) Adjust(
        /* [in] */ ISpannable* content,
        /* [in] */ IInterface* what);

    static CARAPI_(void) ResetLock(
        /* [in] */ ISpannable* content,
        /* [in] */ IInterface* what);

    static CARAPI_(Int64) ResetLock(
        /* [in] */ Int64 state,
        /* [in] */ Int32 what,
        /* [in] */ Int64 mask);

    static CARAPI_(Int32) GetActive(
        /* [in] */ Int64 state,
        /* [in] */ Int32 meta,
        /* [in] */ Int32 on,
        /* [in] */ Int32 lock);

    static CARAPI_(Int64) Adjust(
        /* [in] */ Int64 state,
        /* [in] */ Int32 what,
        /* [in] */ Int64 mask);

    static CARAPI_(Int64) Press(
        /* [in] */ Int64 state,
        /* [in] */ Int32 what,
        /* [in] */ Int64 mask);

    static CARAPI_(Int64) Release(
        /* [in] */ Int64 state,
        /* [in] */ Int32 what,
        /* [in] */ Int64 mask);

private:
    CARAPI_(void) Press(
        /* [in] */ IEditable* content,
        /* [in] */ IInterface* what);

    CARAPI_(void) Release(
        /* [in] */ IEditable* content,
        /* [in] */ IInterface* what);

public:
    static const Int32 META_SHIFT_ON = KeyEvent_META_SHIFT_ON;
    static const Int32 META_ALT_ON = KeyEvent_META_ALT_ON;
    static const Int32 META_SYM_ON = KeyEvent_META_SYM_ON;

private:
    static const Int32 LOCKED_SHIFT = 8;

public:
    static const Int32 META_CAP_LOCKED = KeyEvent_META_SHIFT_ON << LOCKED_SHIFT;
    static const Int32 META_ALT_LOCKED = KeyEvent_META_ALT_ON << LOCKED_SHIFT;
    static const Int32 META_SYM_LOCKED = KeyEvent_META_SYM_ON << LOCKED_SHIFT;

    /**
     * @hide pending API review
     */
    static const Int32 META_SELECTING = 1 << 16;

private:
    static const Int32 USED_SHIFT = 24;

    static const Int64 META_CAP_USED = ((Int64)KeyEvent_META_SHIFT_ON) << USED_SHIFT;
    static const Int64 META_ALT_USED = ((Int64)KeyEvent_META_ALT_ON) << USED_SHIFT;
    static const Int64 META_SYM_USED = ((Int64)KeyEvent_META_SYM_ON) << USED_SHIFT;

    static const Int32 PRESSED_SHIFT = 32;

    static const Int64 META_CAP_PRESSED = ((Int64)KeyEvent_META_SHIFT_ON) << PRESSED_SHIFT;
    static const Int64 META_ALT_PRESSED = ((Int64)KeyEvent_META_ALT_ON) << PRESSED_SHIFT;
    static const Int64 META_SYM_PRESSED = ((Int64)KeyEvent_META_SYM_ON) << PRESSED_SHIFT;

    static const Int32 RELEASED_SHIFT = 40;

    static const Int64 META_CAP_RELEASED = ((Int64)KeyEvent_META_SHIFT_ON) << RELEASED_SHIFT;
    static const Int64 META_ALT_RELEASED = ((Int64)KeyEvent_META_ALT_ON) << RELEASED_SHIFT;
    static const Int64 META_SYM_RELEASED = ((Int64)KeyEvent_META_SYM_ON) << RELEASED_SHIFT;

    static const Int64 META_SHIFT_MASK = META_SHIFT_ON
                    | META_CAP_LOCKED | META_CAP_USED
                    | META_CAP_PRESSED | META_CAP_RELEASED;
    static const Int64 META_ALT_MASK = META_ALT_ON
                    | META_ALT_LOCKED | META_ALT_USED
                    | META_ALT_PRESSED | META_ALT_RELEASED;
    static const Int64 META_SYM_MASK = META_SYM_ON
                    | META_SYM_LOCKED | META_SYM_USED
                    | META_SYM_PRESSED | META_SYM_RELEASED;

    static const AutoPtr<IInterface> CAP;
    static const AutoPtr<IInterface> ALT;
    static const AutoPtr<IInterface> SYM;
    static const AutoPtr<IInterface> SELECTING;

    /**
     * The meta key has been pressed but has not yet been used.
     */
    static const Int32 PRESSED =
        Spanned_SPAN_MARK_MARK | (1 << Spanned_SPAN_USER_SHIFT);

    /**
     * The meta key has been pressed and released but has still
     * not yet been used.
     */
    static const Int32 RELEASED =
        Spanned_SPAN_MARK_MARK | (2 << Spanned_SPAN_USER_SHIFT);

    /**
     * The meta key has been pressed and used but has not yet been released.
     */
    static const Int32 USED =
        Spanned_SPAN_MARK_MARK | (3 << Spanned_SPAN_USER_SHIFT);

    /**
     * The meta key has been pressed and released without use, and then
     * pressed again; it may also have been released again.
     */
    static const Int32 LOCKED =
        Spanned_SPAN_MARK_MARK | (4 << Spanned_SPAN_USER_SHIFT);
};
#endif//__METAKEYKEYLISTENER_H__
