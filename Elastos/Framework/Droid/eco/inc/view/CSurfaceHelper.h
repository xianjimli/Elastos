
#ifndef __CSURFACEHELPER_H__
#define __CSURFACEHELPER_H__

#include "_CSurfaceHelper.h"

CarClass(CSurfaceHelper)
{
public:
    CARAPI OpenTransaction();

    CARAPI CloseTransaction();
};

#endif //__CSURFACEHELPER_H__
