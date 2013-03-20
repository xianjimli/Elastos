
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
            /* [in] */ IInterface* Object,
            /* [out] */ InterfaceID* iID);

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
        AutoPtr<ICharSequence> mName;
        AutoPtr<ICharSequence> mDescription;
        AutoPtr<ICharSequence> mRStringOk;
        
    private:
        AutoPtr<IAlertDialog> mDialog;

    };

    //friend class DefaultClickHandler;
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

private:
    String mName;
    String mPath;
    String mFileName;
    String mDescription;
    AutoPtr<IPluginPreferencesClickHandler> mHandler;

};

#endif // __CPLUGIN_H__
