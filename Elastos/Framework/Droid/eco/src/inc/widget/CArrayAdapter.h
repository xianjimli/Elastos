#ifndef __CARRAYADAPTER_H__
#define __CARRAYADAPTER_H__

#include "_CArrayAdapter.h"
#include "widget/AdapterMacro.h"
#include "widget/ArrayAdapter.h"

CarClass(CArrayAdapter), public ArrayAdapter
{
public:
    static CARAPI_(AutoPtr<IArrayAdapter>) CreateFromResource(
        /* [in] */ IContext* context,
        /* [in] */ Int32 textArrayResId,
        /* [in] */ Int32 textViewResId);

public:
    IADAPTER_METHODS_DECL();

    ILISTADAPTER_METHODS_DECL();

    IBASEADAPTER_METHODS_DECL();

    CARAPI constructor(
        /* [in] */ IContext* context,
        /* [in] */ Int32 textViewResourceId);

    CARAPI constructor(
        /* [in] */ IContext* context,
        /* [in] */ Int32 resource,
        /* [in] */ Int32 textViewResourceId);

    CARAPI constructor(
        /* [in] */ IContext* context,
        /* [in] */ Int32 textViewResourceId,
        /* [in] */ IObjectContainer* objects);

    CARAPI constructor(
        /* [in] */ IContext* context,
        /* [in] */ Int32 resource,
        /* [in] */ Int32 textViewResourceId,
        /* [in] */ IObjectContainer* objects);

    CARAPI Add(
        /* [in] */ IInterface* object);

    CARAPI Insert(
        /* [in] */ IInterface* object,
        /* [in] */ Int32 index);

    CARAPI Remove(
        /* [in] */ IInterface* object);

    CARAPI Clear();

    CARAPI Sort(
        /* [in] */ IComparator* comparator);

    CARAPI SetNotifyOnChange(
        /* [in] */ Boolean notifyOnChange);

    CARAPI GetContext(
        /* [out] */ IContext** context);

    CARAPI GetPosition(
        /* [in] */ IInterface* item,
        /* [out] */ Int32* position);

    CARAPI SetDropDownViewResource(
        /* [in] */ Int32 resource);

    CARAPI GetDropDownView(
        /* [in] */ Int32 position,
        /* [in] */ IView* convertView,
        /* [in] */ IViewGroup* parent,
        /* [out] */ IView** view);

    CARAPI GetFilter(
        /* [out] */ IFilter** filter);
};
#endif //__CARRAYADAPTER_H__
