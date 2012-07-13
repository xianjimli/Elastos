
#include <eltypes.h>
#include <elstring.h>
#include <elaerror.h>
#include <stdio.h>
#include <ctype.h>
#include <stdarg.h>

/*
 * The default path separator for the platform
 */
#define OS_PATH_SEPARATOR '/'

#define RES_PATH_SEPARATOR '/'

_ELASTOS_NAMESPACE_BEGIN

#ifdef _UNDEFDLLEXP
#define DLLEXP
#else
#define DLLEXP __declspec(dllimport)
#endif

EXTERN_C DLLEXP SharedBuffer* gElEmptyStringBuf;
EXTERN_C DLLEXP char* gElEmptyString;

static inline char* _getEmptyString()
{
    gElEmptyStringBuf->Acquire();
    return gElEmptyString;
}

static char* _allocFromUTF8(const char* in, UInt32 len)
{
    if (len > 0) {
        SharedBuffer* buf = SharedBuffer::Alloc(len + 1);
        if (buf) {
            char* str = (char*)buf->GetData();
            memcpy(str, in, len);
            str[len] = 0;
            return str;
        }
        return NULL;
    }

    return _getEmptyString();
}

String::String()
    : mString(NULL)
{
}

String::String(const String& o)
    : mString(o.mString)
{
    if (mString != NULL) {
        SharedBuffer::GetBufferFromData(mString)->Acquire();
    }
}

String::String(const char* o)
{
    if (o == NULL) {
        mString = NULL;
    }
    else {
        mString = _allocFromUTF8(o, strlen(o));
        if (mString == NULL) {
            mString = _getEmptyString();
        }
    }
}

String::String(const char* o, UInt32 len)
    : mString(_allocFromUTF8(o, len))
{
    if (mString == NULL) {
        mString = _getEmptyString();
    }
}

String::~String()
{
    if (mString != NULL) {
        SharedBuffer::GetBufferFromData(mString)->Release();
    }
}

void String::SetTo(const String& other)
{
    if (other.mString != NULL) {
        SharedBuffer::GetBufferFromData(other.mString)->Acquire();
    }
    if (mString != NULL) {
        SharedBuffer::GetBufferFromData(mString)->Release();
    }
    mString = other.mString;
}

ECode String::SetTo(const char* other)
{
    if (other == NULL) {
        mString = NULL;
        return NOERROR;
    }

    const char *newString = _allocFromUTF8(other, strlen(other));
    if (mString != NULL) {
        SharedBuffer::GetBufferFromData(mString)->Release();
    }
    mString = newString;
    if (mString) return NOERROR;

    mString = _getEmptyString();
    return E_OUT_OF_MEMORY;
}

ECode String::SetTo(const char* other, UInt32 len)
{
    const char *newString = _allocFromUTF8(other, len);
    if (mString != NULL) {
        SharedBuffer::GetBufferFromData(mString)->Release();
    }
    mString = newString;
    if (mString) return NOERROR;

    mString = _getEmptyString();
    return E_OUT_OF_MEMORY;
}

ECode String::Append(const String& other)
{
    const UInt32 otherLen = other.GetBytes();
    if (GetBytes() == 0) {
        SetTo(other);
        return NOERROR;
    }
    else if (otherLen == 0) {
        return NOERROR;
    }

    return RealAppend(other.string(), otherLen);
}

ECode String::Append(const char* other)
{
    return Append(other, strlen(other));
}

ECode String::Append(const char* other, UInt32 otherLen)
{
    if (GetBytes() == 0) {
        return SetTo(other, otherLen);
    }
    else if (otherLen == 0) {
        return NOERROR;
    }

    return RealAppend(other, otherLen);
}

String String::Replace(const char* oldStr, const char* newStr) const
{
    if (oldStr == NULL || newStr == NULL) {
        return String(*this);
    }

    Int32 matchStart = IndexOf(oldStr);
    if (matchStart == -1) {
        // If there's nothing to replace, return the original string untouched.
        return String(*this);
    }

    Int32 count = GetCharCount();
    Int32 newStrLength = strlen(newStr);
    Int32 resultLength = (count + 1) * newStrLength + GetLength() + 1;
    char* buf = (char*)malloc(resultLength);
    if (buf == NULL) {
        return String(NULL);
    }

    // The empty target matches at the start and end and between each character.
    if (oldStr[0] == '\0') {
        memcpy(buf, newStr, newStrLength);
        UInt32 myOff = 0, myNextOff = 0, bufOff = newStrLength;
        while (myOff < GetLength()) {
            GetCharByOffset(myOff, &myNextOff);
            memcpy(buf + bufOff, mString + myOff, myNextOff - myOff);
            bufOff += myNextOff - myOff;
            memcpy(buf + bufOff, newStr, newStrLength);
            bufOff += newStrLength;
            myOff = myNextOff;
        }
        return String(buf, resultLength);
    }

    Int32 oldStrLength = strlen(oldStr);
    UInt32 searchStart = 0;
    char* bufPtr = NULL;
    char* p = NULL;
    while ((p = strstr(mString + searchStart, oldStr)) != NULL) {
        matchStart = p - mString;
        // Copy characters before the match...
        memcpy(bufPtr, mString + searchStart, matchStart - searchStart);
        bufPtr += matchStart - searchStart;
        // Insert the replacement...
        memcpy(bufPtr, newStr, newStrLength);
        bufPtr += newStrLength;
        searchStart = matchStart + oldStrLength;
    }
    if (searchStart < GetLength()) {
        // Copy any trailing chars...
        memcpy(buf, mString + searchStart, GetLength() - searchStart);
    }
    return String(buf, bufPtr - buf);
}

ECode String::AppendFormat(const char* fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);

    Int32 result = NOERROR;
    Int32 n = vsnprintf(NULL, 0, fmt, ap);
    if (n != 0) {
        UInt32 oldLength = GetLength();
        char* buf = LockBuffer(oldLength + n);
        if (buf) {
            vsnprintf(buf + oldLength, n + 1, fmt, ap);
        }
        else {
            result = E_OUT_OF_MEMORY;
        }
    }

    va_end(ap);
    return result;
}

ECode String::RealAppend(const char* other, UInt32 otherLen)
{
    UInt32 myLen;

    SharedBuffer* buf;
    if (mString != NULL) {
        myLen = GetBytes();
        buf = SharedBuffer::GetBufferFromData(mString)
                ->EditResize(myLen + otherLen + 1);
    }
    else {
        myLen = 0;
        buf = SharedBuffer::Alloc(otherLen + 1);
    }

    if (buf) {
        char* str = (char*)buf->GetData();
        mString = str;
        str += myLen;
        memcpy(str, other, otherLen);
        str[otherLen] = '\0';
        return NOERROR;
    }
    return E_OUT_OF_MEMORY;
}

char* String::LockBuffer(UInt32 size)
{
    SharedBuffer* buf;
    if (mString != NULL) {
        buf = SharedBuffer::GetBufferFromData(mString)
                ->EditResize(size + 1);
    }
    else {
        buf = SharedBuffer::Alloc(size + 1);
    }
    if (buf) {
        char* str = (char*)buf->GetData();
        mString = str;
        return str;
    }
    return NULL;
}

void String::UnlockBuffer()
{
    UnlockBuffer(strlen(mString));
}

ECode String::UnlockBuffer(UInt32 size)
{
    if (size != this->GetSize()) {
        assert(mString != NULL);

        SharedBuffer* buf = SharedBuffer::GetBufferFromData(mString)
                    ->EditResize(size + 1);
        if (!buf) {
            return E_OUT_OF_MEMORY;
        }

        char* str = (char*)buf->GetData();
        str[size] = 0;
        mString = str;
    }

    return NOERROR;
}

Int32 String::Find(const char* other, UInt32 start) const
{
    UInt32 len = GetSize();
    if (start >= len) {
        return -1;
    }
    const char* s = mString + start;
    const char* p = strstr(s, other);
    return p ? p - mString : -1;
}

void String::ToLowerCase()
{
    ToLowerCase(0, GetSize());
}

void String::ToLowerCase(UInt32 start, UInt32 length)
{
    const UInt32 len = GetSize();
    if (start >= len) {
        return;
    }
    if (start + length > len) {
        length = len - start;
    }
    char* buf = LockBuffer(len);
    buf += start;
    while (length > 0) {
        *buf = tolower(*buf);
        buf++;
        length--;
    }
    UnlockBuffer(len);
}

void String::ToUpperCase()
{
    ToUpperCase(0, GetSize());
}

void String::ToUpperCase(UInt32 start, UInt32 length)
{
    const UInt32 len = GetSize();
    if (start >= len) {
        return;
    }
    if (start + length > len) {
        length = len - start;
    }
    char* buf = LockBuffer(len);
    buf += start;
    while (length > 0) {
        *buf = toupper(*buf);
        buf++;
        length--;
    }
    UnlockBuffer(len);
}

UInt32 String::GetCharCount() const
{
    return _String_GetCharCount(mString, GetLength());
}

Int32 String::GetChar(UInt32 index) const
{
    return _String_GetChar(mString, GetLength(), index);
}

Int32 String::GetCharByOffset(UInt32 offset, UInt32* nextOffset) const
{
    return _String_GetCharByOffset(mString, GetLength(), offset, nextOffset);
}

//---- Substring ----
String String::Substring(Int32 start) const
{
    if (!mString || start < 0 || start > (Int32)GetLength()) return String(NULL);

    return String(mString + start, GetLength() - start);
}

String String::Substring(Int32 start, Int32 len) const
{
    if (!mString || start < 0 || start > (Int32)GetLength() || len < 0) return String(NULL);

    return String(mString + start, len);
}

//---- Trim ----
String String::TrimStart() const
{
    if (!mString) return String(NULL);

    const char* str = mString;
    while (isspace(*str) && *str) {
        str++;
    }

    return String(str, GetLength() - (str - mString));
}

String String::TrimEnd() const
{
    if (!mString) return String(NULL);

    const char* end = mString + GetLength() - 1;
    while (isspace(*end) && end >= mString) {
        end--;
    }

    return String(mString, end - mString + 1);
}

String String::Trim() const
{
    if (!mString) return String(NULL);

    const char* start = mString;
    while (isspace(*start) && *start) {
        start++;
    }

    const char* end = mString + GetLength() - 1;
    while (isspace(*end) && end >= mString) {
        end--;
    }

    return String(start, end - start + 1);
}

String String::FromInt32(Int32 value)
{
    char str[12];

    sprintf(str, "%d", value);
    return String(str, strlen(str));
}

String String::FromInt64(Int64 value)
{
    char str[24];

    sprintf(str, "%ld", value);
    return String(str, strlen(str));
}

String String::FromBoolean(Boolean value)
{
    return value ? String("TRUE", 4) : String("FALSE", 5);
}

String String::FromDouble(Double value)
{
    //todo: not implemented;
    return String(NULL);
}

_ELASTOS_NAMESPACE_END
