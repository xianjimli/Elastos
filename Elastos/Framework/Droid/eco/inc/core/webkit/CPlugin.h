
#ifndef __CPLUGIN_H__
#define __CPLUGIN_H__

#include "_CPlugin.h"
#include "../app/CAlertDialogBuilder.h"
#include <elastos/ElRefBase.h>

CarClass(CPlugin)
{
    /**
    * Default click handler. The plugins should implement their own.
    *
    * @deprecated This interface was intended to be used by Gears. Since Gears was
    * deprecated, so is this class.
    */
    //@Deprecated  
    class DefaultClickHandler: public ElRefBase, public IPluginPreferencesClickHandler, public IDialogInterfaceOnClickListener
    {
    public:
        CARAPI_(PInterface) Probe(
            /* [in] */ REIID riid);

        CARAPI_(UInt32) AddRef();

        CARAPI_(UInt32) Release();

        CARAPI GetInterfaceID(
            /* [in] */ IInterface *pObject,
            /* [out] */ InterfaceID *pIID);

        //@Deprecated    
        CARAPI HandleClickEvent(
            /* [in] */ IContext* context);
        
        /**
         * @deprecated This interface was intended to be used by Gears. Since Gears was
         * deprecated, so is this class.
         */
        //@Deprecated
        CARAPI OnClick(
            /* [in] */ IDialogInterface* dialog, 
            /* [in] */ Int32 which);

    public:
        AutoPtr<ICharSequence> pName;
        AutoPtr<ICharSequence> pDescription;
        AutoPtr<ICharSequence> pRStringOk;
        
    private:
        AutoPtr<IAlertDialog> mDialog;

    };

    //friend class DefaultClickHandler;
public:
    CARAPI ToString(
        /* [out] */ String * pName);

    CARAPI GetName(
        /* [out] */ String * pName);

    CARAPI GetPath(
        /* [out] */ String * pPath);

    CARAPI GetFileName(
        /* [out] */ String * pFileName);

    CARAPI GetDescription(
        /* [out] */ String * pDescription);

    CARAPI SetName(
        /* [in] */ String name);

    CARAPI SetPath(
        /* [in] */ String path);

    CARAPI SetFileName(
        /* [in] */ String fileName);

    CARAPI SetDescription(
        /* [in] */ String description);

    CARAPI SetClickHandler(
        /* [in] */ IPluginPreferencesClickHandler * pHandler);

    CARAPI DispatchClickEvent(
        /* [in] */ IContext * pContext);

    CARAPI constructor(
        /* [in] */ const String& name,
        /* [in] */ const String& path,
        /* [in] */ const String& fileName,
        /* [in] */ const String& description);

private:
    String mName;
    String mPath;
    String mFileName;
    String mDescription;
    AutoPtr<IPluginPreferencesClickHandler> mHandler;

};

#endif // __CPLUGIN_H__
