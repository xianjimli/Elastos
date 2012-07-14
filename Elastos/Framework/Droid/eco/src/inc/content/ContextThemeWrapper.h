
#ifndef __CONTEXTTHEMEWRAPPER_H__
#define __CONTEXTTHEMEWRAPPER_H__

#include "content/ContextWrapper.h"

class ContextThemeWrapper : public ContextWrapper
{
public:
    ContextThemeWrapper();

    ContextThemeWrapper(
        /* [in] */ IContext* base,
        /* [in] */ Int32 themeres);

    CARAPI SetTheme(
        /* [in] */ Int32 resid);

    CARAPI GetTheme(
        /* [out] */ ITheme** theme);

    CARAPI GetSystemService(
        /* [in] */ const String& name,
        /* [out] */ IInterface** object);

protected:
    CARAPI AttachBaseContext(
        /* [in] */ IContext* base);

    CARAPI OnApplyThemeResource(
        /* [in] */ ITheme* theme,
        /* [in] */ Int32 resid,
        /* [in] */ Boolean first);

    CARAPI Init(
        /* [in] */ IContext* base,
        /* [in] */ Int32 themeres);

private:
    CARAPI InitializeTheme();

private:
    AutoPtr<IContext> mBase;
    Int32 mThemeResource;
    AutoPtr<ITheme> mTheme;
    AutoPtr<ILayoutInflater> mInflater;
};

#endif //__CONTEXTTHEMEWRAPPER_H__
