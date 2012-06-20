
#ifndef __HH_AUTOSTRING_H
#define __HH_AUTOSTRING_H

#include <elastos.h>

_ELASTOS_NAMESPACE_BEGIN

// Auto free the buffer in the String object.
class AutoString : public String
{
public:
    AutoString(const char* str = NULL) : String(str) {}

    ~AutoString() { Free(*this); }

    void operator=(const String& str) { // e.g., str1 = str2;
        String::operator=(str);
    }
};

_ELASTOS_NAMESPACE_END

#endif // __HH_AUTOSTRING_H

