
#include "CMatcher.h"
#include "CPattern.h"
#include "MatchResultImpl.h"
#include "cmdef.h"
#include "StringBuffer.h"
#include "Elastos.Core.h"
#include <stdio.h>

/**
* Checks if an error has occurred, throwing a suitable exception if so.
* @param env JNI environment
* @param errorCode code to determine if it is an error
* @return 0 if errorCode is not an error, 1 if errorCode is an error, but the
*         creation of the exception to be thrown fails
 * @exception thrown if errorCode represents an error
*/
ECode icu4_error(UErrorCode errorCode)
{
//    const char* message = u_errorName(errorCode);
    if (errorCode <= U_ZERO_ERROR || errorCode >= U_ERROR_LIMIT) {
        return NOERROR;
    }

    switch (errorCode) {
    case U_ILLEGAL_ARGUMENT_ERROR:
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    case U_INDEX_OUTOFBOUNDS_ERROR:
    case U_BUFFER_OVERFLOW_ERROR:
        return E_ARRAY_INDEX_OUT_OF_BOUNDS_EXCEPTION;
    case U_UNSUPPORTED_ERROR:
        return E_UNSUPPORTED_OPERATION_EXCEPTION;
    default:
        return E_RUNTIME_EXCEPTION;
    }
}


/**
 * We use ICU4C's RegexMatcher class, but our input is on the Java heap and potentially moving
 * around between calls. This wrapper class ensures that our RegexMatcher is always pointing at
 * the current location of the char[]. Earlier versions of Android simply copied the data to the
 * native heap, but that's wasteful and hides allocations from the garbage collector.
 */
class MatcherAccessor
{
public:
    MatcherAccessor(
        /* [in] */ RegexMatcher* matcher,
        /* [in] */ const String& input,
        /* [in] */ Boolean reset)
    {
        Init(matcher);

        mInput = input;
        if (mInput.IsNull()) {
            return;
        }

        //mUText = utext_openUTF8(NULL, mInput.string(), mInput.GetLength(), &mStatus);
        //UChar pchar[256] = {0};
        //StringBuf put((char *)(const char *)mInput, mInput.GetLength());
        //char *p = "abcd";
        //StringBuf x(p, 4);
        char *p = NULL;
        UnicodeString in = UNICODE_STRING((const char *)input, input.GetLength());
        
        Int64 len = in.length();
        mUText = utext_openUChars(NULL, in.getBuffer(), len, &mStatus);
        
        if(U_FAILURE(mStatus))
 	{
        } else {
        }


        if (mUText == NULL) {
            return;
        }

        if (reset) {
            mMatcher->reset(mUText);
        }
        else {
            mMatcher->refreshInputText(mUText, mStatus);
        }
    }

    MatcherAccessor(RegexMatcher* matcher)
    {
        Init(matcher);
    }

    ~MatcherAccessor()
    {
        utext_close(mUText);
        icu4_error(mStatus);
    }

    RegexMatcher* operator->()
    {
        return mMatcher;
    }

    UErrorCode& Status()
    {
        return mStatus;
    }

    void UpdateOffsets(ArrayOf<Int32>* offsets)
    {
        if (offsets == NULL) {
            return;
        }

        for (size_t i = 0, groupCount = mMatcher->groupCount(); i <= groupCount; ++i) {
            (*offsets)[2 * i + 0] = mMatcher->start(i, mStatus);
            (*offsets)[2 * i + 1] = mMatcher->end(i, mStatus);
        }
    }

private:
    void Init(RegexMatcher* matcher)
    {
        mMatcher = matcher;
        mStatus = U_ZERO_ERROR;
        mUText = NULL;
    }

    String mInput;
    RegexMatcher* mMatcher;
    UErrorCode mStatus;
    UText* mUText;

    // Disallow copy and assignment.
    MatcherAccessor(const MatcherAccessor&);
    void operator=(const MatcherAccessor&);
};

CMatcher::CMatcher()
    : mRegionStart(0)
    , mRegionEnd(0)
    , mFindPos(0)
    , mAppendPos(0)
    , mMatchFound(FALSE)
    , mAnchoringBounds(TRUE)
    , mTransparentBounds(FALSE)
{
}

ECode CMatcher::AppendReplacement(
    /* [in] */ const String& replacement,
    /* [out] */ String* result)
{
    VALIDATE_NOT_NULL(result);

    Int32 index;
    Start(&index);
    *result += (mInput.Substring(mAppendPos, index - mAppendPos));
    AppendEvaluated(replacement, result);
    End(&mAppendPos);

    return NOERROR;
}

ECode CMatcher::AppendEvaluated(
    /* [in] */ const String& s,
    /* [out] */ String* result)
{
    VALIDATE_NOT_NULL(result);

    Boolean escape = FALSE;
    Boolean dollar = FALSE;

    for (UInt32 i = 0; i < s.GetLength(); i++) {
        Char8 c = s[i];
        if (c == '\\' && !escape) {
            escape = TRUE;
        }
        else if (c == '$' && !escape) {
            dollar = TRUE;
        }
        else if (c >= '0' && c <= '9' && dollar) {
            String text;
            GroupEx((c - '0'), &text);

            *result += text;
            dollar = FALSE;
        }
        else {
            *result += &c;
            dollar = FALSE;
            escape = FALSE;
        }
    }

    // This seemingly stupid piece of code reproduces a JDK bug.
    if (escape) {
        return E_ARRAY_INDEX_OUT_OF_BOUNDS_EXCEPTION;
//        throw new ArrayIndexOutOfBoundsException(s.length());
    }

    return NOERROR;
}

ECode CMatcher::Reset()
{
    AutoPtr<ICharSequence> charSeq;
    CStringWrapper::New(mInput, (ICharSequence**)&charSeq);

    return ResetEx2(charSeq, 0, mInput.GetLength());
}

ECode CMatcher::ResetEx(
    /* [in] */ ICharSequence* input)
{
    Int32 length;
    input->GetLength(&length);

    return ResetEx2(input, 0, length);
}

ECode CMatcher::ResetEx2(
    /* [in] */ ICharSequence* input,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end)
{
    VALIDATE_NOT_NULL(input);

    Int32 length;
    input->GetLength(&length);
    if (start < 0 || end < 0 || start > length || end > length || start > end) {
        return E_ARRAY_INDEX_OUT_OF_BOUNDS_EXCEPTION;
//        throw new IndexOutOfBoundsException();
    }

    input->ToString(&mInput);
    mRegionStart = start;
    mRegionEnd = end;
    ResetForInput();

    mMatchFound = FALSE;
    mFindPos = mRegionStart;
    mAppendPos = 0;

    return NOERROR;
}

ECode CMatcher::UsePattern(
    /* [in] */ IPattern* pattern)
{
    VALIDATE_NOT_NULL(pattern);

    mPattern = pattern;

    if (mNativeMatcher != NULL) {
        CloseImpl(mNativeMatcher);
        mNativeMatcher = NULL;
    }
    mNativeMatcher = OpenImpl(((CPattern*)pattern)->mNativeMatcher);
    if (!mInput.IsNull()) {
        ResetForInput();
    }

    Int32 count;
    GroupCount(&count);
    mMatchOffsets = ArrayOf<Int32>::Alloc((count + 1) * 2);
    mMatchFound = FALSE;

    return NOERROR;
}

void CMatcher::ResetForInput()
{
    SetInputImpl(mNativeMatcher, mInput, mRegionStart, mRegionEnd);
    UseAnchoringBoundsImpl(mNativeMatcher, mAnchoringBounds);
    UseTransparentBoundsImpl(mNativeMatcher, mTransparentBounds);
}

ECode CMatcher::Region(
    /* [in] */ Int32 start,
    /* [in] */ Int32 end)
{
    AutoPtr<ICharSequence> charSeq;
    CStringWrapper::New(mInput, (ICharSequence**)&charSeq);

    return ResetEx2(charSeq, start, end);
}

ECode CMatcher::AppendTail(
    /* [out] */ String* result)
{
    VALIDATE_NOT_NULL(result);

    if (mAppendPos < mRegionEnd) {
        *result += mInput.Substring(mAppendPos, mRegionEnd);
    }

    return NOERROR;
}

ECode CMatcher::ReplaceFirst(
    /* [in] */ const String& replacement,
    /* [out] */ String* result)
{
    VALIDATE_NOT_NULL(result);

    Reset();

    Boolean found;
    Find(&found);
    if (found) {
        AppendReplacement(replacement, result);
    }
    return AppendTail(result);
}

ECode CMatcher::ReplaceAll(
    /* [in] */ const String& replacement,
    /* [out] */ String* result)
{
    VALIDATE_NOT_NULL(result);

    Reset();

    Boolean found;
    Find(&found);
    while (found) {
        AppendReplacement(replacement, result);
        Find(&found);
    }
    return AppendTail(result);
}

ECode CMatcher::Pattern(
    /* [out] */ IPattern** pattern)
{
    VALIDATE_NOT_NULL(pattern);
    *pattern = mPattern;

    return NOERROR;
}

ECode CMatcher::Group(
    /* [out] */ String* text)
{
    return GroupEx(0, text);
}

ECode CMatcher::GroupEx(
    /* [in] */ Int32 group,
    /* [out] */ String* text)
{
    VALIDATE_NOT_NULL(text);

    ECode ec = EnsureMatch();
    if (FAILED(ec)) {
        return ec;
    }

    Int32 from = (*mMatchOffsets)[group * 2];
    Int32 to = (*mMatchOffsets)[(group * 2) + 1];
    if (from == -1 || to == -1) {
        *text = String(NULL);
        return NOERROR;
    }
    else {
        *text = mInput.Substring(from, to);
    }

    return NOERROR;
}

ECode CMatcher::Find(
    /* [out] */ Boolean* found)
{
    VALIDATE_NOT_NULL(found);

    mMatchFound = FindNextImpl(mNativeMatcher, mInput, mMatchOffsets);
    if (mMatchFound) {
        mFindPos = (*mMatchOffsets)[1];
    }
    *found = mMatchFound;

    return NOERROR;
}

ECode CMatcher::FindEx(
    /* [in] */ Int32 start,
    /* [out] */ Boolean* found)
{
    VALIDATE_NOT_NULL(found);

    mFindPos = start;

    if (mFindPos < mRegionStart) {
        mFindPos = mRegionStart;
    }
    else if (mFindPos >=mRegionEnd) {
        mMatchFound = FALSE;
        *found = FALSE;
        return NOERROR;
    }

    mMatchFound = FindImpl(mNativeMatcher, mInput, mFindPos, mMatchOffsets);
    if (mMatchFound) {
        mFindPos = (*mMatchOffsets)[1];
    }
    *found = mMatchFound;

    return NOERROR;
}

ECode CMatcher::LookingAt(
    /* [out] */ Boolean* matched)
{
    VALIDATE_NOT_NULL(matched);

    mMatchFound = LookingAtImpl(mNativeMatcher, mInput, mMatchOffsets);
    if (mMatchFound) {
        mFindPos = (*mMatchOffsets)[1];
    }
    *matched = mMatchFound;

    return NOERROR;
}

ECode CMatcher::Matches(
    /* [out] */ Boolean* matched)
{
    VALIDATE_NOT_NULL(matched);

    mMatchFound = MatchesImpl(mNativeMatcher, mInput, mMatchOffsets);
    if (mMatchFound) {
        mFindPos = (*mMatchOffsets)[0];
    }
    *matched = mMatchFound;

    return NOERROR;
}

ECode CMatcher::StartEx(
    /* [in] */ Int32 group,
    /* [out] */ Int32* index)
{
    VALIDATE_NOT_NULL(index);

    ECode ec = EnsureMatch();
    if (FAILED(ec)) {
        return ec;
    }

    *index = (*mMatchOffsets)[group * 2];

    return NOERROR;
}

ECode CMatcher::EndEx(
    /* [in] */ Int32 group,
    /* [out] */ Int32* index)
{
    VALIDATE_NOT_NULL(index);

    ECode ec = EnsureMatch();
    if (FAILED(ec)) {
        return ec;
    }

    *index = (*mMatchOffsets)[(group * 2) + 1];

    return NOERROR;
}

String CMatcher::QuoteReplacement(
    /* [in] */ const String& s)
{
    StringBuffer result;
    for (UInt32 i = 0; i < s.GetLength(); i++) {
        Char8 c = s[i];
        if (c == '\\' || c == '$') {
            result += '\\';
        }
        result += c;
    }
    return (String)result;
}

ECode CMatcher::Start(
    /* [out] */ Int32* index)
{
    VALIDATE_NOT_NULL(index);

    return StartEx(0, index);
}

ECode CMatcher::GroupCount(
    /* [out] */ Int32* count)
{
    VALIDATE_NOT_NULL(count);

    *count = GroupCountImpl(mNativeMatcher);

    return NOERROR;
}

ECode CMatcher::End(
    /* [out] */ Int32* index)
{
    return EndEx(0, index);
}

ECode CMatcher::ToMatchResult(
    /* [out] */ IMatchResult** result)
{
    VALIDATE_NOT_NULL(result);

    ECode ec = EnsureMatch();
    if (FAILED(ec)) {
        return ec;
    }

    *result = (IMatchResult*)new MatchResultImpl(mInput, mMatchOffsets);

    return NOERROR;
}

ECode CMatcher::UseAnchoringBounds(
    /* [in] */ Boolean value)
{
    mAnchoringBounds = value;
    UseAnchoringBoundsImpl(mNativeMatcher, value);

    return NOERROR;
}

ECode CMatcher::HasAnchoringBounds(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);

    *result = mAnchoringBounds;

    return NOERROR;
}

ECode CMatcher::UseTransparentBounds(
    /* [in] */ Boolean value)
{
    mTransparentBounds = value;
    UseTransparentBoundsImpl(mNativeMatcher, value);

    return NOERROR;
}

ECode CMatcher::EnsureMatch()
{
    if (!mMatchFound) {
        return E_ILLEGAL_STATE_EXCEPTION;
//        throw new IllegalStateException("No successful match so far");
    }

    return NOERROR;
}

ECode CMatcher::HasTransparentBounds(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);

    *result = mTransparentBounds;

    return NOERROR;
}

ECode CMatcher::RegionStart(
    /* [out] */ Int32* start)
{
    VALIDATE_NOT_NULL(start);

    *start = mRegionStart;

    return NOERROR;
}

ECode CMatcher::RegionEnd(
    /* [out] */ Int32* end)
{
    VALIDATE_NOT_NULL(end);

    *end = mRegionEnd;

    return NOERROR;
}

ECode CMatcher::RequireEnd(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);

    RequireEndImpl(mNativeMatcher);

    return NOERROR;
}

ECode CMatcher::HitEnd(
    /* [out] */ Boolean* hit)
{
    VALIDATE_NOT_NULL(hit);

    HitEndImpl(mNativeMatcher);

    return NOERROR;
}

//void CMatcher::Finalize()
//{
//    try {
//    CloseImpl(mNativeMatcher);
//    } finally {
//        super.finalize();
//    }
//}

ECode CMatcher::constructor(
    /* [in] */ IPattern* pattern,
    /* [in] */ ICharSequence* input)
{
    UsePattern(pattern);
    ResetEx(input);

    return NOERROR;
}

void CMatcher::CloseImpl(
    /* [in] */ RegexMatcher* matcher)
{
    delete matcher;
}

Boolean CMatcher::FindImpl(
    /* [in] */ RegexMatcher* matcher,
    /* [in] */ String s,
    /* [in] */ Int32 startIndex,
    /* [in] */ ArrayOf<Int32>* offsets)
{
    MatcherAccessor matcherAccessor(matcher, s, FALSE);
    UBool result = matcherAccessor->find(startIndex, matcherAccessor.Status());
    if (result) {
        matcherAccessor.UpdateOffsets(offsets);
    }
    return result;
}

Boolean CMatcher::FindNextImpl(
    /* [in] */ RegexMatcher* matcher,
    /* [in] */ String s,
    /* [in] */ ArrayOf<Int32>* offsets)
{
    MatcherAccessor matcherAccessor(matcher, s, FALSE);
    if (matcherAccessor.Status() != U_ZERO_ERROR) {
        return -1;
    }
    UBool result = matcherAccessor->find();
    if (result) {
        matcherAccessor.UpdateOffsets(offsets);
    }
    return result;
}

Int32 CMatcher::GroupCountImpl(
    /* [in] */ RegexMatcher* matcher)
{
    MatcherAccessor matcherAccessor(matcher);
    return matcherAccessor->groupCount();
}

Boolean CMatcher::HitEndImpl(
    /* [in] */ RegexMatcher* matcher)
{
    MatcherAccessor matcherAccessor(matcher);
    return matcherAccessor->hitEnd();
}

Boolean CMatcher::LookingAtImpl(
    /* [in] */ RegexMatcher* matcher,
    /* [in] */ String s,
    /* [in] */ ArrayOf<Int32>* offsets)
{
    MatcherAccessor matcherAccessor(matcher, s, FALSE);
    UBool result = matcherAccessor->lookingAt(matcherAccessor.Status());
    if (result) {
        matcherAccessor.UpdateOffsets(offsets);
    }
    return result;
}

Boolean CMatcher::MatchesImpl(
    /* [in] */ RegexMatcher* matcher,
    /* [in] */ String s,
    /* [in] */ ArrayOf<Int32>* offsets)
{
    MatcherAccessor matcherAccessor(matcher, s, FALSE);
    UBool result = matcherAccessor->matches(matcherAccessor.Status());
    /*{
        UParseError err;
        UErrorCode errcode;
        RegexPattern *pat = RegexPattern::compile(UNICODE_STRING("a*b", 3), 0, err, errcode);
        RegexMatcher *ma = pat->matcher(errcode);
        UChar a[2] = {'a', 'b'}; 
        UText *pt = utext_openUChars(NULL, a, 2, &errcode);
      
        
        ma->reset(pt);
        if (ma->matches(errcode)) {
            printf("Greate Flag is Success!\n");
        } else {
            printf("Greate Flag is FAILED!\n");
        } 
    }
    UParseError err;
    UErrorCode errCode;
    UBool result = matcherAccessor->matches(xUNICODE_STRING("3", 1), UNICODE_STRING("a3b", 3), err, errCode);
    
    UErrorCode errCode;
    RegexMatcher *tst =new  RegexMatcher(UNICODE_STRING("a*b", 3), UNICODE_STRING("ab", 2), 0, errCode);
    UBool result = tst->matches(errCode);
    printf("the UBool is %d\n", result);
    */
    if (result) {
        matcherAccessor.UpdateOffsets(offsets);
    }
    return result;
}

RegexMatcher* CMatcher::OpenImpl(
    /* [in] */ RegexPattern* pattern)
{
    UErrorCode status = U_ZERO_ERROR;
    RegexMatcher* result = pattern->matcher(status);
    icu4_error(status);
    return result;
}

Boolean CMatcher::RequireEndImpl(
    /* [in] */ RegexMatcher* matcher)
{
    MatcherAccessor matcherAccessor(matcher);
    return matcherAccessor->requireEnd();
}

void CMatcher::SetInputImpl(
    /* [in] */ RegexMatcher* matcher,
    /* [in] */ String s,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end)
{
    MatcherAccessor matcherAccessor(matcher, s, TRUE);
    matcherAccessor->region(start, end, matcherAccessor.Status());
}

void CMatcher::UseAnchoringBoundsImpl(
    /* [in] */ RegexMatcher* matcher,
    /* [in] */ Boolean value)
{
    MatcherAccessor matcherAccessor(matcher);
    matcherAccessor->useAnchoringBounds(value);
}

void CMatcher::UseTransparentBoundsImpl(
    /* [in] */ RegexMatcher* matcher,
    /* [in] */ Boolean value)
{
    MatcherAccessor matcherAccessor(matcher);
    matcherAccessor->useTransparentBounds(value);
}
