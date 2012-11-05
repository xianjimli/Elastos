
#ifndef __CEXPANDABLELISTCONTEXTMENUINFO_H__
#define __CEXPANDABLELISTCONTEXTMENUINFO_H__

#include "_CExpandableListContextMenuInfo.h"

CarClass(CExpandableListContextMenuInfo)
{
public:
    CARAPI constructor(
        /* [in] */ IView * pTargetView,
        /* [in] */ Int64 packedPosition,
        /* [in] */ Int64 id);

private:
    // TODO: Add your private member variables here.
};

#endif // __CEXPANDABLELISTCONTEXTMENUINFO_H__
