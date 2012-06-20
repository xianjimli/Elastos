
#ifndef __TEXTUTILS_H__
#define __TEXTUTILS_H__

#include "ext/frameworkext.h"
#include <elastos/AutoPtr.h>
#include <elastos/Mutex.h>

using namespace Elastos::System::Threading;

class TextUtils
{
public:
    static CARAPI_(void) GetChars(
        /* [in] */ ICharSequence* s,
        /* [in] */ Int32 start,
        /* [in] */ Int32 end,
        /* [out] */ BufferOf<Byte>* dest,
        /* [in] */ Int32 destoff);

    static CARAPI_(Int32) IndexOf(
        /* [in] */ ICharSequence* s,
        /* [in] */ Char32 ch);

    static CARAPI_(Int32) IndexOf(
        /* [in] */ ICharSequence* s,
        /* [in] */ Char32 ch,
        /* [in] */ Int32 start);

    static CARAPI_(Int32) IndexOf(
        /* [in] */ ICharSequence* s,
        /* [in] */ Char32 ch,
        /* [in] */ Int32 start,
        /* [in] */ Int32 end);

    static CARAPI_(Int32) LastIndexOf(
        /* [in] */ ICharSequence* s,
        /* [in] */ Char32 ch);

    static CARAPI_(Int32) LastIndexOf(
        /* [in] */ ICharSequence* s,
        /* [in] */ Char32 ch,
        /* [in] */ Int32 last);

    static CARAPI_(Int32) LastIndexOf(
        /* [in] */ ICharSequence* s,
        /* [in] */ Char32 ch,
        /* [in] */ Int32 start,
        /* [in] */ Int32 last);

    static CARAPI_(Int32) IndexOf(
        /* [in] */ ICharSequence* s,
        /* [in] */ ICharSequence* needle);

    static CARAPI_(Int32) IndexOf(
        /* [in] */ ICharSequence* s,
        /* [in] */ ICharSequence* needle,
        /* [in] */ Int32 start);

    static CARAPI_(Int32) IndexOf(
        /* [in] */ ICharSequence* s,
        /* [in] */ ICharSequence* needle,
        /* [in] */ Int32 start,
        /* [in] */ Int32 end);

    static CARAPI_(Boolean) RegionMatches(
        /* [in] */ ICharSequence* one,
        /* [in] */ Int32 toffset,
        /* [in] */ ICharSequence* two,
        /* [in] */ Int32 ooffset,
        /* [in] */ Int32 len);

    /**
     * Create a new String object containing the given range of characters
     * from the source string.  This is different than simply calling
     * {@link CharSequence#subSequence(int, int) CharSequence.subSequence}
     * in that it does not preserve any style runs in the source sequence,
     * allowing a more efficient implementation.
     */
    static CARAPI Substring(
        /* [in] */ ICharSequence* source,
        /* [in] */ Int32 start,
        /* [in] */ Int32 end,
        /* [out] */ String* substr);

//    /**
//     * Returns a string containing the tokens joined by delimiters.
//     * @param tokens an array objects to be joined. Strings will be formed from
//     *     the objects by calling object.toString().
//     */
//    public static String join(CharSequence delimiter, Object[] tokens);
//
//    /**
//     * Returns a string containing the tokens joined by delimiters.
//     * @param tokens an array objects to be joined. Strings will be formed from
//     *     the objects by calling object.toString().
//     */
//    public static String join(CharSequence delimiter, Iterable tokens);
//
//    /**
//     * String.split() returns [''] when the string to be split is empty. This returns []. This does
//     * not remove any empty strings from the result. For example split("a,", ","  ) returns {"a", ""}.
//     *
//     * @param text the string to split
//     * @param expression the regular expression to match
//     * @return an array of strings. The array will be empty if text is empty
//     *
//     * @throws NullPointerException if expression or text is null
//     */
//    public static String[] split(String text, String expression);
//
//    /**
//     * Splits a string on a pattern. String.split() returns [''] when the string to be
//     * split is empty. This returns []. This does not remove any empty strings from the result.
//     * @param text the string to split
//     * @param pattern the regular expression to match
//     * @return an array of strings. The array will be empty if text is empty
//     *
//     * @throws NullPointerException if expression or text is null
//     */
//    public static String[] split(String text, Pattern pattern);

    static CARAPI_(AutoPtr<ICharSequence>) StringOrSpannedString(
        /* [in] */ ICharSequence* source);

    /**
     * Returns true if the string is null or 0-length.
     * @param str the string to be examined
     * @return true if str is null or zero length
     */
    static CARAPI_(Boolean) IsEmpty(
        /* [in] */ ICharSequence* str);

    /**
     * Returns the length that the specified CharSequence would have if
     * spaces and control characters were trimmed from the start and end,
     * as by {@link String#trim}.
     */
    static CARAPI_(Int32) GetTrimmedLength(
        /* [in] */ ICharSequence* s);

    /**
     * Returns true if a and b are equal, including if they are both null.
     * <p><i>Note: In platform versions 1.1 and earlier, this method only worked well if
     * both the arguments were instances of String.</i></p>
     * @param a first CharSequence to check
     * @param b second CharSequence to check
     * @return true if a and b are equal
     */
    static CARAPI_(Boolean) Equals(
        /* [in] */ ICharSequence* a,
        /* [in] */ ICharSequence* b);

    // XXX currently this only reverses chars, not spans
    static CARAPI_(AutoPtr<ICharSequence>) GetReverse(
        /* [in] */ ICharSequence* source,
        /* [in] */ Int32 start,
        /* [in] */ Int32 end);

    static CARAPI_(Int32) GetOffsetBefore(
        /* [in] */ ICharSequence* text,
        /* [in] */ Int32 offset);

    static CARAPI_(Int32) GetOffsetAfter(
        /* [in] */ ICharSequence* text,
        /* [in] */ Int32 offset);

    /**
     * Copies the spans from the region <code>start...end</code> in
     * <code>source</code> to the region
     * <code>destoff...destoff+end-start</code> in <code>dest</code>.
     * Spans in <code>source</code> that begin before <code>start</code>
     * or end after <code>end</code> but overlap this range are trimmed
     * as if they began at <code>start</code> or ended at <code>end</code>.
     *
     * @throws IndexOutOfBoundsException if any of the copied spans
     * are out of range in <code>dest</code>.
     */
    static CARAPI_(void) CopySpansFrom(
        /* [in] */ ISpanned* source,
        /* [in] */ Int32 start,
        /* [in] */ Int32 end,
        /* [in] */ InterfaceID kind,
        /* [in] */ ISpannable* dest,
        /* [in] */ Int32 destoff);

    /**
     * Returns the original text if it fits in the specified width
     * given the properties of the specified Paint,
     * or, if it does not fit, a truncated
     * copy with ellipsis character added at the specified edge or center.
     */
    static CARAPI_(AutoPtr<ICharSequence>) Ellipsize(
        /* [in] */ ICharSequence* text,
        /* [in] */ ITextPaint* p,
        /* [in] */ Float avail,
        /* [in] */ TextUtilsTruncateAt where);

    /**
     * Returns the original text if it fits in the specified width
     * given the properties of the specified Paint,
     * or, if it does not fit, a copy with ellipsis character added
     * at the specified edge or center.
     * If <code>preserveLength</code> is specified, the returned copy
     * will be padded with zero-width spaces to preserve the original
     * length and offsets instead of truncating.
     * If <code>callback</code> is non-null, it will be called to
     * report the start and end of the ellipsized range.
     */
    static CARAPI_(AutoPtr<ICharSequence>) Ellipsize(
        /* [in] */ ICharSequence* text,
        /* [in] */ ITextPaint* p,
        /* [in] */ Float avail,
        /* [in] */ TextUtilsTruncateAt where,
        /* [in] */ Boolean preserveLength,
        /* [in] */ IEllipsizeCallback* callback);

    /* package */
    static CARAPI_(void) Obtain(
        /* [in] */ Int32 len,
        /* [out] */ BufferOf<Byte>** buf);

    /* package */
    static CARAPI_(void) Recycle(
        /* [in] */ BufferOf<Byte>** temp);

private:
    static Mutex sLock;
    static BufferOf<Byte>* sTemp;
};

#endif //__TEXTUTILS_H__
