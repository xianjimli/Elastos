//==========================================================================
// Copyright (c) 2000-2008,  Elastos, Inc.  All Rights Reserved.
//==========================================================================

#include <eltypes.h>
#include <stdlib.h>
#include <ctype.h>

#include <wchar.h>
#include <string.h>

#ifdef _MSVC
#define _stricmp strcasecmp
#endif

#define _atoi64 atol

#ifdef  __cplusplus
extern "C" {
#endif

_ELASTOS_NAMESPACE_USING

Char32 _getStringCharAt(const char *str, Int32 index);
Int32 _getStringIndexOf(const char *str, Char32 ch,
            StringCase strCase);
Int32 _getStringLastIndexOf(const char *str, Char32 ch,
            StringCase strCase);
Int32 _getStringLength(const char *str);
char* _getStringSubstring(const char *str, Int32 start,
            Int32 len, char *substr);
Int32 _getStringIndexOfAny(const char *str1, Char32 *str2,
            StringCase strCase);
Int32 _getStringLastIndexOfAny(const char *str1, Char32 *str2,
            StringCase strCase);
Int32 _getStringLowerCase(char *dest, Int32 dlen,
            const char *string, Int32 slen);
Int32 _getStringUpperCase(char *dest, Int32 dlen,
            const char *string, Int32 slen);
Int32 _StringCaseCompare(const char *str1, const char *str2,
            Int32 count);

char* _getStringCaseStrstr(const char *string, const char *substr);
char* _getStringCaseStrpbrk(const char *string, const char *strCharSet);
char* _getStringCaseStrchr(const char *string, char ch);
char* _getStringCaseStrrchr(const char *string, char ch);

Int32 __cdecl _String_GetLength(const char *str, Int32 maxLen)
{
    if (!str || maxLen < -1) return -1;
    if (maxLen == 0) return 0;
    if (maxLen == -1)  return strlen(str);

    const char *p = str;
    int i = 0;
    while (*p) {
        if (i >= maxLen) return -1;
        p++;
        i++;
    }
    return (p - str);
}

Int32 __cdecl _String_Compare(const char *string1, const char *string2,
    StringCase stringCase)
{
    if (string1 == string2) return 0;
    if (!string1) return -2;
    if (!string2) return 2;

    if (stringCase == StringCase_Sensitive) {
        return strcmp(string1, string2);
    }
    else {
        Int32 count = MAX(strlen(string1), strlen(string2));
        return _StringCaseCompare(string1, string2, count);
    }
}

char* __cdecl _String_Duplicate(const char *strSource)
{
    size_t len;
    char * copy;

    if (!strSource) {
        return NULL;
    }

    len = strlen(strSource) + 1;
    if ((copy = (char *)malloc(len)) == NULL) return NULL;
    memcpy(copy, strSource, len);
    return(copy);
}

void __cdecl _String_Free(char * str)
{
	if (str) {
	    free((void *)str);
	}
}

//---------
Int32 __cdecl _String_ToInt32(const char *string)
{
    if (!string) return 0;
    return atoi(string);
}

Int64 __cdecl _String_ToInt64(const char *string)
{
    if (!string) return 0;
    return _atoi64(string);
}

Boolean __cdecl _String_ToBoolean(const char *string)
{
    if (!string) return FALSE;
    if (strcmp(string, "1") == 0 || strcasecmp(string, "true") == 0) {
        return TRUE;
    }
    else {
        return FALSE;
    }
}

Double __cdecl _String_ToDouble(const char *string)
{
    if (!string) return 0.0;
    return strtod(string, NULL);
}

//---------
Int32 __cdecl _String_GetCharCount(const char *string)
{
    if (!string) return -1;
    return _getStringLength(string);
}

Char32 __cdecl _String_GetChar(const char *string, Int32 index)
{
    if (!string) return 0xFFFFFFFF;
    if ((index < 0) || (index >= (Int32)strlen(string))) {
        return 0xFFFFFFFF;
    }

    return _getStringCharAt(string, index);
}

//---- Contains ----
Boolean __cdecl _String_Contains(const char *string, const char *substr,
            StringCase stringCase)
{
   if (!string || !substr) return FALSE;

    if (stringCase == StringCase_Sensitive) {
        return !!strstr(string, substr);
    }
    else {
        return !!_getStringCaseStrstr(string, substr);
    }
}

//---- StartWith ----
Boolean __cdecl _String_StartWith(const char *string, const char *substr,
            StringCase stringCase)
{
    if (!string || !substr) return FALSE;
    if (substr[0] == '\0') return TRUE;

    if (stringCase == StringCase_Sensitive) {
        return !strncmp(string, substr, strlen(substr));
    }
    else {
        return !_StringCaseCompare(string, substr, strlen(substr));
    }
}

//---- EndWith ----
Boolean __cdecl _String_EndWith(const char *string, const char *substr,
            StringCase stringCase)
{
    if (!string || !substr) return FALSE;
    if (substr[0] == '\0') return TRUE;

    const char *endstr = string + strlen(string) - strlen(substr);
    if (endstr < string) return FALSE;

    return _String_StartWith(endstr, substr, stringCase);
}

//---- IndexOf ----
Int32 __cdecl _String_IndexOf_Char8(const char *string, char ch,
            StringCase stringCase)
{
    if (!string || !ch) return -1;

    const char *first = NULL;
    if (stringCase == StringCase_Sensitive) {
        first = strchr(string, ch);
    }
    else {
        first = _getStringCaseStrchr(string, ch);
    }
    if (first) return first - string;
    else return -1;
}

Int32 __cdecl _String_IndexOf_AnyChar8(const char *string, const char* strCharSet,
            StringCase stringCase)
{
    if (!string || !strCharSet) return -1;

    const char *ret = NULL;
    if (stringCase == StringCase_Sensitive) {
        ret = strpbrk(string, strCharSet);
    }
    else {
        ret = _getStringCaseStrpbrk(string, strCharSet);
    }
    if (ret) return ret - string;
    else return -1;
}

Int32 __cdecl _String_IndexOf_Char(const char *string, Char32 ch,
            StringCase stringCase)
{
    if (!string) return -1;

    return _getStringIndexOf(string, ch, stringCase);
}

Int32 __cdecl _String_IndexOf_AnyChar(const char *string, Char32 *strCharSet,
            StringCase stringCase)
{
    if (!string || !strCharSet) return -1;

    return _getStringIndexOfAny(string, strCharSet, stringCase);
}

Int32 __cdecl _String_IndexOf_Substring(const char *string, const char *substr,
            StringCase stringCase)
{
    if (!string || !substr) return -1;

    const char *index = NULL;
    if (stringCase == StringCase_Sensitive) {
        index = strstr(string, substr);
    }
    else {
        index = _getStringCaseStrstr(string, substr);
    }
    return index ? index - string : -1;
}

//---- LastIndexOf ----
Int32 __cdecl _String_LastIndexOf_Char8(const char *string, char ch,
            StringCase stringCase)
{
    if (!string || !ch) return -1;

    const char *last = NULL;
    if (stringCase == StringCase_Sensitive) {
        last = strrchr(string, ch);
    }
    else {
        last = _getStringCaseStrrchr(string, ch);
    }
    if (last) return last - string;
    else return -1;
}

Int32 __cdecl _String_LastIndexOf_AnyChar8(const char *string, const char* strCharSet,
            StringCase stringCase)
{
    if (!string) return -1;

    const char *ret = string;
    const char *last;
    while(*ret) {
        if (stringCase == StringCase_Sensitive) {
            last = strpbrk(ret, strCharSet);
        }
        else {
            last = _getStringCaseStrpbrk(ret, strCharSet);
        }
        if (last) ret = last + 1;
        else break;
    }
    return ret - string - 1;
}

Int32 __cdecl _String_LastIndexOf_Char(const char *string, Char32 ch,
            StringCase stringCase)
{
    if (!string) return -1;

    return _getStringLastIndexOf(string, ch, stringCase);
}

Int32 __cdecl _String_LastIndexOf_AnyChar(const char *string, Char32 *strCharSet,
            StringCase stringCase)
{
    if (!string || !strCharSet) return -1;

    return _getStringLastIndexOfAny(string, strCharSet, stringCase);
}

Int32 __cdecl _String_LastIndexOf_Substring(const char *string, const char *substr,
            StringCase stringCase)
{
    if (!string || !substr) return -1;

    if (substr[0] == '\0') {
        int len = strlen(string);
        return len ? len - 1 : 0;
    }

    char *index = NULL;
    if (stringCase == StringCase_Sensitive) {
        index = strstr(string, substr);
    }
    else {
        index = _getStringCaseStrstr(string, substr);
    }
    if (!index) return -1;

    char *last;
    do {
    	last = index;
    	if (stringCase == StringCase_Sensitive) {
        	index = strstr(last + 1, substr);
        }
        else {
            index = _getStringCaseStrstr(last + 1, substr);
        }
    } while(index);

    return last - string;
}

//---- Substring ----
char* __cdecl _String_Substring_Buffer(const char *string, Int32 start, PCarQuintet pCq)
{
    if (!string || !pCq || !pCq->m_pBuf || start < 0) return NULL;

	Int32 nlength = (int)strlen(string);
	if (start > nlength) return NULL;

    Int32 len = MIN(nlength - start + 1, pCq->m_size - 1);
    _getStringSubstring(string, start, len,
                (char *)pCq->m_pBuf);
    pCq->m_used = strlen((char *)pCq->m_pBuf) + 1;
    return  (char *)pCq->m_pBuf;
}

char* __cdecl _String_Substring(const char *string, Int32 start)
{
    if (!string || start < 0 || start > (int)strlen(string)) return NULL;

    return _getStringSubstring(string, start, -1, NULL);
}

char* __cdecl _String_Substring_Length_Buffer(const char *string, Int32 start, Int32 len,
    PCarQuintet pCq)
{
    if (!string || !pCq || !pCq->m_pBuf || start < 0 || len < 0) return NULL;

	Int32 nlength = (int)strlen(string);
	if (start > nlength) return NULL;

    len = MIN(nlength - start + 1, len);
    len = MIN(len, pCq->m_size - 1);
    _getStringSubstring(string, start, len, (char *)pCq->m_pBuf);
    pCq->m_used = strlen((char *)pCq->m_pBuf) + 1;
    return  (char *)pCq->m_pBuf;
}

//---- ToLowerCase ----
char* __cdecl _String_ToLowerCase(const char *string, PCarQuintet pCq)
{
    if (!string || !pCq || !pCq->m_pBuf) return NULL;
    Int32 len = strlen(string);
    Int32 Cap = pCq->m_size / sizeof(Char8) - 1;

    Int32 ret = _getStringLowerCase((char *)pCq->m_pBuf,
            Cap, string, len);
    if (ret == -1) return NULL;

    pCq->m_used = (ret + 1) * sizeof(Char8);
    *(Char8 *)((Byte *)pCq->m_pBuf + pCq->m_used - sizeof(Char8)) = '\0';
    return (char *)pCq->m_pBuf;
}

//---- ToUpperCase ----
char* __cdecl _String_ToUpperCase(const char *string, PCarQuintet pCq)
{
    if (!string || !pCq || !pCq->m_pBuf) return NULL;
    Int32 len = strlen(string) + 1;
    Int32 Cap = pCq->m_size / sizeof(Char8) - 1;

    Int32 ret = _getStringUpperCase((char *)pCq->m_pBuf,
            Cap, string, len);
    if (ret == -1) return NULL;

    pCq->m_used = (ret + 1) * sizeof(Char8);
    *(Char8 *)((Byte *)pCq->m_pBuf + pCq->m_used - sizeof(Char8)) = '\0';
    return (char *)pCq->m_pBuf;
}

//---- TrimStart ----
char* __cdecl _String_TrimStart(const char *string, PCarQuintet pCq)
{
    if (!string || !pCq || !pCq->m_pBuf) return NULL;

    while (isspace(*string) && *string) {
        string++;
    }

    _StringBuf_Copy(pCq, string);

    return (char *)pCq->m_pBuf;
}

//---- TrimEnd ----
char* __cdecl _String_TrimEnd(const char *string, PCarQuintet pCq)
{
    if (!string || !pCq || !pCq->m_pBuf) return NULL;

    const char* pend = string + strlen(string) - 1;
    while (isspace(*pend) && pend >= string) {
        pend--;
    }

    Int32 nLen = (pend - string + 1) * sizeof(Char8);
    Int32 nCap = pCq->m_size - sizeof(Char8);
    nLen = MIN(nLen, nCap);

    memcpy(pCq->m_pBuf, string, nLen);
    *(Char8 *)((Byte *)pCq->m_pBuf + nLen) = '\0';
    pCq->m_used = nLen + sizeof(Char8);

    return (char *)pCq->m_pBuf;
}

//---- Trim ----
char* __cdecl _String_Trim(const char *string, PCarQuintet pCq)
{
    return _String_TrimEnd(_String_TrimStart(string, pCq), pCq);
}

//StringBuf& StringBuf::Trim(String encoding = NULL);
//

//---- HashCode ----
Int32 _String_GetHashCode(const char *string)
{
    Int32 h = 0;
    for ( ; *string; ++string) {
        h = 5 * h + *string;
    }
    return h;
}

#ifdef  __cplusplus
}
#endif
