#ifndef __KEYCHARACTERMAP_H__
#define __KEYCHARACTERMAP_H__

#include "ext/frameworkext.h"
#include "utils/ElRefBase.h"
#include <elastos/HashMap.h>
#include <elastos/Vector.h>
#include <elastos/Mutex.h>
#include <elastos/AutoPtr.h>

using namespace Elastos::System::Threading;

class KeyCharacterMap;

class ElKeyCharacterMap : public ElRefBase
{
public:
    /**
     * The id of the device's primary built in keyboard is always 0.
     */
    static const Int32 BUILT_IN_KEYBOARD = 0;

    /** A numeric (12-key) keyboard. */
    static const Int32 NUMERIC = 1;

    /** A keyboard with all the letters, but with more than one letter
     *  per key. */
    static const Int32 PREDICTIVE = 2;

    /** A keyboard with all the letters, and maybe some numbers. */
    static const Int32 ALPHA = 3;

    /**
     * This private-use character is used to trigger Unicode character
     * input by hex digits.
     */
    static const Char16 HEX_INPUT = 0xEF00;

    /**
     * This private-use character is used to bring up a character picker for
     * miscellaneous symbols.
     */
    static const Char16 PICKER_DIALOG_INPUT = 0xEF01;

    /*
     * This bit will be set in the return value of {@link #get(Int32, Int32)} if the
     * key is a "dead key."
     */
    static const Int32 COMBINING_ACCENT = 0x80000000;

    /**
     * Mask the return value from {@link #get(Int32, Int32)} with this value to get
     * a printable representation of the accent character of a "dead key."
     */
    static const Int32 COMBINING_ACCENT_MASK = 0x7FFFFFFF;

private:
    static Mutex sLock;
    static HashMap<Int32, AutoPtr<ElKeyCharacterMap> > sInstances;

    /*
     * TODO: Change the table format to support full 21-bit-wide
     * accent characters and combined characters if ever necessary.
     */
    static const Int32 ACUTE = 0x00B4 << 16;
    static const Int32 GRAVE = '`' << 16;
    static const Int32 CIRCUMFLEX = '^' << 16;
    static const Int32 TILDE = '~' << 16;
    static const Int32 UMLAUT = 0x00A8 << 16;

    /**
     * Maps Unicode combining diacritical to display-form dead key
     * (display character shifted left 16 bits).
     */
    static HashMap<Int32, Int32> COMBINING;

    /**
     * Maps combinations of (display-form) dead key and second character
     * to combined output character.
     */
    static HashMap<Int32, Int32> DEAD;

public:
    class KeyData
    {
    public:
        static const Int32 META_LENGTH = 4;

        /**
         * The display label (see {@link #getDisplayLabel}).
         */
        Char16 mDisplayLabel;

        /**
        * The "number" value (see {@link #getNumber}).
        */
        Char16 mNumber;
        /**
         * The character that will be generated in various meta states
         * (the same ones used for {@link #get} and defined as
         * {@link KeyEvent#META_SHIFT_ON} and {@link KeyEvent#META_ALT_ON}).
         *      <table>
         *          <tr><th>Index</th><th align="left">Value</th></tr>
         *          <tr><td>0</td><td>no modifiers</td></tr>
         *          <tr><td>1</td><td>caps</td></tr>
         *          <tr><td>2</td><td>alt</td></tr>
         *          <tr><td>3</td><td>caps + alt</td></tr>
         *      </table>
         */
        Char16 mMeta[META_LENGTH];
    };

public:
    static CARAPI_(void) Initialize();

    /**
     * Loads the key character maps for the keyboard with the specified device id.
     * @param keyboard The device id of the keyboard.
     * @return The associated key character map.
     */
    static CARAPI_(ElKeyCharacterMap*) Load(
        /* [in] */ Int32 keyboard);

    /**
     * Get the character that is produced by putting accent on the character
     * c.
     * For example, getDeadChar('`', 'e') returns &egrave;.
     */
    static CARAPI_(Int32) GetDeadChar(
        /* [in] */ Int32 accent,
        /* [in] */ Int32 c);

    /**
     * Queries the framework about whether any physical keys exist on the
     * device that are capable of producing the given key codes.
     */
    static CARAPI_(Boolean) DeviceHasKey(
        /* [in] */ Int32 keyCode);

    static CARAPI DeviceHasKeys(
        /* [in] */ Int32* keyCodes,
        /* [in] */ Int32 length,
        /* [out] */ Boolean** results);

private:
    ElKeyCharacterMap(
        /* [in] */ Int32 keyboardDevice);

public:
    ~ElKeyCharacterMap();

    CARAPI_(UInt32) AddRef();

    CARAPI_(UInt32) Release();

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
    CARAPI_(Int32) Get(
        /* [in] */ Int32 keyCode,
        /* [in] */ Int32 meta);

    /**
     * Gets the number or symbol associated with the key.  The character value
     * is returned, not the numeric value.  If the key is not a number, but is
     * a symbol, the symbol is retuned.
     */
    CARAPI_(Char16) GetNumber(
        /* [in] */ Int32 keyCode);

    /**
     * The same as {@link #getMatch(Int32,Char16[],Int32) getMatch(keyCode, chars, 0)}.
     */
    CARAPI_(Char16) GetMatch(
        /* [in] */ Int32 keyCode,
        /* [in] */ Char16* chars,
        /* [in] */ Int32 charSize);

    /**
     * If one of the chars in the array can be generated by keyCode,
     * return the Char16; otherwise return '\0'.
     * @param keyCode the key code to look at
     * @param chars the characters to try to find
     * @param modifiers the modifier bits to prefer.  If any of these bits
     *                  are set, if there are multiple choices, that could
     *                  work, the one for this modifier will be set.
     */
    CARAPI_(Char16) GetMatch(
        /* [in] */ Int32 keyCode,
        /* [in] */ Char16* chars,
        /* [in] */ Int32 charSize,
        /* [in] */ Int32 modifiers);

    /**
     * Get the primary character for this key.  In other words, the label
     * that is physically printed on it.
     */
    CARAPI_(Char16) GetDisplayLabel(
        /* [in] */ Int32 keyCode);

    /**
     * Get the characters conversion data for a given keyCode.
     *
     * @param keyCode the keyCode to look for
     * @param results a {@link KeyData} that will be filled with the results.
     *
     * @return whether the key was mapped or not.  If the key was not mapped,
     *         results is not modified.
     */
    CARAPI_(Boolean) GetKeyData(
        /* [in] */ Int32 keyCode,
        /* [in] */ KeyData* results);

    /**
     * Get an array of KeyEvent objects that if put into the input stream
     * could plausibly generate the provided sequence of characters.  It is
     * not guaranteed that the sequence is the only way to generate these
     * events or that it is optimal.
     *
     * @return an array of KeyEvent objects, or NULL if the given Char16 array
     *         can not be generated using the current key character map.
     */
    CARAPI GetEvents(
        /* [in] */ Char16* chars,
        /* [in] */ Int32 charSize,
        /* [out, callee] */ Elastos::Vector<AutoPtr<IKeyEvent> >** rv);

    /**
     * Does this character key produce a glyph?
     */
    CARAPI_(Boolean) IsPrintingKey(
        /* [in] */ Int32 keyCode);

    /**
     * Returns {@link #NUMERIC}, {@link #PREDICTIVE} or {@link #ALPHA}.
     */
    CARAPI_(Int32) GetKeyboardType();

protected:
    CARAPI_(void) Finalize();

private:
    KeyCharacterMap* mNative;
    Int32 mKeyboardDevice;
};
#endif //__KEYCHARACTERMAP_H__