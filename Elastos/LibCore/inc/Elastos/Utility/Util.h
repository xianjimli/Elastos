
#ifndef __UTIL_H__
#define __UTIL_H__

#include <elastos.h>

using namespace Elastos;

class Util
{
public:
    static CARAPI_(String) ToASCIILowerCase(
        /* [in] */ const String& s);

    static CARAPI_(String) ToASCIIUpperCase(
        /* [in] */ const String& s);
};

#endif //__UTIL_H__
