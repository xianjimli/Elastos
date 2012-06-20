
#ifndef __CTEXTFACTORY_H__
#define __CTEXTFACTORY_H__

#include "_CTextFactory.h"

CarClass(CTextFactory)
{
public:
    CARAPI ProcessText(
        /* [in] */ ArrayOf<Byte>* text,
        /* [in] */ Int32 width,
        /* [in] */ Int32 height,
        /* [out] */ IObjectContainer** texts);
};

#endif //__CTEXTFACTORY_H__
