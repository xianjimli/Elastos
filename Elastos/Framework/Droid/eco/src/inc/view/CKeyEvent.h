
#ifndef __CKEYEVENT_H__
#define __CKEYEVENT_H__

#include "_CKeyEvent.h"
#include "view/InputEvent.h"
#include "view/ElKeyCharacterMap.h"
#include <elastos/AutoPtr.h>

CarClass(CKeyEvent), public InputEvent
{
    friend class CDispatcherState;
public:
    /**
     * Returns the maximum keycode.
     */
    static CARAPI_(Int32) GetMaxKeyCode();

    /**
     * Get the character that is produced by putting accent on the character
     * c.
     * For example, getDeadChar('`', 'e') returns &egrave;.
     */
    static CARAPI_(Int32) GetDeadChar(
        /* [in] */ Int32 accent,
        /* [in] */ Int32 c);

    /**
     * Create a new key event that is the same as the given one, but whose
     * event time and repeat count are replaced with the given value.
     *
     * @param event The existing event to be copied.  This is not modified.
     * @param eventTime The new event time
     * (in {@link android.os.SystemClock#uptimeMillis}) of the event.
     * @param newRepeat The new repeat count of the event.
     */
    static CARAPI ChangeTimeRepeat(
        /* [in] */ IKeyEvent* event,
        /* [in] */ Int64 eventTime,
        /* [in] */ Int32 newRepeat,
        /* [out] */ IKeyEvent** newEvent);

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
    static CARAPI ChangeTimeRepeat(
        /* [in] */ IKeyEvent* event,
        /* [in] */ Int64 eventTime,
        /* [in] */ Int32 newRepeat,
        /* [in] */ Int32 newFlags,
        /* [out] */ IKeyEvent** newEvent);

    /**
     * Create a new key event that is the same as the given one, but whose
     * action is replaced with the given value.
     *
     * @param event The existing event to be copied.  This is not modified.
     * @param action The new action code of the event.
     */
    static CARAPI ChangeAction(
        /* [in] */ IKeyEvent* event,
        /* [in] */ Int32 action,
        /* [out] */ IKeyEvent** newEvent);

    /**
     * Create a new key event that is the same as the given one, but whose
     * flags are replaced with the given value.
     *
     * @param event The existing event to be copied.  This is not modified.
     * @param flags The new flags constant.
     */
    static CARAPI ChangeFlags(
        /* [in] */ IKeyEvent* event,
        /* [in] */ Int32 flags,
        /* [out] */ IKeyEvent** newEvent);

    /**
     * Returns true if this key code is a modifier key.
     *
     * @return whether the provided keyCode is one of
     * {@link #KEYCODE_SHIFT_LEFT} {@link #KEYCODE_SHIFT_RIGHT},
     * {@link #KEYCODE_ALT_LEFT}, {@link #KEYCODE_ALT_RIGHT}
     * or {@link #KEYCODE_SYM}.
     */
    static CARAPI_(Boolean) IsModifierKey(
        /* [in] */ Int32 keyCode);

    /** @hide */
    static CARAPI CreateFromParcelBody(
        /* [in] */ IParcel* in,
        /* [out] */ IKeyEvent** newEvent);

public:
    CARAPI constructor(
        /* [in] */ Int32 action,
        /* [in] */ Int32 code);

    CARAPI constructor(
        /* [in] */ Int64 downTime,
        /* [in] */ Int64 eventTime,
        /* [in] */ Int32 action,
        /* [in] */ Int32 code,
        /* [in] */ Int32 repeat);

    CARAPI constructor(
        /* [in] */ Int64 downTime,
        /* [in] */ Int64 eventTime,
        /* [in] */ Int32 action,
        /* [in] */ Int32 code,
        /* [in] */ Int32 repeat,
        /* [in] */ Int32 metaState);

    CARAPI constructor(
        /* [in] */ Int64 downTime,
        /* [in] */ Int64 eventTime,
        /* [in] */ Int32 action,
        /* [in] */ Int32 code,
        /* [in] */ Int32 repeat,
        /* [in] */ Int32 metaState,
        /* [in] */ Int32 deviceId,
        /* [in] */ Int32 scancode);

    CARAPI constructor(
        /* [in] */ Int64 downTime,
        /* [in] */ Int64 eventTime,
        /* [in] */ Int32 action,
        /* [in] */ Int32 code,
        /* [in] */ Int32 repeat,
        /* [in] */ Int32 metaState,
        /* [in] */ Int32 deviceId,
        /* [in] */ Int32 scancode,
        /* [in] */ Int32 flags);

    CARAPI constructor(
        /* [in] */ Int64 downTime,
        /* [in] */ Int64 eventTime,
        /* [in] */ Int32 action,
        /* [in] */ Int32 code,
        /* [in] */ Int32 repeat,
        /* [in] */ Int32 metaState,
        /* [in] */ Int32 deviceId,
        /* [in] */ Int32 scancode,
        /* [in] */ Int32 flags,
        /* [in] */ Int32 source);

    CARAPI constructor(
        /* [in] */ Int64 time,
        /* [in] */ String characters,
        /* [in] */ Int32 deviceId,
        /* [in] */ Int32 flags);

    CARAPI constructor(
        /* [in] */ IKeyEvent* origEvent);

    //@Deprecated
    CARAPI constructor(
        /* [in] */ IKeyEvent* origEvent,
        /* [in] */ Int64 eventTime,
        /* [in] */ Int32 newRepeat);

    // private
    CARAPI constructor(
        /* [in] */ IKeyEvent* origEvent,
        /* [in] */ Int32 action);

    // private
    CARAPI constructor(
        /* [in] */ IParcel* in);

    CARAPI GetDeviceId(
        /* [out] */ Int32* deviceId);

    CARAPI GetDevice(
        /* [out] */ IInputDevice** device);

    CARAPI GetSource(
        /* [out] */ Int32* source);

    CARAPI SetSource(
        /* [in] */ Int32 source);

    CARAPI IsDown(
        /* [out] */ Boolean* isDown);

    CARAPI IsSystem(
        /* [out] */ Boolean* isSystem);

    CARAPI HasDefaultAction(
        /* [out] */ Boolean* hasDefaultAction);

    CARAPI GetMetaState(
        /* [out] */ Int32* metaState);

    CARAPI GetFlags(
        /* [out] */ Int32* flags);

    CARAPI IsAltPressed(
        /* [out] */ Boolean* isAltPressed);

    CARAPI IsShiftPressed(
        /* [out] */ Boolean* isShiftPressed);

    CARAPI IsSymPressed(
        /* [out] */ Boolean* isSymPressed);

    CARAPI GetAction(
        /* [out] */ Int32* action);

    CARAPI IsCanceled(
        /* [out] */ Boolean* isCanceled);

    CARAPI StartTracking();

    CARAPI IsTracking(
        /* [out] */ Boolean* isTracking);

    CARAPI IsLongPress(
        /* [out] */ Boolean* isLongPress);

    CARAPI GetKeyCode(
        /* [out] */ Int32* keyCode);

    CARAPI GetCharacters(
        /* [out] */ String* characters);

    CARAPI GetScanCode(
        /* [out] */ Int32* scanCode);

    CARAPI GetRepeatCount(
        /* [out] */ Int32* repeatCount);

    CARAPI GetDownTime(
        /* [out] */ Int64* downTime);

    CARAPI GetEventTime(
        /* [out] */ Int64* eventTime);

    CARAPI GetKeyboardDevice(
        /* [out] */ Int32* deviceId);

    CARAPI GetDisplayLabel(
        /* [out] */ Char16* displayLabel);

    CARAPI GetUnicodeChar(
        /* [out] */ Int32* unicodeChar);

    CARAPI GetUnicodeCharEx(
        /* [in] */ Int32 meta,
        /* [out] */ Int32* unicodeChar);

    CARAPI GetKeyData(
        /* [in] */ Handle32 keyData,
        /* [out] */ Boolean* result);

    CARAPI GetMatch(
        /* [in] */ const ArrayOf<Char16>& chars,
        /* [out] */ Char16* match);

    CARAPI GetMatchEx(
        /* [in] */ const ArrayOf<Char16>& chars,
        /* [in] */ Int32 modifiers,
        /* [out] */ Char16* match);

    CARAPI GetNumber(
        /* [out] */ Char16* ch);

    CARAPI IsPrintingKey(
        /* [out] */ Boolean* result);

    CARAPI Dispatch(
        /* [in] */ IKeyEventCallback* receiver,
        /* [out] */ Boolean* result);

    CARAPI DispatchEx(
        /* [in] */ IKeyEventCallback* receiver,
        /* [in] */ IDispatcherState* state,
        /* [in] */ IInterface* target,
        /* [out] */ Boolean* result);

    CARAPI ReadFromParcel(
        /* [in] */ IParcel* source);

    CARAPI WriteToParcel(
        /* [in] */ IParcel* dest);

    //public String toString() {
    //    return "KeyEvent{action=" + mAction + " code=" + mKeyCode
    //        + " repeat=" + mRepeatCount
    //        + " meta=" + mMetaState + " scancode=" + mScanCode
    //        + " mFlags=" + mFlags + "}";
    //}

    //public static final Parcelable.Creator<KeyEvent> CREATOR
    //        = new Parcelable.Creator<KeyEvent>() {
    //    public KeyEvent createFromParcel(Parcel in) {
    //        in.readInt(); // skip token, we already know this is a KeyEvent
    //        return KeyEvent.createFromParcelBody(in);
    //    }

    //    public KeyEvent[] newArray(Int32 size) {
    //        return new KeyEvent[size];
    //    }
    //};

private:
    static const Int32 LAST_KEYCODE = KeyEvent_KEYCODE_BUTTON_MODE;

    static const Boolean DEBUG = FALSE;
    static const String TAG;

    Int32 mMetaState;
    Int32 mAction;
    Int32 mKeyCode;
    Int32 mScanCode;
    Int32 mRepeatCount;
    Int32 mFlags;
    Int64 mDownTime;
    Int64 mEventTime;
    String mCharacters;
};

#endif //__CKEYEVENT_H__
