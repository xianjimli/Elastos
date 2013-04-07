
#ifndef __CREMOTEVIEWS_H__
#define __CREMOTEVIEWS_H__

#include "_CRemoteViews.h"
#include "ext/frameworkext.h"
#include <elastos/AutoPtr.h>
#include <elastos/List.h>

CarClass(CRemoteViews)
{
public:
    CRemoteViews();

    CARAPI Clone(
        /* [out] */ IRemoteViews ** ppResult);

    /**
     * Add an action to be executed on the remote side when apply is called.
     *
     * @param a The action to add
     */
    CARAPI AddAction(
        /* [in] */ IRemoteViewsAction* a);

    CARAPI GetPackage(
        /* [out] */ String * pPackage);

    CARAPI GetLayoutId(
        /* [out] */ Int32 * pLayoutId);

    CARAPI AddView(
        /* [in] */ Int32 viewId,
        /* [in] */ IRemoteViews * pNestedView);

    CARAPI RemoveAllViews(
        /* [in] */ Int32 viewId);

    CARAPI SetViewVisibility(
        /* [in] */ Int32 viewId,
        /* [in] */ Int32 visibility);

    CARAPI SetTextViewText(
        /* [in] */ Int32 viewId,
        /* [in] */ ICharSequence * pText);

    CARAPI SetImageViewResource(
        /* [in] */ Int32 viewId,
        /* [in] */ Int32 srcId);

    CARAPI SetImageViewUri(
        /* [in] */ Int32 viewId,
        /* [in] */ IUri * pUri);

    CARAPI SetImageViewBitmap(
        /* [in] */ Int32 viewId,
        /* [in] */ IBitmap * pBitmap);

    CARAPI SetChronometer(
        /* [in] */ Int32 viewId,
        /* [in] */ Int64 base,
        /* [in] */ const String& format,
        /* [in] */ Boolean started);

    CARAPI SetProgressBar(
        /* [in] */ Int32 viewId,
        /* [in] */ Int32 max,
        /* [in] */ Int32 progress,
        /* [in] */ Boolean indeterminate);

    CARAPI SetOnClickPendingIntent(
        /* [in] */ Int32 viewId,
        /* [in] */ IPendingIntent * pPendingIntent);

    CARAPI SetDrawableParameters(
        /* [in] */ Int32 viewId,
        /* [in] */ Boolean targetBackground,
        /* [in] */ Int32 alpha,
        /* [in] */ Int32 colorFilter,
        /* [in] */ PorterDuffMode mode,
        /* [in] */ Int32 level);

    CARAPI SetTextColor(
        /* [in] */ Int32 viewId,
        /* [in] */ Int32 color);

    CARAPI SetBoolean(
        /* [in] */ Int32 viewId,
        /* [in] */ const String& methodName,
        /* [in] */ Boolean value);

    CARAPI SetByte(
        /* [in] */ Int32 viewId,
        /* [in] */ const String& methodName,
        /* [in] */ Byte value);

    CARAPI SetInt16(
        /* [in] */ Int32 viewId,
        /* [in] */ const String& methodName,
        /* [in] */ Int16 value);

    CARAPI SetInt32(
        /* [in] */ Int32 viewId,
        /* [in] */ const String& methodName,
        /* [in] */ Int32 value);

    CARAPI SetInt64(
        /* [in] */ Int32 viewId,
        /* [in] */ const String& methodName,
        /* [in] */ Int64 value);

    CARAPI SetFloat(
        /* [in] */ Int32 viewId,
        /* [in] */ const String& methodName,
        /* [in] */ Float value);

    CARAPI SetDouble(
        /* [in] */ Int32 viewId,
        /* [in] */ const String& methodName,
        /* [in] */ Double value);

    CARAPI SetChar16(
        /* [in] */ Int32 viewId,
        /* [in] */ const String& methodName,
        /* [in] */ Char16 value);

    CARAPI SetString(
        /* [in] */ Int32 viewId,
        /* [in] */ const String& methodName,
        /* [in] */ const String& value);

    CARAPI SetCharSequence(
        /* [in] */ Int32 viewId,
        /* [in] */ const String& methodName,
        /* [in] */ ICharSequence * pValue);

    CARAPI SetUri(
        /* [in] */ Int32 viewId,
        /* [in] */ const String& methodName,
        /* [in] */ IUri * pValue);

    CARAPI SetBitmap(
        /* [in] */ Int32 viewId,
        /* [in] */ const String& methodName,
        /* [in] */ IBitmap * pValue);

    CARAPI SetBundle(
        /* [in] */ Int32 viewId,
        /* [in] */ const String& methodName,
        /* [in] */ IBundle * pValue);

    CARAPI Apply(
        /* [in] */ IContext * pContext,
        /* [in] */ IViewGroup * pParent,
        /* [out] */ IView ** ppView);

    CARAPI Reapply(
        /* [in] */ IContext* context,
        /* [in] */ IView* v);

    CARAPI ReadFromParcel(
        /* [in] */ IParcel* source);

    CARAPI WriteToParcel(
        /* [in] */ IParcel* dest);

    CARAPI OnLoadClass(
        /* [in] */ Handle32 clazz,
        /* [out] */ Boolean* allowed);

    CARAPI constructor(
        /* [in] */ const String& packageName,
        /* [in] */ Int32 layoutId);

    CARAPI constructor(
        /* [in] */ IParcel * pParcel);

private:
    CARAPI PerformApply(
        /* [in] */ IView* v);

    CARAPI PrepareContext(
        /* [in] */ IContext* context,
        /* [out] */ IContext** result);

private:
    /**
     * The package name of the package containing the layout
     * resource. (Added to the parcel)
     */
    String mPackage;

    /**
     * The resource ID of the layout file. (Added to the parcel)
     */
    Int32 mLayoutId;

    /**
     * An array of actions to perform on the view tree once it has been
     * inflated
     */
    List<AutoPtr<IRemoteViewsAction> > mActions;
};

#endif // __CREMOTEVIEWS_H__
