
#ifndef __LISTVIEWMACRO_H__
#define __LISTVIEWMACRO_H__

#define ILISTVIEW_METHODS_DECL()                                        \
    CARAPI GetMaxScrollAmount(                                          \
        /* [out] */ Int32* amount);                                     \
                                                                        \
    CARAPI AddHeaderView(                                               \
        /* [in] */ IView* v,                                            \
        /* [in] */ IInterface* data,                                    \
        /* [in] */ Boolean isSelectable);                               \
                                                                        \
    CARAPI AddHeaderViewEx(                                             \
        /* [in] */ IView* v);                                           \
                                                                        \
    CARAPI RemoveHeaderView(                                            \
        /* [in] */ IView* v,                                            \
        /* [out] */ Boolean* result);                                   \
                                                                        \
    CARAPI AddFooterView(                                               \
        /* [in] */ IView* v,                                            \
        /* [in] */ IInterface* data,                                    \
        /* [in] */ Boolean isSelectable);                               \
                                                                        \
    CARAPI AddFooterViewEx(                                             \
        /* [in] */ IView* v);                                           \
                                                                        \
    CARAPI RemoveFooterView(                                            \
        /* [in] */ IView* v,                                            \
        /* [out] */ Boolean* result);                                   \
                                                                        \
    CARAPI SetSelectionFromTop(                                         \
        /* [in] */ Int32 position,                                      \
        /* [in] */ Int32 y);                                            \
                                                                        \
    CARAPI SetSelectionAfterHeaderView();                               \
                                                                        \
    CARAPI SetItemsCanFocus(                                            \
        /* [in] */ Boolean itemsCanFocus);                              \
                                                                        \
    CARAPI GetItemsCanFocus(                                            \
        /* [out] */ Boolean* canFocus);                                 \
                                                                        \
    CARAPI GetDivider(                                                  \
        /* [out] */ IDrawable** divider);                               \
                                                                        \
    CARAPI SetDivider(                                                  \
        /* [in] */ IDrawable* divider);                                 \
                                                                        \
    CARAPI GetDividerHeight(                                            \
        /* [out] */ Int32* height);                                     \
                                                                        \
    CARAPI SetDividerHeight(                                            \
        /* [in] */ Int32 height);                                       \
                                                                        \
    CARAPI SetHeaderDividersEnabled(                                    \
        /* [in] */ Boolean headerDividersEnabled);                      \
                                                                        \
    CARAPI SetFooterDividersEnabled(                                    \
        /* [in] */ Boolean footerDividersEnabled);                      \
                                                                        \
    CARAPI SetOverscrollHeader(                                         \
        /* [in] */ IDrawable* header);                                  \
                                                                        \
    CARAPI GetOverscrollHeader(                                         \
        /* [out] */ IDrawable** overScrollHeader);                      \
                                                                        \
    CARAPI SetOverscrollFooter(                                         \
        /* [in] */ IDrawable* footer);                                  \
                                                                        \
    CARAPI GetOverscrollFooter(                                         \
        /* [out] */ IDrawable** overScrollFooter);                      \
                                                                        \
    CARAPI GetChoiceMode(                                               \
        /* [out] */ Int32* choiceMode);                                 \
                                                                        \
    CARAPI SetChoiceMode(                                               \
        /* [in] */ Int32 choiceMode);                                   \
                                                                        \
    CARAPI SetItemChecked(                                              \
        /* [in] */ Int32 position,                                      \
        /* [in] */ Boolean value);                                      \
                                                                        \
    CARAPI IsItemChecked(                                               \
        /* [in] */ Int32 position,                                      \
        /* [out] */ Boolean* checked);                                  \
                                                                        \
    CARAPI GetCheckedItemPosition(                                      \
        /* [out] */ Int32* position);                                   \
                                                                        \
    /*SparseBooleanArray GetCheckedItemPositions();*/                   \
                                                                        \
    CARAPI GetCheckedItemIds(                                           \
        /* [out, callee] */ ArrayOf<Int64>** ids);                      \
                                                                        \
    CARAPI ClearChoices();

#define ILISTVIEW_METHODS_IMPL(className, superClass, overRideClass)    \
ECode className::GetMaxScrollAmount(                                    \
    /* [out] */ Int32* amount)                                          \
{                                                                       \
    VALIDATE_NOT_NULL(amount);                                          \
    *amount = superClass::GetMaxScrollAmount();                         \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::AddHeaderView(                                         \
    /* [in] */ IView* v,                                                \
    /* [in] */ IInterface* data,                                        \
    /* [in] */ Boolean isSelectable)                                    \
{                                                                       \
    return superClass::AddHeaderView(v, data, isSelectable);            \
}                                                                       \
                                                                        \
ECode className::AddHeaderViewEx(                                       \
    /* [in] */ IView* v)                                                \
{                                                                       \
    return superClass::AddHeaderView(v);                                \
}                                                                       \
                                                                        \
ECode className::RemoveHeaderView(                                      \
    /* [in] */ IView* v,                                                \
    /* [out] */ Boolean* result)                                        \
{                                                                       \
    VALIDATE_NOT_NULL(result);                                          \
    *result = superClass::RemoveHeaderView(v);                          \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::AddFooterView(                                         \
    /* [in] */ IView* v,                                                \
    /* [in] */ IInterface* data,                                        \
    /* [in] */ Boolean isSelectable)                                    \
{                                                                       \
    return superClass::AddFooterView(v, data, isSelectable);            \
}                                                                       \
                                                                        \
ECode className::AddFooterViewEx(                                       \
    /* [in] */ IView* v)                                                \
{                                                                       \
    return superClass::AddFooterView(v);                                \
}                                                                       \
                                                                        \
ECode className::RemoveFooterView(                                      \
    /* [in] */ IView* v,                                                \
    /* [out] */ Boolean* result)                                        \
{                                                                       \
    VALIDATE_NOT_NULL(result);                                          \
    *result = superClass::RemoveFooterView(v);                          \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::SetSelectionFromTop(                                   \
    /* [in] */ Int32 position,                                          \
    /* [in] */ Int32 y)                                                 \
{                                                                       \
    return superClass::SetSelectionFromTop(position, y);                \
}                                                                       \
                                                                        \
ECode className::SetSelectionAfterHeaderView()                          \
{                                                                       \
    return superClass::SetSelectionAfterHeaderView();                   \
}                                                                       \
                                                                        \
ECode className::SetItemsCanFocus(                                      \
    /* [in] */ Boolean itemsCanFocus)                                   \
{                                                                       \
    return superClass::SetItemsCanFocus(itemsCanFocus);                 \
}                                                                       \
                                                                        \
ECode className::GetItemsCanFocus(                                      \
    /* [out] */ Boolean* canFocus)                                      \
{                                                                       \
    VALIDATE_NOT_NULL(canFocus);                                        \
    *canFocus = superClass::GetItemsCanFocus();                         \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::GetDivider(                                            \
    /* [out] */ IDrawable** divider)                                    \
{                                                                       \
    VALIDATE_NOT_NULL(divider);                                         \
    AutoPtr<IDrawable> temp = superClass::GetDivider();                 \
    *divider = temp;                                                    \
    if (*divider) {                                                     \
        (*divider)->AddRef();                                           \
    }                                                                   \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::SetDivider(                                            \
    /* [in] */ IDrawable* divider)                                      \
{                                                                       \
    return superClass::SetDivider(divider);                             \
}                                                                       \
                                                                        \
ECode className::GetDividerHeight(                                      \
    /* [out] */ Int32* height)                                          \
{                                                                       \
    VALIDATE_NOT_NULL(height);                                          \
    *height = superClass::GetDividerHeight();                           \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::SetDividerHeight(                                      \
    /* [in] */ Int32 height)                                            \
{                                                                       \
    return superClass::SetDividerHeight(height);                        \
}                                                                       \
                                                                        \
ECode className::SetHeaderDividersEnabled(                              \
    /* [in] */ Boolean headerDividersEnabled)                           \
{                                                                       \
    return superClass::SetHeaderDividersEnabled(headerDividersEnabled); \
}                                                                       \
                                                                        \
ECode className::SetFooterDividersEnabled(                              \
    /* [in] */ Boolean footerDividersEnabled)                           \
{                                                                       \
    return superClass::SetFooterDividersEnabled(footerDividersEnabled); \
}                                                                       \
                                                                        \
ECode className::SetOverscrollHeader(                                   \
    /* [in] */ IDrawable* header)                                       \
{                                                                       \
    return superClass::SetOverscrollHeader(header);                     \
}                                                                       \
                                                                        \
ECode className::GetOverscrollHeader(                                   \
    /* [out] */ IDrawable** overScrollHeader)                           \
{                                                                       \
    VALIDATE_NOT_NULL(overScrollHeader);                                \
    AutoPtr<IDrawable> temp = superClass::GetOverscrollHeader();        \
    *overScrollHeader = temp;                                           \
    if (*overScrollHeader) {                                            \
        (*overScrollHeader)->AddRef();                                  \
    }                                                                   \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::SetOverscrollFooter(                                   \
    /* [in] */ IDrawable* footer)                                       \
{                                                                       \
    return superClass::SetOverscrollFooter(footer);                     \
}                                                                       \
                                                                        \
ECode className::GetOverscrollFooter(                                   \
    /* [out] */ IDrawable** overScrollFooter)                           \
{                                                                       \
    VALIDATE_NOT_NULL(overScrollFooter);                                \
    AutoPtr<IDrawable> temp = superClass::GetOverscrollFooter();        \
    *overScrollFooter = temp;                                           \
    if (*overScrollFooter) {                                            \
        (*overScrollFooter)->AddRef();                                  \
    }                                                                   \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::GetChoiceMode(                                         \
    /* [out] */ Int32* choiceMode)                                      \
{                                                                       \
    VALIDATE_NOT_NULL(choiceMode);                                      \
    *choiceMode = superClass::GetChoiceMode();                          \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::SetChoiceMode(                                         \
    /* [in] */ Int32 choiceMode)                                        \
{                                                                       \
    return superClass::SetChoiceMode(choiceMode);                       \
}                                                                       \
                                                                        \
ECode className::SetItemChecked(                                        \
    /* [in] */ Int32 position,                                          \
    /* [in] */ Boolean value)                                           \
{                                                                       \
    return superClass::SetItemChecked(position, value);                 \
}                                                                       \
                                                                        \
ECode className::IsItemChecked(                                         \
    /* [in] */ Int32 position,                                          \
    /* [out] */ Boolean* checked)                                       \
{                                                                       \
    VALIDATE_NOT_NULL(checked);                                         \
    *checked = superClass::IsItemChecked(position);                     \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::GetCheckedItemPosition(                                \
    /* [out] */ Int32* position)                                        \
{                                                                       \
    VALIDATE_NOT_NULL(position);                                        \
    *position = superClass::GetCheckedItemPosition();                   \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
/*SparseBooleanArray GetCheckedItemPositions();*/                       \
                                                                        \
ECode className::GetCheckedItemIds(                                     \
    /* [out, callee] */ ArrayOf<Int64>** ids)                           \
{                                                                       \
    VALIDATE_NOT_NULL(ids);                                             \
    return superClass::GetCheckedItemIds(ids);                          \
}                                                                       \
                                                                        \
ECode className::ClearChoices()                                         \
{                                                                       \
    return superClass::ClearChoices();                                  \
}

#endif  //__LISTVIEWMACRO_H__
