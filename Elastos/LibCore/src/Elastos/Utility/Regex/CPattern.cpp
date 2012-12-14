
#include "CPattern.h"
#include "Splitter.h"
#include "cmdef.h"
#include "StringBuffer.h"
#include <stdio.h>

CPattern::CPattern()
    : mFlags(0)
{
}

ECode CPattern::Matcher(
    /* [in] */ ICharSequence* input,
    /* [out] */ IMatcher** matcher)
{
    VALIDATE_NOT_NULL(matcher);
    return CMatcher::New((IPattern*)this, input, matcher);
}

ECode CPattern::Split(
    /* [in] */ ICharSequence* input,
    /* [out, callee]*/ ArrayOf<String>** result)
{
    return SplitEx(input, 0, result);
}

ECode CPattern::SplitEx(
    /* [in] */ ICharSequence* input,
    /* [in] */ Int32 limit,
    /* [out, callee]*/ ArrayOf<String>** result)
{
    VALIDATE_NOT_NULL(result);

    String s;
    input->ToString(&s);
    *result = Splitter::Split((IPattern*)this, mPattern, s, limit);

    return NOERROR;
}

ECode CPattern::Pattern(
    /* [out] */ String* pattern)
{
    VALIDATE_NOT_NULL(pattern);
    *pattern = mPattern;
    return NOERROR;
}

ECode CPattern::Flags(
    /* [out] */ Int32* flags)
{
    VALIDATE_NOT_NULL(flags);
    *flags = mFlags;

    return NOERROR;
}

ECode CPattern::constructor(
    /* [in] */ const String& regex,
    /* [in] */ Int32 flags)
{
    mPattern = regex;
    mFlags = flags;
    CompileEx2();
    return NOERROR;
}

ECode CPattern::Compile(
    /* [in] */ const String& regularExpression,
    /* [in] */ Int32 flags,
    /* [out] */ CPattern** obj)
{
    VALIDATE_NOT_NULL(obj);

    if ((flags & IPattern_CANON_EQ) != 0) {
//        return E_UNSUPPORTED_OPERATION_EXCEPTION;
//        throw new UnsupportedOperationException("CANON_EQ flag not supported");
    }

    *obj = new CPattern(regularExpression, flags);

    return (*obj)->CompileEx2();
    //return NOERROR;
}

ECode CPattern::CompileEx(
    /* [in] */ const String& pattern,
    /* [in] */ CPattern** obj)
{
    return Compile(pattern, 0, obj);
}

CPattern::CPattern(
    /* [in] */ const String& pattern,
    /* [in] */ Int32 flags)
{
    mPattern = pattern;
    mFlags = flags;
}

ECode CPattern::CompileEx2()
{
    if (mPattern.IsNull()) {
        return E_NULL_POINTER_EXCEPTION;
//        throw new NullPointerException("pattern == null");
    }

    String icuPattern = mPattern;
    if ((mFlags & IPattern_LITERAL) != 0) {
        icuPattern = Quote(mPattern);
    }

    // These are the flags natively supported by ICU.
    // They even have the same value in native code.
    Int32 icuFlags = mFlags & (IPattern_CASE_INSENSITIVE | IPattern_COMMENTS
            | IPattern_MULTILINE | IPattern_DOTALL | IPattern_UNIX_LINES);

    mNativeMatcher = CompileImpl(icuPattern, icuFlags);
    //mNativeMatcher = CompileImpl(icuPattern, 0);

    return NOERROR;
}

Boolean CPattern::Matches(
    /* [in] */ const String& regularExpression,
    /* [in] */ ICharSequence* input)
{
    CPattern* pattern = new CPattern(regularExpression, 0);
    pattern->CompileEx2();

    AutoPtr<CMatcher> matcher;
    ASSERT_SUCCEEDED(CMatcher::NewByFriend((IPattern*)pattern, input, (CMatcher**)&matcher));
    Boolean result;
    matcher->Matches(&result);

    return result;
}

String CPattern::Quote(
    /* [in] */ const String& string)
{
    StringBuffer sb;
    sb += "\\Q";
    Int32 apos = 0;
    Int32 k;
    while ((k = string.IndexOf("\\E", apos)) >= 0) {
        sb += (string.Substring(apos, k + 2)) += "\\\\E\\Q";
        apos = k + 2;
    }
    return (String)(sb += (string.Substring(apos)) += "\\E");
}

void CPattern::CloseImpl(
    /* [in] */ RegexPattern* pattern)
{
    delete pattern;
}

RegexPattern* CPattern::CompileImpl(
    /* [in] */ const String& regex,
    /* [in] */ Int32 flags)
{
    flags |= UREGEX_ERROR_ON_UNKNOWN_ESCAPES;

    UErrorCode status = U_ZERO_ERROR;
    UParseError error;
    error.offset = -1;

    //UnicodeString regexString(temp.unicodeString());
    //UText *pUText = utext_openUTF8(NULL, regex.string(), regex.GetLength(), &status);
    //UnicodeString regexString("ad", 2, 0);
    UnicodeString regexString = UNICODE_STRING((const char*)regex, regex.GetLength());
    RegexPattern* result = RegexPattern::compile(regexString, flags, error, status);
    if (!U_SUCCESS(status)) {
          return NULL;
//        return E_PATTERN_SYNTAX_EXCEPTION;
//        throwPatternSyntaxException(env, status, regex, error);
    }
    return result;
}
