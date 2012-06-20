
#include <ui/KeyCharacterMap.h>
#include "view/ElKeyCharacterMap.h"
#include "view/CKeyEvent.h"
#include "text/method/MetaKeyKeyListener.h"
#include "os/SystemClock.h"

using namespace Elastos::System;

const Int32 ElKeyCharacterMap::BUILT_IN_KEYBOARD;
const Int32 ElKeyCharacterMap::NUMERIC;
const Int32 ElKeyCharacterMap::PREDICTIVE;
const Int32 ElKeyCharacterMap::ALPHA;
const Char16 ElKeyCharacterMap::HEX_INPUT;
const Char16 ElKeyCharacterMap::PICKER_DIALOG_INPUT;
const Int32 ElKeyCharacterMap::COMBINING_ACCENT;
const Int32 ElKeyCharacterMap::COMBINING_ACCENT_MASK;

const Int32 ElKeyCharacterMap::ACUTE;
const Int32 ElKeyCharacterMap::GRAVE;
const Int32 ElKeyCharacterMap::CIRCUMFLEX;
const Int32 ElKeyCharacterMap::TILDE;
const Int32 ElKeyCharacterMap::UMLAUT;

Mutex ElKeyCharacterMap::sLock;
HashMap<Int32, AutoPtr<ElKeyCharacterMap> > ElKeyCharacterMap::sInstances;
HashMap<Int32, Int32> ElKeyCharacterMap::COMBINING;
HashMap<Int32, Int32> ElKeyCharacterMap::DEAD;

const Int32 ElKeyCharacterMap::KeyData::META_LENGTH;

void ElKeyCharacterMap::Initialize()
{
    static Boolean isFirst = TRUE;
    if (!isFirst) {
        return;
    }
    isFirst = FALSE;

    COMBINING[0x0300] = (GRAVE >> 16) | COMBINING_ACCENT;
    COMBINING[0x0301] = (ACUTE >> 16) | COMBINING_ACCENT;
    COMBINING[0x0302] = (CIRCUMFLEX >> 16) | COMBINING_ACCENT;
    COMBINING[0x0303] = (TILDE >> 16) | COMBINING_ACCENT;
    COMBINING[0x0308] = (UMLAUT >> 16) | COMBINING_ACCENT;

    DEAD[ACUTE | 'A'] = 0x00C1;
    DEAD[ACUTE | 'C'] = 0x0106;
    DEAD[ACUTE | 'E'] = 0x00C9;
    DEAD[ACUTE | 'G'] = 0x01F4;
    DEAD[ACUTE | 'I'] = 0x00CD;
    DEAD[ACUTE | 'K'] = 0x1E30;
    DEAD[ACUTE | 'L'] = 0x0139;
    DEAD[ACUTE | 'M'] = 0x1E3E;
    DEAD[ACUTE | 'N'] = 0x0143;
    DEAD[ACUTE | 'O'] = 0x00D3;
    DEAD[ACUTE | 'P'] = 0x1E54;
    DEAD[ACUTE | 'R'] = 0x0154;
    DEAD[ACUTE | 'S'] = 0x015A;
    DEAD[ACUTE | 'U'] = 0x00DA;
    DEAD[ACUTE | 'W'] = 0x1E82;
    DEAD[ACUTE | 'Y'] = 0x00DD;
    DEAD[ACUTE | 'Z'] = 0x0179;
    DEAD[ACUTE | 'a'] = 0x00E1;
    DEAD[ACUTE | 'c'] = 0x0107;
    DEAD[ACUTE | 'e'] = 0x00E9;
    DEAD[ACUTE | 'g'] = 0x01F5;
    DEAD[ACUTE | 'i'] = 0x00ED;
    DEAD[ACUTE | 'k'] = 0x1E31;
    DEAD[ACUTE | 'l'] = 0x013A;
    DEAD[ACUTE | 'm'] = 0x1E3F;
    DEAD[ACUTE | 'n'] = 0x0144;
    DEAD[ACUTE | 'o'] = 0x00F3;
    DEAD[ACUTE | 'p'] = 0x1E55;
    DEAD[ACUTE | 'r'] = 0x0155;
    DEAD[ACUTE | 's'] = 0x015B;
    DEAD[ACUTE | 'u'] = 0x00FA;
    DEAD[ACUTE | 'w'] = 0x1E83;
    DEAD[ACUTE | 'y'] = 0x00FD;
    DEAD[ACUTE | 'z'] = 0x017A;
    DEAD[CIRCUMFLEX | 'A'] = 0x00C2;
    DEAD[CIRCUMFLEX | 'C'] = 0x0108;
    DEAD[CIRCUMFLEX | 'E'] = 0x00CA;
    DEAD[CIRCUMFLEX | 'G'] = 0x011C;
    DEAD[CIRCUMFLEX | 'H'] = 0x0124;
    DEAD[CIRCUMFLEX | 'I'] = 0x00CE;
    DEAD[CIRCUMFLEX | 'J'] = 0x0134;
    DEAD[CIRCUMFLEX | 'O'] = 0x00D4;
    DEAD[CIRCUMFLEX | 'S'] = 0x015C;
    DEAD[CIRCUMFLEX | 'U'] = 0x00DB;
    DEAD[CIRCUMFLEX | 'W'] = 0x0174;
    DEAD[CIRCUMFLEX | 'Y'] = 0x0176;
    DEAD[CIRCUMFLEX | 'Z'] = 0x1E90;
    DEAD[CIRCUMFLEX | 'a'] = 0x00E2;
    DEAD[CIRCUMFLEX | 'c'] = 0x0109;
    DEAD[CIRCUMFLEX | 'e'] = 0x00EA;
    DEAD[CIRCUMFLEX | 'g'] = 0x011D;
    DEAD[CIRCUMFLEX | 'h'] = 0x0125;
    DEAD[CIRCUMFLEX | 'i'] = 0x00EE;
    DEAD[CIRCUMFLEX | 'j'] = 0x0135;
    DEAD[CIRCUMFLEX | 'o'] = 0x00F4;
    DEAD[CIRCUMFLEX | 's'] = 0x015D;
    DEAD[CIRCUMFLEX | 'u'] = 0x00FB;
    DEAD[CIRCUMFLEX | 'w'] = 0x0175;
    DEAD[CIRCUMFLEX | 'y'] = 0x0177;
    DEAD[CIRCUMFLEX | 'z'] = 0x1E91;
    DEAD[GRAVE | 'A'] = 0x00C0;
    DEAD[GRAVE | 'E'] = 0x00C8;
    DEAD[GRAVE | 'I'] = 0x00CC;
    DEAD[GRAVE | 'N'] = 0x01F8;
    DEAD[GRAVE | 'O'] = 0x00D2;
    DEAD[GRAVE | 'U'] = 0x00D9;
    DEAD[GRAVE | 'W'] = 0x1E80;
    DEAD[GRAVE | 'Y'] = 0x1EF2;
    DEAD[GRAVE | 'a'] = 0x00E0;
    DEAD[GRAVE | 'e'] = 0x00E8;
    DEAD[GRAVE | 'i'] = 0x00EC;
    DEAD[GRAVE | 'n'] = 0x01F9;
    DEAD[GRAVE | 'o'] = 0x00F2;
    DEAD[GRAVE | 'u'] = 0x00F9;
    DEAD[GRAVE | 'w'] = 0x1E81;
    DEAD[GRAVE | 'y'] = 0x1EF3;
    DEAD[TILDE | 'A'] = 0x00C3;
    DEAD[TILDE | 'E'] = 0x1EBC;
    DEAD[TILDE | 'I'] = 0x0128;
    DEAD[TILDE | 'N'] = 0x00D1;
    DEAD[TILDE | 'O'] = 0x00D5;
    DEAD[TILDE | 'U'] = 0x0168;
    DEAD[TILDE | 'V'] = 0x1E7C;
    DEAD[TILDE | 'Y'] = 0x1EF8;
    DEAD[TILDE | 'a'] = 0x00E3;
    DEAD[TILDE | 'e'] = 0x1EBD;
    DEAD[TILDE | 'i'] = 0x0129;
    DEAD[TILDE | 'n'] = 0x00F1;
    DEAD[TILDE | 'o'] = 0x00F5;
    DEAD[TILDE | 'u'] = 0x0169;
    DEAD[TILDE | 'v'] = 0x1E7D;
    DEAD[TILDE | 'y'] = 0x1EF9;
    DEAD[UMLAUT | 'A'] = 0x00C4;
    DEAD[UMLAUT | 'E'] = 0x00CB;
    DEAD[UMLAUT | 'H'] = 0x1E26;
    DEAD[UMLAUT | 'I'] = 0x00CF;
    DEAD[UMLAUT | 'O'] = 0x00D6;
    DEAD[UMLAUT | 'U'] = 0x00DC;
    DEAD[UMLAUT | 'W'] = 0x1E84;
    DEAD[UMLAUT | 'X'] = 0x1E8C;
    DEAD[UMLAUT | 'Y'] = 0x0178;
    DEAD[UMLAUT | 'a'] = 0x00E4;
    DEAD[UMLAUT | 'e'] = 0x00EB;
    DEAD[UMLAUT | 'h'] = 0x1E27;
    DEAD[UMLAUT | 'i'] = 0x00EF;
    DEAD[UMLAUT | 'o'] = 0x00F6;
    DEAD[UMLAUT | 't'] = 0x1E97;
    DEAD[UMLAUT | 'u'] = 0x00FC;
    DEAD[UMLAUT | 'w'] = 0x1E85;
    DEAD[UMLAUT | 'x'] = 0x1E8D;
    DEAD[UMLAUT | 'y'] = 0x00FF;
}

UInt32 ElKeyCharacterMap::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 ElKeyCharacterMap::Release()
{
    return ElRefBase::Release();
}

/**
 * Loads the key character maps for the keyboard with the specified device id.
 * @param keyboard The device id of the keyboard.
 * @return The associated key character map.
 */
ElKeyCharacterMap* ElKeyCharacterMap::Load(
    /* [in] */ Int32 keyboard)
{
    Initialize();

    Mutex::Autolock lock(sLock);

    AutoPtr<ElKeyCharacterMap> result;
    HashMap<Int32, AutoPtr<ElKeyCharacterMap> >::Iterator find = sInstances.Find(keyboard);
    if (find != sInstances.End()) {
        result = find->mSecond;
        return result;
    }

    result = new ElKeyCharacterMap(keyboard);
    sInstances[keyboard] = result;

    return result;
}

ElKeyCharacterMap::ElKeyCharacterMap(
    /* [in] */ Int32 keyboardDevice) :
    mKeyboardDevice(keyboardDevice)
{
    mNative = KeyCharacterMap::load(keyboardDevice);
}

ElKeyCharacterMap::~ElKeyCharacterMap()
{
    Finalize();
}

/**
 * <p>
 * Returns the Unicode character that the specified key would produce
 * when the specified meta bits (see {@link MetaKeyKeyListener})
 * were active.
 * </p><p>
 * Returns 0 if the key is not one that is used to type Unicode
 * characters.
 * </p><p>
 * If the return value has bit {@link #COMBINING_ACCENT} set, the
 * key is a "dead key" that should be combined with another to
 * actually produce a character -- see {@link #getDeadChar} --
 * after masking with {@link #COMBINING_ACCENT_MASK}.
 * </p>
 */
Int32 ElKeyCharacterMap::Get(
    /* [in] */ Int32 keyCode,
    /* [in] */ Int32 meta)
{
    if ((meta & MetaKeyKeyListener::META_CAP_LOCKED) != 0) {
        meta |= KeyEvent_META_SHIFT_ON;
    }

    if ((meta & MetaKeyKeyListener::META_ALT_LOCKED) != 0) {
        meta |= KeyEvent_META_ALT_ON;
    }

    // Ignore caps lock on keys where alt and shift have the same effect.
    if ((meta & MetaKeyKeyListener::META_CAP_LOCKED) != 0) {
        if (mNative->get(keyCode, KeyEvent_META_SHIFT_ON) ==
            mNative->get(keyCode, KeyEvent_META_ALT_ON)) {
            meta &= ~KeyEvent_META_SHIFT_ON;
        }
    }

    Int32 ret = mNative->get(keyCode, meta);
    HashMap<Int32, Int32>::Iterator find = COMBINING.Find(ret);
    if (find != COMBINING.End()) {
        ret = find->mSecond;
    }

    return ret;
}

/**
 * Gets the number or symbol associated with the key.  The character value
 * is returned, not the numeric value.  If the key is not a number, but is
 * a symbol, the symbol is retuned.
 */
Char16 ElKeyCharacterMap::GetNumber(
    /* [in] */ Int32 keyCode)
{
    return mNative->getNumber(keyCode);
}

/**
 * The same as {@link #getMatch(Int32,Char16[],Int32) getMatch(keyCode, chars, 0)}.
 */
Char16 ElKeyCharacterMap::GetMatch(
    /* [in] */ Int32 keyCode,
    /* [in] */ Char16* chars,
    /* [in] */ Int32 charSize)
{
    return GetMatch(keyCode, chars, charSize, 0);
}

/**
 * If one of the chars in the array can be generated by keyCode,
 * return the Char16; otherwise return '\0'.
 * @param keyCode the key code to look at
 * @param chars the characters to try to find
 * @param modifiers the modifier bits to prefer.  If any of these bits
 *                  are set, if there are multiple choices, that could
 *                  work, the one for this modifier will be set.
 */
Char16 ElKeyCharacterMap::GetMatch(
    /* [in] */ Int32 keyCode,
    /* [in] */ Char16* chars,
    /* [in] */ Int32 charSize,
    /* [in] */ Int32 modifiers)
{
    assert(chars);

    return mNative->getMatch(keyCode, chars, charSize, modifiers);
}

/**
 * Get the primary character for this key.  In other words, the label
 * that is physically printed on it.
 */
Char16 ElKeyCharacterMap::GetDisplayLabel(
    /* [in] */ Int32 keyCode)
{
    return mNative->getDisplayLabel(keyCode);
}

/**
 * Get the character that is produced by putting accent on the character
 * c.
 * For example, getDeadChar('`', 'e') returns &egrave;.
 */
Int32 ElKeyCharacterMap::GetDeadChar(
    /* [in] */ Int32 accent,
    /* [in] */ Int32 c)
{
    Initialize();

    HashMap<Int32, Int32>::Iterator find = DEAD.Find((accent << 16) | c);
    if (find != DEAD.End()) {
        return find->mSecond;
    }

    return 0;
}

/**
 * Get the characters conversion data for a given keyCode.
 *
 * @param keyCode the keyCode to look for
 * @param results a {@link KeyData} that will be filled with the results.
 *
 * @return whether the key was mapped or not.  If the key was not mapped,
 *         results is not modified.
 */
Boolean ElKeyCharacterMap::GetKeyData(
    /* [in] */ Int32 keyCode,
    /* [in] */ KeyData* results)
{
    assert(results);
    return mNative->getKeyData(
        keyCode, &results->mDisplayLabel,
        &results->mNumber, results->mMeta);
}

/**
 * Get an array of KeyEvent objects that if put into the input stream
 * could plausibly generate the provided sequence of characters.  It is
 * not guaranteed that the sequence is the only way to generate these
 * events or that it is optimal.
 *
 * @return an array of KeyEvent objects, or NULL if the given Char16 array
 *         can not be generated using the current key character map.
 */
ECode ElKeyCharacterMap::GetEvents(
    /* [in] */ Char16* chars,
    /* [in] */ Int32 charSize,
    /* [out, callee] */ Elastos::Vector<AutoPtr<IKeyEvent> >** rv)
{
    assert(chars);
    VALIDATE_NOT_NULL(rv);

    android::Vector<Int32> keys;
    android::Vector<UInt32> modifiers;
    if (!mNative->getEvents(chars, charSize, &keys, &modifiers)) {
        return E_INVALID_ARGUMENT;
    }

    Int32 N = keys.size();

    // create the events
    *rv = new Elastos::Vector<AutoPtr<IKeyEvent> >();
    AutoPtr<IKeyEvent> keyEvent;

    ECode ec;
    Int64 now = SystemClock::GetUptimeMillis();
    Int32 device = mKeyboardDevice;
    for (Int32 i=0; i<N; i++) {
        Int32 mods = modifiers[i];
        Int32 meta = 0;

        if ((mods & KeyEvent_META_ALT_ON) != 0) {
            meta |= KeyEvent_META_ALT_ON;

            ec = CKeyEvent::New(now, now, KeyEvent_ACTION_DOWN,
                KeyEvent_KEYCODE_ALT_LEFT, 0, meta, device, 0,
                (IKeyEvent**)&keyEvent);
            if (FAILED(ec)) {
                return ec;
            }

            (*rv)->PushBack(keyEvent);
        }

        if ((mods & KeyEvent_META_SHIFT_ON) != 0) {
            meta |= KeyEvent_META_SHIFT_ON;

            ec = CKeyEvent::New(now, now, KeyEvent_ACTION_DOWN,
                KeyEvent_KEYCODE_SHIFT_LEFT, 0, meta, device, 0,
                (IKeyEvent**)&keyEvent);
            if (FAILED(ec)) {
                return ec;
            }

            (*rv)->PushBack(keyEvent);
        }

        if ((mods & KeyEvent_META_SYM_ON) != 0) {
            meta |= KeyEvent_META_SYM_ON;

            ec = CKeyEvent::New(now, now, KeyEvent_ACTION_DOWN,
                KeyEvent_KEYCODE_SYM, 0, meta, device, 0,
                (IKeyEvent**)&keyEvent);
            if (FAILED(ec)) {
                return ec;
            }

            (*rv)->PushBack(keyEvent);
        }

        Int32 key = keys[i];
        ec = CKeyEvent::New(now, now, KeyEvent_ACTION_DOWN,
            key, 0, meta, device, 0, (IKeyEvent**)&keyEvent);
        if (FAILED(ec)) {
            return ec;
        }

        (*rv)->PushBack(keyEvent);

        ec = CKeyEvent::New(now, now, KeyEvent_ACTION_UP,
            key, 0, meta, device, 0, (IKeyEvent**)&keyEvent);
        if (FAILED(ec)) {
            return ec;
        }

        (*rv)->PushBack(keyEvent);

        if ((mods & KeyEvent_META_ALT_ON) != 0) {
            meta &= ~KeyEvent_META_ALT_ON;

            ec = CKeyEvent::New(now, now, KeyEvent_ACTION_UP,
                KeyEvent_KEYCODE_ALT_LEFT, 0, meta, device, 0,
                (IKeyEvent**)&keyEvent);
            if (FAILED(ec)) {
                return ec;
            }

            (*rv)->PushBack(keyEvent);

        }

        if ((mods & KeyEvent_META_SHIFT_ON) != 0) {
            meta &= ~KeyEvent_META_SHIFT_ON;

            ec = CKeyEvent::New(now, now, KeyEvent_ACTION_UP,
                KeyEvent_KEYCODE_SHIFT_LEFT, 0, meta, device, 0,
                (IKeyEvent**)&keyEvent);
            if (FAILED(ec)) {
                return ec;
            }

            (*rv)->PushBack(keyEvent);
        }

        if ((mods & KeyEvent_META_SYM_ON) != 0) {
            meta &= ~KeyEvent_META_SYM_ON;

            ec = CKeyEvent::New(now, now, KeyEvent_ACTION_UP,
                KeyEvent_KEYCODE_SYM, 0, meta, device, 0,
                (IKeyEvent**)&keyEvent);
            if (FAILED(ec)) {
                return ec;
            }

            (*rv)->PushBack(keyEvent);
        }
    }

    return NOERROR;
}

/**
 * Does this character key produce a glyph?
 */
Boolean ElKeyCharacterMap::IsPrintingKey(
    /* [in] */ Int32 keyCode)
{
    //Int32 type = Character.getType(Get(keyCode, 0));

    //switch (type)
    //{
    //    case Character.SPACE_SEPARATOR:
    //    case Character.LINE_SEPARATOR:
    //    case Character.PARAGRAPH_SEPARATOR:
    //    case Character.CONTROL:
    //    case Character.FORMAT:
    //        return false;
    //    default:
    //        return true;
    //}
    return TRUE;
}

void ElKeyCharacterMap::Finalize()
{
    delete mNative;
    mNative = NULL;
}

/**
 * Returns {@link #NUMERIC}, {@link #PREDICTIVE} or {@link #ALPHA}.
 */
Int32 ElKeyCharacterMap::GetKeyboardType()
{
    return mNative->getKeyboardType();
}

/**
 * Queries the framework about whether any physical keys exist on the
 * device that are capable of producing the given key codes.
 */
Boolean ElKeyCharacterMap::DeviceHasKey(
    /* [in] */ Int32 keyCode)
{
    Int32 codeArray[1];
    codeArray[0] = keyCode;

    Boolean* rets = NULL;
    DeviceHasKeys(codeArray, 1, &rets);

    Boolean ret = rets[0];
    delete[] rets;

    return ret;
}

ECode ElKeyCharacterMap::DeviceHasKeys(
    /* [in] */ Int32* keyCodes,
    /* [in] */ Int32 length,
    /* [out] */ Boolean** results)
{
    VALIDATE_NOT_NULL(results);

    *results = new Boolean[length];

    AutoPtr<IServiceManager> sm;
    FAIL_RETURN(GetServiceManager((IServiceManager**)&sm));

    AutoPtr<IWindowManagerEx> wm;
    FAIL_RETURN(sm->GetService("window", (IInterface**)(IWindowManagerEx**)&wm));

    //wm->HasKeys(keyCodes, *results);

    return NOERROR;
}
