
#ifndef __CCALBAKSVR_H__
#define __CCALBAKSVR_H__

#include <carcallback13_server.h>

ECode JCBTest2_OnEnterCoalescer(String oldStr, String *newStr);

ECode JCBTest3_OnRectCoalescer(Int32 x_old, Int32 y_old, Int32 width_old, Int32 height_old,
    Int32 *pX_new, Int32 *pY_new, Int32 *pWidth_new, Int32 *pHeight_new);

ECode JCBTest3_OnArrayCoalescer(const BufferOf<Int32> & oldBuf, BufferOf<Int32> *pNewBuf);

#include "_CCalBakSvr.h"

CarClass(CCalBakSvr)
{
public:
    CARAPI Trigger(
        /* [in] */ Int32 nTriggerTimes,
        /* [in] */ String wstr);

private:
    // TODO: Add your private member variables here.
};

#endif // __CCALBAKSVR_H__
