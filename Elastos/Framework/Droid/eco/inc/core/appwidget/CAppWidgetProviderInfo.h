
#ifndef __CAPPWIDGETPROVIDERINFO_H__
#define __CAPPWIDGETPROVIDERINFO_H__

#include "_CAppWidgetProviderInfo.h"
#include <elastos/AutoPtr.h>

CarClass(CAppWidgetProviderInfo)
{
public:
    CAppWidgetProviderInfo();

    CARAPI SetProvider(
        /* [in] */ IComponentName* name);

    CARAPI GetProvider(
        /* [out] */ IComponentName** name);

    CARAPI SetMinWidth(
        /* [in] */ Int32 width);

    CARAPI GetMinWidth(
        /* [out] */ Int32* width);

    CARAPI SetMinHeight(
        /* [in] */ Int32 height);

    CARAPI GetMinHeight(
        /* [out] */ Int32* height);

    CARAPI SetUpdatePeriodMillis(
        /* [in] */ Int32 period);

    CARAPI GetUpdatePeriodMillis(
        /* [out] */ Int32* period);

    CARAPI SetInitialLayout(
        /* [in] */ Int32 layout);

    CARAPI GetInitialLayout(
        /* [out] */ Int32* layout);

    CARAPI SetConfigure(
        /* [in] */ IComponentName* name);

    CARAPI GetConfigure(
        /* [out] */ IComponentName** name);

    CARAPI SetLabel(
        /* [in] */ const String& label);

    CARAPI GetLabel(
        /* [out] */ String* label);

    CARAPI SetIcon(
        /* [in] */ Int32 icon);

    CARAPI GetIcon(
        /* [out] */ Int32* icon);

    CARAPI SetPreviousName(
        /* [in] */ const String& name);

    CARAPI GetPreviousName(
        /* [out] */ String* name);

    CARAPI ReadFromParcel(
        /* [in] */ IParcel *source);

    CARAPI WriteToParcel(
        /* [in] */ IParcel *dest);

    CARAPI constructor();

    CARAPI constructor(
        /* [in] */ IParcel* source);

public:
    /**
     * Identity of this AppWidget component.  This component should be a {@link
     * android.content.BroadcastReceiver}, and it will be sent the AppWidget intents
     * {@link android.appwidget as described in the AppWidget package documentation}.
     *
     * <p>This field corresponds to the <code>android:name</code> attribute in
     * the <code>&lt;receiver&gt;</code> element in the AndroidManifest.xml file.
     */
    AutoPtr<IComponentName> mProvider;

    /**
     * Minimum width of the AppWidget, in dp.
     *
     * <p>This field corresponds to the <code>android:minWidth</code> attribute in
     * the AppWidget meta-data file.
     */
    Int32 mMinWidth;

    /**
     * Minimum height of the AppWidget, in dp.
     *
     * <p>This field corresponds to the <code>android:minHeight</code> attribute in
     * the AppWidget meta-data file.
     */
    Int32 mMinHeight;

    /**
     * How often, in milliseconds, that this AppWidget wants to be updated.
     * The AppWidget manager may place a limit on how often a AppWidget is updated.
     *
     * <p>This field corresponds to the <code>android:updatePeriodMillis</code> attribute in
     * the AppWidget meta-data file.
     *
     * <p class="note"><b>Note:</b> Updates requested with <code>updatePeriodMillis</code>
     * will not be delivered more than once every 30 minutes.</p>
     */
    Int32 mUpdatePeriodMillis;

    /**
     * The resource id of the initial layout for this AppWidget.  This should be
     * displayed until the RemoteViews for the AppWidget is available.
     *
     * <p>This field corresponds to the <code>android:initialLayout</code> attribute in
     * the AppWidget meta-data file.
     */
    Int32 mInitialLayout;

    /**
     * The activity to launch that will configure the AppWidget.
     *
     * <p>This class name of field corresponds to the <code>android:configure</code> attribute in
     * the AppWidget meta-data file.  The package name always corresponds to the package containing
     * the AppWidget provider.
     */
    AutoPtr<IComponentName> mConfigure;

    /**
     * The label to display to the user in the AppWidget picker.  If not supplied in the
     * xml, the application label will be used.
     *
     * <p>This field corresponds to the <code>android:label</code> attribute in
     * the <code>&lt;receiver&gt;</code> element in the AndroidManifest.xml file.
     */
    String mLabel;

    /**
     * The icon to display for this AppWidget in the AppWidget picker.  If not supplied in the
     * xml, the application icon will be used.
     *
     * <p>This field corresponds to the <code>android:icon</code> attribute in
     * the <code>&lt;receiver&gt;</code> element in the AndroidManifest.xml file.
     */
    Int32 mIcon;
    
    
    /**
     * The previous name, if any, of the app widget receiver. If not supplied, it will be
     * ignored.
     *
     * <p>This field corresponds to the <code>&lt;meta-data /&gt;</code> with the name
     * <code>android.appwidget.oldName</code>.
     * 
     * @hide Pending API approval
     */
    String mOldName;
};

#endif //__CAPPWIDGETPROVIDERINFO_H__
