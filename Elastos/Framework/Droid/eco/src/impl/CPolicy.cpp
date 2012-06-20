
#include "impl/CPolicy.h"
#include "impl/CPhoneWindow.h"
#include "impl/CPhoneLayoutInflater.h"
#include "impl/CPhoneWindowManager.h"

ECode CPolicy::MakeNewWindow(
    /* [in] */ IContext* ctx,
    /* [out] */ IWindow** window)
{
    return CPhoneWindow::New(ctx, window);
}

ECode CPolicy::MakeNewLayoutInflater(
    /* [in] */ IContext* ctx,
    /* [out] */ ILayoutInflater** inflater)
{
    return CPhoneLayoutInflater::New(ctx, inflater);
}

ECode CPolicy::MakeNewWindowManager(
    /* [out] */ IWindowManagerPolicy** wm)

{
    return CPhoneWindowManager::New(wm);
}
