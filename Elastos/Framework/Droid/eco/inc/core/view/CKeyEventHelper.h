
#ifndef __CKEYEVENTHELPER_H__
#define __CKEYEVENTHELPER_H__

#include "_CKeyEventHelper.h"

CarClass(CKeyEventHelper)
{
public:
    CARAPI GetMaxKeyCode(
        /* [out] */ Int32* keyCode);

    CARAPI GetDeadChar(
        /* [in] */ Int32 accent,
        /* [in] */ Int32 c,
        /* [out] */ Int32* deadChar);

    CARAPI ChangeTimeRepeat(
        /* [in] */ IKeyEvent* event,
        /* [in] */ Int64 eventTime,
        /* [in] */ Int32 newRepeat,
        /* [out] */ IKeyEvent** newEvent);

    CARAPI ChangeTimeRepeatEx(
        /* [in] */ IKeyEvent* event,
        /* [in] */ Int64 eventTime,
        /* [in] */ Int32 newRepeat,
        /* [in] */ Int32 newFlags,
        /* [out] */ IKeyEvent** newEvent);

    CARAPI ChangeAction(
        /* [in] */ IKeyEvent* event,
        /* [in] */ Int32 action,
        /* [out] */ IKeyEvent** newEvent);

    CARAPI ChangeFlags(
        /* [in] */ IKeyEvent* event,
        /* [in] */ Int32 flags,
        /* [out] */ IKeyEvent** newEvent);

    CARAPI IsModifierKey(
        /* [in] */ Int32 keyCode,
        /* [out] */ Boolean* res);
};

#endif //__CKEYEVENTHELPER_H__
