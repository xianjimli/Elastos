
#ifndef __MENUBUILDER_H__
#define __MENUBUILDER_H__

#include "ext/frameworkext.h"
#include "view/MenuMacro.h"
#include "view/menu/MenuBuilderBase.h"
#include "view/menu/MenuBuilderMacro.h"
#include <elastos/ElRefBase.h>


class MenuBuilder
    : public ElRefBase
    , public MenuBuilderBase
    , public IMenuBuilder
{
public:
    MenuBuilder(
        /*[in]*/ IContext* context);

    CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid);

    CARAPI_(UInt32) AddRef();

    CARAPI_(UInt32) Release();

    CARAPI GetInterfaceID(
        /* [in] */ IInterface *pObject,
        /* [out] */ InterfaceID *pIID);

    IMENU_METHODS_DECL();
    IMENUBUILDER_METHODS_DECL();
};

#endif    //__MENUBUILDER_H__
