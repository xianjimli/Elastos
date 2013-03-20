#ifndef __STRINGCHARACTERITERATOR_H__
#define __STRINGCHARACTERITERATOR_H__

#include "cmdef.h"
#include "Elastos.Text_server.h"
#include <elastos.h>

using namespace Elastos;

class StringCharacterIterator {
protected:
    String string;

    Int32 start;

    Int32 end;

    Int32 offset;

public:
    /**
     * Constructs a new {@code StringCharacterIterator} on the specified string.
     * The begin and current indices are set to the beginning of the string, the
     * end index is set to the length of the string.
     *
     * @param value
     *            the source string to iterate over.
     */
    CARAPI Init(
        /* [in] */ String value);

    /**
     * Constructs a new {@code StringCharacterIterator} on the specified string
     * with the current index set to the specified value. The begin index is set
     * to the beginning of the string, the end index is set to the length of the
     * string.
     *
     * @param value
     *            the source string to iterate over.
     * @param location
     *            the current index.
     * @throws IllegalArgumentException
     *            if {@code location} is negative or greater than the length
     *            of the source string.
     */
    CARAPI Init(
        /* [in] */ String value,
        /* [in] */ Int32 location);

    /**
     * Constructs a new {@code StringCharacterIterator} on the specified string
     * with the begin, end and current index set to the specified values.
     *
     * @param value
     *            the source string to iterate over.
     * @param start
     *            the index of the first character to iterate.
     * @param end
     *            the index one past the last character to iterate.
     * @param location
     *            the current index.
     * @throws IllegalArgumentException
     *            if {@code start < 0}, {@code start > end}, {@code location <
     *            start}, {@code location > end} or if {@code end} is greater
     *            than the length of {@code value}.
     */
    CARAPI Init(
        /* [in] */ String value,
        /* [in] */ int start,
        /* [in] */ int end,
        /* [in] */ int location);

    /**
     * Returns a new {@code StringCharacterIterator} with the same source
     * string, begin, end, and current index as this iterator.
     *
     * @return a shallow copy of this iterator.
     * @see java.lang.Cloneable
     */
//    @Override
//    public Object clone() {
//        try {
//            return super.clone();
//        } catch (CloneNotSupportedException e) {
//            throw new AssertionError(e); // android-changed
//        }
//    }

    /**
     * Returns the character at the current index in the source string.
     *
     * @return the current character, or {@code DONE} if the current index is
     *         past the end.
     */
    CARAPI Current(
        /* [out] */ Char32* currentCharacter);

    /**
     * Compares the specified object with this {@code StringCharacterIterator}
     * and indicates if they are equal. In order to be equal, {@code object}
     * must be an instance of {@code StringCharacterIterator} that iterates over
     * the same sequence of characters with the same index.
     *
     * @param object
     *            the object to compare with this object.
     * @return {@code true} if the specified object is equal to this
     *         {@code StringCharacterIterator}; {@code false} otherwise.
     * @see #hashCode
     */
    //@Override
    //public boolean equals(Object object) {
    //    if (!(object instanceof StringCharacterIterator)) {
    //        return false;
    //    }
    //    StringCharacterIterator it = (StringCharacterIterator) object;
    //    return string.equals(it.string) && start == it.start && end == it.end
    //            && offset == it.offset;
    //}

    /**
     * Sets the current position to the begin index and returns the character at
     * the new position in the source string.
     *
     * @return the character at the begin index or {@code DONE} if the begin
     *         index is equal to the end index.
     */
    CARAPI First(
        /* [out] */ Char32* firstCharacter);

    /**
     * Returns the begin index in the source string.
     *
     * @return the index of the first character of the iteration.
     */
    CARAPI GetBeginIndex(
        /* [out] */ Int32* index);

    /**
     * Returns the end index in the source string.
     *
     * @return the index one past the last character of the iteration.
     */
    CARAPI GetEndIndex(
        /* [out] */ Int32* index);

    /**
     * Returns the current index in the source string.
     *
     * @return the current index.
     */
    CARAPI GetIndex(
        /* [out] */ Int32* index);

    //@Override
    //public int hashCode() {
    //    return string.hashCode() + start + end + offset;
    //}

    /**
     * Sets the current position to the end index - 1 and returns the character
     * at the new position.
     *
     * @return the character before the end index or {@code DONE} if the begin
     *         index is equal to the end index.
     */
    CARAPI Last(
        /* [out] */ Char32* endCharacter);

    /**
     * Increments the current index and returns the character at the new index.
     *
     * @return the character at the next index, or {@code DONE} if the next
     *         index would be past the end.
     */
    CARAPI Next(
        /* [out] */ Char32* nextCharacter);

    /**
     * Decrements the current index and returns the character at the new index.
     *
     * @return the character at the previous index, or {@code DONE} if the
     *         previous index would be past the beginning.
     */
    CARAPI Previous(
        /* [out] */ Char32* previousCharacter);

    /**
     * Sets the current index in the source string.
     *
     * @param location
     *            the index the current position is set to.
     * @return the character at the new index, or {@code DONE} if
     *         {@code location} is set to the end index.
     * @throws IllegalArgumentException
     *            if {@code location} is smaller than the begin index or greater
     *            than the end index.
     */
    CARAPI SetIndex(
        /* [in] */ Int32 location,
        /* [out] */ Char32* index);

    /**
     * Sets the source string to iterate over. The begin and end positions are
     * set to the start and end of this string.
     *
     * @param value
     *            the new source string.
     */
    CARAPI SetText(
        /* [in] */ String value);
};
#endif //__STRINGCHARACTERITERATOR_H__