
#include "widget/CSimpleAdapter.h"

IADAPTER_METHODS_IMPL(CSimpleAdapter, SimpleAdapter, SimpleAdapter);

IListADAPTER_METHODS_IMPL(CSimpleAdapter, SimpleAdapter, SimpleAdapter);

IBASEADAPTER_METHODS_IMPL(CSimpleAdapter, SimpleAdapter, SimpleAdapter);

ECode CSimpleAdapter::constructor(
    /* [in] */ IContext* context,
    /* [in] */ IObjectContainer* data,
    /* [in] */ Int32 resource,
    /* [in] */ const ArrayOf<String>& from,
    /* [in] */ const ArrayOf<Int32>& to)
{
    return Init(context, data, resource, from, to);
}

ECode CSimpleAdapter::GetViewBinder(
    /* [out] */ IViewBinder** viewBinder)
{
    VALIDATE_NOT_NULL(viewBinder);
    AutoPtr<IViewBinder> temp = SimpleAdapter::GetViewBinder();
    *viewBinder = temp;
    if (*viewBinder) {
        (*viewBinder)->AddRef();
    }

    return NOERROR;
}

ECode CSimpleAdapter::SetViewBinder(
    /* [in] */ IViewBinder* viewBinder)
{
    return SimpleAdapter::SetViewBinder(viewBinder);
}

ECode CSimpleAdapter::SetViewImage(
    /* [in] */ IImageView* v,
    /* [in] */ Int32 value)
{
    return SimpleAdapter::SetViewImage(v, value);
}

ECode CSimpleAdapter::SetViewImageEx(
    /* [in] */ IImageView* v,
    /* [in] */ String value)
{
    return SimpleAdapter::SetViewImage(v, value);
}

ECode CSimpleAdapter::SetViewText(
    /* [in] */ ITextView* v,
    /* [in] */ String text)
{
    return SimpleAdapter::SetViewText(v, text);
}

ECode CSimpleAdapter::SetDropDownViewResource(
    /* [in] */ Int32 resource)
{
    return SimpleAdapter::SetDropDownViewResource(resource);
}

ECode CSimpleAdapter::GetDropDownView(
    /* [in] */ Int32 position,
    /* [in] */ IView* convertView,
    /* [in] */ IViewGroup* parent,
    /* [out] */ IView** view)
{
    VALIDATE_NOT_NULL(view);
    AutoPtr<IView> temp = SimpleAdapter::GetDropDownView(
        position, convertView, parent);
    *view = temp;
    if (*view) {
        (*view)->AddRef();
    }

    return NOERROR;
}

ECode CSimpleAdapter::GetFilter(
    /* [out] */ IFilter** filter)
{
    VALIDATE_NOT_NULL(filter);
    AutoPtr<IFilter> temp = SimpleAdapter::GetFilter();
    *filter = temp;
    if (*filter) {
        (*filter)->AddRef();
    }

    return NOERROR;
}
