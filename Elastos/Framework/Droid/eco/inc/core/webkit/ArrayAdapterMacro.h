#ifndef __ARRAYADAPTERMACRO_H__
#define __ARRAYADAPTERMACRO_H__

#define IARRAYADAPTER_METHODS_DECL()                                        \    
     CARAPI Add(                                                            \
        /* [in] */ IInterface* object);                                     \
                                                                            \
    CARAPI Insert(                                                          \
        /* [in] */ IInterface* object,                                      \
        /* [in] */ Int32 index);                                            \
                                                                            \
    CARAPI Remove(                                                          \
        /* [in] */ IInterface* object);                                     \
                                                                            \
    CARAPI Clear();                                                         \
                                                                            \
    CARAPI Sort(                                                            \
        /* [in] */ IComparator* comparator);                                \
                                                                            \
    CARAPI SetNotifyOnChange(                                               \
        /* [in] */ Boolean notifyOnChange);                                 \
                                                                            \
    CARAPI GetContext(                                                      \
        /* [out] */ IContext** context);                                    \
                                                                            \
    CARAPI GetPosition(                                                     \
        /* [in] */ IInterface* item,                                        \
        /* [out] */ Int32* position);                                       \
                                                                            \
    CARAPI SetDropDownViewResource(                                         \
        /* [in] */ Int32 resource);                                         \
                                                                            \
    CARAPI GetDropDownView(                                                 \
        /* [in] */ Int32 position,                                          \
        /* [in] */ IView* convertView,                                      \
        /* [in] */ IViewGroup* parent,                                      \
        /* [out] */ IView** view);                                          \
                                                                            \
    CARAPI GetFilter(                                                       \
        /* [out] */ IFilter** filter);                                      \

#define IARRAYADAPTER_METHODS_IMPL(className, superClass, overRideClass)            \
ECode className::Add(                                                               \
    /* [in] */ IInterface* object)                                                  \
{                                                                                   \
    return superClass::Add(object);                                                 \
}                                                                                   \
                                                                                    \
ECode className::Insert(                                                            \
    /* [in] */ IInterface* object,                                                  \
    /* [in] */ Int32 index)                                                         \
{                                                                                   \
    return superClass::Insert(object, index);                                       \
}                                                                                   \
                                                                                    \
ECode className::Remove(                                                            \
    /* [in] */ IInterface* object)                                                  \
{                                                                                   \
    return superClass::Remove(object);                                              \
}                                                                                   \
                                                                                    \
ECode className::Clear()                                                            \
{                                                                                   \
    return className::Clear();                                                      \
}                                                                                   \
                                                                                    \
ECode className::Sort(                                                              \
    /* [in] */ IComparator* comparator)                                             \
{                                                                                   \
    return superClass::Sort(comparator);                                            \
}                                                                                   \
                                                                                    \
ECode className::SetNotifyOnChange(                                                 \
    /* [in] */ Boolean notifyOnChange)                                              \
{                                                                                   \
    return superClass::SetNotifyOnChange(notifyOnChange);                           \
}                                                                                   \
                                                                                    \
ECode className::GetContext(                                                        \
    /* [out] */ IContext** context)                                                 \
{                                                                                   \
    VALIDATE_NOT_NULL(context);                                                     \
    AutoPtr<IContext> temp = superClass::GetContext();                              \
    *context = temp;                                                                \
    if (*context) {                                                                 \
        (*context)->AddRef();                                                       \
    }                                                                               \
                                                                                    \
    return NOERROR;                                                                 \
}                                                                                   \
                                                                                    \
ECode className::GetPosition(                                                       \
    /* [in] */ IInterface* item,                                                    \
    /* [out] */ Int32* position)                                                    \
{                                                                                   \
    VALIDATE_NOT_NULL(position);                                                    \
    *position = superClass::GetPosition(item);                                      \
                                                                                    \
    return NOERROR;                                                                 \
}                                                                                   \
                                                                                    \
ECode className::SetDropDownViewResource(                                           \
    /* [in] */ Int32 resource)                                                      \
{                                                                                   \
    return superClass::SetDropDownViewResource(resource);                           \
}                                                                                   \
                                                                                    \
ECode className::GetDropDownView(                                                   \
    /* [in] */ Int32 position,                                                      \
    /* [in] */ IView* convertView,                                                  \
    /* [in] */ IViewGroup* parent,                                                  \
    /* [out] */ IView** view)                                                       \
{                                                                                   \
    VALIDATE_NOT_NULL(view);                                                        \
    AutoPtr<IView> temp = superClass::GetDropDownView(                              \
        position, convertView, parent);                                             \
    *view = temp;                                                                   \
    if (*view) {                                                                    \
        (*view)->AddRef();                                                          \
    }                                                                               \
                                                                                    \
    return NOERROR;                                                                 \
}                                                                                   \
                                                                                    \
ECode className::GetFilter(                                                         \
    /* [out] */ IFilter** filter)                                                   \
{                                                                                   \
    VALIDATE_NOT_NULL(filter);                                                      \
    AutoPtr<IFilter> temp = superClass::GetFilter();                                \
    *filter = temp;                                                                 \
    if (*filter) {                                                                  \
        (*filter)->AddRef();                                                        \
    }                                                                               \
                                                                                    \
    return NOERROR;                                                                 \
}                                                                                   \

#endif  //__ARRAYADAPTERMACRO_H__                                                    