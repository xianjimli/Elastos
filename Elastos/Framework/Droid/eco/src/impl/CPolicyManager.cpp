
#include "impl/CPolicyManager.h"
#include "impl/CPolicy.h"

CPolicyManager::CPolicyManager()
{
    assert(SUCCEEDED(CPolicy::New((IPolicy**)&mPolicy)));
}

ECode CPolicyManager::MakeNewWindow(
    /* [in] */ IContext* ctx,
    /* [out] */ IWindow** window)
{
    return mPolicy->MakeNewWindow(ctx, window);
}

ECode CPolicyManager::MakeNewLayoutInflater(
    /* [in] */ IContext* ctx,
    /* [out] */ ILayoutInflater** inflater)
{
    return mPolicy->MakeNewLayoutInflater(ctx, inflater);
}

ECode CPolicyManager::MakeNewWindowManager(
    /* [out] */ IWindowManagerPolicy** wm)

{
    return mPolicy->MakeNewWindowManager(wm);
}
