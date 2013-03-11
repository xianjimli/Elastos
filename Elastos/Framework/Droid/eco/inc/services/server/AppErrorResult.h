
#ifndef __APPERRORRESULT_H__
#define __APPERRORRESULT_H__

#include "ext/frameworkext.h"
#include <elastos/Mutex.h>

using namespace Elastos;
using namespace Elastos::Core;
using namespace Elastos::Core::Threading;

class AppErrorResult
{
public:
    AppErrorResult();

    CARAPI_(void) SetResult(
        /* [in] */ Int32 res);

    /*
     *  Return true if package has been added false if not
     */
    CARAPI_(Int32) GetResult();

public:
    Boolean mHasResult;// = false;
    Int32 mResult;

private:
    Mutex mLock;
};

#endif //__APPERRORRESULT_H__
