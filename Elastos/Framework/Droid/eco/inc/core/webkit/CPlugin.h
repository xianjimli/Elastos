
#ifndef __CPLUGIN_H__
#define __CPLUGIN_H__

#include "_CPlugin.h"

CarClass(CPlugin)
{
public:
    CARAPI ToString(
        /* [out] */ String * pName);

    CARAPI GetName(
        /* [out] */ String * pName);

    CARAPI GetPath(
        /* [out] */ String * pPath);

    CARAPI getFileName(
        /* [out] */ String * pFileName);

    CARAPI GetDescription(
        /* [out] */ String * pDescription);

    CARAPI SetName(
        /* [in] */ CString name);

    CARAPI SetPath(
        /* [in] */ CString path);

    CARAPI SetFileName(
        /* [in] */ CString fileName);

    CARAPI SetDescription(
        /* [in] */ CString description);

    CARAPI SetClickHandler(
        /* [in] */ IPluginPreferencesClickHandler * pHandler);

    CARAPI DispatchClickEvent(
        /* [in] */ IContext * pContext);

    CARAPI constructor(
        /* [in] */ CString name,
        /* [in] */ CString path,
        /* [in] */ CString fileName,
        /* [in] */ CString description);

private:
    // TODO: Add your private member variables here.
};

#endif // __CPLUGIN_H__
