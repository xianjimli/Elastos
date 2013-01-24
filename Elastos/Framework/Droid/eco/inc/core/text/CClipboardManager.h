
#ifndef __CCLIPBOARDMANAGER_H__
#define __CCLIPBOARDMANAGER_H__

#include "_CClipboardManager.h"
#include <elastos/AutoPtr.h>

CarClass(CClipboardManager)
{
public:
    /**
     * Returns the text on the clipboard.  It will eventually be possible
     * to store types other than text too, in which case this will return
     * null if the type cannot be coerced to text.
     */
    CARAPI GetText(
        /* [out] */ ICharSequence** text);

    /**
     * Sets the contents of the clipboard to the specified text.
     */
    CARAPI SetText(
        /* [in] */ ICharSequence* text);

    /**
     * Returns true if the clipboard contains text; false otherwise.
     */
    CARAPI HasText(
        /* [out] */ Boolean* result);

    /** {@hide} */
    CARAPI constructor(
        /* [in] */ IContext* context);

private:

    static AutoPtr<ILocationManager> mService;
    AutoPtr<IContext> mContext;
};

#endif // __CCLIPBOARDMANAGER_H__
