
#ifndef __CPLUGIN_H__
#define __CPLUGIN_H__

#include "_CPlugin.h"
#include "../app/CAlertDialogBuilder.h"
#include <elastos/ElRefBase.h>
#include "webkit/Plugin.h"

CarClass(CPlugin), public Plugin
{
public:
    CARAPI ToString(
        /* [out] */ String* name);

    CARAPI GetName(
        /* [out] */ String* name);

    CARAPI GetPath(
        /* [out] */ String* path);

    CARAPI GetFileName(
        /* [out] */ String* fileName);

    CARAPI GetDescription(
        /* [out] */ String* description);

    CARAPI SetName(
        /* [in] */ const String& name);

    CARAPI SetPath(
        /* [in] */ const String& path);

    CARAPI SetFileName(
        /* [in] */ const String& fileName);

    CARAPI SetDescription(
        /* [in] */ const String& description);

    CARAPI SetClickHandler(
        /* [in] */ IPluginPreferencesClickHandler* handler);

    CARAPI DispatchClickEvent(
        /* [in] */ IContext* context);

    CARAPI constructor(
        /* [in] */ const String& name,
        /* [in] */ const String& path,
        /* [in] */ const String& fileName,
        /* [in] */ const String& description);

};

#endif // __CPLUGIN_H__
