
//==========================================================================
// Copyright (c) 2000-2008,  Elastos, Inc.  All Rights Reserved.
//==========================================================================

#if _MSC_VER > 1000
#pragma once
#endif

#ifndef __ELSTRING_H__
#define __ELSTRING_H__

#include <elstringapi.h>
#include <elcstring.h>
#include <elsharedbuf.h>

#ifdef __cplusplus

_ELASTOS_NAMESPACE_BEGIN


//! This is a string holding UTF-8 characters. Does not allow the value more
// than 0x10FFFF, which is not valid unicode codepoint.
class String
{
public:
    String();
    String(const String& o);
    explicit String(const char* o);
    explicit String(const char* o, UInt32 numChars);

    ~String();

    inline const char* string() const;
    inline UInt32 GetSize() const;
    inline UInt32 GetLength() const;
    inline UInt32 GetBytes() const;
    inline Boolean IsNull() const;
    inline Boolean IsEmpty() const;
    inline Boolean IsNullOrEmpty() const;

    inline const SharedBuffer* GetSharedBuffer() const;

    void SetTo(const String& other);
    ECode SetTo(const char* other);
    ECode SetTo(const char* other, UInt32 numChars);

    ECode Append(const String& other);
    ECode Append(const char* other);
    ECode Append(const char* other, UInt32 numChars);

    String Replace(const char* oldStr, const char* newStr) const;

#ifdef _GNUC_
    ECode AppendFormat(const char* fmt, ...)
            __attribute__((format (printf, 2, 3)));
#else
    ECode AppendFormat(const char* fmt, ...);
#endif

    // Note that this function takes O(N) time to calculate the value.
    // No cache value is stored.
    UInt32 GetCharCount() const;
    Int32 GetChar(UInt32 index) const;
    Int32 GetCharByOffset(UInt32 offset, UInt32 *nextOffset = NULL) const;

    inline String& operator=(const String& other);
    inline String& operator=(const char* other);

    inline String& operator+=(const String& other);
    inline String operator+(const String& other) const;

    inline String& operator+=(const char* other);
    inline String operator+(const char* other) const;

    inline const Char8 operator[](Int32 idx) const;

    inline Int32 Compare(const String& other,
            StringCase stringCase = StringCase_Sensitive) const;
    inline Int32 Compare(const char* other,
            StringCase stringCase = StringCase_Sensitive) const;

    inline Int32 Equals(const String& str) const;
    inline Int32 Equals(const char* str) const;
    inline Int32 EqualsIgnoreCase(const String& str) const;
    inline Int32 EqualsIgnoreCase(const char* str) const;

    inline Boolean operator<(const String& other) const;
    inline Boolean operator<=(const String& other) const;
    inline Boolean operator==(const String& other) const;
    inline Boolean operator!=(const String& other) const;
    inline Boolean operator>=(const String& other) const;
    inline Boolean operator>(const String& other) const;

    inline Boolean operator<(const char* other) const;
    inline Boolean operator<=(const char* other) const;
    inline Boolean operator==(const char* other) const;
    inline Boolean operator!=(const char* other) const;
    inline Boolean operator>=(const char* other) const;
    inline Boolean operator>(const char* other) const;

    inline operator const char*() const;
    inline operator CString() const;

    char* LockBuffer(UInt32 size);
    void UnlockBuffer();
    ECode UnlockBuffer(UInt32 size);

    // return the index of the first byte of other in this at or after
    // start, or -1 if not found
    Int32 Find(const char* other, UInt32 start = 0) const;

    void ToLowerCase();
    void ToLowerCase(UInt32 start, UInt32 numChars);
    void ToUpperCase();
    void ToUpperCase(UInt32 start, UInt32 numChars);

    inline Int32 ToInt32() const;
    inline Int64 ToInt64() const;
    inline Boolean ToBoolean() const;
    inline Double ToDouble() const;

    static String FromInt32(Int32 value);
    static String FromInt64(Int64 value);
    static String FromBoolean(Boolean value);
    static String FromDouble(Double value);

    //---- Contains ----
    inline Boolean Contains(const String& substr,
            StringCase stringCase = StringCase_Sensitive) const;

    inline Boolean StartWith(const String& substr,
            StringCase stringCase = StringCase_Sensitive) const;

    inline Boolean EndWith(const String& substr,
            StringCase stringCase = StringCase_Sensitive) const;

    inline Boolean Contains(const char* substr,
            StringCase stringCase = StringCase_Sensitive) const;

    inline Boolean StartWith(const char* substr,
            StringCase stringCase = StringCase_Sensitive) const;

    inline Boolean EndWith(const char* substr,
            StringCase stringCase = StringCase_Sensitive) const;

    //---- IndexOf ----
    inline Int32 IndexOf(Char32 ch,
            StringCase stringCase = StringCase_Sensitive) const;

    inline Int32 IndexOf(const String& str,
            StringCase stringCase = StringCase_Sensitive) const;

    inline Int32 IndexOf(const char* str,
            StringCase stringCase = StringCase_Sensitive) const;

    //---- LastIndexOf ----
    inline Int32 LastIndexOf(Char32 ch,
            StringCase stringCase = StringCase_Sensitive) const;

    inline Int32 LastIndexOf(const String& str,
            StringCase stringCase = StringCase_Sensitive) const;

    inline Int32 LastIndexOf(const char* str,
            StringCase stringCase = StringCase_Sensitive) const;

    //---- Substring ----
    String Substring(Int32 start) const;

    String Substring(Int32 start, Int32 len) const;

    //---- TrimStart ----
    String TrimStart() const;

    //---- TrimEnd ----
    String TrimEnd() const;

    //---- Trim ----
    String Trim() const;

    inline Int32 GetHashCode();

private:
    ECode RealAppend(const char* other, UInt32 numChars);

    const char* mString;
};


inline const char* String::string() const
{
    return mString;
}

inline UInt32 String::GetLength() const
{
    assert(mString != NULL);
    return (Int32)SharedBuffer::GetSizeFromData(mString) - 1;
}

inline UInt32 String::GetSize() const
{
    return GetLength();
}

inline UInt32 String::GetBytes() const
{
    assert(mString != NULL);
    return (Int32)SharedBuffer::GetSizeFromData(mString) - 1;
}

inline Boolean String::IsNull() const
{   // e.g., if (str.IsNull()) {...} or Boolean b = str.IsNull();
    return mString == NULL;
}

inline Boolean String::IsEmpty() const
{
    assert(mString);
    return mString[0] == '\0';
}

inline Boolean String::IsNullOrEmpty() const
{
    return (mString == NULL || mString[0] == '\0');
}

inline const SharedBuffer* String::GetSharedBuffer() const
{
    return SharedBuffer::GetBufferFromData(mString);
}

inline String& String::operator=(const String& other)
{
    SetTo(other);
    return *this;
}

inline String& String::operator=(const char* other)
{
    SetTo(other);
    return *this;
}

inline String& String::operator+=(const String& other)
{
    Append(other);
    return *this;
}

inline String String::operator+(const String& other) const
{
    String tmp(*this);
    tmp += other;
    return tmp;
}

inline String& String::operator+=(const char* other)
{
    Append(other);
    return *this;
}

inline String String::operator+(const char* other) const
{
    String tmp(*this);
    tmp += other;
    return tmp;
}

inline const Char8 String::operator[](Int32 idx) const
{
    assert(0 <= idx && (UInt32)idx < GetSize());
    return mString[idx];
}

inline Int32 String::Compare(const String& other, StringCase stringCase) const
{
    return _String_Compare(mString, other.mString, stringCase);
}

inline Int32 String::Compare(const char* other, StringCase stringCase) const
{
    return _String_Compare(mString, other, stringCase);
}

inline Int32 String::Equals(const String& str) const
{
    return !Compare(str, StringCase_Sensitive);
}

inline Int32 String::Equals(const char* str) const
{
    return !Compare(str, StringCase_Sensitive);
}

inline Int32 String::EqualsIgnoreCase(const String& str) const
{
    return !Compare(str, StringCase_Insensitive);
}

inline Int32 String::EqualsIgnoreCase(const char* str) const
{
    return !Compare(str, StringCase_Insensitive);
}

inline Boolean String::operator<(const String& other) const
{
    assert(mString != NULL && other.mString != NULL);
    return strcmp(mString, other.mString) < 0;
}

inline Boolean String::operator<=(const String& other) const
{
    assert(mString != NULL && other.mString != NULL);
    return strcmp(mString, other.mString) <= 0;
}

inline Boolean String::operator==(const String& other) const
{
    assert(mString != NULL && other.mString != NULL);
    return strcmp(mString, other.mString) == 0;
}

inline Boolean String::operator!=(const String& other) const
{
    assert(mString != NULL && other.mString != NULL);
    return strcmp(mString, other.mString) != 0;
}

inline Boolean String::operator>=(const String& other) const
{
    assert(mString != NULL && other.mString != NULL);
    return strcmp(mString, other.mString) >= 0;
}

inline Boolean String::operator>(const String& other) const
{
    assert(mString != NULL && other.mString != NULL);
    return strcmp(mString, other.mString) > 0;
}

inline Boolean String::operator<(const char* other) const
{
    assert(mString != NULL && other != NULL);
    return strcmp(mString, other) < 0;
}

inline Boolean String::operator<=(const char* other) const
{
    assert(mString != NULL && other != NULL);
    return strcmp(mString, other) <= 0;
}

inline Boolean String::operator==(const char* other) const
{
    assert(mString != NULL && other != NULL);
    return strcmp(mString, other) == 0;
}

inline Boolean String::operator!=(const char* other) const
{
    assert(mString != NULL && other != NULL);
    return strcmp(mString, other) != 0;
}

inline Boolean String::operator>=(const char* other) const
{
    assert(mString != NULL && other != NULL);
    return strcmp(mString, other) >= 0;
}

inline Boolean String::operator>(const char* other) const
{
    assert(mString != NULL && other != NULL);
    return strcmp(mString, other) > 0;
}

inline String::operator const char*() const
{
    return mString;
}

inline String::operator CString() const
{
    return mString;
}

inline Int32 String::ToInt32() const
{
    return _String_ToInt32(mString);
}

inline Int64 String::ToInt64() const
{
    return _String_ToInt64(mString);
}

inline Boolean String::ToBoolean() const
{
    return _String_ToBoolean(mString);
}

inline Double String::ToDouble() const
{
    return _String_ToDouble(mString);
}

//---- Contains ----
inline Boolean String::Contains(const String& substr,
        StringCase stringCase) const
{
    return _String_Contains(mString, substr.mString, stringCase);
}

inline Boolean String::StartWith(const String& substr,
        StringCase stringCase) const
{
    return _String_StartWith(mString, substr.mString, stringCase);
}

inline Boolean String::EndWith(const String& substr,
        StringCase stringCase) const
{
    return _String_EndWith(mString, substr.mString, stringCase);
}

inline Boolean String::Contains(const char* substr,
        StringCase stringCase) const
{
    return _String_Contains(mString, substr, stringCase);
}

inline Boolean String::StartWith(const char* substr,
        StringCase stringCase) const
{
    return _String_StartWith(mString, substr, stringCase);
}

inline Boolean String::EndWith(const char* substr,
        StringCase stringCase) const
{
    return _String_EndWith(mString, substr, stringCase);
}

//---- IndexOf ----
inline Int32 String::IndexOf(Char32 ch, StringCase stringCase) const
{
    return _String_IndexOf_Char(mString, ch, stringCase);
}

inline Int32 String::IndexOf(const String& str, StringCase stringCase) const
{
    return _String_IndexOf_Substring(mString, str.mString, stringCase);
}

inline Int32 String::IndexOf(const char* str, StringCase stringCase) const
{
    return _String_IndexOf_Substring(mString, str, stringCase);
}

//---- LastIndexOf ----
inline Int32 String::LastIndexOf(Char32 ch, StringCase stringCase) const
{
    return _String_LastIndexOf_Char(mString, ch, stringCase);
}

inline Int32 String::LastIndexOf(const String& str, StringCase stringCase) const
{
    return _String_LastIndexOf_Substring(mString, str.mString, stringCase);
}

inline Int32 String::LastIndexOf(const char* str, StringCase stringCase) const
{
    return _String_LastIndexOf_Substring(mString, str, stringCase);
}

inline Int32 String::GetHashCode()
{
    return _String_GetHashCode(mString);
}


_ELASTOS_NAMESPACE_END


#else //!__cplusplus

typedef const char* String;

#endif //__cplusplus

#endif // __ELSTRING_H__
