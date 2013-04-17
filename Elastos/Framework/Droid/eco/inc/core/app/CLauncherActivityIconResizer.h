
#ifndef __CLAUNCHERACTIVITYICONRESIZER_H__
#define __CLAUNCHERACTIVITYICONRESIZER_H__

#include "_CLauncherActivityIconResizer.h"
#include "app/CLauncherActivity.h"
#include <elastos/AutoPtr.h>

CarClass(CLauncherActivityIconResizer)
{
public:
    CARAPI constructor(ILauncherActivity* lActivity);

    /**
     * Returns a Drawable representing the thumbnail of the specified Drawable.
     * The size of the thumbnail is defined by the dimension
     * android.R.dimen.launcher_application_icon_size.
     *
     * This method is not thread-safe and should be invoked on the UI thread only.
     *
     * @param icon The icon to get a thumbnail of.
     *
     * @return A thumbnail for the specified icon or the icon itself if the
     *         thumbnail could not be created. 
     */
    CARAPI CreateIconThumbnail(
        /* [in] */ IDrawable* icon,
        /* [out] */ IDrawable** drawable);

private:
    // Code is borrowed from com.android.launcher.Utilities. 
    Int32 mIconWidth;

    Int32 mIconHeight;

    AutoPtr<IRect> mOldBounds;

    AutoPtr<ICanvas> mCanvas;

    CLauncherActivity* mLauncherActivity;

};

#endif //__CLAUNCHERACTIVITYICONRESIZER_H__
