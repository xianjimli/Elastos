
#ifndef __WINDOWCALLBACKMACRO_H__
#define __WINDOWCALLBACKMACRO_H__

#define IWINDOWCALLBACK_METHODS_DECL()                                  \
    CARAPI DispatchKeyEvent(                                            \
        /* [in] */ IKeyEvent* event,                                    \
        /* [out] */ Boolean* isConsumed);                               \
                                                                        \
    CARAPI DispatchTouchEvent(                                          \
        /* [in] */ IMotionEvent* event,                                 \
        /* [out] */ Boolean* isConsumed);                               \
                                                                        \
    CARAPI DispatchTrackballEvent(                                      \
        /* [in] */ IMotionEvent* event,                                 \
        /* [out] */ Boolean* isConsumed);                               \
                                                                        \
    CARAPI DispatchPopulateAccessibilityEvent(                          \
        /* [in] */ IAccessibilityEvent* event,                          \
        /* [out] */ Boolean* isCompleted);                              \
                                                                        \
    CARAPI OnCreatePanelView(                                           \
        /* [in] */ Int32 featureId,                                     \
        /* [out] */ IView** view);                                      \
                                                                        \
    CARAPI OnCreatePanelMenu(                                           \
        /* [in] */ Int32 featureId,                                     \
        /* [in] */ IMenu* menu,                                         \
        /* [out] */ Boolean* allowToShow);                              \
                                                                        \
    CARAPI OnPreparePanel(                                              \
        /* [in] */ Int32 featureId,                                     \
        /* [in] */ IView* view,                                         \
        /* [in] */ IMenu* menu,                                         \
        /* [out] */ Boolean* allowToShow);                              \
                                                                        \
    CARAPI OnMenuOpened(                                                \
        /* [in] */ Int32 featureId,                                     \
        /* [in] */ IMenu* menu,                                         \
        /* [out] */ Boolean* allowToOpen);                              \
                                                                        \
    CARAPI OnMenuItemSelected(                                          \
        /* [in] */ Int32 featureId,                                     \
        /* [in] */ IMenuItem* item,                                     \
        /* [out] */ Boolean* toFinish);                                 \
                                                                        \
    CARAPI OnWindowAttributesChanged(                                   \
        /* [in] */ IWindowManagerLayoutParams* attrs);                  \
                                                                        \
    CARAPI OnContentChanged();                                          \
                                                                        \
    CARAPI OnWindowFocusChanged(                                        \
        /* [in] */ Boolean hasFocus);                                   \
                                                                        \
    CARAPI OnAttachedToWindow();                                        \
                                                                        \
    CARAPI OnDetachedFromWindow();                                      \
                                                                        \
    CARAPI OnPanelClosed(                                               \
        /* [in] */ Int32 featureId,                                     \
        /* [in] */ IMenu* menu);                                        \
                                                                        \
    CARAPI OnSearchRequested(                                           \
        /* [out] */ Boolean* isLaunched);

#define IWINDOWCALLBACK_METHODS_IMPL(className, superClass, overRideClass)\
ECode className::DispatchKeyEvent(                                      \
    /* [in] */ IKeyEvent* event,                                        \
    /* [out] */ Boolean* isConsumed)                                    \
{                                                                       \
    VALIDATE_NOT_NULL(isConsumed);                                      \
    *isConsumed = superClass::DispatchKeyEvent(event);                  \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::DispatchTouchEvent(                                    \
    /* [in] */ IMotionEvent* event,                                     \
    /* [out] */ Boolean* isConsumed)                                    \
{                                                                       \
    VALIDATE_NOT_NULL(isConsumed);                                      \
    *isConsumed = superClass::DispatchTouchEvent(event);                \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::DispatchTrackballEvent(                                \
    /* [in] */ IMotionEvent* event,                                     \
    /* [out] */ Boolean* isConsumed)                                    \
{                                                                       \
    VALIDATE_NOT_NULL(isConsumed);                                      \
    *isConsumed = superClass::DispatchTrackballEvent(event);            \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::DispatchPopulateAccessibilityEvent(                    \
    /* [in] */ IAccessibilityEvent* event,                              \
    /* [out] */ Boolean* isCompleted)                                   \
{                                                                       \
    VALIDATE_NOT_NULL(isCompleted);                                     \
    *isCompleted = superClass::DispatchPopulateAccessibilityEvent(event);\
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::OnCreatePanelView(                                     \
    /* [in] */ Int32 featureId,                                         \
    /* [out] */ IView** view)                                           \
{                                                                       \
    VALIDATE_NOT_NULL(view);                                            \
    AutoPtr<IView> temp = superClass::OnCreatePanelView(featureId);     \
    *view = temp;                                                       \
    if (*view) {                                                        \
	    (*view)->AddRef();                                              \
    }                                                                   \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::OnCreatePanelMenu(                                     \
    /* [in] */ Int32 featureId,                                         \
    /* [in] */ IMenu* menu,                                             \
    /* [out] */ Boolean* allowToShow)                                   \
{                                                                       \
    VALIDATE_NOT_NULL(allowToShow);                                     \
    *allowToShow = superClass::OnCreatePanelMenu(featureId, menu);      \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::OnPreparePanel(                                        \
    /* [in] */ Int32 featureId,                                         \
    /* [in] */ IView* view,                                             \
    /* [in] */ IMenu* menu,                                             \
    /* [out] */ Boolean* allowToShow)                                   \
{                                                                       \
    VALIDATE_NOT_NULL(allowToShow);                                     \
    *allowToShow = superClass::OnPreparePanel(featureId, view, menu);   \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::OnMenuOpened(                                          \
    /* [in] */ Int32 featureId,                                         \
    /* [in] */ IMenu* menu,                                             \
    /* [out] */ Boolean* allowToOpen)                                   \
{                                                                       \
    VALIDATE_NOT_NULL(allowToOpen);                                     \
    *allowToOpen = superClass::OnMenuOpened(featureId, menu);           \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::OnMenuItemSelected(                                    \
    /* [in] */ Int32 featureId,                                         \
    /* [in] */ IMenuItem* item,                                         \
    /* [out] */ Boolean* toFinish)                                      \
{                                                                       \
    VALIDATE_NOT_NULL(toFinish);                                        \
    *toFinish = superClass::OnMenuItemSelected(featureId, item);        \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::OnWindowAttributesChanged(                             \
    /* [in] */ IWindowManagerLayoutParams* attrs)                       \
{                                                                       \
    return superClass::OnWindowAttributesChanged(attrs);                \
}                                                                       \
                                                                        \
ECode className::OnContentChanged()                                     \
{                                                                       \
    return superClass::OnContentChanged();                              \
}                                                                       \
                                                                        \
ECode className::OnWindowFocusChanged(                                  \
    /* [in] */ Boolean hasFocus)                                        \
{                                                                       \
    return superClass::OnWindowFocusChanged(hasFocus);                  \
}                                                                       \
                                                                        \
ECode className::OnAttachedToWindow()                                   \
{                                                                       \
    return superClass::OnAttachedToWindow();                            \
}                                                                       \
                                                                        \
ECode className::OnDetachedFromWindow()                                 \
{                                                                       \
    return superClass::OnDetachedFromWindow();                          \
}                                                                       \
                                                                        \
ECode className::OnPanelClosed(                                         \
    /* [in] */ Int32 featureId,                                         \
    /* [in] */ IMenu* menu)                                             \
{                                                                       \
    return superClass::OnPanelClosed(featureId, menu);                  \
}                                                                       \
                                                                        \
ECode className::OnSearchRequested(                                     \
    /* [out] */ Boolean* isLaunched)                                    \
{                                                                       \
    VALIDATE_NOT_NULL(isLaunched);                                      \
    *isLaunched = superClass::OnSearchRequested();                      \
                                                                        \
    return NOERROR;                                                     \
}

#endif  //__WINDOWCALLBACKMACRO_H__
