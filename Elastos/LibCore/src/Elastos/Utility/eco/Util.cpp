
#include "Util.h"
#include <StringBuffer.h>
#include <stdio.h>
using namespace Elastos::Core;

String Util::ToASCIILowerCase(
    /* [in] */ const String& s)
{
    Int32 len = s.GetLength();
    if (len == 0) {
        return s;
    }

    StringBuffer buffer(len);
    for (Int32 i = 0; i < len; i++) {
         String c = s.Substring(i, 1);

         if ('A' <= (char)(*(const char*)c) && (char)(*(const char *)c) <= 'Z') {
             char c1 = (*(const char *)c) + ('a' - 'A');
             buffer += String(&c1, 1);
         } else {
             buffer += c;
         }
    }
    return buffer.Substring(0,len);
}

String Util::ToASCIIUpperCase(
    /* [in] */ const String& s)
{
    Int32 len = s.GetLength();
    if (len == 0) {
        return s;
    }

    StringBuffer buffer(len);
    for (Int32 i = 0; i < len; i++) {
         String c = s.Substring(i, 1);
         if ('a' <= (char)(*(const char*)c) && (char)(*(const char *)c) <= 'z') {
             char c1 = (*(const char *)c) -  ('a' - 'A');
             buffer += String(&c1, 1);
         } else {
             buffer += c;
         }
    }
    return buffer.Substring(0,len);
    // int len = s.length();
    // StringBuilder buffer = new StringBuilder(len);
    // for (int i = 0; i < len; i++) {
    //     char c = s.charAt(i);
    //     if ('a' <= c && c <= 'z') {
    //         buffer.append((char) (c - ('a' - 'A')));
    //     } else {
    //         buffer.append(c);
    //     }
    // }
    // return buffer.toString();
}
