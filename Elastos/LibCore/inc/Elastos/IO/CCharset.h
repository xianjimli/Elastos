#ifndef __CCHAARSET_H__
#define __CCHAARSET_H__

#include "_CCharset.h"
#include "Charset.h"

CarClass(CCharset), public Charset
{
public:
    CARAPI constructor(
        /**in**/ String canonicalName, 
        /**in**/const ArrayOf<String>& aliases);

    CARAPI DisplayName(
        /**out**/ String* name);

    CARAPI DefaultCharset(
        /**out**/ ICharset** charset);
};
#endif //__CCHAARSET_H__