
#ifndef __CFOOBARAPPLET_H__
#define __CFOOBARAPPLET_H__

#include "_CFooBarApplet.h"

CarClass(CFooBarApplet)
{
public:
	CFooBarApplet();

    ~CFooBarApplet();

    CARAPI Main(
        /* [in] */ const BufferOf<CString>& args);

    CARAPI AtQuit(
        /* [in] */ Int32 n);

};

#endif //__CFOOBARAPPLET_H__
