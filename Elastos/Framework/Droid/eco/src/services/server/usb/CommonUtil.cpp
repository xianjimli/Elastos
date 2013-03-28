
#include "server/usb/CommonUtil.h"

void CommonUtil::SplitString(
    /* [in] */ const String& str,
    /* [in] */ Char32 sep,
    /* [out] */ ArrayOf<String>** outStrs)
{
    Int32 count = 1;
    Int32 i = 0;

    while ((i = str.IndexOf(sep, i)) >= 0) {
        count++;
        i++;
    }

    ArrayOf<String>* res = ArrayOf<String>::Alloc(count);

    i=0;
    count = 0;
    Int32 lastI=0;

    while ((i = str.IndexOf(sep, i)) >= 0) {
        (*res)[count] = str.Substring(lastI, i);
        count++;
        i++;
        lastI = i;
    }

    (*res)[count] = str.Substring(lastI, str.GetLength());
    *outStrs = res;
}
