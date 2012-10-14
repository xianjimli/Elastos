
#include "utils/ListUtils.h"

Boolean ListUtils::Contains(
    /* [in] */ const List<String>& strs,
    /* [in] */ const String& str)
{
    Boolean result = FALSE;
    List<String>::ConstIterator itor;
    for (itor = strs.Begin();
         itor != strs.End();
         itor++) {
        if (str.Equals(*itor)) {
            result = TRUE;
            break;
        }
    }
    return result;
}

