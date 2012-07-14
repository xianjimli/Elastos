
#include "content/ContextThemeWrapper.h"
#include "view/LayoutInflater.h"

ContextThemeWrapper::ContextThemeWrapper()
{
}

ContextThemeWrapper::ContextThemeWrapper(
    /* [in] */ IContext* base,
    /* [in] */ Int32 themeres)
    : ContextWrapper(base)
{
    mBase = base;
    mThemeResource = themeres;
}

ECode ContextThemeWrapper::AttachBaseContext(
    /* [in] */ IContext* newBase)
{
    ContextWrapper::AttachBaseContext(newBase);
    mBase = newBase;
    return NOERROR;
}

ECode ContextThemeWrapper::SetTheme(
    /* [in] */ Int32 resid)
{
    mThemeResource = resid;
    return InitializeTheme();
}

ECode ContextThemeWrapper::GetTheme(
    /* [in] */ ITheme** theme)
{
    if (mTheme == NULL) {
        if (mThemeResource == 0) {
            mThemeResource = 0x01030005/*com.android.internal.R.style.Theme*/;
        }
        FAIL_RETURN(InitializeTheme());
    }

    *theme = mTheme;
    mTheme->AddRef();

    return NOERROR;
}

ECode ContextThemeWrapper::GetSystemService(
    /* [in] */ const String& name,
    /* [out] */ IInterface** object)
{
    if (!String(Context_LAYOUT_INFLATER_SERVICE).Compare(name)) {
        if (mInflater == NULL) {
            AutoPtr<ILayoutInflater> inflater;
            LayoutInflater::From(mBase, (ILayoutInflater**)&inflater);
            inflater->CloneInContext(
                (IContext*)this->Probe(EIID_IContext),
                (ILayoutInflater**)&mInflater);
        }
        *object = mInflater;
        if (*object) {
            (*object)->AddRef();
        }

        return NOERROR;
    }

    return mBase->GetSystemService(name, object);
}

/**
 * Called by {@link #setTheme} and {@link #getTheme} to apply a theme
 * resource to the current Theme object.  Can override to change the
 * default (simple) behavior.  This method will not be called in multiple
 * threads simultaneously.
 *
 * @param theme The Theme object being modified.
 * @param resid The theme style resource being applied to <var>theme</var>.
 * @param first Set to true if this is the first time a style is being
 *              applied to <var>theme</var>.
 */
ECode ContextThemeWrapper::OnApplyThemeResource(
    /* [in] */ ITheme* theme,
    /* [in] */ Int32 resid,
    /* [in] */ Boolean first)
{
    return theme->ApplyStyle(resid, TRUE);
}

ECode ContextThemeWrapper::InitializeTheme()
{
    Boolean first = mTheme == NULL;
    if (first) {
        AutoPtr<IResources> resources;
        GetResources((IResources**)&resources);
        resources->NewTheme((ITheme**)&mTheme);
        AutoPtr<ITheme> theme;
        mBase->GetTheme((ITheme**)&theme);
        if (theme != NULL) {
            mTheme->SetTo(theme);
        }
    }

    return OnApplyThemeResource(mTheme, mThemeResource, first);
}

ECode ContextThemeWrapper::Init(
    /* [in] */ IContext* base,
    /* [in] */ Int32 themeres)
{
    ContextWrapper::Init(base);
    mBase = base;
    mThemeResource = themeres;

    return NOERROR;
}
