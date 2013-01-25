
#ifndef __CCONSOLEMESSAGE_H__
#define __CCONSOLEMESSAGE_H__

#include "_CConsoleMessage.h"

CarClass(CConsoleMessage)
{
public:
    CARAPI MessageLevel(
        /* [out] */ Int32 * pMessageLevel);

    CARAPI Message(
        /* [out] */ String * pMessage);

    CARAPI SourceId(
        /* [out] */ String * pSourceId);

    CARAPI LineNumber(
        /* [out] */ Int32 * pLineNumber);

    CARAPI constructor(
        /* [in] */ CString message,
        /* [in] */ CString sourceId,
        /* [in] */ Int32 lineNumber,
        /* [in] */ Int32 msgLevel);

private:
    // TODO: Add your private member variables here.
};

#endif // __CCONSOLEMESSAGE_H__
