
#ifndef __METAKEYKEYLISTENER_H__
#define __METAKEYKEYLISTENER_H__

#include "ext/frameworkext.h"

class MetaKeyKeyListener
{
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
};
#endif//__METAKEYKEYLISTENER_H__
