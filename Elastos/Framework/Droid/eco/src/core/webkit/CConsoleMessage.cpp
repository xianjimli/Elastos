
#include "webkit/CConsoleMessage.h"

ECode CConsoleMessage::MessageLevel(
    /* [out] */ Int32 * pMessageLevel)
{
    if (pMessageLevel == NULL)
    {
        return E_INVALID_ARGUMENT;
    }

    *pMessageLevel = mLevel;
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CConsoleMessage::Message(
    /* [out] */ String * pMessage)
{
    if (pMessage == NULL)
    {
        return E_INVALID_ARGUMENT;
    }

    *pMessage = mMessage;
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CConsoleMessage::SourceId(
    /* [out] */ String * pSourceId)
{
    if (pSourceId == NULL)
    {
        return E_INVALID_ARGUMENT;
    }

    *pSourceId = mSourceId;
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CConsoleMessage::LineNumber(
    /* [out] */ Int32 * pLineNumber)
{
    if (pLineNumber == NULL)
    {
        return E_INVALID_ARGUMENT;
    }

    *pLineNumber = mLineNumber;
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CConsoleMessage::constructor(
    /* [in] */ CString message,
    /* [in] */ CString sourceId,
    /* [in] */ Int32 lineNumber,
    /* [in] */ Int32 msgLevel)
{
    mMessage = message;
    mSourceId = sourceId;
    mLineNumber = lineNumber;
    mLevel = msgLevel;
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

