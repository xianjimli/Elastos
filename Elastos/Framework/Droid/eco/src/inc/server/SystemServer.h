
#ifndef __SYSTEMSERVER_H__
#define __SYSTEMSERVER_H__

#include "ext/frameworkext.h"

class SystemServer
{
public:
    static const char* TAG;

    static const Int32 FACTORY_TEST_OFF = 0;
    static const Int32 FACTORY_TEST_LOW_LEVEL = 1;
    static const Int32 FACTORY_TEST_HIGH_LEVEL = 2;
};

#endif //__SYSTEMSERVER_H__
