
#ifndef __MENUBUILDERMACRO_H__
#define __MENUBUILDERMACRO_H__


#define IMENUBUILDER_METHODS_DECL()                                              \
    CARAPI SetCallback(                                                          \
        /* [in] */ IMenuBuilderCallback* callback);                              \
                                                                                 \
    CARAPI GetMenuView(                                                          \
        /* [in] */ Int32 menuType,                                               \
        /* [in] */ IViewGroup* parent,                                           \
        /* [out] */ IView** menuView);                                           \
                                                                                 \
    CARAPI ClearMenuViews();                                                     \
                                                                                 \
    CARAPI ClearAll();                                                           \
                                                                                 \
    CARAPI FindItemIndex(                                                        \
        /* [in] */ Int32 id,                                                     \
        /* [out] */ Int32* index);                                               \
                                                                                 \
    CARAPI FindGroupIndex(                                                       \
        /* [in] */ Int32 group,                                                  \
        /* [out] */ Int32* index);                                               \
                                                                                 \
    CARAPI FindGroupIndexEx(                                                     \
        /* [in] */ Int32 group,                                                  \
        /* [in] */ Int32 start,                                                  \
        /* [out] */ Int32* index);                                               \
                                                                                 \
    CARAPI SetShortcutsVisible(                                                  \
        /* [in] */ Boolean shortcutsVisible);                                    \
                                                                                 \
    CARAPI IsShortcutsVisible(                                                   \
        /* [out] */ Boolean* qwertyMode);                                        \
                                                                                 \
     CARAPI GetCallback(                                                         \
        /* [out] */ IMenuBuilderCallback** callback);                            \
                                                                                 \
    CARAPI GetContext(                                                           \
        /* [out] */ IContext** context);                                         \
                                                                                 \
    CARAPI PerformItemAction(                                                    \
        /* [in] */ IMenuItem* item,                                              \
        /* [in] */ Int32 flags,                                                  \
        /* [out] */ Boolean* state);                                             \
                                                                                 \
    CARAPI CloseEx(                                                              \
        /* [in] */ Boolean allMenusAreClosing);                                  \
                                                                                 \
    CARAPI GetHeaderTitle(                                                       \
        /* [out] */ ICharSequence** title);                                      \
                                                                                 \
    CARAPI GetHeaderIcon(                                                        \
        /* [out] */ IDrawable** drawable);                                       \
                                                                                 \
    CARAPI GetHeaderView(                                                        \
        /* [out] */ IView** headerView);                                         \
                                                                                 \
    CARAPI GetRootMenu(                                                          \
        /* [out] */ IMenuBuilder** menu);                                        \
                                                                                 \
    CARAPI SetCurrentMenuInfo(                                                   \
        /* [in] */ IContextMenuInfo* menuInfo);                                  \
                                                                                 \
    CARAPI GetMenuAdapter(                                                       \
        /* [in] */ Int32 menuType,                                               \
        /* [out] */ IMenuAdapter** menuAdapter);                                 \
                                                                                 \
    CARAPI SaveHierarchyState(                                                   \
        /* [in] */ IBundle* outState);                                           \
                                                                                 \
    CARAPI RestoreHierarchyState(                                                \
        /* [in] */ IBundle* inState);                                            \


#define IMENUBUILDER_METHODS_IMPL(className, superClass, overRideClass)          \
ECode className::SetCallback(                                                    \
    /* [in] */ IMenuBuilderCallback* callback)                                   \
{                                                                                \
    return superClass::SetCallback(callback);                                    \
}                                                                                \
                                                                                 \
ECode className::GetMenuView(                                                    \
    /* [in] */ Int32 menuType,                                                   \
    /* [in] */ IViewGroup* parent,                                               \
    /* [out] */ IView** menuView)                                                \
{                                                                                \
    VALIDATE_NOT_NULL(menuView);                                                 \
                                                                                 \
    AutoPtr<IView> view = superClass::superClass::GetMenuView(menuType, parent); \
    *menuView = view.Get();                                                      \
    if (*menuView != NULL) (*menuView)->AddRef();                                \
    return NOERROR;                                                              \
}                                                                                \
                                                                                 \
ECode className::ClearMenuViews()                                                \
{                                                                                \
    return superClass::ClearMenuViews();                                         \
}                                                                                \
                                                                                 \
ECode className::ClearAll()                                                      \
{                                                                                \
    return superClass::ClearAll();                                               \
}                                                                                \
                                                                                 \
ECode className::FindItemIndex(                                                  \
    /* [in] */ Int32 id,                                                         \
    /* [out] */ Int32* index)                                                    \
{                                                                                \
    VALIDATE_NOT_NULL(index);                                                    \
                                                                                 \
    *index = superClass::FindItemIndex(id);                                      \
    return NOERROR;                                                              \
}                                                                                \
                                                                                 \
ECode className::FindGroupIndex(                                                 \
    /* [in] */ Int32 group,                                                      \
    /* [out] */ Int32* index)                                                    \
{                                                                                \
    VALIDATE_NOT_NULL(index);                                                    \
                                                                                 \
    *index = superClass::FindGroupIndex(group);                                  \
    return NOERROR;                                                              \
}                                                                                \
                                                                                 \
ECode className::FindGroupIndexEx(                                               \
    /* [in] */ Int32 group,                                                      \
    /* [in] */ Int32 start,                                                      \
    /* [out] */ Int32* index)                                                    \
{                                                                                \
    VALIDATE_NOT_NULL(index);                                                    \
                                                                                 \
    *index = superClass::FindGroupIndex(group, start);                           \
    return NOERROR;                                                              \
}                                                                                \
                                                                                 \
ECode className::SetShortcutsVisible(                                            \
    /* [in] */ Boolean shortcutsVisible)                                         \
{                                                                                \
    return superClass::SetShortcutsVisible(shortcutsVisible);                    \
}                                                                                \
                                                                                 \
ECode className::IsShortcutsVisible(                                             \
    /* [out] */ Boolean* qwertyMode)                                             \
{                                                                                \
    VALIDATE_NOT_NULL(qwertyMode);                                               \
                                                                                 \
    *qwertyMode = superClass::IsShortcutsVisible();                              \
    return NOERROR;                                                              \
}                                                                                \
                                                                                 \
ECode className::GetCallback(                                                    \
    /* [out] */ IMenuBuilderCallback** callback)                                 \
{                                                                                \
    VALIDATE_NOT_NULL(callback);                                                 \
                                                                                 \
    AutoPtr<IMenuBuilderCallback> _callback = superClass::GetCallback();         \
    *callback = _callback.Get();                                                 \
    if (*callback != NULL) (*callback)->AddRef();                                \
    return NOERROR;                                                              \
}                                                                                \
                                                                                 \
ECode className::GetContext(                                                     \
    /* [out] */ IContext** context)                                              \
{                                                                                \
    VALIDATE_NOT_NULL(context);                                                  \
                                                                                 \
    AutoPtr<IContext> _context = superClass::GetContext();                       \
    *context = _context.Get();                                                   \
    if (*context != NULL) (*context)->AddRef();                                  \
    return NOERROR;                                                              \
}                                                                                \
                                                                                 \
ECode className::PerformItemAction(                                              \
    /* [in] */ IMenuItem* item,                                                  \
    /* [in] */ Int32 flags,                                                      \
    /* [out] */ Boolean* state)                                                  \
{                                                                                \
    VALIDATE_NOT_NULL(state);                                                    \
                                                                                 \
    *state = superClass::PerformItemAction(item, flags);                         \
    return NOERROR;                                                              \
}                                                                                \
                                                                                 \
ECode className::CloseEx(                                                        \
    /* [in] */ Boolean allMenusAreClosing)                                       \
{                                                                                \
    return superClass::Close(allMenusAreClosing);                                \
}                                                                                \
                                                                                 \
ECode className::GetHeaderTitle(                                                 \
    /* [out] */ ICharSequence** title)                                           \
{                                                                                \
    VALIDATE_NOT_NULL(title);                                                    \
                                                                                 \
    AutoPtr<ICharSequence> t = superClass::GetHeaderTitle();                     \
    *title = t.Get();                                                            \
    if (*title != NULL) (*title)->AddRef();                                      \
    return NOERROR;                                                              \
}                                                                                \
                                                                                 \
ECode className::GetHeaderIcon(                                                  \
    /* [out] */ IDrawable** drawable)                                            \
{                                                                                \
    VALIDATE_NOT_NULL(drawable);                                                 \
                                                                                 \
    AutoPtr<IDrawable> d = superClass::GetHeaderIcon();                          \
    *drawable = d.Get();                                                         \
    if (*drawable != NULL) (*drawable)->AddRef();                                \
    return NOERROR;                                                              \
}                                                                                \
                                                                                 \
ECode className::GetHeaderView(                                                  \
    /* [out] */ IView** view)                                                    \
{                                                                                \
    VALIDATE_NOT_NULL(view);                                                     \
                                                                                 \
    AutoPtr<IView> _view = superClass::GetHeaderView();                          \
    *view = _view.Get();                                                         \
    if (*view != NULL) (*view)->AddRef();                                        \
    return NOERROR;                                                              \
}                                                                                \
                                                                                 \
ECode className::GetRootMenu(                                                    \
    /* [out] */ IMenuBuilder** menu)                                             \
{                                                                                \
    VALIDATE_NOT_NULL(menu);                                                     \
                                                                                 \
    AutoPtr<IMenuBuilder> _menu = superClass::GetRootMenu();                     \
    *menu = _menu.Get();                                                         \
    if (*menu != NULL) (*menu)->AddRef();                                        \
    return NOERROR;                                                              \
}                                                                                \
                                                                                 \
ECode className::SetCurrentMenuInfo(                                             \
    /* [in] */ IContextMenuInfo* menuInfo)                                       \
{                                                                                \
    return superClass::SetCurrentMenuInfo(menuInfo);                             \
}                                                                                \
                                                                                 \
ECode className::GetMenuAdapter(                                                 \
    /* [in] */ Int32 menuType,                                                   \
    /* [out] */ IMenuAdapter** menuAdapter)                                      \
{                                                                                \
    VALIDATE_NOT_NULL(menuAdapter);                                              \
                                                                                 \
    AutoPtr<IMenuAdapter> adapter = superClass::GetMenuAdapter(menuType);        \
    *menuAdapter = adapter.Get();                                                \
    if (*menuAdapter != NULL) (*menuAdapter)->AddRef();                          \
    return NOERROR;                                                              \
}                                                                                \
                                                                                 \
ECode className::SaveHierarchyState(                                             \
    /* [in] */ IBundle* outState)                                                \
{                                                                                \
    return superClass::SaveHierarchyState(outState);                             \
}                                                                                \
                                                                                 \
ECode className::RestoreHierarchyState(                                          \
    /* [in] */ IBundle* inState)                                                 \
{                                                                                \
    return superClass::RestoreHierarchyState(inState);                           \
}                                                                                \

#endif //__MENUBUILDERMACRO_H__