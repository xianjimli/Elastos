
#ifndef __CDATABASEHELPER_H__
#define __CDATABASEHELPER_H__

#include "_CDatabaseHelper.h"

CarClass(CDatabaseHelper)
{
public:
    CARAPI Complete(
        /* [in] */ const String& sql,
        /* [out] */ Boolean* result);
};

#endif //__CDATABASEHELPER_H__