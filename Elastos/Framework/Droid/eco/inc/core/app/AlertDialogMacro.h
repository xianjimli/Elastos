
#ifndef __ALERTDIALOGMACRO_H__
#define __ALERTDIALOGMACRO_H__

#define IALERTDIALOG_METHODS_DECL()                                         \
    CARAPI GetButton(                                                       \
        /* [in] */ Int32 whichButton,                                       \
        /* [out] */ IButton** button);                                      \
                                                                            \
    CARAPI GetListView(                                                     \
        /* [out] */ IListView** listView);                                  \
                                                                            \
    CARAPI SetCustomTitle(                                                  \
        /* [in] */ IView* customTitleView);                                 \
                                                                            \
    CARAPI SetMessage(                                                      \
        /* [in] */ ICharSequence* message);                                 \
                                                                            \
    CARAPI SetView(                                                         \
        /* [in] */ IView* view);                                            \
                                                                            \
    CARAPI SetViewEx(                                                       \
        /* [in] */ IView* view,                                             \
        /* [in] */ Int32 viewSpacingLeft,                                   \
        /* [in] */ Int32 viewSpacingTop,                                    \
        /* [in] */ Int32 viewSpacingRight,                                  \
        /* [in] */ Int32 viewSpacingBottom);                                \
                                                                            \
    CARAPI SetButton(                                                       \
        /* [in] */ Int32 whichButton,                                       \
        /* [in] */ ICharSequence* text);                                    \
                                                                            \
    CARAPI SetButtonEx(                                                     \
        /* [in] */ Int32 whichButton,                                       \
        /* [in] */ ICharSequence* text,                                     \
        /* [in] */ IDialogInterfaceOnClickListener* listener);              \
                                                                            \
    CARAPI SetIcon(                                                         \
        /* [in] */ Int32 resId);                                            \
                                                                            \
    CARAPI SetIconEx(                                                       \
        /* [in] */ IDrawable* icon);                                        \
                                                                            \
    CARAPI SetInverseBackgroundForced(                                      \
        /* [in] */ Boolean forceInverseBackground);

#define IALERTDIALOG_METHODS_IMPL(className, superClass, overRideClass)     \
ECode className::GetButton(                                                 \
    /* [in] */ Int32 whichButton,                                           \
    /* [out] */ IButton** button)                                           \
{                                                                           \
    VALIDATE_NOT_NULL(button);                                              \
    AutoPtr<IButton> temp = superClass::GetButton(whichButton);             \
    *button = temp;                                                         \
    if (*button) {                                                          \
        (*button)->AddRef();                                                \
    }                                                                       \
                                                                            \
    return NOERROR;                                                         \
}                                                                           \
                                                                            \
ECode className::GetListView(                                               \
    /* [out] */ IListView** listView)                                       \
{                                                                           \
    VALIDATE_NOT_NULL(listView);                                            \
    AutoPtr<IListView> temp = superClass::GetListView();                    \
    *listView = temp;                                                       \
    if (*listView) {                                                        \
        (*listView)->AddRef();                                              \
    }                                                                       \
                                                                            \
    return NOERROR;                                                         \
}                                                                           \
                                                                            \
ECode className::SetCustomTitle(                                            \
    /* [in] */ IView* customTitleView)                                      \
{                                                                           \
    return superClass::SetCustomTitle(customTitleView);                     \
}                                                                           \
                                                                            \
ECode className::SetMessage(                                                \
    /* [in] */ ICharSequence* message)                                      \
{                                                                           \
    return superClass::SetMessage(message);                                 \
}                                                                           \
                                                                            \
ECode className::SetView(                                                   \
    /* [in] */ IView* view)                                                 \
{                                                                           \
    return superClass::SetView(view);                                       \
}                                                                           \
                                                                            \
ECode className::SetViewEx(                                                 \
    /* [in] */ IView* view,                                                 \
    /* [in] */ Int32 viewSpacingLeft,                                       \
    /* [in] */ Int32 viewSpacingTop,                                        \
    /* [in] */ Int32 viewSpacingRight,                                      \
    /* [in] */ Int32 viewSpacingBottom)                                     \
{                                                                           \
    return superClass::SetView(                                             \
        view, viewSpacingLeft, viewSpacingTop,                              \
        viewSpacingRight, viewSpacingBottom);                               \
}                                                                           \
                                                                            \
ECode className::SetButton(                                                 \
    /* [in] */ Int32 whichButton,                                           \
    /* [in] */ ICharSequence* text)                                         \
{                                                                           \
    return superClass::SetButton(whichButton, text);                        \
}                                                                           \
                                                                            \
ECode className::SetButtonEx(                                               \
    /* [in] */ Int32 whichButton,                                           \
    /* [in] */ ICharSequence* text,                                         \
    /* [in] */ IDialogInterfaceOnClickListener* listener)                   \
{                                                                           \
    return superClass::SetButton(whichButton, text, listener);              \
}                                                                           \
                                                                            \
ECode className::SetIcon(                                                   \
    /* [in] */ Int32 resId)                                                 \
{                                                                           \
    return superClass::SetIcon(resId);                                      \
}                                                                           \
                                                                            \
ECode className::SetIconEx(                                                 \
    /* [in] */ IDrawable* icon)                                             \
{                                                                           \
    return superClass::SetIcon(icon);                                       \
}                                                                           \
                                                                            \
ECode className::SetInverseBackgroundForced(                                \
    /* [in] */ Boolean forceInverseBackground)                              \
{                                                                           \
    return superClass::SetInverseBackgroundForced(forceInverseBackground);  \
}

#endif  //__ALERTDIALOGMACRO_H__