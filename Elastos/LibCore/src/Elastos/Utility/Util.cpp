
#include "Util.h"
#include <StringBuffer.h>

using namespace Elastos::Core;

String Util::ToASCIILowerCase(
    /* [in] */ const String& s)
{
    // int len = s.GetLength();
    // StringBuilder buffer = new StringBuilder(len);
    // for (int i = 0; i < len; i++) {
    //     char c = s.charAt(i);
    //     if ('A' <= c && c <= 'Z') {
    //         buffer.append((char) (c + ('a' - 'A')));
    //     } else {
    //         buffer.append(c);
    //     }
    // }
    // return buffer.toString();
    return s;
}

String Util::ToASCIIUpperCase(
    /* [in] */ const String& s)
{
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
    return s;
}
