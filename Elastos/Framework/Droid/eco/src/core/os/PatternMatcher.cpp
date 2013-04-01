
#include "ext/frameworkdef.h"
#include "os/PatternMatcher.h"


PatternMatcher::PatternMatcher()
{}

PatternMatcher::PatternMatcher(
    /* [in] */ const String& pattern,
    /* [in] */ Int32 type)
{
    constructor(pattern, type);
}

PatternMatcher::PatternMatcher(
    /* [in] */ IParcel* src)
{
    constructor(src);
}

ECode PatternMatcher::constructor(
    /* [in] */ const String& pattern,
    /* [in] */ Int32 type)
{
    mPattern = pattern;
    mType = type;
    return NOERROR;
}

ECode PatternMatcher::constructor(
    /* [in] */ IParcel* src)
{
    src->ReadString(&mPattern);
    src->ReadInt32(&mType);
    return NOERROR;
}

ECode PatternMatcher::GetPath(
    /* [in] */ String* path)
{
    *path = mPattern;
    return NOERROR;
}

ECode PatternMatcher::GetType(
    /* [out] */ Int32* type)
{
    VALIDATE_NOT_NULL(type);

    *type = mType;
    return NOERROR;
}

ECode PatternMatcher::Match(
    /* [in] */ const String& str,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);

    *result = MatchPattern(mPattern, str, mType);
    return NOERROR;
}

ECode PatternMatcher::DescribeContents(
    /* [out] */ Int32* des)
{
    VALIDATE_NOT_NULL(des);

    *des = NULL;
    return NOERROR;
}

ECode PatternMatcher::WriteToParcel(
    /* [in] */ IParcel* dest)
{
    dest->WriteString(mPattern);
    dest->WriteInt32(mType);
    return NOERROR;
}


Boolean PatternMatcher::MatchPattern(
    /* [in] */ const String& pattern,
    /* [in] */ const String& match,
    /* [in] */ Int32 type)
{
    if (match == NULL) {
        return FALSE;
    }

    if (type == PATTERN_LITERAL) {
        return pattern.Equals(match);
    } 

    if (type == PATTERN_PREFIX) {
        return match.StartWith(pattern);
    } 
    else if (type != PATTERN_SIMPLE_GLOB) {
        return FALSE;
    }
    
    const Int32 NP = pattern.GetLength();
    if (NP <= 0) {
        return match.GetLength() <= 0;
    }
    const Int32 NM = match.GetLength();
    Int32 ip = 0, im = 0;
    char nextChar = pattern.GetChar(0);
    while ((ip<NP) && (im<NM)) {
        char c = nextChar;
        ip++;
        nextChar = ip < NP ? pattern.GetChar(ip) : 0;
        const Boolean escaped = (c == '\\');
        if (escaped) {
            c = nextChar;
            ip++;
            nextChar = ip < NP ? pattern.GetChar(ip) : 0;
        }
        if (nextChar == '*') {
            if (!escaped && c == '.') {
                if (ip >= (NP-1)) {
                    // at the end with a pattern match, so
                    // all is good without checking!
                    return TRUE;
                }
                ip++;
                nextChar = pattern.GetChar(ip);
                // Consume everything until the next character in the
                // pattern is found.
                if (nextChar == '\\') {
                    ip++;
                    nextChar = ip < NP ? pattern.GetChar(ip) : 0;
                }

                do {
                    if (match.GetChar(im) == nextChar) {
                        break;
                    }
                    im++;
                } while (im < NM);

                if (im == NM) {
                    // Whoops, the next character in the pattern didn't
                    // exist in the match.
                    return FALSE;
                }
                ip++;
                nextChar = ip < NP ? pattern.GetChar(ip) : 0;
                im++;
            } 
            else {
                // Consume only characters matching the one before '*'.
                do {
                    if (match.GetChar(im) != c) {
                        break;
                    }
                    im++;
                } while (im < NM);
                ip++;
                nextChar = ip < NP ? pattern.GetChar(ip) : 0;
            }
        } 
        else {
            if (c != '.' && match.GetChar(im) != c) {
                return FALSE;
            }
            im++;
        }
    }
    
    if (ip >= NP && im >= NM) {
        // Reached the end of both strings, all is good!
        return TRUE;
    }
    
    // One last check: we may have finished the match string, but still
    // have a '.*' at the end of the pattern, which should still count
    // as a match.
    if (ip == NP-2 && pattern.GetChar(ip) == '.'
        && pattern.GetChar(ip+1) == '*') {
        return TRUE;
    }
    
    return FALSE;
}