
#include "widget/CMenuAdapter.h"

#include <elastos/AutoPtr.h>
#include <elastos/List.h>
#include "view/menu/CMenuBuilder.h"

ECode CMenuAdapter::GetCount(
    /* [out] */ Int32* count) {
    *count = 0;
    if (m_cmb != NULL) {
        AutoPtr<IObjectContainer> list;
        CObjectContainer::New((IObjectContainer**)&list);
        m_cmb->GetVisibleItems((IObjectContainer**) &list);

        Int32 offset = 0, objCount = 0;
        GetOffset(&offset);
        list->GetObjectCount(&objCount);
        *count = objCount - offset;
    }

    return NOERROR;
}

ECode CMenuAdapter::GetItem(
    /* [in] */ Int32 position,
    /* [out] */ IInterface** i) {
    if (m_cmb != NULL) {
        Int32 offset;
        GetOffset(&offset);

        Int32 pos = position + offset;
        AutoPtr<IObjectContainer> visibleItems;
        CObjectContainer::New((IObjectContainer**)&visibleItems);
        m_cmb->GetVisibleItems((IObjectContainer**) &visibleItems);

        Int32 objCount = 0, index = 0;
        visibleItems->GetObjectCount(&objCount);

        if (visibleItems != NULL && objCount > pos) {
            AutoPtr<IObjectEnumerator> itemIt;
            visibleItems->GetObjectEnumerator((IObjectEnumerator**)&itemIt);
            Boolean hasNext;
            while(itemIt->MoveNext(&hasNext), hasNext) {
                AutoPtr<IMenuItem> item;
                itemIt->Current((IInterface**)&item);
                if (index == pos) {
                    *i = item;
                    break;
                }

                index++;
            }

            return NOERROR;
        }
    }

    *i = AutoPtr<IInterface>(NULL);
    if (*i != NULL) {
        (*i)->AddRef();
    }

    return NOERROR;
}

ECode CMenuAdapter::GetView(
    /* [in] */ Int32 position,
    /* [in] */ IView* convertView,
    /* [in] */ IViewGroup* parent,
    /* [out] */ IView** view) {
    AutoPtr<IMenuItem> menuItem;
    GetItem(position, (IInterface**) &menuItem);

    return menuItem->GetItemView(mMenuType, parent, view);
}

ECode CMenuAdapter::GetOffset(
    /* [out] */ Int32* offset) {

    *offset = 0;
    if (mMenuType == CMenuBuilder::TYPE_EXPANDED) {
        if (m_cmb != NULL) {
            return m_cmb->GetNumIconMenuItemsShown(offset);
        }
    }

    return NOERROR;
}


ECode CMenuAdapter::RegisterDataSetObserver(
    /* [in] */ IDataSetObserver* observer) {
    return E_NOT_IMPLEMENTED;
}

ECode CMenuAdapter::UnregisterDataSetObserver(
    /* [in] */ IDataSetObserver* observer) {
    return E_NOT_IMPLEMENTED;
}

ECode CMenuAdapter::HasStableIds(
    /* [out] */ Boolean* has) {
    return E_NOT_IMPLEMENTED;
}

ECode CMenuAdapter::GetItemViewType(
    /* [in] */ Int32 position,
    /* [out] */ Int32* viewType) {
    return E_NOT_IMPLEMENTED;
}

ECode CMenuAdapter::GetViewTypeCount(
    /* [out] */ Int32* count) {
    return E_NOT_IMPLEMENTED;
}

ECode CMenuAdapter::IsEmpty(
    /* [out] */ Boolean* empty) {
    return E_NOT_IMPLEMENTED;
}

ECode CMenuAdapter::AreAllItemsEnabled(
    /* [out] */ Boolean* enabled) {
    return E_NOT_IMPLEMENTED;
}

ECode CMenuAdapter::IsEnabled(
    /* [in] */ Int32 position,
    /* [out] */ Boolean* enabled) {
    return E_NOT_IMPLEMENTED;
}

ECode CMenuAdapter::NotifyDataSetChanged() {
    return E_NOT_IMPLEMENTED;
}

ECode CMenuAdapter::NotifyDataSetInvalidated() {
    return E_NOT_IMPLEMENTED;
}
