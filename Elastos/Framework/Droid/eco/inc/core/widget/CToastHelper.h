
#ifndef __CTOASTHELPER_H__
#define __CTOASTHELPER_H__

#include "_CToastHelper.h"

CarClass(CToastHelper)
{
public:
    /**
     * Make a standard toast that just contains a text view.
     *
     * @param context  The context to use.  Usually your {@link android.app.Application}
     *                 or {@link android.app.Activity} object.
     * @param text     The text to show.  Can be formatted text.
     * @param duration How long to display the message.  Either {@link #LENGTH_SHORT} or
     *                 {@link #LENGTH_LONG}
     *
     */
    CARAPI MakeText(
        /* [in] */ IContext* context,
        /* [in] */ ICharSequence* text,
        /* [in] */ Int32 duration,
        /* [out] */ IToast** toast);

    /**
         * Make a standard toast that just contains a text view with the text from a resource.
         *
         * @param context  The context to use.  Usually your {@link android.app.Application}
         *                 or {@link android.app.Activity} object.
         * @param resId    The resource id of the string resource to use.  Can be formatted text.
         * @param duration How long to display the message.  Either {@link #LENGTH_SHORT} or
         *                 {@link #LENGTH_LONG}
         *
         * @throws Resources.NotFoundException if the resource can't be found.
         */
    CARAPI MakeTextEx(
        /* [in] */ IContext* context,
        /* [in] */ Int32 resId,
        /* [in] */ Int32 duration,
        /* [out] */ IToast** toast);
};
#endif //__CTOASTHELPER_H__
