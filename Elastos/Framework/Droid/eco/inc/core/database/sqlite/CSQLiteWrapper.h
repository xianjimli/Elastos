
#ifndef __CSQLITEWRAPPER_H__
#define __CSQLITEWRAPPER_H__

#include "_CSQLiteWrapper.h"
#include "SQLiteWrapper.h"

CarClass(CSQLiteWrapper),public SQLiteWrapper
{
public:
    CARAPI constructor();

};

#endif // __CSQLITEWRAPPER_H__
