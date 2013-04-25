
#ifndef __CCLIPBOARDSERVICE_H__
#define __CCLIPBOARDSERVICE_H__

#include "_CClipboardService.h"
#include <elastos/AutoPtr.h>
#include <elastos/Mutex.h>

using namespace Elastos::Core::Threading;

CarClass(CClipboardService)
{
public:
    CClipboardService();

    /**
     * Returns the text on the clipboard.  It will eventually be possible
     * to store types other than text too, in which case this will return
     * null if the type cannot be coerced to text.
     */
    CARAPI GetClipboardText(
        /* [out] */ ICharSequence** text);

    /**
     * Sets the contents of the clipboard to the specified text.
     */
    CARAPI SetClipboardText(
        /* [in] */ ICharSequence* text);

    /**
     * Returns true if the clipboard contains text; false otherwise.
     */
    CARAPI HasClipboardText(
        /* [out] */ Boolean* result);

    /**
     * Instantiates the clipboard.
     */
    CARAPI constructor(
        /*[in] */ IContext* context);

private:
    AutoPtr<ICharSequence> mClipboard;
};

#endif // __CCLIPBOARDSERVICE_H__

