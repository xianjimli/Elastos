
#ifndef __CLAUNCHERACTIVITYLISTITEM_H__
#define __CLAUNCHERACTIVITYLISTITEM_H__

#include "_CLauncherActivityListItem.h"
#include <elastos/AutoPtr.h>


CarClass(CLauncherActivityListItem)
{
public:
    CARAPI constructor(
        /* [in] */ ILocalCapsuleManager* pm,
        /* [in] */ IResolveInfo* resolveInfo,
        /* [in] */ ILauncherActivityIconResizer* resizer);

    CARAPI constructor();

public:
    AutoPtr<IResolveInfo> mResolveInfo;
    AutoPtr<ICharSequence> mLabel;
    AutoPtr<IDrawable> mIcon;
    String mCapsuleName;
    String mClassName;
    AutoPtr<IBundle> mExtras;
};

#endif //__CLAUNCHERACTIVITYLISTITEM_H__
