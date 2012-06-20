
#include "view/CKeyEvent.h"
#include "view/CInputDevice.h"
#include <ui/Input.h>
#include <Logger.h>

using namespace Elastos::Utility;
using namespace Elastos::Utility::Logging;

const Int32 CKeyEvent::LAST_KEYCODE;
const Boolean CKeyEvent::DEBUG;
const String CKeyEvent::TAG = "KeyEvent";

/**
 * Create a new key event.
 *
 * @param action Action code: either {@link #ACTION_DOWN},
 * {@link #ACTION_UP}, or {@link #ACTION_MULTIPLE}.
 * @param code The key code.
 */
ECode CKeyEvent::constructor(
    /* [in] */ Int32 action,
    /* [in] */ Int32 code)
{
    mAction = action;
    mKeyCode = code;
    mRepeatCount = 0;

    return NOERROR;
}

/**
 * Create a new key event.
 *
 * @param downTime The time (in {@link android.os.SystemClock#uptimeMillis})
 * at which this key code originally went down.
 * @param eventTime The time (in {@link android.os.SystemClock#uptimeMillis})
 * at which this event happened.
 * @param action Action code: either {@link #ACTION_DOWN},
 * {@link #ACTION_UP}, or {@link #ACTION_MULTIPLE}.
 * @param code The key code.
 * @param repeat A repeat count for down events (> 0 if this is after the
 * initial down) or event count for multiple events.
 */
ECode CKeyEvent::constructor(
    /* [in] */ Int64 downTime,
    /* [in] */ Int64 eventTime,
    /* [in] */ Int32 action,
    /* [in] */ Int32 code,
    /* [in] */ Int32 repeat)
{
    mDownTime = downTime;
    mEventTime = eventTime;
    mAction = action;
    mKeyCode = code;
    mRepeatCount = repeat;

    return NOERROR;
}

/**
 * Create a new key event.
 *
 * @param downTime The time (in {@link android.os.SystemClock#uptimeMillis})
 * at which this key code originally went down.
 * @param eventTime The time (in {@link android.os.SystemClock#uptimeMillis})
 * at which this event happened.
 * @param action Action code: either {@link #ACTION_DOWN},
 * {@link #ACTION_UP}, or {@link #ACTION_MULTIPLE}.
 * @param code The key code.
 * @param repeat A repeat count for down events (> 0 if this is after the
 * initial down) or event count for multiple events.
 * @param metaState Flags indicating which meta keys are currently pressed.
 */
ECode CKeyEvent::constructor(
    /* [in] */ Int64 downTime,
    /* [in] */ Int64 eventTime,
    /* [in] */ Int32 action,
    /* [in] */ Int32 code,
    /* [in] */ Int32 repeat,
    /* [in] */ Int32 metaState)
{
    mDownTime = downTime;
    mEventTime = eventTime;
    mAction = action;
    mKeyCode = code;
    mRepeatCount = repeat;
    mMetaState = metaState;

    return NOERROR;
}

/**
 * Create a new key event.
 *
 * @param downTime The time (in {@link android.os.SystemClock#uptimeMillis})
 * at which this key code originally went down.
 * @param eventTime The time (in {@link android.os.SystemClock#uptimeMillis})
 * at which this event happened.
 * @param action Action code: either {@link #ACTION_DOWN},
 * {@link #ACTION_UP}, or {@link #ACTION_MULTIPLE}.
 * @param code The key code.
 * @param repeat A repeat count for down events (> 0 if this is after the
 * initial down) or event count for multiple events.
 * @param metaState Flags indicating which meta keys are currently pressed.
 * @param deviceId The device ID that generated the key event.
 * @param scancode Raw device scan code of the event.
 */
ECode CKeyEvent::constructor(
    /* [in] */ Int64 downTime,
    /* [in] */ Int64 eventTime,
    /* [in] */ Int32 action,
    /* [in] */ Int32 code,
    /* [in] */ Int32 repeat,
    /* [in] */ Int32 metaState,
    /* [in] */ Int32 deviceId,
    /* [in] */ Int32 scancode)
{
    mDownTime = downTime;
    mEventTime = eventTime;
    mAction = action;
    mKeyCode = code;
    mRepeatCount = repeat;
    mMetaState = metaState;
    mDeviceId = deviceId;
    mScanCode = scancode;

    return NOERROR;
}

/**
 * Create a new key event.
 *
 * @param downTime The time (in {@link android.os.SystemClock#uptimeMillis})
 * at which this key code originally went down.
 * @param eventTime The time (in {@link android.os.SystemClock#uptimeMillis})
 * at which this event happened.
 * @param action Action code: either {@link #ACTION_DOWN},
 * {@link #ACTION_UP}, or {@link #ACTION_MULTIPLE}.
 * @param code The key code.
 * @param repeat A repeat count for down events (> 0 if this is after the
 * initial down) or event count for multiple events.
 * @param metaState Flags indicating which meta keys are currently pressed.
 * @param deviceId The device ID that generated the key event.
 * @param scancode Raw device scan code of the event.
 * @param flags The flags for this key event
 */
ECode CKeyEvent::constructor(
    /* [in] */ Int64 downTime,
    /* [in] */ Int64 eventTime,
    /* [in] */ Int32 action,
    /* [in] */ Int32 code,
    /* [in] */ Int32 repeat,
    /* [in] */ Int32 metaState,
    /* [in] */ Int32 deviceId,
    /* [in] */ Int32 scancode,
    /* [in] */ Int32 flags)
{
    mDownTime = downTime;
    mEventTime = eventTime;
    mAction = action;
    mKeyCode = code;
    mRepeatCount = repeat;
    mMetaState = metaState;
    mDeviceId = deviceId;
    mScanCode = scancode;
    mFlags = flags;

    return NOERROR;
}

/**
 * Create a new key event.
 *
 * @param downTime The time (in {@link android.os.SystemClock#uptimeMillis})
 * at which this key code originally went down.
 * @param eventTime The time (in {@link android.os.SystemClock#uptimeMillis})
 * at which this event happened.
 * @param action Action code: either {@link #ACTION_DOWN},
 * {@link #ACTION_UP}, or {@link #ACTION_MULTIPLE}.
 * @param code The key code.
 * @param repeat A repeat count for down events (> 0 if this is after the
 * initial down) or event count for multiple events.
 * @param metaState Flags indicating which meta keys are currently pressed.
 * @param deviceId The device ID that generated the key event.
 * @param scancode Raw device scan code of the event.
 * @param flags The flags for this key event
 * @param source The input source such as {@link InputDevice#SOURCE_KEYBOARD}.
 */
ECode CKeyEvent::constructor(
    /* [in] */ Int64 downTime,
    /* [in] */ Int64 eventTime,
    /* [in] */ Int32 action,
    /* [in] */ Int32 code,
    /* [in] */ Int32 repeat,
    /* [in] */ Int32 metaState,
    /* [in] */ Int32 deviceId,
    /* [in] */ Int32 scancode,
    /* [in] */ Int32 flags,
    /* [in] */ Int32 source)
{
    mDownTime = downTime;
    mEventTime = eventTime;
    mAction = action;
    mKeyCode = code;
    mRepeatCount = repeat;
    mMetaState = metaState;
    mDeviceId = deviceId;
    mScanCode = scancode;
    mFlags = flags;
    mSource = source;

    return NOERROR;
}

/**
 * Create a new key event for a string of characters.  The key code,
 * action, repeat count and source will automatically be set to
 * {@link #KEYCODE_UNKNOWN}, {@link #ACTION_MULTIPLE}, 0, and
 * {@link InputDevice#SOURCE_KEYBOARD} for you.
 *
 * @param time The time (in {@link android.os.SystemClock#uptimeMillis})
 * at which this event occured.
 * @param characters The string of characters.
 * @param deviceId The device ID that generated the key event.
 * @param flags The flags for this key event
 */
ECode CKeyEvent::constructor(
    /* [in] */ Int64 time,
    /* [in] */ String characters,
    /* [in] */ Int32 deviceId,
    /* [in] */ Int32 flags)
{
    mDownTime = time;
    mEventTime = time;
    mCharacters = String::Duplicate(characters);
    mAction = KeyEvent_ACTION_MULTIPLE;
    mKeyCode = KeyEvent_KEYCODE_UNKNOWN;
    mRepeatCount = 0;
    mDeviceId = deviceId;
    mFlags = flags;
    mSource = CInputDevice::SOURCE_KEYBOARD;

    return NOERROR;
}

/**
 * Make an exact copy of an existing key event.
 */
ECode CKeyEvent::constructor(
    /* [in] */ IKeyEvent* origEvent)
{
    if (origEvent == NULL) {
        return E_INVALID_ARGUMENT;
    }

    CKeyEvent* event = (CKeyEvent*)origEvent;
    mDownTime = event->mDownTime;
    mEventTime = event->mEventTime;
    mAction = event->mAction;
    mKeyCode = event->mKeyCode;
    mRepeatCount = event->mRepeatCount;
    mMetaState = event->mMetaState;
    mDeviceId = event->mDeviceId;
    mSource = event->mSource;
    mScanCode = event->mScanCode;
    mFlags = event->mFlags;
    mCharacters = String::Duplicate(event->mCharacters);

    return NOERROR;
}

/**
 * Copy an existing key event, modifying its time and repeat count.
 *
 * @deprecated Use {@link #changeTimeRepeat(KeyEvent, Int64, Int32)}
 * instead.
 *
 * @param origEvent The existing event to be copied.
 * @param eventTime The new event time
 * (in {@link android.os.SystemClock#uptimeMillis}) of the event.
 * @param newRepeat The new repeat count of the event.
 */
//@Deprecated
ECode CKeyEvent::constructor(
    /* [in] */ IKeyEvent* origEvent,
    /* [in] */ Int64 eventTime,
    /* [in] */ Int32 newRepeat)
{
    if (origEvent == NULL) {
        return E_INVALID_ARGUMENT;
    }

    CKeyEvent* event = (CKeyEvent*)origEvent;
    mDownTime = event->mDownTime;
    mEventTime = eventTime;
    mAction = event->mAction;
    mKeyCode = event->mKeyCode;
    mRepeatCount = newRepeat;
    mMetaState = event->mMetaState;
    mDeviceId = event->mDeviceId;
    mSource = event->mSource;
    mScanCode = event->mScanCode;
    mFlags = event->mFlags;
    mCharacters = String::Duplicate(event->mCharacters);

    return NOERROR;
}

/**
 * Copy an existing key event, modifying its action.
 *
 * @param origEvent The existing event to be copied.
 * @param action The new action code of the event.
 */
ECode CKeyEvent::constructor(
    /* [in] */ IKeyEvent* origEvent,
    /* [in] */ Int32 action)
{
    if (origEvent == NULL) {
        return E_INVALID_ARGUMENT;
    }

    CKeyEvent* event = (CKeyEvent*)origEvent;
    mDownTime = event->mDownTime;
    mEventTime = event->mEventTime;
    mAction = action;
    mKeyCode = event->mKeyCode;
    mRepeatCount = event->mRepeatCount;
    mMetaState = event->mMetaState;
    mDeviceId = event->mDeviceId;
    mSource = event->mSource;
    mScanCode = event->mScanCode;
    mFlags = event->mFlags;
    // Don't copy mCharacters, since one way or the other we'll lose it
    // when changing the action.

    return NOERROR;
}

ECode CKeyEvent::constructor(
    /* [in] */ IParcel* in)
{
    if (in == NULL) {
        return E_INVALID_ARGUMENT;
    }

    ReadBaseFromParcel(in);

    in->ReadInt32(&mAction);
    in->ReadInt32(&mKeyCode);
    in->ReadInt32(&mRepeatCount);
    in->ReadInt32(&mMetaState);
    in->ReadInt32(&mScanCode);
    in->ReadInt32(&mFlags);
    in->ReadInt64(&mDownTime);
    in->ReadInt64(&mEventTime);

    return NOERROR;
}

/**
 * Returns the maximum keycode.
 */
Int32 CKeyEvent::GetMaxKeyCode()
{
    return LAST_KEYCODE;
}

/**
 * Get the character that is produced by putting accent on the character
 * c.
 * For example, getDeadChar('`', 'e') returns &egrave;.
 */
Int32 CKeyEvent::GetDeadChar(
    /* [in] */ Int32 accent,
    /* [in] */ Int32 c)
{
    return ElKeyCharacterMap::GetDeadChar(accent, c);
}

/**
 * Create a new key event that is the same as the given one, but whose
 * event time and repeat count are replaced with the given value.
 *
 * @param event The existing event to be copied.  This is not modified.
 * @param eventTime The new event time
 * (in {@link android.os.SystemClock#uptimeMillis}) of the event.
 * @param newRepeat The new repeat count of the event.
 */
ECode CKeyEvent::ChangeTimeRepeat(
    /* [in] */ IKeyEvent* event,
    /* [in] */ Int64 eventTime,
    /* [in] */ Int32 newRepeat,
    /* [out] */ IKeyEvent** newEvent)
{
    return CKeyEvent::New(event, eventTime, newRepeat, newEvent);
}

/**
 * Create a new key event that is the same as the given one, but whose
 * event time and repeat count are replaced with the given value.
 *
 * @param event The existing event to be copied.  This is not modified.
 * @param eventTime The new event time
 * (in {@link android.os.SystemClock#uptimeMillis}) of the event.
 * @param newRepeat The new repeat count of the event.
 * @param newFlags New flags for the event, replacing the entire value
 * in the original event.
 */
ECode CKeyEvent::ChangeTimeRepeat(
    /* [in] */ IKeyEvent* event,
    /* [in] */ Int64 eventTime,
    /* [in] */ Int32 newRepeat,
    /* [in] */ Int32 newFlags,
    /* [out] */ IKeyEvent** newEvent)
{
    ECode ec = CKeyEvent::New(event, eventTime, newRepeat, newEvent);
    if (FAILED(ec)) {
        return ec;
    }

    ((CKeyEvent*)*newEvent)->mEventTime = eventTime;
    ((CKeyEvent*)*newEvent)->mRepeatCount = newRepeat;
    ((CKeyEvent*)*newEvent)->mFlags = newFlags;

    return NOERROR;
}

/**
 * Create a new key event that is the same as the given one, but whose
 * action is replaced with the given value.
 *
 * @param event The existing event to be copied.  This is not modified.
 * @param action The new action code of the event.
 */
ECode CKeyEvent::ChangeAction(
    /* [in] */ IKeyEvent* event,
    /* [in] */ Int32 action,
    /* [out] */ IKeyEvent** newEvent)
{
    return CKeyEvent::New(event, action, newEvent);
}

/**
 * Create a new key event that is the same as the given one, but whose
 * flags are replaced with the given value.
 *
 * @param event The existing event to be copied.  This is not modified.
 * @param flags The new flags constant.
 */
ECode CKeyEvent::ChangeFlags(
    /* [in] */ IKeyEvent* event,
    /* [in] */ Int32 flags,
    /* [out] */ IKeyEvent** newEvent)
{
    ECode ec = CKeyEvent::New(event, newEvent);
    if (FAILED(ec)) {
        return ec;
    }

    ((CKeyEvent*)*newEvent)->mFlags = flags;

    return NOERROR;
}

/**
 * Don't use in new code, instead explicitly check
 * {@link #getAction()}.
 *
 * @return If the action is ACTION_DOWN, returns true; else FALSE.
 *
 * @deprecated
 * @hide
 */
//@Deprecated
ECode CKeyEvent::IsDown(
    /* [out] */ Boolean* isDown)
{
    if (isDown == NULL) {
        return E_INVALID_ARGUMENT;
    }

    *isDown = (mAction == KeyEvent_ACTION_DOWN);

    return NOERROR;
}

/**
 * Is this a system key?  System keys can not be used for menu shortcuts.
 *
 * TODO: this information should come from a table somewhere.
 * TODO: should the dpad keys be here?  arguably, because they also shouldn't be menu shortcuts
 */
ECode CKeyEvent::IsSystem(
    /* [out] */ Boolean* isSystem)
{
    if (isSystem == NULL) {
        return E_INVALID_ARGUMENT;
    }

    *isSystem = android::KeyEvent::isSystemKey(mKeyCode);

    return NOERROR;
}

/** @hide */
ECode CKeyEvent::HasDefaultAction(
    /* [out] */ Boolean* hasDefaultAction)
{
    if (hasDefaultAction == NULL) {
        return E_INVALID_ARGUMENT;
    }

    *hasDefaultAction = android::KeyEvent::hasDefaultAction(mKeyCode);

    return NOERROR;
}

/**
 * <p>Returns the state of the meta keys.</p>
 *
 * @return an integer in which each bit set to 1 represents a pressed
 *         meta key
 *
 * @see #isAltPressed()
 * @see #isShiftPressed()
 * @see #isSymPressed()
 * @see #META_ALT_ON
 * @see #META_SHIFT_ON
 * @see #META_SYM_ON
 */
ECode CKeyEvent::GetMetaState(
    /* [out] */ Int32* metaState)
{
    if (metaState == NULL) {
        return E_INVALID_ARGUMENT;
    }

    *metaState = mMetaState;

    return NOERROR;
}

/**
 * Returns the flags for this key event.
 *
 * @see #FLAG_WOKE_HERE
 */
ECode CKeyEvent::GetFlags(
    /* [out] */ Int32* flags)
{
    if (flags == NULL) {
        return E_INVALID_ARGUMENT;
    }

    *flags = mFlags;

    return NOERROR;
}

/**
 * Returns true if this key code is a modifier key.
 *
 * @return whether the provided keyCode is one of
 * {@link #KEYCODE_SHIFT_LEFT} {@link #KEYCODE_SHIFT_RIGHT},
 * {@link #KEYCODE_ALT_LEFT}, {@link #KEYCODE_ALT_RIGHT}
 * or {@link #KEYCODE_SYM}.
 */
Boolean CKeyEvent::IsModifierKey(
    /* [in] */ Int32 keyCode)
{
    return keyCode == KeyEvent_KEYCODE_SHIFT_LEFT
            || keyCode == KeyEvent_KEYCODE_SHIFT_RIGHT
            || keyCode == KeyEvent_KEYCODE_ALT_LEFT
            || keyCode == KeyEvent_KEYCODE_ALT_RIGHT
            || keyCode == KeyEvent_KEYCODE_SYM;
}

/**
 * <p>Returns the pressed state of the ALT meta key.</p>
 *
 * @return true if the ALT key is pressed, FALSE otherwise
 *
 * @see #KEYCODE_ALT_LEFT
 * @see #KEYCODE_ALT_RIGHT
 * @see #META_ALT_ON
 */
ECode CKeyEvent::IsAltPressed(
    /* [out] */ Boolean* isAltPressed)
{
    if (isAltPressed == NULL) {
        return E_INVALID_ARGUMENT;
    }

    *isAltPressed = (mMetaState & KeyEvent_META_ALT_ON) != 0;

    return NOERROR;
}

/**
 * <p>Returns the pressed state of the SHIFT meta key.</p>
 *
 * @return true if the SHIFT key is pressed, FALSE otherwise
 *
 * @see #KEYCODE_SHIFT_LEFT
 * @see #KEYCODE_SHIFT_RIGHT
 * @see #META_SHIFT_ON
 */
ECode CKeyEvent::IsShiftPressed(
    /* [out] */ Boolean* isShiftPressed)
{
    if (isShiftPressed == NULL) {
        return E_INVALID_ARGUMENT;
    }

    *isShiftPressed = (mMetaState & KeyEvent_META_SHIFT_ON) != 0;

    return NOERROR;
}

/**
 * <p>Returns the pressed state of the SYM meta key.</p>
 *
 * @return true if the SYM key is pressed, FALSE otherwise
 *
 * @see #KEYCODE_SYM
 * @see #META_SYM_ON
 */
ECode CKeyEvent::IsSymPressed(
    /* [out] */ Boolean* isSymPressed)
{
    if (isSymPressed == NULL) {
        return E_INVALID_ARGUMENT;
    }

    *isSymPressed = (mMetaState & KeyEvent_META_SYM_ON) != 0;

    return NOERROR;
}

/**
 * Retrieve the action of this key event.  May be either
 * {@link #ACTION_DOWN}, {@link #ACTION_UP}, or {@link #ACTION_MULTIPLE}.
 *
 * @return The event action: ACTION_DOWN, ACTION_UP, or ACTION_MULTIPLE.
 */
ECode CKeyEvent::GetAction(
    /* [out] */ Int32* action)
{
    if (action == NULL) {
        return E_INVALID_ARGUMENT;
    }

    *action = mAction;

    return NOERROR;
}

/**
 * For {@link #ACTION_UP} events, indicates that the event has been
 * canceled as per {@link #FLAG_CANCELED}.
 */
ECode CKeyEvent::IsCanceled(
    /* [out] */ Boolean* isCanceled)
{
    if (isCanceled == NULL) {
        return E_INVALID_ARGUMENT;
    }

    *isCanceled = (mFlags&KeyEvent_FLAG_CANCELED) != 0;

    return NOERROR;
}

/**
 * Call this during {@link Callback#onKeyDown} to have the system track
 * the key through its final up (possibly including a Int64 press).  Note
 * that only one key can be tracked at a time -- if another key down
 * event is received while a previous one is being tracked, tracking is
 * stopped on the previous event.
 */
ECode CKeyEvent::StartTracking()
{
    mFlags |= KeyEvent_FLAG_START_TRACKING;

    return NOERROR;
}

/**
 * For {@link #ACTION_UP} events, indicates that the event is still being
 * tracked from its initial down event as per
 * {@link #FLAG_TRACKING}.
 */
ECode CKeyEvent::IsTracking(
    /* [out] */ Boolean* isTracking)
{
    if (isTracking == NULL) {
        return E_INVALID_ARGUMENT;
    }

    *isTracking = (mFlags&KeyEvent_FLAG_TRACKING) != 0;

    return NOERROR;
}

/**
 * For {@link #ACTION_DOWN} events, indicates that the event has been
 * canceled as per {@link #FLAG_LONG_PRESS}.
 */
ECode CKeyEvent::IsLongPress(
    /* [out] */ Boolean* isLongPress)
{
    if (isLongPress == NULL) {
        return E_INVALID_ARGUMENT;
    }

    *isLongPress = (mFlags&KeyEvent_FLAG_LONG_PRESS) != 0;

    return NOERROR;
}

/**
 * Retrieve the key code of the key event.  This is the physical key that
 * was pressed, <em>not</em> the Unicode character.
 *
 * @return The key code of the event.
 */
ECode CKeyEvent::GetKeyCode(
    /* [out] */ Int32* keyCode)
{
    if (keyCode == NULL) {
        return E_INVALID_ARGUMENT;
    }

    *keyCode = mKeyCode;

    return NOERROR;
}

/**
 * For the special case of a {@link #ACTION_MULTIPLE} event with key
 * code of {@link #KEYCODE_UNKNOWN}, this is a raw string of characters
 * associated with the event.  In all other cases it is NULL.
 *
 * @return Returns a String of 1 or more characters associated with
 * the event.
 */
ECode CKeyEvent::GetCharacters(
    /* [out] */ String* characters)
{
    if (characters == NULL) {
        return E_INVALID_ARGUMENT;
    }

    *characters = String::Duplicate(mCharacters);

    return NOERROR;
}

/**
 * Retrieve the hardware key id of this key event.  These values are not
 * reliable and vary from device to device.
 *
 * {@more}
 * Mostly this is here for debugging purposes.
 */
ECode CKeyEvent::GetScanCode(
    /* [out] */ Int32* scanCode)
{
    if (scanCode == NULL) {
        return E_INVALID_ARGUMENT;
    }

    *scanCode = mScanCode;

    return NOERROR;
}

/**
 * Retrieve the repeat count of the event.  For both key up and key down
 * events, this is the number of times the key has repeated with the first
 * down starting at 0 and counting up from there.  For multiple key
 * events, this is the number of down/up pairs that have occurred.
 *
 * @return The number of times the key has repeated.
 */
ECode CKeyEvent::GetRepeatCount(
    /* [out] */ Int32* repeatCount)
{
    if (repeatCount == NULL) {
        return E_INVALID_ARGUMENT;
    }

    *repeatCount = mRepeatCount;

    return NOERROR;
}

/**
 * Retrieve the time of the most recent key down event,
 * in the {@link android.os.SystemClock#uptimeMillis} time base.  If this
 * is a down event, this will be the same as {@link #getEventTime()}.
 * Note that when chording keys, this value is the down time of the
 * most recently pressed key, which may <em>not</em> be the same physical
 * key of this event.
 *
 * @return Returns the most recent key down time, in the
 * {@link android.os.SystemClock#uptimeMillis} time base
 */
ECode CKeyEvent::GetDownTime(
    /* [out] */ Int64* downTime)
{
    if (downTime == NULL) {
        return E_INVALID_ARGUMENT;
    }

    *downTime = mDownTime;

    return NOERROR;
}

/**
 * Retrieve the time this event occurred,
 * in the {@link android.os.SystemClock#uptimeMillis} time base.
 *
 * @return Returns the time this event occurred,
 * in the {@link android.os.SystemClock#uptimeMillis} time base.
 */
ECode CKeyEvent::GetEventTime(
    /* [out] */ Int64* eventTime)
{
    if (eventTime == NULL) {
        return E_INVALID_ARGUMENT;
    }

    *eventTime = mEventTime;

    return NOERROR;
}

/**
 * Renamed to {@link #getDeviceId}.
 *
 * @hide
 * @deprecated
 */
ECode CKeyEvent::GetKeyboardDevice(
    /* [out] */ Int32* deviceId)
{
    if (deviceId == NULL) {
        return E_INVALID_ARGUMENT;
    }

    *deviceId = mDeviceId;

    return NOERROR;
}

/**
 * Get the primary character for this key.  In other words, the label
 * that is physically printed on it.
 */
ECode CKeyEvent::GetDisplayLabel(
    /* [out] */ Char16* displayLabel)
{
    if (displayLabel == NULL) {
        return E_INVALID_ARGUMENT;
    }

    *displayLabel = ElKeyCharacterMap::Load(mDeviceId)->GetDisplayLabel(mKeyCode);

    return NOERROR;
}

/**
 * <p>
 * Returns the Unicode character that the key would produce.
 * </p><p>
 * Returns 0 if the key is not one that is used to type Unicode
 * characters.
 * </p><p>
 * If the return value has bit
 * {@link KeyCharacterMap#COMBINING_ACCENT}
 * set, the key is a "dead key" that should be combined with another to
 * actually produce a character -- see {@link #getDeadChar} --
 * after masking with
 * {@link KeyCharacterMap#COMBINING_ACCENT_MASK}.
 * </p>
 */
ECode CKeyEvent::GetUnicodeChar(
    /* [out] */ Int32* unicodeChar)
{
    return GetUnicodeCharEx(mMetaState, unicodeChar);
}

/**
 * <p>
 * Returns the Unicode character that the key would produce.
 * </p><p>
 * Returns 0 if the key is not one that is used to type Unicode
 * characters.
 * </p><p>
 * If the return value has bit
 * {@link KeyCharacterMap#COMBINING_ACCENT}
 * set, the key is a "dead key" that should be combined with another to
 * actually produce a character -- see {@link #getDeadChar} -- after masking
 * with {@link KeyCharacterMap#COMBINING_ACCENT_MASK}.
 * </p>
 */
ECode CKeyEvent::GetUnicodeCharEx(
    /* [in] */ Int32 meta,
    /* [out] */ Int32* unicodeChar)
{
    if (unicodeChar == NULL) {
        return E_INVALID_ARGUMENT;
    }
    *unicodeChar = ElKeyCharacterMap::Load(mDeviceId)->Get(mKeyCode, meta);

    return NOERROR;
}

/**
 * Get the characters conversion data for the key event..
 *
 * @param results a {@link KeyData} that will be filled with the results.
 *
 * @return whether the key was mapped or not.  If the key was not mapped,
 *         results is not modified.
 */
ECode CKeyEvent::GetKeyData(
    /* [in] */ Handle32 keyData,
    /* [out] */ Boolean* result)
{
    if (result == NULL) {
        return NOERROR;
    }
    *result = ElKeyCharacterMap::Load(mDeviceId)->GetKeyData(
        mKeyCode, (ElKeyCharacterMap::KeyData*)keyData);

    return NOERROR;
}

/**
 * The same as {@link #getMatch(char[],Int32) getMatch(chars, 0)}.
 */
ECode CKeyEvent::GetMatch(
    /* [in] */ const ArrayOf<Char16>& chars,
    /* [out] */ Char16* match)
{
    return GetMatchEx(chars, 0, match);
}

/**
 * If one of the chars in the array can be generated by the keyCode of this
 * key event, return the char; otherwise return '\0'.
 * @param chars the characters to try to find
 * @param modifiers the modifier bits to prefer.  If any of these bits
 *                  are set, if there are multiple choices, that could
 *                  work, the one for this modifier will be set.
 */
ECode CKeyEvent::GetMatchEx(
    /* [in] */ const ArrayOf<Char16>& chars,
    /* [in] */ Int32 modifiers,
    /* [out] */ Char16* match)
{
    if (match == NULL) {
        return E_INVALID_ARGUMENT;
    }

    *match = ElKeyCharacterMap::Load(mDeviceId)->GetMatch(
        mKeyCode, chars.GetPayload(), chars.GetLength(), modifiers);

    return NOERROR;
}

/**
 * Gets the number or symbol associated with the key.  The character value
 * is returned, not the numeric value.  If the key is not a number, but is
 * a symbol, the symbol is retuned.
 */
ECode CKeyEvent::GetNumber(
    /* [out] */ Char16* ch)
{
    if (ch == NULL) {
        return E_INVALID_ARGUMENT;
    }
    *ch = ElKeyCharacterMap::Load(mDeviceId)->GetNumber(mKeyCode);

    return NOERROR;
}

/**
 * Does the key code of this key produce a glyph?
 */
ECode CKeyEvent::IsPrintingKey(
    /* [out] */ Boolean* result)
{
    if (result == NULL) {
        return E_INVALID_ARGUMENT;
    }
    *result = ElKeyCharacterMap::Load(mDeviceId)->IsPrintingKey(mKeyCode);

    return NOERROR;
}

/**
 * @deprecated Use {@link #dispatch(Callback, DispatcherState, Object)} instead.
 */
//@Deprecated
ECode CKeyEvent::Dispatch(
    /* [in] */ IKeyEventCallback* receiver,
    /* [out] */ Boolean* result)
{
    return DispatchEx(receiver, NULL, NULL, result);
}

/**
 * Deliver this key event to a {@link Callback} interface.  If this is
 * an ACTION_MULTIPLE event and it is not handled, then an attempt will
 * be made to deliver a single normal event.
 *
 * @param receiver The Callback that will be given the event.
 * @param state State information retained across events.
 * @param target The target of the dispatch, for use in tracking.
 *
 * @return The return value from the Callback method that was called.
 */
ECode CKeyEvent::DispatchEx(
    /* [in] */ IKeyEventCallback* receiver,
    /* [in] */ IDispatcherState* state,
    /* [in] */ IInterface* target,
    /* [out] */ Boolean* result)
{
    ECode ec;
    Boolean res = FALSE;

    switch (mAction) {
    case KeyEvent_ACTION_DOWN:
        {
            mFlags &= ~KeyEvent_FLAG_START_TRACKING;

            //if (DEBUG) {
            //    Logger::D(TAG, "Key down to " + target + " in " + state
            //        + ": " + this);
            //}

            receiver->OnKeyDown(mKeyCode, this, &res);
            if (state != NULL) {
                Boolean isLongPress;
                IsLongPress(&isLongPress);

                Boolean isTracking;
                state->IsTracking(this, &isTracking);

                if (res && mRepeatCount == 0
                    && (mFlags&KeyEvent_FLAG_START_TRACKING) != 0) {
                    if (DEBUG)
                        Logger::D(TAG, "  Start tracking!");

                    state->StartTracking(this, target);
                }
                else if (isLongPress && isTracking) {
                    ECode ec =receiver->OnKeyLongPress(mKeyCode, this, &res);
                    if (FAILED(ec)) {
                        return ec;
                    }

                    if (res) {
                        if (DEBUG)
                            Logger::D(TAG, "  Clear from Int64 press!");

                        state->PerformedLongPress(this);
                        res = TRUE;
                    }
                }
            }
        }
        break;
    case KeyEvent_ACTION_UP:
        {
            //if (DEBUG) {
            //    Logger::D(TAG, "Key up to " + target + " in " + state
            //    + ": " + this);
            //}

            if (state != NULL) {
                state->HandleUpEvent(this);
            }

            ec = receiver->OnKeyUp(mKeyCode, this, &res);
            if (FAILED(ec)) {
                return ec;
            }
        }
        break;
    case KeyEvent_ACTION_MULTIPLE:
        {
            Int32 count = mRepeatCount;
            Int32 code = mKeyCode;

            receiver->OnKeyMultiple(code, count, this, &res);
            if (res) {
                break;
            }

            if (code != KeyEvent_KEYCODE_UNKNOWN) {
                mAction = KeyEvent_ACTION_DOWN;
                mRepeatCount = 0;

                Boolean handled;
                receiver->OnKeyDown(code, this, &handled);
                if (handled) {
                    mAction = KeyEvent_ACTION_UP;
                    receiver->OnKeyUp(code, this, &res);
                }
                mAction = KeyEvent_ACTION_MULTIPLE;
                mRepeatCount = count;
                res = handled;
            }
        }
        break;
    default:
        break;
    }

    if (result) {
        *result = res;
    }

    return NOERROR;
}

/** @hide */
ECode CKeyEvent::CreateFromParcelBody(
    /* [in] */ IParcel* in,
    /* [out] */ IKeyEvent** newEvent)
{
    return CKeyEvent::New(in, newEvent);
}



ECode CKeyEvent::GetDeviceId(
    /* [out] */ Int32* deviceId)
{
    if (deviceId == NULL) {
        return E_INVALID_ARGUMENT;
    }

    *deviceId = InputEvent::GetDeviceId();

    return NOERROR;
}

ECode CKeyEvent::GetDevice(
    /* [out] */ IInputDevice** device)
{
    if (device == NULL) {
        return E_INVALID_ARGUMENT;
    }

    *device = InputEvent::GetDevice();

    return NOERROR;
}

ECode CKeyEvent::GetSource(
    /* [out] */ Int32* source)
{
    if (source == NULL) {
        return E_INVALID_ARGUMENT;
    }

    *source = InputEvent::GetSource();

    return NOERROR;
}

ECode CKeyEvent::SetSource(
    /* [in] */ Int32 source)
{
    InputEvent::SetSource(source);

    return NOERROR;
}

ECode CKeyEvent::ReadFromParcel(
    /* [in] */ IParcel *source)
{
    assert(source);

    Int32 token;
    source->ReadInt32(&token);

    ReadBaseFromParcel(source);

    source->ReadInt32(&mAction);
    source->ReadInt32(&mKeyCode);
    source->ReadInt32(&mRepeatCount);
    source->ReadInt32(&mMetaState);
    source->ReadInt32(&mScanCode);
    source->ReadInt32(&mFlags);
    source->ReadInt64(&mDownTime);
    source->ReadInt64(&mEventTime);

    return NOERROR;
}

ECode CKeyEvent::WriteToParcel(
    /* [in] */ IParcel* dest)
{
    assert(dest);

    dest->WriteInt32(PARCEL_TOKEN_KEY_EVENT);

    WriteBaseToParcel(dest);

    dest->WriteInt32(mAction);
    dest->WriteInt32(mKeyCode);
    dest->WriteInt32(mRepeatCount);
    dest->WriteInt32(mMetaState);
    dest->WriteInt32(mScanCode);
    dest->WriteInt32(mFlags);
    dest->WriteInt64(mDownTime);
    dest->WriteInt64(mEventTime);

    return NOERROR;
}
