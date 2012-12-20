
#ifndef  _CWINDOWMANAGERLAYOUTPARAMSHELPER_H__
#define  _CWINDOWMANAGERLAYOUTPARAMSHELPER_H__

#include "_CWindowManagerLayoutParamsHelper.h"

CarClass(CWindowManagerLayoutParamsHelper)
{
public:
    CARAPI MayUseInputMethodHelper(
        /* [in] */ Int32 flags,
        /* [out] */ Boolean* mayUse);
};

#endif  //_CWINDOWMANAGERLAYOUTPARAMSHELPER_H__
