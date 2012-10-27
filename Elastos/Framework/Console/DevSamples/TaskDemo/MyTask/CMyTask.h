
#ifndef __CMYTASK_H__
#define __CMYTASK_H__

#include "_CMyTask.h"

CarClass(CMyTask)
{
public:
    ~CMyTask();

    CARAPI Main(
        /* [in] */ const BufferOf<CString>& args);
};

#endif //__CMYTASK_H__
