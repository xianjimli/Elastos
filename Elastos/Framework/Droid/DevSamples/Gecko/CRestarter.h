#ifndef __CRESTARTER_H__
#define __CRESTARTER_H__

#include "Activity.h"
#include "_CRestarter.h"

class CRestarter : public Activity
{
public:
    CARAPI OnCreate(
        /* [in] */ IBundle* savedInstanceState);
};

#endif //__CRESTARTER_H__

