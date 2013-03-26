
#include "ext/frameworkdef.h"
#include "view/CKeyEventHelper.h"
#include "view/CKeyEvent.h"

ECode CKeyEventHelper::GetMaxKeyCode(
    /* [out] */ Int32* keyCode)
{
	VALIDATE_NOT_NULL(keyCode);
	*keyCode = CKeyEvent::GetMaxKeyCode();

	return NOERROR;
}

ECode CKeyEventHelper::GetDeadChar(
    /* [in] */ Int32 accent,
    /* [in] */ Int32 c,
    /* [out] */ Int32* deadChar)
{
	VALIDATE_NOT_NULL(deadChar);
	*deadChar = CKeyEvent::GetDeadChar(accent, c);

	return NOERROR;
}

ECode CKeyEventHelper::ChangeTimeRepeat(
    /* [in] */ IKeyEvent* event,
    /* [in] */ Int64 eventTime,
    /* [in] */ Int32 newRepeat,
    /* [out] */ IKeyEvent** newEvent)
{
	VALIDATE_NOT_NULL(newEvent);
	return CKeyEvent::ChangeTimeRepeat(event, eventTime, newRepeat, newEvent);
}

ECode CKeyEventHelper::ChangeTimeRepeatEx(
    /* [in] */ IKeyEvent* event,
    /* [in] */ Int64 eventTime,
    /* [in] */ Int32 newRepeat,
    /* [in] */ Int32 newFlags,
    /* [out] */ IKeyEvent** newEvent)
{
	VALIDATE_NOT_NULL(newEvent);
	return CKeyEvent::ChangeTimeRepeat(event, eventTime, newRepeat, newFlags, newEvent);
}

ECode CKeyEventHelper::ChangeAction(
    /* [in] */ IKeyEvent* event,
    /* [in] */ Int32 action,
    /* [out] */ IKeyEvent** newEvent)
{
	VALIDATE_NOT_NULL(newEvent);
	return CKeyEvent::ChangeAction(event, action, newEvent);
}

ECode CKeyEventHelper::ChangeFlags(
    /* [in] */ IKeyEvent* event,
    /* [in] */ Int32 flags,
    /* [out] */ IKeyEvent** newEvent)
{
	VALIDATE_NOT_NULL(event);
	return CKeyEvent::ChangeFlags(event, flags, newEvent);
}

ECode CKeyEventHelper::IsModifierKey(
    /* [in] */ Int32 keyCode,
    /* [out] */ Boolean* res)
{
	VALIDATE_NOT_NULL(res);
	*res = CKeyEvent::IsModifierKey(keyCode);

	return NOERROR;
}
