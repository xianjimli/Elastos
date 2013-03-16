
#include "webkit/CConsoleMessage.h"

ECode CConsoleMessage::MessageLevel(
    /* [out] */ Int32* messageLevel)
{
    VALIDATE_NOT_NULL(messageLevel);

    *messageLevel = mLevel;
    
    return NOERROR;
}

ECode CConsoleMessage::Message(
    /* [out] */ String* message)
{
    VALIDATE_NOT_NULL(message);

    *message = mMessage;
    
    return NOERROR;
}

ECode CConsoleMessage::SourceId(
    /* [out] */ String* sourceId)
{
    VALIDATE_NOT_NULL(sourceId);

    *sourceId = mSourceId;
    
    return NOERROR;
}

ECode CConsoleMessage::LineNumber(
    /* [out] */ Int32* lineNumber)
{
    VALIDATE_NOT_NULL(lineNumber);

    *lineNumber = mLineNumber;

    return NOERROR;
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

    return NOERROR;
}

