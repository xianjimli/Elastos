
#ifndef __XFERMODE_H__
#define __XFERMODE_H__

#include <sys/atomics.h>
#include <SkXfermode.h>
#include "ext/frameworkext.h"

extern "C" const InterfaceID EIID_Xfermode;

class Xfermode
{
    friend class Paint;

protected:
    ~Xfermode();

private:
    static CARAPI_(void) Finalizer(
        /* [in] */ SkXfermode* xfermode);

//protected:
public:
    SkXfermode* mNativeInstance;
};

#endif //__XFERMODE_H__
