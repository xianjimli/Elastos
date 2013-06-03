#ifndef __PLUGIN_H__
#define __PLUGIN_H__

#include "../app/CAlertDialogBuilder.h"
#include <elastos/ElRefBase.h>
/**
 * Represents a plugin (Java equivalent of the PluginPackageAndroid
 * C++ class in libs/WebKitLib/WebKit/WebCore/plugins/android/)
 *
 * @deprecated This interface was intended to be used by Gears. Since Gears was
 * deprecated, so is this class.
 */
//@Deprecated
class Plugin {

//public:
//    class PreferencesClickHandler {
//    public:
//        virtual CARAPI_(void) handleClickEvent(
//            /* [in] */ IContext* context) = 0;
//    };

private:
   /**
    * Default click handler. The plugins should implement their own.
    *
    * @deprecated This interface was intended to be used by Gears. Since Gears was
    * deprecated, so is this class.
    */
    //@Deprecated
    class DefaultClickHandler : public ElRefBase, public IPluginPreferencesClickHandler, public IDialogInterfaceOnClickListener
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

public:
    /**
     * @deprecated This interface was intended to be used by Gears. Since Gears was
     * deprecated, so is this class.
     */
    //@Deprecated
    Plugin(
        /* [in] */ const String& name,
        /* [in] */ const String& path,
        /* [in] */ const String& fileName,
        /* [in] */ const String& description);

    /**
     * @deprecated This interface was intended to be used by Gears. Since Gears was
     * deprecated, so is this class.
     */
    //@Deprecated
    virtual CARAPI_(String) ToString();

    /**
     * @deprecated This interface was intended to be used by Gears. Since Gears was
     * deprecated, so is this class.
     */
    //@Deprecated
    virtual CARAPI_(String) GetName();

    /**
     * @deprecated This interface was intended to be used by Gears. Since Gears was
     * deprecated, so is this class.
     */
    //@Deprecated
    virtual CARAPI_(String) GetPath();

    /**
     * @deprecated This interface was intended to be used by Gears. Since Gears was
     * deprecated, so is this class.
     */
    //@Deprecated
    virtual CARAPI_(String) GetFileName();

    /**
     * @deprecated This interface was intended to be used by Gears. Since Gears was
     * deprecated, so is this class.
     */
    //@Deprecated
    virtual CARAPI_(String) GetDescription();

    /**
     * @deprecated This interface was intended to be used by Gears. Since Gears was
     * deprecated, so is this class.
     */
    //@Deprecated
    virtual CARAPI SetName(
        /* [in] */ const String& name);

    /**
     * @deprecated This interface was intended to be used by Gears. Since Gears was
     * deprecated, so is this class.
     */
    //@Deprecated
    virtual CARAPI SetPath(
        /* [in] */ const String& path);

    /**
     * @deprecated This interface was intended to be used by Gears. Since Gears was
     * deprecated, so is this class.
     */
    //@Deprecated
    virtual CARAPI SetFileName(
        /* [in] */ const String& fileName);

    /**
     * @deprecated This interface was intended to be used by Gears. Since Gears was
     * deprecated, so is this class.
     */
    //@Deprecated
    virtual CARAPI SetDescription(
        /* [in] */ const String& description);

    /**
     * @deprecated This interface was intended to be used by Gears. Since Gears was
     * deprecated, so is this class.
     */
    //@Deprecated
    virtual CARAPI SetClickHandler(
        /* [in] */ IPluginPreferencesClickHandler* handler);

   /**
    * Invokes the click handler for this plugin.
    *
    * @deprecated This interface was intended to be used by Gears. Since Gears was
    * deprecated, so is this class.
    */
    //@Deprecated
    virtual CARAPI DispatchClickEvent(
        /* [in] */ IContext* context);

protected:
    void Init(
        /* [in] */ const String& name,
        /* [in] */ const String& path,
        /* [in] */ const String& fileName,
        /* [in] */ const String& description);

    Plugin();

private:
    String mName;
    String mPath;
    String mFileName;
    String mDescription;
    AutoPtr<IPluginPreferencesClickHandler> mHandler;
};

#endif //__PLUGIN_H__