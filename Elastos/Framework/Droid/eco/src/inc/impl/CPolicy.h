
#ifndef __CPOLICY_H__
#define __CPOLICY_H__

#include "_CPolicy.h"

CarClass(CPolicy)
{
public:
    CARAPI MakeNewWindow(
        /* [in] */ IContext* ctx,
        /* [out] */ IWindow** window);

    CARAPI MakeNewLayoutInflater(
        /* [in] */ IContext* ctx,
        /* [out] */ ILayoutInflater** inflater);

    CARAPI MakeNewWindowManager(
        /* [out] */ IWindowManagerPolicy** wm);

private:
    // TODO: Add your private member variables here.
};

#endif // __CPOLICY_H__
