
#ifndef __CMERGECURSOR_H__
#define __CMERGECURSOR_H__

#include "_CMergeCursor.h"
#include "database/MergeCursor.h"
#include "database/CursorMacro.h"

CarClass(CMergeCursor), public MergeCursor
{
public:
    CARAPI constructor(
        /* [in] */ ArrayOf<ICursor*>* cursors);

    ICURSOR_METHODS_DECL();

    CARAPI GetWindow(
        /* [out] */ ICursorWindow** window);

    CARAPI FillWindow(
        /* [in] */ Int32 pos,
        /* [in] */ ICursorWindow* window);

    CARAPI OnMove(
        /* [in] */ Int32 oldPosition,
        /* [in] */ Int32 newPosition,
        /* [out] */ Boolean* succeeded);

private:
    // TODO: Add your private member variables here.
};

#endif // __CMERGECURSOR_H__
