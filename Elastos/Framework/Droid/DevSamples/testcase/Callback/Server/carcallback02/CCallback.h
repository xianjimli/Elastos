#ifndef __CCALLBACK_H__
#define __CCALLBACK_H__

#include "_CCallback.h"

class CCallback : public _CCallback
{
public:
    CARAPI Hello();
    CARAPI Foo();
    CARAPI Bar();
    CARAPI GetCount(Int32* pnCount);

private:
    int m_nCount;
};

#endif // __CCALLBACK_H__
