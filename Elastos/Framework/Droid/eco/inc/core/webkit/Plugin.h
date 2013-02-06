#ifndef __PLUGIN_H__
#define __PLUGIN_H__

/**
 * Represents a plugin (Java equivalent of the PluginPackageAndroid
 * C++ class in libs/WebKitLib/WebKit/WebCore/plugins/android/)
 *
 * @deprecated This interface was intended to be used by Gears. Since Gears was
 * deprecated, so is this class.
 */
//@Deprecated
class Plugin {

public:
	class PreferencesClickHandler {
	public:
		virtual CARAPI_(void) handleClickEvent(
			/* [in] */ IContext* context) = 0;
    };

    /**
     * @deprecated This interface was intended to be used by Gears. Since Gears was
     * deprecated, so is this class.
     */
    //@Deprecated
	Plugin(
		/* [in] */ CString name,
		/* [in] */ String path,
		/* [in] */ String fileName,
		/* [in] */ String description);

    /**
     * @deprecated This interface was intended to be used by Gears. Since Gears was
     * deprecated, so is this class.
     */
    //@Deprecated
	virtual CARAPI_(CString) ToString();

    /**
     * @deprecated This interface was intended to be used by Gears. Since Gears was
     * deprecated, so is this class.
     */
    //@Deprecated
	virtual CARAPI_(CString) GetName();

    /**
     * @deprecated This interface was intended to be used by Gears. Since Gears was
     * deprecated, so is this class.
     */
    //@Deprecated
	virtual CARAPI_(CString) GetPath();

    /**
     * @deprecated This interface was intended to be used by Gears. Since Gears was
     * deprecated, so is this class.
     */
    //@Deprecated
	virtual CARAPI_(CString) GetFileName();

    /**
     * @deprecated This interface was intended to be used by Gears. Since Gears was
     * deprecated, so is this class.
     */
    //@Deprecated
	virtual CARAPI_(CString) GetDescription();

    /**
     * @deprecated This interface was intended to be used by Gears. Since Gears was
     * deprecated, so is this class.
     */
    //@Deprecated
	virtual CARAPI_(void) SetName(
		/* [in] */ CString name);

    /**
     * @deprecated This interface was intended to be used by Gears. Since Gears was
     * deprecated, so is this class.
     */
    //@Deprecated
	virtual CARAPI_(void) SetPath(
		/* [in] */ CString path);

    /**
     * @deprecated This interface was intended to be used by Gears. Since Gears was
     * deprecated, so is this class.
     */
    //@Deprecated
	virtual CARAPI_(void) SetFileName(
		/* [in] */ CString fileName);

    /**
     * @deprecated This interface was intended to be used by Gears. Since Gears was
     * deprecated, so is this class.
     */
    //@Deprecated
	virtual CARAPI_(void) SetDescription(
		/* [in] */ CString description);

    /**
     * @deprecated This interface was intended to be used by Gears. Since Gears was
     * deprecated, so is this class.
     */
    //@Deprecated
	virtual CARAPI_(void) SetClickHandler(
		/* [in] */ PreferencesClickHandler* handler);

   /**
    * Invokes the click handler for this plugin.
    *
    * @deprecated This interface was intended to be used by Gears. Since Gears was
    * deprecated, so is this class.
    */
    //@Deprecated
	virtual CARAPI_(void) DispatchClickEvent(
		/* [in] */ IContext* context);

private:
   /**
    * Default click handler. The plugins should implement their own.
    *
    * @deprecated This interface was intended to be used by Gears. Since Gears was
    * deprecated, so is this class.
    */
    //@Deprecated
    class DefaultClickHandler : public PreferencesClickHandler,
                                public DialogInterface::OnClickListener 
	{
	public:       
        //@Deprecated
		virtual CARAPI_(void) HandleClickEvent(
			/* [in] */ IContext* context);

        /**
         * @deprecated This interface was intended to be used by Gears. Since Gears was
         * deprecated, so is this class.
         */
        //@Deprecated
		virtual CARAPI_(void) OnClick(
			/* [in] */ IDialogInterface* dialog, 
			/* [in] */ Int32 which);

    private:
		AlertDialog mDialog;
    }

private:
	CString mName;
	CString mPath;
	CString mFileName;
	CString mDescription;
	PreferencesClickHandler mHandler;
};

#endif //__PLUGIN_H__