
#ifdef _MSVC
#include "androidlog.h"
#else
#include <android/log.h>
#endif
#include "Logger.h"
#include <errno.h>

#ifdef __cplusplus
extern "C" {
#endif
/*
 * Send a simple string to the log.
 */
int __android_log_buf_write(int bufID, int prio, const char *tag, const char *text);
int __android_log_buf_print(int bufID, int prio, const char *tag, const char *fmt, ...);
#ifdef __cplusplus
}
#endif


#ifdef _openkode
    #include <stdio.h>
#else // _openkode
    ECode ErrToECode(int err)
    {
        ECode ec = NOERROR;
        switch (err) {
        case 0:
            break;
        case EINTR:
            ec = E_INVALID_OPERATION;
            break;
        case EBADF:
            ec = E_FILE_NOT_FOUND;
            break;
        default:
            ec = E_FAIL;
            break;
        }
        return ec;
    }
#endif // _openkode

namespace Elastos {
namespace Utility {
namespace Logging {

const Int32 Logger::VERBOSE;
const Int32 Logger::___DEBUG;
const Int32 Logger::INFO;
const Int32 Logger::WARN;
const Int32 Logger::ERROR;
const Int32 Logger::ASSERT;
const Int32 Logger::LOG_ID_MAIN;
const Int32 Logger::LOG_ID_RADIO;
const Int32 Logger::LOG_ID_EVENTS;
const Int32 Logger::LOG_ID_SYSTEM;

ECode Logger::D(CString tag, CString msg)
{
    return PrintlnImpl(LOG_ID_MAIN, ANDROID_LOG_DEBUG, tag, msg);
}

ECode Logger::E(CString tag, CString msg)
{
    return PrintlnImpl(LOG_ID_MAIN, ANDROID_LOG_ERROR, tag, msg);
}

ECode Logger::I(CString tag, CString msg)
{
    return PrintlnImpl(LOG_ID_MAIN, ANDROID_LOG_INFO, tag, msg);
}

ECode Logger::V(CString tag, CString msg)
{
    return PrintlnImpl(LOG_ID_MAIN, ANDROID_LOG_VERBOSE, tag, msg);
}

ECode Logger::W(CString tag, CString msg)
{
    return PrintlnImpl(LOG_ID_MAIN, ANDROID_LOG_WARN, tag, msg);
}

ECode Logger::IsLoggable(CString tag, Int32 level, Boolean * pIsLoggable)
{
    return E_NOT_IMPLEMENTED;
}

ECode Logger::Println(Int32 priority, CString tag, CString msg)
{
    return PrintlnImpl(LOG_ID_MAIN, priority, tag, msg);
}

ECode Logger::PrintlnImpl(Int32 bufID, Int32 priority, CString tag, CString msg)
{
    if (bufID < LOG_ID_MAIN || bufID > LOG_ID_SYSTEM || tag.IsNull()
        || msg.IsNull() || priority < VERBOSE || priority > ASSERT) {
        return E_INVALID_ARGUMENT;
    }

#ifdef _openkode
    char ch = '0';

    switch (priority) {
    case VERBOSE:
        ch = 'V';
        break;
    case DEBUG:
        ch = 'D';
        break;
    case INFO:
        ch = 'I';
        break;
    case WARN:
        ch = 'W';
        break;
    case ERROR:
        ch = 'E';
        break;
    case ASSERT:
        ch = 'A';
        break;
    }

    printf("%c/%s: %s", ch, (const char *)tag, (const char *)msg);
    return NOERROR;
#else // _openkode
    return ErrToECode(
        __android_log_buf_write(bufID, priority,
            (const char *)tag, (const char *)msg));
#endif // _openkode
}

} // Logging
} // Utility
} // Elastos

