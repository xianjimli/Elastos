
#ifndef __HH_LOG_H
#define __HH_LOG_H

#include <elastos.h>
using namespace Elastos;

namespace Elastos {
namespace Utility {
namespace Logging {
/*!
  @brief
  @since
  @sa
*/
class Logger
{
public:
    /*!
     * @brief Priority constant for the println method; use Logger.V.
     */
    static const Int32 VERBOSE = 2;

    /*!
     * @brief Priority constant for the println method; use Logger.D.
     */
    static const Int32 ___DEBUG = 3;

    /*!
     * @brief Priority constant for the println method; use Logger.I.
     */
    static const Int32 INFO = 4;

    /*!
     * @brief Priority constant for the println method; use Logger.W.
     */
    static const Int32 WARN = 5;

    /*!
     * @brief Priority constant for the println method; use Logger.E.
     */
    static const Int32 ERROR = 6;

    /*!
     * @brief Priority constant for the println method.
     */
    static const Int32 ASSERT = 7;

    /*!
     * @brief Send a DEBUG log message.
	 * @param tag
	 * @param msg
     */
    static ECode D(String tag, String msg);

    /*!
     * @brief Send an ERROR log message.
	 * @param tag
	 * @param msg
     */
    static ECode E(String tag, String msg);

    /*!
     * @brief Send an INFO log message.
	 * @param tag
	 * @param msg
     */
    static ECode I(String tag, String msg);

    /*!
     * @brief Send an VERBOSE log message.
	 * @param tag
	 * @param msg
     */
    static ECode V(String tag, String msg);

    /*!
     * @brief Send a WARN log message.
	 * @param tag
	 * @param msg
     */
    static ECode W(String tag, String msg);

    /*!
     * @brief Checks to see whether or not a log for the specified tag is
     * loggable at the specified level.
	 * @param tag
	 * @param level
     * @param pIsLoggable
     */
    static ECode IsLoggable(String tag, Int32 level, Boolean * pIsLoggable);

    /*!
     * @brief Low-level logging call.
 	 * @param priority
	 * @param tag
	 * @param msg
     */
    static ECode Println(Int32 priority, String tag, String msg);

    /*!
     * @brief
     */
    static const Int32 LOG_ID_MAIN = 0;
    /*!
     * @brief
     */
    static const Int32 LOG_ID_RADIO = 1;
    /*!
     * @brief
     */
    static const Int32 LOG_ID_EVENTS = 2;
    /*!
     * @brief
     */
    static const Int32 LOG_ID_SYSTEM = 3;
	/*!
	 * @brief According the Logger's priority,output corresponding infomation.
	 * @param bufID
	 * @param priority
     * @param tag
     * @param msg
	 */
    static ECode PrintlnImpl(Int32 bufID, Int32 priority, String tag, String msg);
};

} // Logging
} // Utility
} // Elastos

#endif /* __HH_CLOG_H */

