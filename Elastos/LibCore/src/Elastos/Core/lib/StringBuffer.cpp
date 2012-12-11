
#include "StringBuffer.h"
#include <elastos/Character.h>
#include <stdio.h>

#define DEFAULT_STEP 16

namespace Elastos {
namespace Core {

void StringBuffer::Init(const char *s, Int32 step)
{
    mStep = step;
    mCapacity = mLength + mStep + 1;
    mString = new char[mCapacity];
    strcpy(mString, s ? s : "\0");
}

StringBuffer::StringBuffer(const char* s)
    : mLength(s ? strlen(s) : 0)
{
    Init(s, DEFAULT_STEP);
}

StringBuffer::StringBuffer(const String& s)
    : mLength(s.IsNull() ? 0 : s.GetLength())
{
    Init((const char *)s, DEFAULT_STEP);
}

StringBuffer::StringBuffer(const StringBuffer& s)
    : mLength(s.mLength)
{
    Init(s.mString, DEFAULT_STEP);
}

StringBuffer::StringBuffer(Int32 capacity)
    : mLength(0)
{
    Init(NULL, capacity);
}

StringBuffer::~StringBuffer()
{
    delete [] mString;
}

void StringBuffer::Break(Int32 size)
{
    assert(size >= 0);

    if (mCapacity >= mLength + size + 1) return;

    while (mCapacity + mStep < mLength + size + 1) {
        mStep *= 2;
    }

    mCapacity += mStep;
    char * tmpS = new char[mCapacity];
    strcpy(tmpS, mString);
    delete [] mString;
    mString = tmpS;
}

String StringBuffer::Substring(Int32 start, Int32 end)
{
    if (0 <= start && start <= end && end <= mLength) {
        if (start == end) {
            return String(NULL);
        }

        Int32 count = 0;
        char *p = mString;
        char *pEnd = p +  mLength + 1;
        char *p1 = p, *p2 = pEnd;
        char *p3 = p;

        if (start == mLength) {
            p1 += mLength;
        } else {
            while (p && *p && p < pEnd) {
                Int32 charlen = 0;

                if ((*p & 0x80) == 0) charlen = 1;
                else if ((*p & 0xE0) == 0xC0) charlen = 2;
                else if ((*p & 0xF0) == 0xE0) charlen = 3;
                else if ((*p & 0xF8) == 0xF0) charlen = 4;

                if (!charlen || p + charlen >= pEnd) break;
                else p += charlen;

                if (count == start) {
                    p1 = p3;
                }
                if (count == end) {
                    p2 = p3;
                    break;
                }

                count++;
                p3 = p;
            }
        }

        char *substr = (char *)malloc(p2 - p1 + 1);
        *(substr + (p2 - p1)) = '\0';
        strncpy(substr, p1, p2 - p1);
        return String(substr);
    }

    return String(NULL);
}

StringBuffer& StringBuffer::operator=(const StringBuffer& right)
{
    if ( &right != this ) {
        delete [] mString;
        mLength = right.mLength;
        Init(right.mString, right.mStep);
    }

    return *this;
}

StringBuffer& StringBuffer::operator+=(const StringBuffer& right)
{
    if (right.mLength == 0 || !right.mString) return *this;

    Int32 appendLen = right.mLength;
    if (this->mString == right.mString) {
        char* tmp = new char[appendLen + 1];
        strcpy(tmp, right.mString);
        Break(appendLen);
        strcat(mString, tmp);
        delete tmp;
    }
    else {
        Break(appendLen);
        strcat(mString, right.mString);
    }
    mLength += appendLen;
    return *this;
}

StringBuffer& StringBuffer::operator+=(const String& right)
{
    StringBuffer tmp(right);
    return *this += tmp;
}

StringBuffer& StringBuffer::operator+=(const Char32 right)
{
    ArrayOf_<Char8, 5> buf;
    Int32 len;
    Character::ToChars(right, buf, 0, &len);
    buf[len] = '\0';
    return *this += (const char*)buf.GetPayload();
}

StringBuffer& StringBuffer::operator+=(const Int32 right)
{
    char buf[12];
    sprintf(buf, "%d", right);
    return *this += buf;
}

StringBuffer& StringBuffer::operator+=(const Int64 right)
{
    char buf[32];
    sprintf(buf, "%d", right);
    return *this += buf;
}

StringBuffer& StringBuffer::operator+=(const Boolean right)
{
    return *this += right ? "TRUE" : "FALSE";
}

StringBuffer& StringBuffer::operator+=(const char* right)
{
    StringBuffer tmp(right);
    return *this += tmp;
}

StringBuffer& StringBuffer::operator+=(const void* right)
{
    char buf[12];
    sprintf(buf, "%p", right);
    StringBuffer tmp(buf);
    return *this += tmp;
}

StringBuffer::operator CString() const
{   // for 3rd party API such as foo(char* pch);
    return (const char*)mString;
}

StringBuffer::operator const char*() const
{
    return (const char*)mString;
}

Boolean StringBuffer::IsNull() const
{
    return mLength == 0;
}

Int32 StringBuffer::GetLength()
{
    return mLength;
}

} // Utility
} // Elastos

StringBuffer operator+(const StringBuffer& left, const StringBuffer& right)
{
    StringBuffer tmp(left);
    tmp += right;
    return tmp;
}

StringBuffer operator+(const StringBuffer& left, const String& right)
{
    StringBuffer tmp(left);
    tmp += right;
    return tmp;
}

StringBuffer operator+(const StringBuffer& left, const Char32 right)
{
    StringBuffer tmp(left);
    tmp += right;
    return tmp;
}

StringBuffer operator+(const StringBuffer& left, const Int32 right)
{
    StringBuffer tmp(left);
    tmp += right;
    return tmp;
}

StringBuffer operator+(const StringBuffer& left, const Int64 right)
{
    StringBuffer tmp(left);
    tmp += right;
    return tmp;
}

StringBuffer operator+(const StringBuffer& left, const Boolean right)
{
    StringBuffer tmp(left);
    tmp += right;
    return tmp;
}

StringBuffer operator+(const StringBuffer& left, const char* right)
{
    StringBuffer tmp(left);
    tmp += right;
    return tmp;
}

StringBuffer operator+(const StringBuffer& left, const void* right)
{
    StringBuffer tmp(left);
    tmp += right;
    return tmp;
}

