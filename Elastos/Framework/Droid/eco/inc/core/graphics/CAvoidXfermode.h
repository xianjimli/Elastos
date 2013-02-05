
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

private:
    static CARAPI_(SkXfermode*) nativeCreate(Int32 opColor,
                                Int32 tolerance,
                                Int32 nativeMode);
};

#endif // __CAVOIDXFERMODE_H__
