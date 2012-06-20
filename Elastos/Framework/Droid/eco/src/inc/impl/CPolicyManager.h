
#ifndef __CPOLICYMANAGER_H__
#define __CPOLICYMANAGER_H__

#include "_CPolicyManager.h"
#include <elastos/AutoPtr.h>

CarClass(CPolicyManager)
{
public:
    CPolicyManager();

    CARAPI MakeNewWindow(
        /* [in] */ IContext* ctx,
        /* [out] */ IWindow** window);

    CARAPI MakeNewLayoutInflater(
        /* [in] */ IContext* ctx,
        /* [out] */ ILayoutInflater** inflater);

    CARAPI MakeNewWindowManager(
        /* [out] */ IWindowManagerPolicy** wm);

private:
    AutoPtr<IPolicy> mPolicy;
};

#endif // __CPOLICYMANAGER_H__
