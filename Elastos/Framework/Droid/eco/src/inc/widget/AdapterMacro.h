
#ifndef __ADAPTERMACRO_H__
#define __ADAPTERMACRO_H__

#define IADAPTER_METHODS_DECL()                                         \
    CARAPI RegisterDataSetObserver(                                     \
        /* [in] */ IDataSetObserver* observer);                         \
                                                                        \
    CARAPI UnregisterDataSetObserver(                                   \
        /* [in] */ IDataSetObserver* observer);                         \
                                                                        \
    CARAPI GetCount(                                                    \
        /* [out] */ Int32* count);                                      \
                                                                        \
    CARAPI GetItem(                                                     \
        /* [in] */ Int32 position,                                      \
        /* [out] */ IInterface** item);                                 \
                                                                        \
    CARAPI GetItemId(                                                   \
        /* [in] */ Int32 position,                                      \
        /* [out] */ Int64* itemId);                                     \
                                                                        \
    CARAPI HasStableIds(                                                \
        /* [out] */ Boolean* hasStableIds);                             \
                                                                        \
    CARAPI GetView(                                                     \
        /* [in] */ Int32 position,                                      \
        /* [in] */ IView* convertView,                                  \
        /* [in] */ IViewGroup* parent,                                  \
        /* [out] */ IView** view);                                      \
                                                                        \
    CARAPI GetItemViewType(                                             \
        /* [in] */ Int32 position,                                      \
        /* [out] */ Int32* viewType);                                   \
                                                                        \
    CARAPI GetViewTypeCount(                                            \
        /* [out] */ Int32* count);                                      \
                                                                        \
    CARAPI IsEmpty(                                                     \
        /* [out] */ Boolean* isEmpty);

#define IADAPTER_METHODS_IMPL(className, superClass, overRideClass)     \
ECode className::RegisterDataSetObserver(                               \
    /* [in] */ IDataSetObserver* observer)                              \
{                                                                       \
    return superClass::RegisterDataSetObserver(observer);               \
}                                                                       \
                                                                        \
ECode className::UnregisterDataSetObserver(                             \
    /* [in] */ IDataSetObserver* observer)                              \
{                                                                       \
    return superClass::UnregisterDataSetObserver(observer);             \
}                                                                       \
                                                                        \
ECode className::GetCount(                                              \
    /* [out] */ Int32* count)                                           \
{                                                                       \
    VALIDATE_NOT_NULL(count);                                           \
    *count = superClass::GetCount();                                    \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::GetItem(                                               \
    /* [in] */ Int32 position,                                          \
    /* [out] */ IInterface** item)                                      \
{                                                                       \
    VALIDATE_NOT_NULL(item);                                            \
    AutoPtr<IInterface> temp = superClass::GetItem(position);           \
    *item = temp;                                                       \
    if (*item) {                                                        \
        (*item)->AddRef();                                              \
    }                                                                   \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::GetItemId(                                             \
    /* [in] */ Int32 position,                                          \
    /* [out] */ Int64* itemId)                                          \
{                                                                       \
    VALIDATE_NOT_NULL(itemId);                                          \
    *itemId = superClass::GetItemId(position);                          \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::HasStableIds(                                          \
    /* [out] */ Boolean* hasStableIds)                                  \
{                                                                       \
    VALIDATE_NOT_NULL(hasStableIds);                                    \
    *hasStableIds = superClass::HasStableIds();                         \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::GetView(                                               \
    /* [in] */ Int32 position,                                          \
    /* [in] */ IView* convertView,                                      \
    /* [in] */ IViewGroup* parent,                                      \
    /* [out] */ IView** view)                                           \
{                                                                       \
    VALIDATE_NOT_NULL(view);                                            \
    AutoPtr<IView> temp = superClass::GetView(position, convertView, parent);\
    *view = temp;                                                       \
    if (*view) {                                                        \
        (*view)->AddRef();                                              \
    }                                                                   \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::GetItemViewType(                                       \
    /* [in] */ Int32 position,                                          \
    /* [out] */ Int32* viewType)                                        \
{                                                                       \
    VALIDATE_NOT_NULL(viewType);                                        \
    *viewType = superClass::GetItemViewType(position);                  \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::GetViewTypeCount(                                      \
    /* [out] */ Int32* count)                                           \
{                                                                       \
    VALIDATE_NOT_NULL(count);                                           \
    *count = superClass::GetViewTypeCount();                            \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::IsEmpty(                                               \
    /* [out] */ Boolean* isEmpty)                                       \
{                                                                       \
    VALIDATE_NOT_NULL(isEmpty);                                         \
    *isEmpty = superClass::IsEmpty();                                   \
                                                                        \
    return NOERROR;                                                     \
}

#define ILISTADAPTER_METHODS_DECL()                                     \
    CARAPI AreAllItemsEnabled(                                          \
        /* [out] */ Boolean* enabled);                                  \
                                                                        \
    CARAPI IsEnabled(                                                   \
        /* [in] */ Int32 position,                                      \
        /* [out] */ Boolean* enabled);

#define IListADAPTER_METHODS_IMPL(className, superClass, overRideClass) \
ECode className::AreAllItemsEnabled(                                    \
    /* [out] */ Boolean* enabled)                                       \
{                                                                       \
    VALIDATE_NOT_NULL(enabled);                                         \
    *enabled = superClass::AreAllItemsEnabled();                        \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::IsEnabled(                                             \
    /* [in] */ Int32 position,                                          \
    /* [out] */ Boolean* enabled)                                       \
{                                                                       \
    VALIDATE_NOT_NULL(enabled);                                         \
    *enabled = superClass::IsEnabled(position);                         \
                                                                        \
    return NOERROR;                                                     \
}

#define IBASEADAPTER_METHODS_DECL()                                     \
    CARAPI NotifyDataSetChanged();                                      \
                                                                        \
    CARAPI NotifyDataSetInvalidated();

#define IBASEADAPTER_METHODS_IMPL(className, superClass, overRideClass) \
ECode className::NotifyDataSetChanged()                                 \
{                                                                       \
    return superClass::NotifyDataSetChanged();                          \
}                                                                       \
                                                                        \
ECode className::NotifyDataSetInvalidated()                             \
{                                                                       \
    return superClass::NotifyDataSetInvalidated();                      \
}


#endif //__ADAPTERMACRO_H__
