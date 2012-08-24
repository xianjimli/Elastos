
#include "text/TextUtils.h"
#include "utils/ArrayUtils.h"
#include <elastos/Character.h>

using namespace Elastos::Core;

Mutex TextUtils::sLock;
ArrayOf<Char8>* TextUtils::sTemp = NULL;

void TextUtils::GetChars(
    /* [in] */ ICharSequence* s,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end,
    /* [out] */ ArrayOf<Char8>* dest,
    /* [in] */ Int32 destoff)
{
//    Class c = s.getClass();
//
//    if (c == String.class)
//        ((String) s).getChars(start, end, dest, destoff);
//    else if (c == StringBuffer.class)
//        ((StringBuffer) s).getChars(start, end, dest, destoff);
//    else if (c == StringBuilder.class)
//        ((StringBuilder) s).getChars(start, end, dest, destoff);
//    else if (s instanceof GetChars)
//        ((GetChars) s).getChars(start, end, dest, destoff);
//    else {
//        for (int i = start; i < end; i++)
//            dest[destoff++] = s.charAt(i);
//    }

    for (Int32 i = start; i < end; i++) {
        Char32 ch;
        s->GetCharAt(i, &ch);
        Int32 bytes;
        ASSERT_SUCCEEDED(Character::ToChars(ch, *dest, destoff, &bytes));
        destoff += bytes;
    }
    (dest->GetPayload())[destoff] = '\0';
}

Int32 TextUtils::IndexOf(
    /* [in] */ ICharSequence* s,
    /* [in] */ Char32 ch)
{
    return IndexOf(s, ch, 0);
}

Int32 TextUtils::IndexOf(
    /* [in] */ ICharSequence* s,
    /* [in] */ Char32 ch,
    /* [in] */ Int32 start)
{
//    Class c = s.getClass();
//
//    if (c == String.class)
//        return ((String) s).indexOf(ch, start);

    Int32 len;
    s->GetLength(&len);
    return IndexOf(s, ch, start, len);
}

Int32 TextUtils::IndexOf(
    /* [in] */ ICharSequence* s,
    /* [in] */ Char32 ch,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end)
{
//    Class c = s.getClass();
//
//    if (s instanceof GetChars || c == StringBuffer.class ||
//        c == StringBuilder.class || c == String.class) {
//        final int INDEX_INCREMENT = 500;
//        char[] temp = obtain(INDEX_INCREMENT);
//
//        while (start < end) {
//            int segend = start + INDEX_INCREMENT;
//            if (segend > end)
//                segend = end;
//
//            getChars(s, start, segend, temp, 0);
//
//            int count = segend - start;
//            for (int i = 0; i < count; i++) {
//                if (temp[i] == ch) {
//                    recycle(temp);
//                    return i + start;
//                }
//            }
//
//            start = segend;
//        }
//
//        recycle(temp);
//        return -1;
//    }
//
    for (Int32 i = start; i < end; i++) {
        Char32 ch_;
        s->GetCharAt(i, &ch_);
        if (ch_ == ch) {
            return i;
        }
    }

    return -1;
}

Int32 TextUtils::LastIndexOf(
    /* [in] */ ICharSequence* s,
    /* [in] */ Char32 ch)
{
    Int32 len;
    s->GetLength(&len);
    return LastIndexOf(s, ch, len - 1);
}

Int32 TextUtils::LastIndexOf(
    /* [in] */ ICharSequence* s,
    /* [in] */ Char32 ch,
    /* [in] */ Int32 last)
{
//    Class c = s.getClass();
//
//    if (c == String.class)
//        return ((String) s).lastIndexOf(ch, last);

    return LastIndexOf(s, ch, 0, last);
}

Int32 TextUtils::LastIndexOf(
    /* [in] */ ICharSequence* s,
    /* [in] */ Char32 ch,
    /* [in] */ Int32 start,
    /* [in] */ Int32 last)
{
    if (last < 0) {
        return -1;
    }

    Int32 len;
    s->GetLength(&len);
    if (last >= len) {
        last = len - 1;
    }

    Int32 end = last + 1;

//    Class c = s.getClass();
//
//    if (s instanceof GetChars || c == StringBuffer.class ||
//        c == StringBuilder.class || c == String.class) {
//        final int INDEX_INCREMENT = 500;
//        char[] temp = obtain(INDEX_INCREMENT);
//
//        while (start < end) {
//            int segstart = end - INDEX_INCREMENT;
//            if (segstart < start)
//                segstart = start;
//
//            getChars(s, segstart, end, temp, 0);
//
//            int count = end - segstart;
//            for (int i = count - 1; i >= 0; i--) {
//                if (temp[i] == ch) {
//                    recycle(temp);
//                    return i + segstart;
//                }
//            }
//
//            end = segstart;
//        }
//
//        recycle(temp);
//        return -1;
//    }

    for (Int32 i = end - 1; i >= start; i--) {
        Char32 ch_;
        s->GetCharAt(i, &ch_);
        if (ch_ == ch) {
            return i;
        }
    }

    return -1;
}

Int32 TextUtils::IndexOf(
    /* [in] */ ICharSequence* s,
    /* [in] */ ICharSequence* needle)
{
    Int32 len;
    s->GetLength(&len);
    return IndexOf(s, needle, 0, len);
}

Int32 TextUtils::IndexOf(
    /* [in] */ ICharSequence* s,
    /* [in] */ ICharSequence* needle,
    /* [in] */ Int32 start)
{
    Int32 len;
    s->GetLength(&len);
    return IndexOf(s, needle, start, len);
}

Int32 TextUtils::IndexOf(
    /* [in] */ ICharSequence* s,
    /* [in] */ ICharSequence* needle,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end)
{
    Int32 nlen;
    needle->GetLength(&nlen);
    if (nlen == 0) {
        return start;
    }

    Char32 ch;
    needle->GetCharAt(0, &ch);

    for (;;) {
        start = IndexOf(s, ch, start);
        if (start > end - nlen) {
            break;
        }

        if (start < 0) {
            return -1;
        }

        if (RegionMatches(s, start, needle, 0, nlen)) {
            return start;
        }

        start++;
    }
    return -1;
}

Boolean TextUtils::RegionMatches(
    /* [in] */ ICharSequence* one,
    /* [in] */ Int32 toffset,
    /* [in] */ ICharSequence* two,
    /* [in] */ Int32 ooffset,
    /* [in] */ Int32 len)
{
//    char[] temp = obtain(2 * len);
//
//    getChars(one, toffset, toffset + len, temp, 0);
//    getChars(two, ooffset, ooffset + len, temp, len);

    Boolean match = TRUE;
//    for (int i = 0; i < len; i++) {
//        if (temp[i] != temp[i + len]) {
//            match = false;
//            break;
//        }
//    }
//
//    recycle(temp);
    return match;
}

/**
 * Create a new String object containing the given range of characters
 * from the source string.  This is different than simply calling
 * {@link CharSequence#subSequence(int, int) CharSequence.subSequence}
 * in that it does not preserve any style runs in the source sequence,
 * allowing a more efficient implementation.
 */
ECode TextUtils::Substring(
    /* [in] */ ICharSequence* source,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end,
    /* [out] */ String* substr)
{
//    if (source instanceof String)
//        return ((String) source).substring(start, end);
//    if (source instanceof StringBuilder)
//        return ((StringBuilder) source).substring(start, end);
//    if (source instanceof StringBuffer)
//        return ((StringBuffer) source).substring(start, end);
//
//    char[] temp = obtain(end - start);
//    getChars(source, start, end, temp, 0);
//    String ret = new String(temp, 0, end - start);
//    recycle(temp);
//
//    return ret;
    return E_NOT_IMPLEMENTED;
}

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

AutoPtr<ICharSequence> TextUtils::StringOrSpannedString(
    /* [in] */ ICharSequence* source)
{
//    if (source == null)
//        return null;
//    if (source instanceof SpannedString)
//        return source;
//    if (source instanceof Spanned)
//        return new SpannedString(source);
//
//    return source.toString();
    return NULL;
}

/**
 * Returns true if the string is null or 0-length.
 * @param str the string to be examined
 * @return true if str is null or zero length
 */
Boolean TextUtils::IsEmpty(
    /* [in] */ ICharSequence* str)
{
    if (str == NULL) return TRUE;

    Int32 len;
    str->GetLength(&len);
    if (len == 0) return TRUE;

    return FALSE;
}

/**
 * Returns the length that the specified CharSequence would have if
 * spaces and control characters were trimmed from the start and end,
 * as by {@link String#trim}.
 */
Int32 TextUtils::GetTrimmedLength(
    /* [in] */ ICharSequence* s)
{
    Int32 len;
    s->GetLength(&len);

    Char32 ch;
    Int32 start = 0;
    while (start < len && (s->GetCharAt(start, &ch), ch <= ' ')) {
        start++;
    }

    Int32 end = len;
    while (end > start && (s->GetCharAt(end - 1, &ch), ch <= ' ')) {
        end--;
    }

    return end - start;
}

/**
 * Returns true if a and b are equal, including if they are both null.
 * <p><i>Note: In platform versions 1.1 and earlier, this method only worked well if
 * both the arguments were instances of String.</i></p>
 * @param a first CharSequence to check
 * @param b second CharSequence to check
 * @return true if a and b are equal
 */
Boolean TextUtils::Equals(
    /* [in] */ ICharSequence* a,
    /* [in] */ ICharSequence* b)
{
    if (a == b) return TRUE;
    Int32 len1, len2;
    if (a != NULL && b != NULL && (a->GetLength(&len1), b->GetLength(&len2), len1 == len2)) {
//        if (a instanceof String && b instanceof String) {
//            return a.equals(b);
//        } else {
            for (Int32 i = 0; i < len1; i++) {
                Char32 ch1, ch2;
                a->GetCharAt(i, &ch1);
                b->GetCharAt(i, &ch2);
                if (ch1 != ch2) return FALSE;
            }
            return TRUE;
//        }
    }
    return FALSE;
}

// XXX currently this only reverses chars, not spans
AutoPtr<ICharSequence> TextUtils::GetReverse(
    /* [in] */ ICharSequence* source,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end)
{
//    return new Reverser(source, start, end);
    return NULL;
}

Int32 TextUtils::GetOffsetBefore(
    /* [in] */ ICharSequence* text,
    /* [in] */ Int32 offset)
{
    if (offset == 0) {
        return 0;
    }
    if (offset == 1) {
        return 0;
    }

    Char32 c;
    text->GetCharAt(offset - 1, &c);

    if (c >= 0xDC00/*'\uDC00'*/ && c <= 0xDFFF/*'\uDFFF'*/) {
        Char32 c1;
        text->GetCharAt(offset - 2, &c1);

        if (c1 >= 0xD800/*'\uD800'*/ && c1 <= 0xDBFF/*'\uDBFF'*/) {
            offset -= 2;
        }
        else {
            offset -= 1;
        }
    }
    else {
        offset -= 1;
    }

    if (ISpanned::Probe(text) != NULL) {
        ArrayOf<IInterface*>* spans;
        ISpanned::Probe(text)->GetSpans(offset, offset,
                EIID_IReplacementSpan, &spans);

        for (Int32 i = 0; i < spans->GetLength(); i++) {
            Int32 start, end;
            ISpanned::Probe(text)->GetSpanStart((*spans)[i], &start);
            ISpanned::Probe(text)->GetSpanEnd((*spans)[i], &end);

            if (start < offset && end > offset) {
                offset = start;
            }

            (*spans)[i]->Release();
        }
        ArrayOf<IInterface*>::Free(spans);
    }

    return offset;
}

Int32 TextUtils::GetOffsetAfter(
    /* [in] */ ICharSequence* text,
    /* [in] */ Int32 offset)
{
    Int32 len;
    text->GetLength(&len);

    if (offset == len) {
        return len;
    }
    if (offset == len - 1) {
        return len;
    }

    Char32 c;
    text->GetCharAt(offset, &c);

    if (c >= 0xD800/*'\uD800'*/ && c <= 0xDBFF/*'\uDBFF'*/) {
        Char32 c1;
        text->GetCharAt(offset + 1, &c1);

        if (c1 >= 0xDC00/*'\uDC00'*/ && c1 <= 0xDFFF/*'\uDFFF'*/) {
            offset += 2;
        }
        else {
            offset += 1;
        }
    }
    else {
        offset += 1;
    }

    if (ISpanned::Probe(text) != NULL) {
        ArrayOf<IInterface*>* spans;
        ISpanned::Probe(text)->GetSpans(offset, offset,
                EIID_IReplacementSpan, &spans);

        for (Int32 i = 0; i < spans->GetLength(); i++) {
            Int32 start, end;
            ISpanned::Probe(text)->GetSpanStart((*spans)[i], &start);
            ISpanned::Probe(text)->GetSpanEnd((*spans)[i], &end);

            if (start < offset && end > offset) {
                offset = end;
            }

            (*spans)[i]->Release();
        }
        ArrayOf<IInterface*>::Free(spans);
    }

    return offset;
}

void TextUtils::CopySpansFrom(
    /* [in] */ ISpanned* source,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end,
    /* [in] */ InterfaceID kind,
    /* [in] */ ISpannable* dest,
    /* [in] */ Int32 destoff)
{
//    if (kind == null) {
//        kind = Object.class;
//    }

    ArrayOf<IInterface*>* spans;
    source->GetSpans(start, end, kind, &spans);

    for (Int32 i = 0; i < spans->GetLength(); i++) {
        Int32 st, en, fl;
        source->GetSpanStart((*spans)[i], &st);
        source->GetSpanEnd((*spans)[i], &en);
        source->GetSpanFlags((*spans)[i], &fl);

        if (st < start) {
            st = start;
        }
        if (en > end) {
            en = end;
        }

        dest->SetSpan((*spans)[i], st - start + destoff,
                en - start + destoff, fl);

        (*spans)[i]->Release();
    }
    ArrayOf<IInterface*>::Free(spans);
}

AutoPtr<ICharSequence> TextUtils::Ellipsize(
    /* [in] */ ICharSequence* text,
    /* [in] */ ITextPaint* p,
    /* [in] */ Float avail,
    /* [in] */ TextUtilsTruncateAt where)
{
    return NULL;
}

AutoPtr<ICharSequence> TextUtils::Ellipsize(
    /* [in] */ ICharSequence* text,
    /* [in] */ ITextPaint* p,
    /* [in] */ Float avail,
    /* [in] */ TextUtilsTruncateAt where,
    /* [in] */ Boolean preserveLength,
    /* [in] */ IEllipsizeCallback* callback)
{
    return NULL;
}

/* package */
void TextUtils::Obtain(
    /* [in] */ Int32 len,
    /* [out] */ ArrayOf<Char8>** buf)
{
    assert(buf != NULL);

    {
        Mutex::Autolock lock(sLock);
        *buf = sTemp;
        sTemp = NULL;
    }

    if (*buf == NULL || (*buf)->GetLength() < len) {
        *buf = ArrayOf<Char8>::Alloc(ArrayUtils::IdealChar8ArraySize(len));
    }
}

/* package */
void TextUtils::Recycle(
    /* [in] */ ArrayOf<Char8>** temp)
{
    if (*temp == NULL) return;

    if ((*temp)->GetLength() > 1000) {
        ArrayOf<Char8>::Free(*temp);
        *temp = NULL;
        return;
    }

    Mutex::Autolock lock(sLock);
    if (sTemp != NULL) ArrayOf<Char8>::Free(sTemp);
    sTemp = *temp;
    *temp = NULL;
}
