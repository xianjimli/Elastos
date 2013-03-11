
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

public:
    // This must be kept in sync with the WebCore enum in WebCore/page/Console.h
    enum MessageLevel
    {
        TIP,
        LOG,
        WARNING,
        ERROR,
        DEBUG
    };

private:
    /*MessageLevel*/Int32 mLevel;
    String mMessage;
    String mSourceId;
    Int32 mLineNumber;
};

#endif // __CCONSOLEMESSAGE_H__
