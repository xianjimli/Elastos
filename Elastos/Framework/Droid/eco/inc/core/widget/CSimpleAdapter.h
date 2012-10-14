#ifndef __CSIMPLEADAPTER_H__
#define __CSIMPLEADAPTER_H__

#include "_CSimpleAdapter.h"
#include "widget/AdapterMacro.h"
#include "widget/SimpleAdapter.h"

CarClass(CSimpleAdapter), public SimpleAdapter
{
public:
    IADAPTER_METHODS_DECL();

    ILISTADAPTER_METHODS_DECL();

    IBASEADAPTER_METHODS_DECL();

    CARAPI constructor(
        /* [in] */ IContext* context,
        /* [in] */ IObjectContainer* data,
        /* [in] */ Int32 resource,
        /* [in] */ const ArrayOf<String>& from,
        /* [in] */ const ArrayOf<Int32>& to);

    CARAPI GetViewBinder(
        /* [out] */ IViewBinder** viewBinder);

    CARAPI SetViewBinder(
        /* [in] */ IViewBinder* viewBinder);

    CARAPI SetViewImage(
        /* [in] */ IImageView* v,
        /* [in] */ Int32 value);

    CARAPI SetViewImageEx(
        /* [in] */ IImageView* v,
        /* [in] */ const String& value);

    CARAPI SetViewText(
        /* [in] */ ITextView* v,
        /* [in] */ const String& text);

    CARAPI GetFilter(
        /* [out] */ IFilter** filter);

    CARAPI SetDropDownViewResource(
        /* [in] */ Int32 resource);

    CARAPI GetDropDownView(
        /* [in] */ Int32 position,
        /* [in] */ IView* convertView,
        /* [in] */ IViewGroup* parent,
        /* [out] */ IView** view);
};
#endif //__CSIMPLEADAPTER_H__
