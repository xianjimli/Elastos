
#include "widget/CArrayAdapter.h"
#include "utils/CParcelableObjectContainer.h"
#include "utils/AutoStringArray.h"

IADAPTER_METHODS_IMPL(CArrayAdapter, ArrayAdapter, ArrayAdapter);

IListADAPTER_METHODS_IMPL(CArrayAdapter, ArrayAdapter, ArrayAdapter);

IBASEADAPTER_METHODS_IMPL(CArrayAdapter, ArrayAdapter, ArrayAdapter);

ECode CArrayAdapter::constructor(
    /* [in] */ IContext* context,
    /* [in] */ Int32 textViewResourceId)
{
    return Init(context, textViewResourceId, 0);
}

ECode CArrayAdapter::constructor(
    /* [in] */ IContext* context,
    /* [in] */ Int32 resource,
    /* [in] */ Int32 textViewResourceId)
{
    return Init(context, resource, textViewResourceId);
}

ECode CArrayAdapter::constructor(
    /* [in] */ IContext* context,
    /* [in] */ Int32 textViewResourceId,
    /* [in] */ IObjectContainer* objects)
{
    return Init(context, textViewResourceId, 0 ,objects);
}

ECode CArrayAdapter::constructor(
    /* [in] */ IContext* context,
    /* [in] */ Int32 resource,
    /* [in] */ Int32 textViewResourceId,
    /* [in] */ IObjectContainer* objects)
{
    return Init(context, resource, textViewResourceId, objects);
}

ECode CArrayAdapter::Add(
    /* [in] */ IInterface* object)
{
    return ArrayAdapter::Add(object);
}

ECode CArrayAdapter::Insert(
    /* [in] */ IInterface* object,
    /* [in] */ Int32 index)
{
    return ArrayAdapter::Insert(object, index);
}

ECode CArrayAdapter::Remove(
    /* [in] */ IInterface* object)
{
    return ArrayAdapter::Remove(object);
}

ECode CArrayAdapter::Clear()
{
    return CArrayAdapter::Clear();
}

ECode CArrayAdapter::Sort(
    /* [in] */ IComparator* comparator)
{
    return ArrayAdapter::Sort(comparator);
}

ECode CArrayAdapter::SetNotifyOnChange(
    /* [in] */ Boolean notifyOnChange)
{
    return ArrayAdapter::SetNotifyOnChange(notifyOnChange);
}

ECode CArrayAdapter::GetContext(
    /* [out] */ IContext** context)
{
    VALIDATE_NOT_NULL(context);
    AutoPtr<IContext> temp = ArrayAdapter::GetContext();
    *context = temp;
    if (*context) {
        (*context)->AddRef();
    }

    return NOERROR;
}

ECode CArrayAdapter::GetPosition(
    /* [in] */ IInterface* item,
    /* [out] */ Int32* position)
{
    VALIDATE_NOT_NULL(position);
    *position = ArrayAdapter::GetPosition(item);

    return NOERROR;
}

ECode CArrayAdapter::SetDropDownViewResource(
    /* [in] */ Int32 resource)
{
    return ArrayAdapter::SetDropDownViewResource(resource);
}

ECode CArrayAdapter::GetDropDownView(
    /* [in] */ Int32 position,
    /* [in] */ IView* convertView,
    /* [in] */ IViewGroup* parent,
    /* [out] */ IView** view)
{
    VALIDATE_NOT_NULL(view);
    AutoPtr<IView> temp = ArrayAdapter::GetDropDownView(
        position, convertView, parent);
    *view = temp;
    if (*view) {
        (*view)->AddRef();
    }

    return NOERROR;
}

ECode CArrayAdapter::GetFilter(
    /* [out] */ IFilter** filter)
{
    VALIDATE_NOT_NULL(filter);
    AutoPtr<IFilter> temp = ArrayAdapter::GetFilter();
    *filter = temp;
    if (*filter) {
        (*filter)->AddRef();
    }

    return NOERROR;
}

/**
 * Creates a new ArrayAdapter from external resources. The content of the array is
 * obtained through {@link android.content.res.Resources#getTextArray(Int32)}.
 *
 * @param context The application's environment.
 * @param textArrayResId The identifier of the array to use as the data source.
 * @param textViewResId The identifier of the layout used to create views.
 *
 * @return An ArrayAdapter<CharSequence>.
 */
AutoPtr<IArrayAdapter> CArrayAdapter::CreateFromResource(
    /* [in] */ IContext* context,
    /* [in] */ Int32 textArrayResId,
    /* [in] */ Int32 textViewResId)
{
    AutoPtr<IResources> rs;
    context->GetResources((IResources**)&rs);
    ArrayOf<ICharSequence*>* strings;
    rs->GetTextArray(textArrayResId, &strings);

    AutoPtr<IObjectContainer> strs;
    CParcelableObjectContainer::New((IObjectContainer**)&strs);

    Int32 size = strings->GetLength();
    for (Int32 i=0; i<size; i++) {
        strs->Add((*strings)[i]);

        (*strings)[i]->Release();
    }
    ArrayOf<ICharSequence*>::Free(strings);

    AutoPtr<IArrayAdapter> adapter;
    CArrayAdapter::New(context, textViewResId, strs, (IArrayAdapter**)&adapter);

    return adapter;
}
