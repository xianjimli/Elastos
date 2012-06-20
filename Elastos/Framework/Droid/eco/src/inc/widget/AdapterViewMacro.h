
#ifndef __ADAPTERVIEWMACRO_H__
#define __ADAPTERVIEWMACRO_H__

#define IADAPTERVIEW_METHODS_DECL()                                     \
    CARAPI SetOnItemClickListener(                                      \
        /* [in] */ IOnItemClickListener* listener);                     \
                                                                        \
    CARAPI GetOnItemClickListener(                                      \
        /* [out] */ IOnItemClickListener** listener);                   \
                                                                        \
    CARAPI PerformItemClick(                                            \
        /* [in] */ IView* view,                                         \
        /* [in] */ Int32 position,                                      \
        /* [in] */ Int64 id,                                            \
        /* [out] */ Boolean* result);                                   \
                                                                        \
    CARAPI SetOnItemLongClickListener(                                  \
        /* [in] */ IOnItemLongClickListener* listener);                 \
                                                                        \
    CARAPI GetOnItemLongClickListener(                                  \
        /* [out] */ IOnItemLongClickListener** listener);               \
                                                                        \
    CARAPI SetOnItemSelectedListener(                                   \
        /* [in] */ IOnItemSelectedListener* listener);                  \
                                                                        \
    CARAPI GetOnItemSelectedListener(                                   \
        /* [out] */ IOnItemSelectedListener** listener);                \
                                                                        \
    CARAPI GetAdapter(                                                  \
        /* [out] */ IAdapter** adapter);                                \
                                                                        \
    CARAPI SetAdapter(                                                  \
        /* [in] */ IAdapter* adapter);                                  \
                                                                        \
    CARAPI GetSelectedItemPosition(                                     \
        /* [out] */ Int32* position);                                   \
                                                                        \
    CARAPI GetSelectedItemId(                                           \
        /* [out] */ Int64* itemId);                                     \
                                                                        \
    CARAPI GetSelectedView(                                             \
        /* [out] */ IView** view);                                      \
                                                                        \
    CARAPI GetSelectedItem(                                             \
        /* [out] */ IInterface** view);                                 \
                                                                        \
    CARAPI GetCount(                                                    \
        /* [out] */ Int32* count);                                      \
                                                                        \
    CARAPI GetPositionForView(                                          \
        /* [in] */ IView* view,                                         \
        /* [out] */ Int32* position);                                   \
                                                                        \
    CARAPI GetFirstVisiblePosition(                                     \
        /* [out] */ Int32* position);                                   \
                                                                        \
    CARAPI GetLastVisiblePosition(                                      \
        /* [out] */ Int32* position);                                   \
                                                                        \
    CARAPI SetSelection(                                                \
        /* [in] */ Int32 position);                                     \
                                                                        \
    CARAPI SetEmptyView(                                                \
        /* [in] */ IView* emptyView);                                   \
                                                                        \
    CARAPI GetEmptyView(                                                \
        /* [out] */ IView** view);                                      \
                                                                        \
    CARAPI IsInFilterMode(                                              \
        /* [out] */ Boolean* isInFilterMode);                           \
                                                                        \
    CARAPI CheckFocus();                                                \
                                                                        \
    CARAPI GetItemAtPosition(                                           \
        /* [in] */ Int32 position,                                      \
        /* [out] */ IInterface** item);                                 \
                                                                        \
    CARAPI GetItemIdAtPosition(                                         \
        /* [in] */ Int32 position,                                      \
        /* [out] */ Int64* itemId);

#define IADAPTERVIEW_METHODS_IMPL(className, superClass, overRideClass) \
ECode className::SetOnItemClickListener(                                \
    /* [in] */ IOnItemClickListener* listener)                          \
{                                                                       \
    return superClass::SetOnItemClickListener(listener);                \
}                                                                       \
                                                                        \
ECode className::GetOnItemClickListener(                                \
    /* [out] */ IOnItemClickListener** listener)                        \
{                                                                       \
    VALIDATE_NOT_NULL(listener);                                        \
    AutoPtr<IOnItemClickListener> temp = superClass::GetOnItemClickListener();\
    *listener = temp;                                                   \
    if (*listener) {                                                    \
        (*listener)->AddRef();                                          \
    }                                                                   \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::PerformItemClick(                                      \
    /* [in] */ IView* view,                                             \
    /* [in] */ Int32 position,                                          \
    /* [in] */ Int64 id,                                                \
    /* [out] */ Boolean* result)                                        \
{                                                                       \
    VALIDATE_NOT_NULL(result);                                          \
    *result = superClass::PerformItemClick(view, position, id);         \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::SetOnItemLongClickListener(                            \
    /* [in] */ IOnItemLongClickListener* listener)                      \
{                                                                       \
    return superClass::SetOnItemLongClickListener(listener);            \
}                                                                       \
                                                                        \
ECode className::GetOnItemLongClickListener(                            \
    /* [out] */ IOnItemLongClickListener ** listener)                   \
{                                                                       \
    VALIDATE_NOT_NULL(listener);                                        \
    AutoPtr<IOnItemLongClickListener> temp =                            \
        superClass::GetOnItemLongClickListener();                       \
    *listener = temp;                                                   \
    if (*listener) {                                                    \
        (*listener)->AddRef();                                          \
    }                                                                   \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::SetOnItemSelectedListener(                             \
    /* [in] */ IOnItemSelectedListener* listener)                       \
{                                                                       \
    return superClass::SetOnItemSelectedListener(listener);             \
}                                                                       \
                                                                        \
ECode className::GetOnItemSelectedListener(                             \
    /* [out] */ IOnItemSelectedListener ** listener)                    \
{                                                                       \
    VALIDATE_NOT_NULL(listener);                                        \
    AutoPtr<IOnItemSelectedListener> temp =                             \
        superClass::GetOnItemSelectedListener();                        \
    *listener = temp;                                                   \
    if (*listener) {                                                    \
        (*listener)->AddRef();                                          \
    }                                                                   \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::GetAdapter(                                            \
    /* [out] */ IAdapter** adapter)                                     \
{                                                                       \
    VALIDATE_NOT_NULL(adapter);                                         \
    AutoPtr<IAdapter> temp = superClass::GetAdapter();                  \
    *adapter = temp;                                                    \
    if (*adapter) {                                                     \
        (*adapter)->AddRef();                                           \
    }                                                                   \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::SetAdapter(                                            \
    /* [in] */ IAdapter* adapter)                                       \
{                                                                       \
    return superClass::SetAdapter(adapter);                             \
}                                                                       \
                                                                        \
ECode className::GetSelectedItemPosition(                               \
    /* [out] */ Int32* position)                                        \
{                                                                       \
    VALIDATE_NOT_NULL(position);                                        \
    *position = superClass::GetSelectedItemPosition();                  \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::GetSelectedItemId(                                     \
    /* [out] */ Int64* itemId)                                          \
{                                                                       \
    VALIDATE_NOT_NULL(itemId);                                          \
    *itemId = superClass::GetSelectedItemId();                          \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::GetSelectedView(                                       \
    /* [out] */ IView** view)                                           \
{                                                                       \
    VALIDATE_NOT_NULL(view);                                            \
    AutoPtr<IView> temp = superClass::GetSelectedView();                \
    *view = temp;                                                       \
    if (*view) {                                                        \
        (*view)->AddRef();                                              \
    }                                                                   \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::GetSelectedItem(                                       \
    /* [out] */ IInterface** view)                                      \
{                                                                       \
    VALIDATE_NOT_NULL(view);                                            \
    AutoPtr<IInterface> temp = superClass::GetSelectedView();           \
    *view = temp;                                                       \
    if (*view) {                                                        \
        (*view)->AddRef();                                              \
    }                                                                   \
                                                                        \
    return NOERROR;                                                     \
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
ECode className::GetPositionForView(                                    \
    /* [in] */ IView* view,                                             \
    /* [out] */ Int32* position)                                        \
{                                                                       \
    VALIDATE_NOT_NULL(position);                                        \
    *position = superClass::GetPositionForView(view);                   \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::GetFirstVisiblePosition(                               \
    /* [out] */ Int32* position)                                        \
{                                                                       \
    VALIDATE_NOT_NULL(position);                                        \
    *position = superClass::GetFirstVisiblePosition();                  \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::GetLastVisiblePosition(                                \
    /* [out] */ Int32* position)                                        \
{                                                                       \
    VALIDATE_NOT_NULL(position);                                        \
    *position = superClass::GetLastVisiblePosition();                   \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::SetSelection(                                          \
    /* [in] */ Int32 position)                                          \
{                                                                       \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::SetEmptyView(                                          \
    /* [in] */ IView* emptyView)                                        \
{                                                                       \
    return superClass::SetEmptyView(emptyView);                         \
}                                                                       \
                                                                        \
ECode className::GetEmptyView(                                          \
    /* [out] */ IView** view)                                           \
{                                                                       \
    VALIDATE_NOT_NULL(view);                                            \
    AutoPtr<IView> temp = superClass::GetEmptyView();                   \
    *view = temp;                                                       \
    if (*view) {                                                        \
        (*view)->AddRef();                                              \
    }                                                                   \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::IsInFilterMode(                                        \
    /* [out] */ Boolean* isInFilterMode)                                \
{                                                                       \
    VALIDATE_NOT_NULL(isInFilterMode);                                  \
    *isInFilterMode = superClass::IsInFilterMode();                     \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::CheckFocus()                                           \
{                                                                       \
    return superClass::CheckFocus();                                    \
}                                                                       \
                                                                        \
ECode className::GetItemAtPosition(                                     \
    /* [in] */ Int32 position,                                          \
    /* [out] */ IInterface** item)                                      \
{                                                                       \
    VALIDATE_NOT_NULL(item);                                            \
    AutoPtr<IInterface> temp = superClass::GetItemAtPosition(position); \
    *item = temp;                                                       \
    if (*item) {                                                        \
        (*item)->AddRef();                                              \
    }                                                                   \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::GetItemIdAtPosition(                                   \
    /* [in] */ Int32 position,                                          \
    /* [out] */ Int64* itemId)                                          \
{                                                                       \
    VALIDATE_NOT_NULL(itemId);                                          \
    *itemId = superClass::GetItemIdAtPosition(position);                \
                                                                        \
    return NOERROR;                                                     \
}

#endif  //__ADAPTERVIEWMACRO_H__
