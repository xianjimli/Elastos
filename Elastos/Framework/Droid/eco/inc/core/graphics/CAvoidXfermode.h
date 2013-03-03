
#ifndef __CAVOIDXFERMODE_H__
#define __CAVOIDXFERMODE_H__

#include "_CAvoidXfermode.h"
#include "graphics/Xfermode.h"

CarClass(CAvoidXfermode), public Xfermode
{
public:
    CARAPI constructor(
        /* [in] */ Int32 opColor,
        /* [in] */ Int32 tolerance,
        /* [in] */ Int32 mode);

    CARAPI_(PInterface) Probe(
            /* [in]  */ REIID riid);

private:
    static CARAPI_(SkXfermode*) NativeCreate(
        /* [in] */ Int32 opColor,
        /* [in] */ Int32 tolerance,
        /* [in] */ Int32 nativeMode);
};

#endif // __CAVOIDXFERMODE_H__
