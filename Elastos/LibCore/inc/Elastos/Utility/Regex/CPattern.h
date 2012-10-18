
#ifndef __CPATTERN_H__
#define __CPATTERN_H__

#include "_CPattern.h"
#include "CMatcher.h"
#include "elastos/AutoPtr.h"

CarClass(CPattern)
{
public:
    CPattern();

    CARAPI Matcher(
        /* [in] */ ICharSequence* input,
        /* [out] */ IMatcher** matcher);

    CARAPI Split(
        /* [in] */ ICharSequence* input,
        /* [out, callee]*/ ArrayOf<String>** result);

    CARAPI SplitEx(
        /* [in] */ ICharSequence* input,
        /* [in] */ Int32 limit,
        /* [out, callee]*/ ArrayOf<String>** result);

    CARAPI Pattern(
        /* [out] */ String* pattern);

    CARAPI Flags(
        /* [out] */ Int32* flags);

    /**
     * Returns a compiled form of the given {@code regularExpression}, as modified by the
     * given {@code flags}. See the <a href="#flags">flags overview</a> for more on flags.
     *
     * @throws PatternSyntaxException if the regular expression is syntactically incorrect.
     *
     * @see #CANON_EQ
     * @see #CASE_INSENSITIVE
     * @see #COMMENTS
     * @see #DOTALL
     * @see #LITERAL
     * @see #MULTILINE
     * @see #UNICODE_CASE
     * @see #UNIX_LINES
     */
    static CARAPI Compile(
        /* [in] */ const String& regularExpression,
        /* [in] */ Int32 flags,
        /* [in] */ CPattern** obj);

    /**
     * Equivalent to {@code Pattern.compile(pattern, 0)}.
     */
    static CARAPI CompileEx(
        /* [in] */ const String& pattern,
        /* [in] */ CPattern** obj);

    /**
     * Tests whether the given {@code regularExpression} matches the given {@code input}.
     * Equivalent to {@code Pattern.compile(regularExpression).matcher(input).matches()}.
     * If the same regular expression is to be used for multiple operations, it may be more
     * efficient to reuse a compiled {@code Pattern}.
     *
     * @see Pattern#compile(java.lang.String, int)
     * @see Matcher#matches()
     */
    static CARAPI_(Boolean) Matches(
        /* [in] */ const String& regularExpression,
        /* [in] */ ICharSequence* input);

    /**
     * Quotes the given {@code string} using "\Q" and "\E", so that all
     * meta-characters lose their special meaning. This method correctly
     * escapes embedded instances of "\Q" or "\E". If the entire result
     * is to be passed verbatim to {@link #compile}, it's usually clearer
     * to use the {@link #LITERAL} flag instead.
     */
    static CARAPI_(String) Quote(
        /* [in] */ const String& string);

private:
    CPattern(
        /* [in] */ const String& pattern,
        /* [in] */ Int32 flags);

    CARAPI CompileEx2();

    static CARAPI_(void) CloseImpl(
        /* [in] */ RegexPattern* pattern);

    static CARAPI_(RegexPattern*) CompileImpl(
        /* [in] */ const String& regex,
        /* [in] */ Int32 flags);

public:
    RegexPattern* mNativeMatcher;

private:
    String mPattern;
    Int32 mFlags;
};

#endif //__CPATTERN_H__
