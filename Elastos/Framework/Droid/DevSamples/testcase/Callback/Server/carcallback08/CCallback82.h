#ifndef __CCALLBACK82_H__
#define __CCALLBACK82_H__

#include "_CCallback82.h"

class CCallback82 : public _CCallback82
{
public:
    CARAPI Hello82();

    CARAPI OnTrigger81();

    CARAPI GetCount(Int32* pnCount);

private:
    Int32 m_nCount;
};

#endif // __CCALLBACK82_H__
