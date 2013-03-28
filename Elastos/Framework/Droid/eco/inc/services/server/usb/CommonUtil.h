
#ifndef __COMMONUTIL_H__
#define __COMMONUTIL_H__

#include "ext/frameworkext.h"

using namespace Elastos;

class CommonUtil
{
public:
    static CARAPI_(void) SplitString(
        /* [in] */ const String& str,
        /* [in] */ Char32 sep,
        /* [out] */ ArrayOf<String>** outStrs);

};

#endif // __COMMONUTIL_H__
