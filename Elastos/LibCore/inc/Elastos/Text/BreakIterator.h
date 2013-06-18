#ifndef __BREAKITERATOR_H__
#define __BREAKITERATOR_H__

#include "cmdef.h"
#include "Elastos.Text_server.h"
#include <elastos.h>
#include "NativeBreakIterator.h"

using namespace Elastos;

class BreakIterator {
public:
    /**
     * Returns an array of locales for which custom {@code BreakIterator} instances
     * are available.
     * <p>Note that Android does not support user-supplied locale service providers.
     */
    static CARAPI GetAvailableLocales(
        /* [out] */ ArrayOf<ILocale*>** locales);

    /**
     * Returns a new instance of {@code BreakIterator} to iterate over
     * characters using the user's default locale.
     * See "<a href="../util/Locale.html#default_locale">Be wary of the default locale</a>".
     * @return a new instance of {@code BreakIterator} using the default locale.
     */
    static CARAPI GetCharacterInstance(
        /* [out] */ IBreakIterator** instance);

    /**
     * Returns a new instance of {@code BreakIterator} to iterate over
     * characters using the given locale.
     *
     * @param where
     *            the given locale.
     * @return a new instance of {@code BreakIterator} using the given locale.
     */
    static CARAPI GetCharacterInstance(
        /* [in] */ ILocale* where,
        /* [out] */ IBreakIterator** instance);

    /**
     * Returns a new instance of {{@code BreakIterator} to iterate over
     * line breaks using the user's default locale.
     * See "<a href="../util/Locale.html#default_locale">Be wary of the default locale</a>".
     * @return a new instance of {@code BreakIterator} using the default locale.
     */
    static CARAPI GetLineInstance(
        /* [out] */ IBreakIterator** instance);

    /**
     * Returns a new instance of {@code BreakIterator} to iterate over
     * line breaks using the given locale.
     *
     * @param where
     *            the given locale.
     * @return a new instance of {@code BreakIterator} using the given locale.
     * @throws NullPointerException if {@code where} is {@code null}.
     */
    static CARAPI GetLineInstance(
        /* [in] */ ILocale* where,
        /* [out] */ IBreakIterator** instance);

    /**
     * Returns a new instance of {@code BreakIterator} to iterate over
     * sentence-breaks using the default locale.
     * See "<a href="../util/Locale.html#default_locale">Be wary of the default locale</a>".
     * @return a new instance of {@code BreakIterator} using the default locale.
     */
    static CARAPI GetSentenceInstance(
        /* [out] */ IBreakIterator** instance);

    /**
     * Returns a new instance of {@code BreakIterator} to iterate over
     * sentence-breaks using the given locale.
     *
     * @param where
     *            the given locale.
     * @return a new instance of {@code BreakIterator} using the given locale.
     * @throws NullPointerException if {@code where} is {@code null}.
     */
    static CARAPI GetSentenceInstance(
        /* [in] */ ILocale* where,
        /* [out] */ IBreakIterator** instance);

    /**
     * Returns a new instance of {@code BreakIterator} to iterate over
     * word-breaks using the default locale.
     * See "<a href="../util/Locale.html#default_locale">Be wary of the default locale</a>".
     * @return a new instance of {@code BreakIterator} using the default locale.
     */
    static CARAPI GetWordInstance(
        /* [out] */ IBreakIterator** instance);

    /**
     * Returns a new instance of {@code BreakIterator} to iterate over
     * word-breaks using the given locale.
     *
     * @param where
     *            the given locale.
     * @return a new instance of {@code BreakIterator} using the given locale.
     * @throws NullPointerException if {@code where} is {@code null}.
     */
    static CARAPI GetWordInstance(
        /* [in] */ ILocale* where,
        /* [out] */ IBreakIterator** instance);

    /**
     * Indicates whether the given offset is a boundary position. If this method
     * returns true, the current iteration position is set to the given
     * position; if the function returns false, the current iteration position
     * is set as though {@link #following(int)} had been called.
     *
     * @param offset
     *            the given offset to check.
     * @return {@code true} if the given offset is a boundary position; {@code
     *         false} otherwise.
     */
    virtual CARAPI IsBoundary(
        /* [in] */ Int32 offset,
        /* [out] */ Boolean* isBoundary);

    /**
     * Returns the position of last boundary preceding the given offset, and
     * sets the current position to the returned value, or {@code DONE} if the
     * given offset specifies the starting position.
     *
     * @param offset
     *            the given start position to be searched for.
     * @return the position of the last boundary preceding the given offset.
     * @throws IllegalArgumentException
     *            if the offset is invalid.
     */
    virtual CARAPI Preceding(
        /* [in] */ Int32 offset,
        /* [out] */ Int32* position);

    /**
     * Sets the new text string to be analyzed, the current position will be
     * reset to the beginning of this new string, and the old string will be
     * lost.
     *
     * @param newText
     *            the new text string to be analyzed.
     */
    virtual CARAPI SetText(
        /* [in] */ String newText);

    /**
     * Returns this iterator's current position.
     *
     * @return this iterator's current position.
     */
    virtual CARAPI Current(
        /* [out] */ Int32* position) = 0;

    /**
     * Sets this iterator's current position to the first boundary and returns
     * that position.
     *
     * @return the position of the first boundary.
     */
    virtual CARAPI First(
        /* [out] */ Int32* position) = 0;

    /**
     * Sets the position of the first boundary to the one following the given
     * offset and returns this position. Returns {@code DONE} if there is no
     * boundary after the given offset.
     *
     * @param offset
     *            the given position to be searched for.
     * @return the position of the first boundary following the given offset.
     * @throws IllegalArgumentException
     *            if the offset is invalid.
     */
    virtual CARAPI Following(
        /* [in] */ Int32 offset,
        /* [out] */ Int32* position) = 0;

    /**
     * Returns a {@code CharacterIterator} which represents the text being
     * analyzed. Please note that the returned value is probably the internal
     * iterator used by this object. If the invoker wants to modify the status
     * of the returned iterator, it is recommended to first create a clone of
     * the iterator returned.
     *
     * @return a {@code CharacterIterator} which represents the text being
     *         analyzed.
     */
    virtual CARAPI GetText(
        /* [out] */ ICharacterIterator** text) = 0;

    /**
     * Sets this iterator's current position to the last boundary and returns
     * that position.
     *
     * @return the position of last boundary.
     */
    virtual CARAPI Last(
        /* [out] */ Int32* position) = 0;

    /**
     * Sets this iterator's current position to the next boundary after the
     * current position, and returns this position. Returns {@code DONE} if no
     * boundary was found after the current position.
     *
     * @return the position of last boundary.
     */
    virtual CARAPI Next(
        /* [out] */ Int32* position) = 0;

    /**
     * Sets this iterator's current position to the next boundary after the
     * given position, and returns that position. Returns {@code DONE} if no
     * boundary was found after the given position.
     *
     * @param n
     *            the given position.
     * @return the position of last boundary.
     */
    virtual CARAPI NextEx(
        /* [in] */ Int32 n,
        /* [out] */ Int32* position) = 0;

    /**
     * Sets this iterator's current position to the previous boundary before the
     * current position and returns that position. Returns {@code DONE} if
     * no boundary was found before the current position.
     *
     * @return the position of last boundary.
     */
    virtual CARAPI Previous(
        /* [out] */ Int32* position) = 0;

    /**
     * Sets the new text to be analyzed by the given {@code CharacterIterator}.
     * The position will be reset to the beginning of the new text, and other
     * status information of this iterator will be kept.
     *
     * @param newText
     *            the {@code CharacterIterator} referring to the text to be
     *            analyzed.
     */
    virtual CARAPI SetTextEx(
        /* [in] */ ICharacterIterator* newText) = 0;

protected:
    /**
     * Default constructor, for use by subclasses.
     */
    BreakIterator();

    /*
     * wrapping constructor
     */
    BreakIterator(
        /* [in] */ NativeBreakIterator* iterator);

public:
    // the wrapped ICU implementation
    NativeBreakIterator* mWrapped;

};
#endif //__BREAKITERATOR_H__