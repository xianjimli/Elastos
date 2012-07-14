
#ifndef __HH_LISTUTILS_H
#define __HH_LISTUTILS_H

#include <elastos.h>
using namespace Elastos;

#include "ext/frameworkdef.h"
#include "elastos/List.h"

class ListUtils
{
public:
    static CARAPI_(Boolean) Contains(
        /* [in] */ const List<String>& strs,
        /* [in] */ const String& str);
};

#endif // __HH_LISTUTILS_H