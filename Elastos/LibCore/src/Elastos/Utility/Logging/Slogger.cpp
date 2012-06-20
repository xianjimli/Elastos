
#include "Logger.h"
#include "Slogger.h"

namespace Elastos {
namespace Utility {
namespace Logging {

ECode Slogger::D(String tag, String msg)
{
    return Logger::PrintlnImpl(Logger::LOG_ID_SYSTEM, Logger::___DEBUG, tag, msg);
}

/**
 * Send an ERROR log message.
 */
ECode Slogger::E(String tag, String msg)
{
    return Logger::PrintlnImpl(Logger::LOG_ID_SYSTEM, Logger::ERROR, tag, msg);
}

/**
 * Send an INFO log message.
 */
ECode Slogger::I(String tag, String msg)
{
    return Logger::PrintlnImpl(Logger::LOG_ID_SYSTEM, Logger::INFO, tag, msg);
}

/**
 * Send an VERBOSE log message.
 */
ECode Slogger::V(String tag, String msg)
{
    return Logger::PrintlnImpl(Logger::LOG_ID_SYSTEM, Logger::VERBOSE, tag, msg);
}

/**
 * Send a WARN log message.
 */
ECode Slogger::W(String tag, String msg)
{
    return Logger::PrintlnImpl(Logger::LOG_ID_SYSTEM, Logger::WARN, tag, msg);
}

/**
 * Low-level logging call.
 */
ECode Slogger::Println(Int32 priority, String tag, String msg)
{
    return Logger::PrintlnImpl(Logger::LOG_ID_SYSTEM, priority, tag, msg);
}

} // Logging
} // Utility
} // Elastos
